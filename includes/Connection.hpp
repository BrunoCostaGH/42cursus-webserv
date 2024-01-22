/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:09:49 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/20 16:30:05 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Server;
class Request;
class Response;
class Connection
{
	const int _connection_id;
	const std::string _client_address;
	const int _socket;
	time_t _timestamp;
	Server* _server;
	Request* _request;
	std::string _response;

	Connection();

public:
	Connection(int connection_id,
			   const std::string& client_address,
			   int socket);
	Connection(const Connection&);
	Connection& operator=(const Connection&);
	~Connection();

	int getConnectionID() const;
	std::string getClientAddress() const;
	int getSocket() const;
	time_t getTimestamp() const;
	Server* getServer() const;
	Request* getRequest() const;
	std::string getResponse() const;

	void setTimestamp(time_t timestamp);
	void setServer(Server* server);
	Request* setRequest(Request* request);
	void setResponse(const std::string& response);
};
