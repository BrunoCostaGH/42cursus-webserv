/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 20:15:28 by maricard          #+#    #+#             */
/*   Updated: 2024/01/17 21:16:44 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi() : _argv(), _envp()
{

}

Cgi::Cgi(Request& request) : _method(request.getMethod()),
							 _path(request.getPath()),
							 _executable(request.getExecutable()),
							 _query(request.getQuery()),
						   	 _uploadStore(request.getUploadStore()),
							 _argv(),
							 _envp(),
							 _header(request.getHeader()),
						   	 _body(request.getBody())
{
	for (unsigned i = 0; i < 3; ++i)
        _argv[i] = NULL;

    for (unsigned i = 0; i < 17; ++i)
        _envp[i] = NULL;

}

Cgi::Cgi(const Cgi& copy) : _argv(), _envp()
{
	*this = copy;
}

Cgi&	Cgi::operator=(const Cgi& other)
{
	if (this == &other)
		return *this;

	_method = other._method;
	_path = other._path;
	_query = other._query;
	_uploadStore = other._uploadStore;
	_header = other._header;
	_body = other._body;
	return *this;
}

Cgi::~Cgi()
{
	for (unsigned i = 0; i < 3; ++i)
        delete[] _argv[i];

    for (unsigned i = 0; i < 17; ++i)
        delete[] _envp[i];
}

std::string	Cgi::runCGI(Connection& connection)
{
	std::string filename = ".tmp";
	int 		file_fd;
	FILE*		file = NULL;

	if ((file_fd = sendDataToCgi(filename, file)) == -1)
		return Response::buildErrorResponse(connection, 500);

	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		return Response::buildErrorResponse(connection, 500);

	std::string response = setArgv(connection);
	if (!(response.empty()))
		return response;
	
	setEnvp();
	int status;
	int pid = fork();

	if (pid == 0)
	{
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);

    	dup2(pipe_fd[WRITE], STDOUT_FILENO);
    	close(pipe_fd[WRITE]);
		close(pipe_fd[READ]);

		execve(_argv[0], _argv, _envp);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(file_fd);
		close(pipe_fd[WRITE]);
		waitpid(pid, &status, 0);

		if (!WIFEXITED(status))
			return Response::buildErrorResponse(connection, 500);
	}
	
	if (std::remove(filename.c_str()) != 0)
		return Response::buildErrorResponse(connection, 500);

	response = readDataFromCgi(connection, pipe_fd[READ]);

	close(pipe_fd[READ]);
	std::fclose(file);

	return (response);
}

int Cgi::sendDataToCgi(const std::string& filename, FILE*& file)
{
	{
		std::ofstream tmp_file(filename.c_str());
		if (tmp_file.is_open())
		{
			for (unsigned i = 0; i < _body.size(); i++)
				tmp_file <<  _body[i];
			tmp_file.close();
		}
		else
			return -1;
	}

    file = std::fopen(filename.c_str(), "r");
	if (file != NULL)
        return (fileno(file));
	else
		return -1;
}

std::string Cgi::readDataFromCgi(Connection& connection, int fd)
{
	char* buffer = new char[4096];
	
	for (unsigned i = 0; i < 4096; i++)
		buffer[i] = '\0';

	if (read(fd, buffer, 4096) <= 0)
	{
		delete [] buffer;
		return Response::buildErrorResponse(connection, 500);
	}
	
	std::string response = buffer;
	std::map<std::string, std::string> header;
	std::vector<char> body;
	
	if (response.find("Status") == std::string::npos)
		header["HTTP/1.1"] = "200 OK";
	else
	{
		std::string status_code = response.substr(8, 3);
		std::string status_message = response.substr(12, response.find(CRLF) - 12);
		header["HTTP/1.1"] = status_code + " " + status_message;
	}

	if (response.find("Content-Type: ") == std::string::npos)
		header["Content-Type"] = "text/html";
	else
	{
		size_t pos = response.find("Content-Type: ");
		std::string content_type = response.substr(pos + 14, response.find(CRLF, pos) - pos - 14);
		header["Content-Type"] = content_type;
	}

	size_t pos = response.find("\r\n\r\n") + 4;
	for (unsigned i = pos; i < response.length(); i++)
		body.push_back(response[i]);

	delete [] buffer;
	return (Response::buildResponse(header, ".html", body));
}	

std::string	Cgi::setArgv(Connection& connection)
{
	if (_executable.empty())
		return Response::buildErrorResponse(connection, 500);
	_argv[0] = myStrdup(_executable.c_str());
	_argv[1] = myStrdup(_path.c_str());
	_argv[2] = NULL;

	return "";
}

void	Cgi::setEnvp()
{
	int i = 0;

	if (!_path.empty())
	{
		std::string str = "PATH_INFO=" + _path;
		_envp[i++] = myStrdup(str.c_str());
	}
	if (!_uploadStore.empty() && i < 17)
	{
		std::string str = "UPLOAD_STORE=" + _uploadStore;
		_envp[i++] = myStrdup(str.c_str());
	}
	else
	{
		std::string str = "UPLOAD_STORE=uploads";
		_envp[i++] = myStrdup(str.c_str());
	}
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
	else if (!_body.empty())
	{
		std::stringstream ss;
		std::string number;
		ss << _body.size();
		ss >> number;
		std::string str = "CONTENT_LENGTH=" + number;
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

char*	Cgi::myStrdup(const char* source)
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


