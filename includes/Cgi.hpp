/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/30 16:15:56 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include "Connection.hpp"
#include <unistd.h>
#include <fstream>
#include <iostream>

#define READ 0
#define WRITE 1

class Request;
class Connection;
class Cgi
{
	private:
		std::string _method;
		std::string _path;
		std::string _executable;
		std::string _query;
		std::string	_uploadStore;
		char*		_argv[3];
		char*		_envp[17];
		std::map<std::string, std::string> 	_header;
		std::vector<char> 	_body;

		Cgi();
		Cgi(const Cgi&);
		Cgi& operator=(const Cgi&);

	public:
		Cgi(Request& request);
		~Cgi();

		std::string	runCGI(Connection& connection);
		static std::string readDataFromCgi(Connection& connection, int fd);
		int 		sendDataToCgi(const std::string& filename, FILE*& file);
		std::string	setArgv(Connection& connection);
		void		setEnvp();
		static char*		myStrdup(const char* source);
};
