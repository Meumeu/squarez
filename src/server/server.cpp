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

// FastCGI
#include <fastcgi++/manager.hpp>

#include "server/requesthandler.h"

int main()
{
	int socket = -1;

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
		socket = SD_LISTEN_FDS_START;
	}
#endif

	// If we don't have an open socket, create it
	if (socket < 0)
	{
		//FIXME: create the socket, really
	}

	// Start listening on the provided socket
	Fastcgipp::Manager<squarez::RequestHandler> manager(socket);
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
