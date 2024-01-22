/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:04 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/20 19:04:55 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"
#include <fstream>

Cluster::Cluster()
	: _connection_id(0), _master_sockets(), _read_sockets(), _write_sockets()
{
}

Cluster::Cluster(const Cluster& value)
	: _connection_id(0),
	  _serverList(value._serverList),
	  _master_sockets(),
	  _read_sockets(),
	  _write_sockets()
{
	*this = value;
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
	for (std::vector<Server*>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		delete *it;
		*it = 0;
	}
	for (std::map<int, Connection*>::iterator it = this->_connections.begin();
		 it != this->_connections.end(); ++it)
	{
		delete (it->second);
		it->second = NULL;
	}
}

int Cluster::getNextConnectionID()
{
	return (this->_connection_id++);
}

std::vector<Server*> Cluster::getServerList() const
{
	return (this->_serverList);
}

static int openFile(const std::string& file_path, std::fstream* fstream)
{
	if (fstream->is_open())
		fstream->close();
	
	fstream->open(file_path.c_str(), std::ios::in);
	if (!fstream->is_open())
	{
		MESSAGE(file_path + ": " + strerror(errno), ERROR)
		fstream->clear();
		return (1);
	}
	return (0);
}

static int getLocationConfig(Location* location, std::fstream* fstream)
{
	std::string line;

	while (getline(*fstream, line))
	{
		std::stringstream ss(line);
		std::string directive;
		std::string value;

		ss >> directive;
		if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
			continue;
		if (line.find('}') != std::string::npos)
			break;
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR)
			return (1);
		}
		getline(ss, value, ';');
		if ((directive == "cgi_pass" && location->getPath().at(0) == '/') ||
			(directive != "cgi_pass" && location->getPath().at(0) == '.'))
		{
			MESSAGE("Unable to configure directive `" + directive +
					"' in specified location block", ERROR)
			return (1);
		}
		if (location->setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'", ERROR)
			return (1);
		}
	}
	return (0);
}

static int getServerConfig(std::vector<Server*>* serverList, std::fstream* fstream)
{
	Server server;
	std::string line;

	while (getline(*fstream, line))
	{
		std::stringstream ss;
		std::string directive;
		std::string value;

		if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
			continue;

		ss << line;
		ss >> directive;
		if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
			continue;
		if (directive == "location")
		{
			if (line.find('}') != std::string::npos)
				continue;
			std::string path;
			ss >> path;
			if (path.empty() || (path.at(0) == '.' && path.length() == 1) ||
				(path.at(0) != '.' && path.find('/')) ||
				(path.at(0) != '/' && path.find('.')) ||
				path.find("//") != std::string::npos) // check if is path
			{
				MESSAGE(path + ": Invalid location path", ERROR)
				return (1);
			}
			if (path.length() > 1 && *(path.end() - 1) == '/')
				path.erase(path.length() - 1);
			ss >> value;
			if (!value.empty() && value != "{")
			{
				MESSAGE(value + ": Unexpected value", ERROR)
				return (1);
			}
			else if (value.empty())
			{
				while (getline(*fstream, line))
				{
					if (directive.empty() || directive.at(0) == ';' || directive.at(0) == '#')
						continue;
					if (line.find('{') != std::string::npos)
						break;
					if (line.find_first_not_of(" \t") != std::string::npos)
					{
						MESSAGE("Expected `{' on location block declaration", ERROR)
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
		if (line.find('}') != std::string::npos)
			break;
		if (line.find_first_of(';') == std::string::npos)
		{
			MESSAGE("expected `;' at end of line", ERROR)
			return (1);
		}
		getline(ss, value, ';');
		if (server.setDirective(directive, value))
		{
			MESSAGE("Unable to configure directive `" + directive + "'", ERROR)
			return (1);
		}
	}
	serverList->push_back(new Server(server));
	return (0);
}

int Cluster::configure(const std::string& path)
{
	std::fstream fstream;
	if (path.empty() || openFile(path, &fstream))
	{
		MESSAGE("No valid configuration file, using default configuration", WARNING)
		this->_serverList.push_back(new Server());
		return (0);
	}

	std::string line;
	while (getline(fstream, line))
	{
		std::stringstream ss(line);
		std::string block_type;

		ss >> block_type;
		if (block_type.empty() || block_type.at(0) == ';' || block_type.at(0) == '#')
			continue;
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
					if (!bracket.empty())
						bracket.clear();
					std::stringstream m(line);
					m >> bracket;
					if (bracket.empty())
					{
						MESSAGE("Expected `{' at the end of server block declaration",
								ERROR)
						return (1);
					}
					if (bracket.at(0) == ';' || bracket.at(0) == '#')
						continue;
					if (line.find('}') != std::string::npos)
						break;
					if (!bracket.empty() && bracket.at(0) != '{')
					{
						MESSAGE("Expected `{' on server block declaration",
								ERROR)
						return (1);
					}
					else if (!bracket.empty()) // if found bracket
						break;
				}
			}
			else if (!bracket.empty() && bracket != "{")
			{
				MESSAGE(bracket + ": Unexpected value", ERROR)
				return (1);
			}
			if (line.find('}') == std::string::npos)
			{
				if (getServerConfig(&this->_serverList, &fstream))
					return (1);
			}
		}
		else if (!block_type.empty())
		{
			MESSAGE(block_type + ": Unexpected block", ERROR)
			return (1);
		}
	}
	return (0);
}

void Cluster::closeConnection(int socket)
{
	FD_CLR(socket, &_master_sockets);
	close(socket);

	LOG(_connections[socket]->getConnectionID(),
		"Connection closed gracefully",
		INFORMATION)
	delete _connections[socket];
	_connections.erase(socket);

}

static bool isAnyServerRunning(fd_set& set)
{
	for (int i = 0; i < FD_SETSIZE; ++i)
	{
		if (FD_ISSET(i, &set))
			return (true);
	}
	return (false);
}

void Cluster::boot()
{
	FD_ZERO(&_master_sockets);

	for (std::vector<Server*>::iterator it = this->_serverList.begin();
		 it != this->_serverList.end(); ++it)
	{
		std::stringstream port;
		port << (*it)->getListenPort();
		if ((*it)->run())
		{
			(*it)->stop();
			continue;
		}
		MESSAGE("Listening on " + (*it)->getAddress() + ":" + port.str(), INFORMATION)
		FD_SET((*it)->getSocket(), &_master_sockets);
	}
}

static void checkConnections(Cluster& cluster,
							 std::map<int, Connection*>& connections)
{
	std::map<int, Connection*>::iterator it = connections.begin();
	for (; it != connections.end(); it++)
	{
		if (std::time(0) >= ((*it).second->getTimestamp() + 60))
		{
			cluster.closeConnection((*it).second->getSocket());
			checkConnections(cluster, connections);
			return;
		}
	}
}

void Cluster::run()
{
	if (!isAnyServerRunning(_master_sockets))
	{
		MESSAGE("No servers were created", ERROR)
		return;
	}

	while (true)
	{
		checkConnections(*this, _connections);
		_read_sockets = _master_sockets;
		_write_sockets = _master_sockets;
		std::string response;

		int selectResult = select(FD_SETSIZE, &_read_sockets, &_write_sockets, NULL, NULL);
		if (selectResult < 0)
		{
			std::stringstream ss;
			ss << errno;
			MESSAGE("select(): " + ss.str() + ": " + (std::string)strerror(errno), ERROR)
			continue;
		}
		else if (selectResult == 0)
			continue;

		acceptNewConnections();

		for (std::map<int, Connection*>::iterator it = _connections.begin();
			 it != _connections.end(); it++)
		{
			if (FD_ISSET((*it).first, &this->_read_sockets))
				readRequest(*it->second);
			if (FD_ISSET((*it).first, &this->_write_sockets))
				sendResponse(*it->second);
		}
	}
}

static std::string in_addr_t_to_ip(in_addr_t addr)
{
	std::ostringstream oss;

	for (int i = 0; i < 4; ++i)
	{
		in_addr_t octet = (addr >> (i * 8)) & 0xFF;
		oss << octet;

		if (i < 3)
		{
			oss << ".";
		}
	}
	return oss.str();
}

void Cluster::acceptNewConnections()
{
	std::vector<Server*>::iterator it = this->_serverList.begin();
	struct sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);

	for (; it != this->_serverList.end(); ++it)
	{
		if (!(*it)->getSocket())
			continue;

		if (FD_ISSET((*it)->getSocket(), &this->_read_sockets))
		{
			int socket;
			if ((socket = accept((*it)->getSocket(),
								 (struct sockaddr*)&clientAddress,
								 (socklen_t*)&clientAddressLength)) < 0)
			{
				std::stringstream ss;
				ss << errno;
				MESSAGE("accept(): " + ss.str() + ": " +
						(std::string)strerror(errno), ERROR)
				continue;
			}

			std::string client_address = in_addr_t_to_ip(clientAddress.sin_addr.s_addr);
			std::stringstream ss;
			ss << socket;

			FD_SET(socket, &this->_master_sockets);
			_connections[socket] = new Connection(getNextConnectionID(), client_address, socket);
			_connections[socket]->setServer(*it);
			LOG(_connections[socket]->getConnectionID(),
				"Connected with client at " +
				_connections[socket]->getClientAddress(),
				INFORMATION)
			_connections[socket]->setTimestamp(std::time(0));
		}
	}
}

void Cluster::readRequest(Connection& connection)
{
	Request* request = connection.getRequest();
	if (!request)
		request = connection.setRequest(new Request());

	int64_t bytesRead;
	int64_t bytesLeftToRead = 4096;
	char buffer[bytesLeftToRead];
	int error = 0;

	for (size_t i = 0; i < sizeof(buffer); ++i)
		buffer[i] = '\0';

	if ((bytesRead = recv(connection.getSocket(), buffer, bytesLeftToRead, 0)) > 0)
	{
		connection.setTimestamp(std::time(0));

		error = request->parseRequest(*this, connection, buffer, bytesRead);
		/*
		 * Continue reading from the socket, if there is content
		 * left to read beyond the specified Content-Length.
		 */
		int bytesToRead = 4096;
		char body_buffer[bytesToRead];
		while ((bytesRead = recv(connection.getSocket(), body_buffer, bytesToRead, 0)) != -1 &&
			bytesRead != 0);

		request->displayVars();

		int selectedOptions = request->isValidRequest(*connection.getServer(), error);

		if (!error)
			connection.setResponse(checkRequestedOption(selectedOptions, connection));
	}
	else
	{
		closeConnection(connection.getSocket());
		Cluster::run();
		return;
	}

	if (error)
		connection.setResponse(Response::buildErrorResponse(connection, error));
}

std::string Cluster::checkRequestedOption(int selectedOptions,
										  Connection& connection)
{
	Server* server = connection.getServer();
	Request& request = *connection.getRequest();

	if (selectedOptions & REDIR)
		return server->redirect(connection);
	else if (selectedOptions & DIR_LIST)
		return Server::directoryListing(connection);
	else if (selectedOptions & CGI)
	{
		Cgi cgi(request);
		return cgi.runCGI(connection);
	}
	else if (selectedOptions & DELETE)
		return Server::deleteFile(connection);
	else if (selectedOptions & GET)
		return Server::getFile(request);
	return Response::buildErrorResponse(connection, 500);
}

void Cluster::sendResponse(Connection& connection)
{
	std::string response = connection.getResponse();
	if (response.empty())
		return;

	if (send(connection.getSocket(), response.c_str(), response.size(), 0) < 0)
	LOG(connection.getConnectionID(),
		"Sending response to socket",
		ERROR)

	std::string status_code = response.substr(9, 3);
	std::string status_message = response.substr(12, response.find(CRLF) - 12);

	std::stringstream ss(status_code);
	int status;
	ss >> status;

	if (status < 400)
		LOG(connection.getConnectionID(),
			connection.getRequest()->getRequestLine() + " - " + status_code + status_message,
			SUCCESS)
	else
		LOG(connection.getConnectionID(),
			connection.getRequest()->getRequestLine() + " - " + status_code + status_message,
			ERROR)

	int hasConnectionField = !connection.getRequest()->getHeaderField("Connection").empty();
	if (hasConnectionField &&
		connection.getRequest()->getHeaderField("Connection") != "keep-alive")
	{
		closeConnection(connection.getSocket());
		Cluster::run();
	}
	else
	{
		delete connection.getRequest();
		connection.setRequest(NULL);
		connection.setResponse(std::string());
	}
}
