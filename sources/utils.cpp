/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 20:09:21 by maricard          #+#    #+#             */
/*   Updated: 2024/01/30 16:15:56 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string getFileExtension(std::string& path)
{
	uint64_t dotIndex = path.find_last_of('.');
	
	if (dotIndex != std::string::npos)
		return path.substr(dotIndex, path.length() - dotIndex);
	return "";
}


uint32_t	getHexSize(const std::vector<char>& body, unsigned pos)
{
	uint32_t k = 0;
	
	while (pos < body.size())
	{
		if (body[pos] == '\r' && body[pos + 1] && body[pos + 1] == '\n')
			break;
		pos++;
		k++;
	}

	return k;
}

uint32_t	getHexFromChunked(const std::vector<char>& body, unsigned pos)
{
	std::string hex;

	while (pos < body.size())
	{
		if (body[pos] == '\r' && body[pos + 1] && body[pos + 1] == '\n')
			break;
		hex += body[pos];
		pos++;
	}

	std::stringstream ss(hex);
	uint32_t value = 0;

	ss >> std::hex >> value;

	return value;
}

bool	searchChunkedRequestEnd(const std::vector<char>& body)
{
	std::string hex;
	size_t pos = 0;

	while (pos < body.size())
	{
		if (body[pos] == '0' &&
			body[pos + 1] && body[pos + 1] == '\r' &&
			body[pos + 2] && body[pos + 2] == '\n' &&
			body[pos + 3] && body[pos + 3] == '\r' &&
			body[pos + 4] && body[pos + 4] == '\n')
			return true;
		pos++;
	}

	return false;
}
