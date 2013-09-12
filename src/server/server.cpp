/*
* Squarez puzzle game server binary
* Copyright (C) 2013  Patrick Nicolas <patricknicolas@laposte.net>
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
#include <memory>
#include <thread>

#define SIZE 8
#define SYMBOLS 3

// Systemd socket activation
#ifndef DISABLE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif

#include <sys/socket.h>
#include <netinet/in.h>

// FastCGI
#include <fastcgi++/manager.hpp>

#include <boost/program_options.hpp>

#include "server/requesthandler.h"
#include "gamestatus.h"

int main(int argc, char ** argv)
{
	// Declare program options
	boost::program_options::options_description options;
	options.add_options()
		("help,h", "this help message")
		("port,p", boost::program_options::value<int>(), "Listen to the given port");

	// Parse command line parameters
	boost::program_options::variables_map parameters;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), parameters);
	boost::program_options::notify(parameters);

	if (parameters.count("help"))
	{
		std::cout << options << std::endl;
		return 0;
	}

	int socket_fd = -1;

#ifndef DISABLE_SYSTEMD
	// Check systemd socket activation
	int systemd_fds = sd_listen_fds(1);
	if (systemd_fds > 1)
	{
		std::cerr << "Expected 1 open socket, got " << systemd_fds << std::endl;
		return 1;
	}
	if (systemd_fds == 1)
	{
		socket_fd = SD_LISTEN_FDS_START;
	}
#endif

	// If we don't have an open socket, create it
	if (socket_fd < 0 and parameters.count("port"))
	{
		socket_fd = socket(PF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
		{
			std::cerr << "Failed to create socket." << std::endl;
			return 1;
		}

		struct sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(parameters["port"].as<int>());
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0
			or listen(socket_fd, 128))
		{
			std::cerr << "Failed to create socket." << std::endl;
			return 1;
		}
	}

	if (socket_fd < 0)
	{
		std::cerr << "No socket available." << std::endl;
		return 1;
	}

	// Initialize the game
	squarez::GameStatus game(squarez::GameBoard(SIZE, SYMBOLS), std::chrono::seconds(10));

	// Start listening on the provided socket
	Fastcgipp::Manager<squarez::RequestHandler> manager(socket_fd);
	try{
		manager.handler();
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
	}
}
