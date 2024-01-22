/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/18 18:04:38 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "macros.hpp"
#include "Server.hpp"
#include "Connection.hpp"

class Server;
class Connection;
class Response
{
	std::map<std::string, std::string> _header;
	std::vector<char> _body;
	static std::map<std::string, std::string> _errorStatus;
	static std::map<std::string, std::string> _redirStatus;
	static std::map<std::string, std::string> _contentType;

public:
	Response();
	Response(const Response&);
	Response& operator=(const Response&);
	~Response();

	static std::string buildResponse(std::map<std::string, std::string>& header,
									 std::string extension,
									 std::vector<char>& body);
	static std::string buildErrorResponse(Connection& connection,
										  int errorCode);
	static std::string buildRedirectResponse(Connection& connection,
											 const std::pair<std::string,
											 std::string>& redirect);
	static void initializeErrorStatus();
	static void initializeRedirStatus();
	static void initializeContentType();
};
