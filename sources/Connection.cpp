/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsilva-c <bsilva-c@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:09:13 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/20 16:51:49 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

Connection::Connection()
	: _connection_id(), _socket(), _timestamp(0), _server(NULL), _request(NULL)
{
}

Connection::Connection(int connection_id,
					   const std::string& client_address,
					   int socket)
	: _connection_id(connection_id),
	  _client_address(client_address),
	  _socket(socket),
	  _timestamp(0),
	  _server(NULL),
	  _request(NULL)
{
}

Connection::Connection(const Connection& value)
	: _connection_id(value._connection_id),
	  _client_address(value._client_address),
	  _socket(value._socket),
	  _timestamp(value._timestamp),
	  _server(value._server),
	  _request(new Request(*value._request))
{
}

Connection& Connection::operator=(const Connection& value)
{
	if (this == &value)
		return (*this);

	Connection tmp(value);

	std::swap(*this, tmp);
	return (*this);
}

Connection::~Connection()
{
	delete _request;
}

int Connection::getConnectionID() const
{
	return (this->_connection_id);
}

std::string Connection::getClientAddress() const
{
	return (this->_client_address);
}

int Connection::getSocket() const
{
	return (this->_socket);
}

time_t Connection::getTimestamp() const
{
	return (this->_timestamp);
}

Server* Connection::getServer() const
{
	return (this->_server);
}

Request* Connection::getRequest() const
{
	return (this->_request);
}

std::string Connection::getResponse() const
{
	return (this->_response);
}

void Connection::setTimestamp(time_t timestamp)
{
	this->_timestamp = timestamp;
}

void Connection::setServer(Server* server)
{
	this->_server = server;
}

Request* Connection::setRequest(Request* request)
{
	this->_request = request;
	return (this->_request);
}

void Connection::setResponse(const std::string& response)
{
	this->_response = response;
}
