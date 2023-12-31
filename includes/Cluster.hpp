/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cluster.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 12:41:18 by bsilva-c          #+#    #+#             */
/*   Updated: 2023/11/29 16:03:12 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Request.hpp"
#include "Cgi.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <fstream>

class Cgi;
class Cluster
{
	std::vector<Server*> _serverList;
	std::map<Server*, int>	_connections;
	fd_set _master_sockets, _read_sockets, _write_sockets;

public:
	Cluster();
	Cluster(const Cluster&);
	Cluster& operator=(const Cluster&);
	~Cluster();

	std::vector<Server*> getServerList() const;

	int configure(const std::string& file_path);
	void run();
	void acceptNewConnections(int connection);
	void readRequest(Server* server, int connection, std::string& response);
	void sendResponse(int connection, std::string& response);
	std::string checkRequestedOption(int selectedOptions, Request& request, Server* server);
};
