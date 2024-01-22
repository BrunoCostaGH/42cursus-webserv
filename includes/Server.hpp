/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:51:53 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/18 18:21:14 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "Connection.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <limits>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>

class Request;
class Location;

class Connection;
class Server : public CommonDirectives
{
	std::vector<std::string> _serverNames;
	std::string _address;
	u_int16_t _listen;
	u_int32_t _clientMaxBodySize; // in bytes
	std::map<int, std::string> _errorPage;
	std::map<std::string, Location*> _locations;

	int32_t _socket;
	struct sockaddr_in _serverAddress;

	static std::map<std::string, int (Server::*)(const std::string&)> _methods;
	static void initializeMethods();
	int setAddress(const std::string& value);

public:
	Server();
	Server(const Server&);
	Server& operator=(const Server&);
	~Server();

	std::vector<std::string> getServerNames() const;
	bool isServerName(const std::string& name) const;
	std::string getAddress() const;
	u_int16_t getListenPort() const;
	u_int32_t getClientMaxBodySize() const;
	std::string getErrorPage(int error_code);
	Location* getLocation(std::string& path);
	Location* getParentLocation(std::string& path);

	int32_t getSocket() const;
	const sockaddr_in& getServerAddress() const;

	int setServerNames(const std::string& value);
	int setListen(const std::string& value);
	int setClientMaxBodySize(const std::string& value);
	int setErrorPage(const std::string& value);
	int setLocation(const std::string& dir, Location* value);
	int setDirective(const std::string& directive, const std::string& value);

	static std::string getFile(Request& request);
	static std::string deleteFile(Connection& connection);
	static std::string directoryListing(Connection& connection);
	std::string redirect(Connection& connection);

	int run();
	void stop();
};
