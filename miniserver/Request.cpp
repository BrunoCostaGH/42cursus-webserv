/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 17:14:44 by maricard          #+#    #+#             */
/*   Updated: 2023/10/27 17:43:20 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{

}

Request::Request(char* buffer, int max_body_size) : 
	_buffer(buffer), 
	_bodyLength(-1), 
	_maxBodySize(max_body_size)
{

}

Request::Request(const Request& copy)
{
	*this = copy;
}

Request::~Request()
{
	_header.clear();
	_body.clear();
}

Request& Request::operator=(const Request& other)
{
	if (this == &other)
		return *this;

	_method = other._method;
	_path = other._path;
	_query = other._query;
	_protocol = other._protocol;
	_header = other._header;
	_body = other._body;
	//_argv = other._argv;
	//_envp = other._envp;
	_bodyLength = other._bodyLength;
	_maxBodySize = other._maxBodySize;
	return *this;
}

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getPath() const
{
	return _path;
}

std::string Request::getQuery() const
{
	return _query;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

int	Request::handleRequest(char* header_buffer, int bytesRead)
{
	int bytesToRead = parseRequest(header_buffer, bytesRead);
	
	return bytesToRead;
}

void	Request::handleBody(char* body_buffer, int bytesRead)
{
	for (int i = 0; i < bytesRead; i++)
		_body.push_back(body_buffer[i]);

	if (_body.size() > _maxBodySize)
	{
		//MESSAGE("413 ENTITY TO LARGE", ERROR);
		return ;
	}
}

int	Request::parseRequest(char* header_buffer, int bytesRead)
{
	(void)bytesRead;
	std::string request = header_buffer;
	std::stringstream ss(request);
	std::string line;

	ss >> _method >> _path >> _protocol;

	if (_path.find("?") != std::string::npos)
	{
		_query = _path.substr(_path.find("?") + 1, _path.length());
		_path = _path.substr(0, _path.find("?"));
	}

	std::getline(ss, line);
	while (std::getline(ss, line) && line != "\r")
	{
    	size_t pos = line.find(':');
    
		if (pos != std::string::npos)
		{
			std::string first = line.substr(0, pos);
			std::string second = line.substr(pos + 2, line.length());
			_header[first] = second;
    	}
    }

	if (line != "\r")
	{
		MESSAGE("413 ENTITY TO LARGE", ERROR);
		return -1;
	}

	if (checkErrors() == true)
		return -1;

	size_t pos = request.find("\r\n\r\n") + 4;
	unsigned int k = 0;

	for(int i = pos; i < bytesRead; i++)
	{
		_body.push_back(header_buffer[i]);
		k++;
	}

	if (k < _bodyLength)
		return _bodyLength - k;
	
	return 0;
}

//! verify config max body!
void	Request::runCGI()
{
	setArgv();
	setEnvp();

	std::string filename = ".tmp";

	{
		std::ofstream file(filename.c_str());
		if (file.is_open())
		{
			for (unsigned i = 0; i < _body.size(); i++)
				file <<  _body[i];
			file.close();
		}
		else
		{
			MESSAGE("CREATE FILE ERROR", ERROR)
			return ;
		}
	}

    FILE*	file = std::fopen(filename.c_str(), "r");
	int file_fd = -1;
	
	if (file != NULL)
        file_fd = fileno(file);
	else
	{
		MESSAGE("file fd", ERROR)
		return ;
	}

	int pipe_write[2];
	if (pipe(pipe_write) == -1)
	{
		MESSAGE("PIPE ERROR", ERROR);
		return ;
	}
	
	int pid = fork();
	if (pid == 0)
	{
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);

    	dup2(pipe_write[WRITE], STDOUT_FILENO);
    	close(pipe_write[WRITE]);
		close(pipe_write[READ]);

		execve(_argv[0], _argv, _envp);
		MESSAGE("EXECVE ERROR", ERROR);
		exit(0);
	}
	else
	{
		close(file_fd);
		close(pipe_write[WRITE]);
		waitpid(pid, NULL, 0);
	}

	char buffer[4096] = "\0";

	if (read(pipe_write[READ], buffer, 4096) <= 0)
	{
		MESSAGE("CGI READ ERROR", ERROR);
		return ;
	}

	std::cout << F_RED "OUTPUT: " RESET 
			  << std::endl 
			  << buffer
			  << std::endl;

	if (std::remove(filename.c_str()) != 0)
	{
       MESSAGE("REMOVE FILE ERROR", ERROR)
	   return ;
    }

	close(pipe_write[READ]);
	std::fclose(file);
	
	deleteMemory();
}

void	Request::setArgv()
{
	_argv[0] = myStrdup("/usr/bin/python3");
	_argv[1] = myStrdup("cgi-bin/cgi_post.py");
	_argv[2] = NULL;
}

void	Request::setEnvp()
{
	int i = 0;

	if (!(_method.empty()) && i < 17)
	{
		std::string str = "REQUEST_METHOD=" + _method;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_query.empty()) && i < 17)
	{
		std::string str = "QUERY_STRING=" + _query;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_header["Content-Length"].empty()) && i < 17)
	{
		std::string str = "CONTENT_LENGTH=" + _header["Content-Length"];
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!(_header["Content-Type"].empty()) && i < 17)
	{
		std::string str = "CONTENT_TYPE=" + _header["Content-Type"];
		_envp[i++] = myStrdup(str.c_str());
	}

	for (; i < 17; i++)
		_envp[i] = NULL;
}

bool Request::hasCGI()
{
	//! temporary solution
	if (getMethod() == "GET" && getPath() == "/getDateTime")
		return true;
	else if (getMethod() == "POST" && getPath() == "/uploadFile")
		return true;
	return false;
}

bool	Request::checkErrors()
{
	if (_method == "POST" && (_header["Content-Type"].empty() ||
		_header["Content-Type"].find("multipart/form-data") == std::string::npos))
	{
		MESSAGE("415 POST request without Content-Type", ERROR);
		return true;
	}
	if (_method == "POST" && _header["Content-Length"].empty())
	{
		MESSAGE("411 POST request without Content-Length", ERROR);
		return true;
	}
	else
	{
		std::istringstream ss(_header["Content-Length"]);
		ss >> _bodyLength;
	}

	return false;
}

void	Request::deleteMemory()
{
	for (unsigned i = 0; i < 3; ++i)
	{
        if (_argv[i] != NULL)
            delete[] _argv[i];
    }

    for (unsigned i = 0; i < 17; ++i)
	{
        if (_envp[i] != NULL)
            delete[] _envp[i];
    }
}

char*	Request::myStrdup(const char* source)
{
    size_t length = 0;
	
	for (; source[length]; length++)
		;

    char* duplicate = new char[length + 1];

    for (size_t i = 0; i < length; ++i)
        duplicate[i] = source[i];

    duplicate[length] = '\0';

    return duplicate;
}

void	Request::displayVars()
{
	std::cout << F_YELLOW "Protocol: " RESET + _protocol << std::endl;
	std::cout << F_YELLOW "Method: " RESET + _method << std::endl;
	std::cout << F_YELLOW "Path: " RESET + _path << std::endl;
	std::cout << F_YELLOW "Query: " RESET + _query << std::endl;

	if (_header.size() > 0)
		std::cout << F_YELLOW "Header" RESET << std::endl;
	
	std::map<std::string, std::string>::iterator it = _header.begin();
	for (; it != _header.end(); it++)
		std::cout << it->first + ": " << it->second << std::endl;

	if (_body.size() > 0)
		std::cout << F_YELLOW "Body: " RESET << _body.size() << std::endl;
}
