/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/10/20 20:51:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include <fstream>

Cluster::Cluster()
{
}

Cluster::Cluster(const Cluster& value)
	: _serverList(value._serverList)
{
}

Cluster& Cluster::operator=(const Cluster& value)
{
	if (this == &value)
		return (*this);

	Cluster tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

Cluster::~Cluster()
{
}

static int openFile(const std::string& file_path, std::fstream* fstream)
{
	if (fstream->is_open())
		fstream->close();
	fstream->open(file_path.c_str(), std::ios::in);
	if (!fstream->is_open())
	{
		MESSAGE(file_path + ": " + strerror(errno), ERROR);
		fstream->clear();
		return (1);
	}
	return (0);
}

static int getLocationConfig(Location* location,
							 std::fstream* fstream)
{
	std::string line;

	while (getline(*fstream, line) && line.find('}') == std::string::npos)
	{
		std::stringstream ss(line);
		std::string directive;
		std::string value;

		ss >> directive;
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR);
			return (1);
		}
		getline(ss, value, ';');
		if (location->setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'",
					ERROR);
			return (1);
		}
	}
	return (0);
}

static int getServerConfig(std::vector<Server*>* serverList,
						   std::fstream* fstream)
{
	Server server;
	std::string line;

	while (getline(*fstream, line) && line.find('}') == std::string::npos)
	{
		std::stringstream ss;
		std::string directive;
		std::string value;

		if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
			continue;
		ss << line;
		ss >> directive;
		if (directive == "location")
		{
			std::string path;
			ss >> path;
			if (path.empty() || path.at(0) != '/') // check if is path
			{
				MESSAGE(path + ": Invalid location path", ERROR);
				return (1);
			}
			ss >> value;
			if (!value.empty() && value != "{")
			{
				MESSAGE(value + ": Unexpected value", ERROR);
				return (1);
			}
			else if (value.empty())
			{
				while (getline(*fstream, line) &&
					   line.find('{') == std::string::npos)
				{
					if (line.find_first_not_of(" \t") != std::string::npos)
					{
						MESSAGE("Expected `{' on location block declaration",
								ERROR);
						return (1);
					}
				}
			}
			Location location(path);

			if (getLocationConfig(&location, fstream))
				return (1);
			server.setLocation(path, new Location(location));
			continue;
		}
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR);
			return (1);
		}
		getline(ss, value, ';');
		if (server.setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'",
					ERROR);
			return (1);
		}
	}
	serverList->push_back(new Server(server));
	return (0);
}

void Cluster::configure(const std::string& file_path)
{
	std::fstream fstream;
	if (file_path.empty() || openFile(file_path, &fstream))
	{
	runDefault:
		MESSAGE("No valid configuration file, using default configuration",
				WARNING);
		this->_serverList.push_back(new Server());
		return;
	}

	std::string line;
	while (getline(fstream, line))
	{
		std::stringstream ss(line);
		std::string block_type;

		ss >> block_type;
		// Check for server block
		if (block_type == "Server")
		{
			std::string bracket;
			ss >> bracket;
			// Check for bracket
			if (bracket.empty())
			{
				while (getline(fstream, line))
				{
					std::stringstream m(line);
					if (!bracket.empty())
						bracket.clear();
					m >> bracket;
					if (!bracket.empty() && bracket.at(0) != '{')
					{
						MESSAGE("Expected `{' on server block declaration",
								ERROR);
						goto runDefault;
					}
					else if (!bracket.empty()) // if found bracket
						break;
				}
			}
			else if (!bracket.empty() && bracket != "{")
			{
				MESSAGE(bracket + ": Unexpected value", ERROR);
				goto runDefault;
			}
			if (getServerConfig(&this->_serverList, &fstream))
				goto runDefault;
			MESSAGE("Finished setting up server!", INFORMATION);
		}
		else if (!block_type.empty())
		{
			MESSAGE(block_type + ": Unexpected block", ERROR);
			goto runDefault;
		}
	}
}

// Check if there are any servers running
static bool isAnyServerRunning(fd_set& set)
{
	for (int i = 0; i < FD_SETSIZE; ++i)
	{
		if (FD_ISSET(i, &set))
			return (true);
	}
	return (false);
}

void Cluster::run()
{
	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	// Bind Server to Socket.
	for (std::vector<Server>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		std::stringstream port;
		port << it->getListenPort();
		MESSAGE("Setting up " + it->getAddress() + ":" + port.str(),
				INFORMATION);
		if (it->run())
		{
			it->stop();
			continue;
		}
		MESSAGE("Listening on " + it->getAddress() + ":" + port.str(),
				INFORMATION);
		FD_SET(it->getSocket(), &current_sockets);
	}
	if (!isAnyServerRunning(current_sockets))
	{
		MESSAGE("No servers were created", ERROR);
		return;
	}
	while (true)
	{
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE(
				"select(): " + ss.str() + ": " + (std::string)strerror(errno),
				ERROR);
			return;
		}

		int connection = -1;
		for (std::vector<Server>::iterator it = this->_serverList.begin();
			 it != this->_serverList.end(); ++it)
		{
			if (!it->getSocket())
				continue;
			if (FD_ISSET(it->getSocket(), &ready_sockets))
			{
				u_int32_t address_length = sizeof(it->getServerAddress());
				if ((connection = accept(it->getSocket(),
										 (struct sockaddr*)&it->getServerAddress(),
										 (socklen_t*)&address_length)) < 0)
				{
					std::stringstream ss;
					ss << errno;
					MESSAGE("accept(): " + ss.str() + ": " +
							(std::string)strerror(errno), ERROR);
					return;
				}
			}
		}
		if (connection == -1)
			continue;
		MESSAGE("Connected with a client", INFORMATION);

		char buffer[8192];
		int64_t bytesRead = read(connection, buffer, 8192);
		if (bytesRead == -1)
		{
			close(connection);
			continue;
		}
		std::cout << buffer << std::endl;

		std::string response =
			"HTTP/1.1 200 OK\r\n\r\nHello how are you?\n\nI am the server\n";
		send(connection, response.c_str(), response.size(), 0);
		MESSAGE("Closed connection", INFORMATION);
		close(connection);
	}
}
