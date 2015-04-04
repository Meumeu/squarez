/*
* Squarez puzzle game server binary
* Copyright (C) 2013-2015  Patrick Nicolas <patricknicolas@laposte.net>
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

// FastCGI
#include <fastcgi++/manager.hpp>

#include <boost/program_options.hpp>

#include "requesthandler.h"
#include "database/database.h"

Fastcgipp::Manager<squarez::RequestHandler> * manager = nullptr;


namespace {
void ctrl_c(int)
{
	if (manager)
		manager->terminate();
}


}

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

	std::string port;
	std::string listen_ip;
	std::string db_filename;
	std::string cfg_filename;

	config_file_options.add_options()
		("port,p", boost::program_options::value<std::string>(&port), "Listen to the given port")
		("listen,l", boost::program_options::value<std::string>(&listen_ip)->default_value("127.0.0.1"), "Listen on the given IP address")
		("database", boost::program_options::value<std::string>(&db_filename)->default_value("/var/lib/squarezd/squarezd.db"), "Database file name");

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
		return 2;
	}

	int socket_fd = -1;

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
		socket_fd = SD_LISTEN_FDS_START;
	}
#endif

	// If we don't have an open socket, create it
	if (socket_fd < 0 and parameters.count("port"))
	{
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;

		struct addrinfo * ai = nullptr;
		int rc = getaddrinfo(listen_ip.c_str(), port.c_str(), &hints, &ai);

		if (rc)
		{
			std::cerr << listen_ip << ":" << port << ": " << gai_strerror(rc) << std::endl;
			return 4;
		}

		for(struct addrinfo * i = ai; i; i = i->ai_next)
		{
			socket_fd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);
			if (socket_fd == -1)
				continue;

			if (bind(socket_fd, i->ai_addr, i->ai_addrlen) < 0
				or listen(socket_fd, 128))
			{
				close(socket_fd);
				socket_fd = -1;
				continue;
			}

			break;
		}

		if (ai)
			freeaddrinfo(ai);
	}

	if (socket_fd < 0)
	{
		std::cerr << "No socket available." << std::endl;
		return 5;
	}

	try
	{
		if (parameters.count("database"))
		{
			squarez::RequestHandler::highScores = std::make_shared<squarez::HighScores>(db_filename);
		}
		else
		{
			squarez::RequestHandler::highScores = std::make_shared<squarez::HighScores>();
		}
	}
	catch(squarez::database::exception& e)
	{
		std::cerr << "Cannot open database: " << e.what() << std::endl;
		return 6;
	}

	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &ctrl_c;
	sigaction(SIGINT, &sa, nullptr);

	// Start listening on the provided socket
	try
	{
		manager = new Fastcgipp::Manager<squarez::RequestHandler>(socket_fd);
		manager->handler();
		delete manager;
		return 0;
	}
	catch (std::exception & e)
	{
		std::cerr << "An exception occured: " << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "An unknown exception occured." << std::endl;
		return 1;
	}
}
