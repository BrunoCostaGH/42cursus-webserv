/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:58:21 by maricard          #+#    #+#             */
/*   Updated: 2024/01/18 17:06:51 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include "macros.hpp"
#include "Server.hpp"
#include "Response.hpp"
#include "Cluster.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstdio>
#include "utils.hpp"
#include "Connection.hpp"

class Cluster;
class Server;

class Connection;
class Request
{
	private:
		std::string _method;
		std::string _uri;
		std::string _protocol;
		std::string _path;
		std::string _executable;
		std::string _query;
		std::map<std::string, std::string> _header;
		std::vector<char> _body;
		u_int32_t 	_bodyLength;
		std::string _uploadStore;

		Request& operator=(const Request& other);
	
	public:
		Request();
		Request(const Request& copy);
		~Request();

		std::string getRequestLine() const;
		std::string getMethod() const;
		std::string getPath() const;
		std::string getQuery() const;
		std::map<std::string, std::string> getHeader() const;
		std::vector<char>	getBody() const;
		std::string getUploadStore() const;
		std::string getExtension();
		std::string getExecutable() const;
		std::string getHeaderField(const std::string& field);

		int		parseRequest(Cluster& cluster,
							 Connection& connection,
							 char* buffer,
							 int64_t bytesAlreadyRead);
		int		parseBody(int socket, char* chunk, int64_t bytesToRead);
		int		parseChunkedRequest(int socket, char* buffer, int64_t bytesToRead);
		int		checkErrors(Connection& connection);
		void	displayVars();
		int		isValidRequest(Server& server, int& error);
};
