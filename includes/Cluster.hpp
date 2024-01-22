/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/18 15:29:14 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include "Connection.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <fstream>

class Cgi;
class Connection;
class Cluster
{
	int _connection_id;
	std::vector<Server*> _serverList;
	std::map<int, Connection*> _connections;
	fd_set _master_sockets, _read_sockets, _write_sockets;

public:
	Cluster();
	Cluster(const Cluster&);
	Cluster& operator=(const Cluster&);
	~Cluster();

	int getNextConnectionID();
	std::vector<Server*> getServerList() const;

	int configure(const std::string& file_path);
	void boot();
	void run();
	void acceptNewConnections();
	void closeConnection(int socket);
	void readRequest(Connection& connection);
	void sendResponse(Connection& connection);
	static std::string checkRequestedOption(int selectedOptions,
									 Connection& connection);
};
