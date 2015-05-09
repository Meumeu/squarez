/*
 * Squarez puzzle game server binary
 * Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
 * Copyright (C) 2015  Guillaume Meunier <guillaume.meunier@centraliens.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>

// Systemd socket activation
#ifndef DISABLE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "requesthandler.h"

#include <Simple-Web-Server/server_http.hpp>


using namespace std::placeholders;

class HttpServer : public SimpleWeb::Server<SimpleWeb::HTTP>
{
private:
	boost::asio::signal_set signals;
	squarez::RequestHandler& handler;

#ifndef NDEBUG
	std::map<std::string, std::string> files;
#endif

	void ctrlC(const boost::system::error_code& error, int /*signal_number*/)
	{
		signals.async_wait(std::bind(&HttpServer::ctrlC, this, _1, _2));
		if (!error)
		{
			// A signal occurred.
			stop();
		}
	}

	void serveFile(Response& response, std::shared_ptr<Request> request)
	{
#ifndef NDEBUG
		std::cerr << "Request: " << request->path << std::endl;

		auto it = files.find(request->path);

		if (it != files.end())
		{
			response << it->second;
		}
		else
#endif
		{
			std::stringstream out;
			out << "URL not found" << std::endl;
			response << "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " << out.str().size() << "\r\n\r\n" << out.str();
			std::cerr << "URL not found: " << request->path << std::endl;
		}
	}

	std::string contentType(const std::string& url)
	{
		size_t pos = url.find_last_of('.');
		if (pos != std::string::npos)
		{
			std::string ext = url.substr(pos);

			if (ext == ".html")
				return "text/html";
			else if (ext == ".css")
				return "text/css";
			else if (ext == ".js")
				return "application/javascript";
			else if (ext == ".svg")
				return "image/svg+xml";
			else if (ext == ".png")
				return "image/png";
			else if (ext == ".mem")
				return "application/octet-stream";
		}

		return "text/plain";
	}

public:
	HttpServer(boost::asio::io_service& io_service, int port, squarez::RequestHandler& handler, const std::string& server_root) :
	SimpleWeb::Server<SimpleWeb::HTTP>(io_service, port, 4), signals(io_service, SIGINT, SIGTERM), handler(handler)
	{
		init(server_root);
	}

	HttpServer(boost::asio::io_service& io_service, boost::asio::ip::tcp::acceptor&& acceptor, squarez::RequestHandler& handler, const std::string& server_root) :
	SimpleWeb::Server<SimpleWeb::HTTP>(io_service, std::move(acceptor), 4), signals(io_service, SIGINT, SIGTERM), handler(handler)
	{
		init(server_root);
	}

	void init(const std::string& server_root)
	{
		signals.async_wait(std::bind(&HttpServer::ctrlC, this, _1, _2));

		resource["^/squarez/" + squarez::onlineSinglePlayer::GameInit::method() + "\\?.*"]["GET"] = std::bind(&squarez::RequestHandler::gameInit, &handler, _1, _2);
		resource["^/squarez/" + squarez::onlineSinglePlayer::PushSelection::method() + "\\?.*"]["GET"] = std::bind(&squarez::RequestHandler::pushSelection, &handler, _1, _2);
		resource["^/squarez/" + squarez::onlineSinglePlayer::Pause::method() + "\\?.*"]["GET"] = std::bind(&squarez::RequestHandler::pause, &handler, _1, _2);
		resource["^/squarez/" + squarez::onlineSinglePlayer::GetScores::method() + "\\?.*"]["GET"] = std::bind(&squarez::RequestHandler::getScores, &handler, _1, _2);

		default_resource["GET"] = std::bind(&HttpServer::serveFile, this, _1, _2);

#ifndef NDEBUG
		if (server_root != "")
		{
			size_t nb_files = 0;
			size_t size_files = 0;
			boost::filesystem::path root(server_root);

			for(auto& i: boost::filesystem::recursive_directory_iterator(root))
			{
				boost::filesystem::path path = i.path();
				std::string url = path.string();

				if (url.substr(0, server_root.size()) == server_root)
					url = url.substr(server_root.size());

				size_t pos = url.find_first_not_of('/');
				if (pos > 0)
					url = url.substr(pos - 1);
				else
					url = "/" + url;

				if (url == "/manifest.appcache")
					continue;

				if (boost::filesystem::is_directory(path))
					path /= "index.html";

				std::ifstream ifs(path.string(), std::ifstream::in);

				if (ifs)
				{
					std::stringstream response;
					ifs.seekg(0, std::ios::end);
					size_t length = ifs.tellg();
					ifs.seekg(0, std::ios::beg);

					response << "HTTP/1.1 200 OK\r\n"
					<< "Content-Length: " << length << "\r\n"
					<< "Content-Type: " << contentType(path.string()) << "\r\n\r\n";

					size_t buffer_size = 131072;
					if (length>buffer_size)
					{
						std::vector<char> buffer(buffer_size);
						size_t read_length;
						while((read_length = ifs.read(&buffer[0], buffer_size).gcount()) > 0)
						{
							response.write(&buffer[0], read_length);
						}
					}
					else
						response << ifs.rdbuf();

					ifs.close();

					files[url] = response.str();

					nb_files++;
					size_files += length;
				}
			}

			if (files.find("/index.html") != files.end())
				files["/"] = files["/index.html"];

			std::cerr << "Loaded " << nb_files << " files (" << size_files << " bytes)" << std::endl;
		}
#else
		(void)server_root;
#endif
	}
};

int main(int argc, char ** argv)
{
	int window_width = boost::program_options::options_description::m_default_line_length;

#ifdef TIOCGWINSZ
	struct winsize ws;

	if (ioctl(1, TIOCGWINSZ, &ws) == 0)
		window_width = ws.ws_col;
#endif

	// Declare program options
	boost::program_options::options_description command_line_options("Command line options");
	boost::program_options::options_description config_file_options("Configuration");
	boost::program_options::options_description visible_options("squarezd options", window_width);

	int port;
	std::string listen_ip;
	std::string db_uri;
	std::string db_username;
	std::string db_password;
	std::string db_name;
	std::string cfg_filename;
	std::string server_root;

	config_file_options.add_options()
		("port,p", boost::program_options::value<int>(&port), "Listen to the given port")
		("listen,l", boost::program_options::value<std::string>(&listen_ip)->default_value("127.0.0.1"), "Listen on the given IP address")
		("uri", boost::program_options::value<std::string>(&db_uri)->default_value("tcp://127.0.0.1:3306"), "Database URI")
		("username", boost::program_options::value<std::string>(&db_username), "Database username")
		("password", boost::program_options::value<std::string>(&db_password), "Database password")
		("database", boost::program_options::value<std::string>(&db_name), "Database name")
#ifndef NDEBUG
		("root", boost::program_options::value<std::string>(&server_root)->default_value("."), "Debug server root")
#endif
		;

	command_line_options.add_options()
		("help,h", "this help message")
		("config,f", boost::program_options::value<std::string>(&cfg_filename)->default_value("/etc/squarezd/squarezd.conf"), "Configuration file");

	visible_options.add(command_line_options).add(config_file_options);

	// Parse command line parameters
	boost::program_options::variables_map parameters;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, visible_options), parameters);

	if (parameters.count("help"))
	{
		std::cout << visible_options << std::endl;
		return 0;
	}

	boost::program_options::notify(parameters);

	std::fstream config_file(cfg_filename, std::ios_base::in);
	if (config_file)
	{
		boost::program_options::store(boost::program_options::parse_config_file(config_file, config_file_options), parameters);
		boost::program_options::notify(parameters);
	}
	else
	{
		std::cerr << "Failed to read configuration file: " << cfg_filename << std::endl;
		return 6;
	}

	std::unique_ptr<squarez::HighScores> highScores;
	try
	{
		highScores.reset(new squarez::HighScores(db_uri, db_username, db_password, db_name));
	}
	catch(std::exception& e)
	{
		std::cerr << "Cannot open database: " << e.what() << std::endl;
		return 5;
	}

	squarez::RequestHandler handler(std::move(highScores));

	boost::asio::io_service io_service;
	std::unique_ptr<HttpServer> server;

#ifndef DISABLE_SYSTEMD
	// Check systemd socket activation
	int systemd_fds = sd_listen_fds(1);
	if (systemd_fds > 1)
	{
		std::cerr << "Expected 1 open socket, got " << systemd_fds << std::endl;
		return 3;
	}
	if (systemd_fds == 1)
	{
		int socket_fd = SD_LISTEN_FDS_START;
		boost::asio::ip::tcp tcp(sd_is_socket_inet(socket_fd, AF_INET, SOCK_STREAM, 1, 0) ? boost::asio::ip::tcp::v4() :
			sd_is_socket_inet(socket_fd, AF_INET6, SOCK_STREAM, 1, 0) ? boost::asio::ip::tcp::v6() : throw std::runtime_error("Unknown socket type"));

		boost::asio::ip::tcp::endpoint endpoint(tcp, 0);
		boost::asio::ip::tcp::acceptor acceptor(io_service);
		acceptor.assign(tcp, socket_fd);
		server = std::unique_ptr<HttpServer>(new HttpServer(io_service, std::move(acceptor), handler, server_root));
	}
	else
#endif
	{
		server = std::unique_ptr<HttpServer>(new HttpServer(io_service, port, handler, server_root));
	}

	try
	{
		std::cerr << "squarez daemon started" << std::endl;
#ifndef NDEBUG
		std::cerr << "Debug version on ";
		if (port == 80)
			std::cerr << "http://localhost/" << std::endl;
		else
			std::cerr << "http://localhost:" << port << "/" << std::endl;
#endif
		server->start();
		std::cerr << "squarez daemon stopped normally" << std::endl;
		return EXIT_SUCCESS;
	}
	catch (std::exception & e)
	{
		std::cerr << "An exception occured: " << e.what() << std::endl;
		std::cerr << "squarez daemon stopped" << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "An unknown exception occured." << std::endl;
		std::cerr << "squarez daemon stopped" << std::endl;
		return EXIT_FAILURE;
	}
}
