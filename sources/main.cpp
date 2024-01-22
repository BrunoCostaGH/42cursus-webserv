/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:34:23 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/17 21:03:39 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cluster.hpp"

int main(int argc, char** argv)
{
	if (argc > 2)
	{
		std::cout << "Usage:\n\t./webserv [configuration_file]" << std::endl;
		return (1);
	}
	std::string config_path;
	Cluster cluster;

	if (argc == 2)
		config_path = argv[1];
	if (cluster.configure(config_path))
		return (1);
	cluster.boot();
	cluster.run();
	return (0);
}
