/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 19:09:05 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/18 19:33:59 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<std::string, std::string> Response::_errorStatus;
std::map<std::string, std::string> Response::_redirStatus;
std::map<std::string, std::string> Response::_contentType;

Response::Response()
{
}

Response::Response(const Response&)
{
}

Response& Response::operator=(const Response&)
{
	return (*this);
}

Response::~Response()
{
}

void Response::initializeErrorStatus()
{
	if (!_errorStatus.empty())
		return;
	Response::_errorStatus["400"] = "Bad Request";
	Response::_errorStatus["401"] = "Unauthorized";
	Response::_errorStatus["402"] = "Payment Required";
	Response::_errorStatus["403"] = "Forbidden";
	Response::_errorStatus["404"] = "Not Found";
	Response::_errorStatus["405"] = "Method Not Allowed";
	Response::_errorStatus["406"] = "Not Acceptable";
	Response::_errorStatus["407"] = "Proxy Authentication Required";
	Response::_errorStatus["408"] = "Request Timeout";
	Response::_errorStatus["409"] = "Conflict";
	Response::_errorStatus["410"] = "Gone";
	Response::_errorStatus["411"] = "Length Required";
	Response::_errorStatus["412"] = "Precondition Failed";
	Response::_errorStatus["413"] = "Request Entity Too Large";
	Response::_errorStatus["414"] = "Request-URI ";
	Response::_errorStatus["415"] = "Unsupported Media Type";
	Response::_errorStatus["416"] = "Range Not Satisfiable";
	Response::_errorStatus["417"] = "Expectation Failed";
	Response::_errorStatus["418"] = "I'm a teapot";
	Response::_errorStatus["421"] = "Misdirected Request";
	Response::_errorStatus["422"] = "Unprocessable Entity";
	Response::_errorStatus["423"] = "Locked";
	Response::_errorStatus["424"] = "Failed Dependency";
	Response::_errorStatus["425"] = "Too Early";
	Response::_errorStatus["426"] = "Upgrade Required";
	Response::_errorStatus["428"] = "Precondition Required";
	Response::_errorStatus["429"] = "Too Many Requests";
	Response::_errorStatus["431"] = "Request Header Fields Too Large";
	Response::_errorStatus["451"] = "Unavailable For Legal Reasons";
	Response::_errorStatus["500"] = "Internal Server Error";
	Response::_errorStatus["501"] = "Not Implemented";
	Response::_errorStatus["502"] = "Bad Gateway";
	Response::_errorStatus["503"] = "Service Unavailable";
	Response::_errorStatus["504"] = "Gateway Timeout";
	Response::_errorStatus["505"] = "HTTP Version Not Supported";
	Response::_errorStatus["506"] = "Variant Also Negotiates";
	Response::_errorStatus["507"] = "Insufficient Storage";
	Response::_errorStatus["508"] = "Loop Detected";
	Response::_errorStatus["510"] = "Not Extended";
	Response::_errorStatus["511"] = "Network Authentication Required";
}

void Response::initializeRedirStatus()
{
	if (!_redirStatus.empty())
		return;
	Response::_redirStatus["300"] = "Multiple Choices";
	Response::_redirStatus["301"] = "Moved Permanently";
	Response::_redirStatus["302"] = "Found";
	Response::_redirStatus["303"] = "See Other";
	Response::_redirStatus["304"] = "Not Modified";
	Response::_redirStatus["305"] = "Use Proxy";
	Response::_redirStatus["307"] = "Temporary Redirect";
}

void Response::initializeContentType()
{
	if (!_contentType.empty())
		return;
	Response::_contentType[".aac"] = "audio/aac";
	Response::_contentType[".abw"] = "application/x-abiword";
	Response::_contentType[".arc"] = "application/x-freearc";
	Response::_contentType[".avif"] = "image/avif";
	Response::_contentType[".avi"] = "video/x-msvideo";
	Response::_contentType[".azw"] = "application/vnd.amazon.ebook";
	Response::_contentType[".bin"] = "application/octet-stream";
	Response::_contentType[".bmp"] = "image/bmp";
	Response::_contentType[".bz"] = "application/x-bzip";
	Response::_contentType[".bz2"] = "application/x-bzip2";
	Response::_contentType[".cda"] = "application/x-cdf";
	Response::_contentType[".csh"] = "application/x-csh";
	Response::_contentType[".css"] = "text/css";
	Response::_contentType[".csv"] = "text/csv";
	Response::_contentType[".doc"] = "application/msword";
	Response::_contentType[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	Response::_contentType[".eot"] = "application/vnd.ms-fontobject";
	Response::_contentType[".epub"] = "application/epub+zip";
	Response::_contentType[".gz"] = "application/gzip";
	Response::_contentType[".gif"] = "image/gif";
	Response::_contentType[".htm"] = "text/html";
	Response::_contentType[".html"] = "text/html";
	Response::_contentType[".ico"] = "image/vnd.microsoft.icon";
	Response::_contentType[".ics"] = "text/calendar";
	Response::_contentType[".jar"] = "application/java-archive";
	Response::_contentType[".jpeg"] = "image/jpeg";
	Response::_contentType[".jpg"] = "image/jpeg";
	Response::_contentType[".js"] = "text/javascript";
	Response::_contentType[".json"] = "application/json";
	Response::_contentType[".jsonld"] = "application/ld+json";
	Response::_contentType[".mid"] = "audio/midi";
	Response::_contentType[".midi"] = "audio/midi";
	Response::_contentType[".mjs"] = "text/javascript";
	Response::_contentType[".mp3"] = "audio/mpeg";
	Response::_contentType[".mp4"] = "video/mp4";
	Response::_contentType[".mpeg"] = "video/mpeg";
	Response::_contentType[".mpkg"] = "application/vnd.apple.installer+xml";
	Response::_contentType[".odp"] = "application/vnd.oasis.opendocument.presentation";
	Response::_contentType[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	Response::_contentType[".odt"] = "application/vnd.oasis.opendocument.text";
	Response::_contentType[".oga"] = "audio/ogg";
	Response::_contentType[".ogv"] = "video/ogg";
	Response::_contentType[".ogx"] = "application/ogg";
	Response::_contentType[".opus"] = "audio/opus";
	Response::_contentType[".otf"] = "font/otf";
	Response::_contentType[".png"] = "image/png";
	Response::_contentType[".pdf"] = "application/pdf";
	Response::_contentType[".php"] = "application/x-httpd-php";
	Response::_contentType[".ppt"] = "application/vnd.ms-powerpoint";
	Response::_contentType[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	Response::_contentType[".rar"] = "application/vnd.rar";
	Response::_contentType[".rtf"] = "application/rtf";
	Response::_contentType[".sh"] = "application/x-sh";
	Response::_contentType[".svg"] = "image/svg+xml";
	Response::_contentType[".tar"] = "application/x-tar";
	Response::_contentType[".tif"] = "image/tiff";
	Response::_contentType[".tiff"] = "image/tiff";
	Response::_contentType[".ts"] = "video/mp2t";
	Response::_contentType[".ttf"] = "font/ttf";
	Response::_contentType[".txt"] = "text/plain";
	Response::_contentType[".vsd"] = "application/vnd.visio";
	Response::_contentType[".wav"] = "audio/wav";
	Response::_contentType[".weba"] = "audio/webm";
	Response::_contentType[".webm"] = "video/webm";
	Response::_contentType[".webp"] = "image/webp";
	Response::_contentType[".woff"] = "font/woff";
	Response::_contentType[".woff2"] = "font/woff2";
	Response::_contentType[".xhtml"] = "application/xhtml+xml";
	Response::_contentType[".xls"] = "application/vnd.ms-excel";
	Response::_contentType[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	Response::_contentType[".xml"] = "application/xml";
	Response::_contentType[".xul"] = "application/vnd.mozilla.xul+xml";
	Response::_contentType[".zip"] = "application/zip";
	Response::_contentType[".3gp"] = "video/3gpp; audio/3gpp";
	Response::_contentType[".3g2"] = "video/3gpp2; audio/3gpp2";
	Response::_contentType[".7z"] = "application/x-7z-compressed";
}

std::string Response::buildResponse(std::map<std::string, std::string>& header,
									const std::string extension,
									std::vector<char>& body)
{
	std::map<std::string, std::string>::iterator it = header.begin();
	std::string response;
	std::stringstream ss;
	ss << body.size();

	response.append("HTTP/1.1 " + header["HTTP/1.1"] + CRLF);
	if (!(extension.empty()))
	{
		initializeContentType();
		if (_contentType[extension].empty())
			response.append("Content-Type: application/octet-stream" CRLF);
		else
			response.append("Content-Type: " + _contentType[extension] + CRLF);
	}
	if (!body.empty())
		response.append("Content-Length: " + ss.str() + CRLF);
	for (; it != header.end(); ++it)
	{
		if ((*it).first != "HTTP/1.1" && (*it).first != "Content-Type")
			response.append((*it).first + ": " + (*it).second + CRLF);
	}
	response.append(std::string("Server: Webserv (Unix)") + CRLF);
	response.append(CRLF);
	if (!body.empty())
	{
		for (unsigned i = 0; i < body.size(); i++)
			response += body[i];
		response.append(CRLF);
	}
	return (response);
}

std::string Response::buildErrorResponse(Connection& connection, int _errorCode)
{
	initializeErrorStatus();

	std::string response;
	std::stringstream errorCode;
	errorCode << _errorCode;
	if (!connection.getServer() || _errorStatus[errorCode.str()].empty())
	{
		if (_errorStatus[errorCode.str()].empty())
			LOG(connection.getConnectionID(),
				"Internal error (Error " + errorCode.str() +
				") encountered during processing",
				ERROR)
		response.append(std::string("HTTP/1.1 500 Internal Server Error") + CRLF);
		response.append(std::string("Content-Type: text/html") + CRLF);
		response.append(std::string("Server: Webserv (Unix)") + CRLF);
		response.append(CRLF);
		response.append("<h1>Internal Server Error</h1>");
		response.append(CRLF);
		return (response);
	}
	std::string htmlCode;
	if (!connection.getServer()->getErrorPage(_errorCode).empty())
	{
		char c;
		std::fstream file;
		std::string file_name = connection.getServer()->getErrorPage(_errorCode);

		file.open((connection.getServer()->getRoot() + file_name).c_str());
		if (file.is_open())
		{
			while (file.get(c))
				htmlCode.push_back(c);
			file.close();
		}
		else
		LOG(connection.getConnectionID(),
			file_name + ": " + (std::string)strerror(errno),
			WARNING)
		
		goto createHttpResponse; 
	}
	
	htmlCode = "<!DOCTYPE html>\n"
				"<html lang=\"en\">\n"
				"\n"
				"<head>\n"
				"    <meta charset=\"UTF-8\">\n"
				"    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
				"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
				"    <title>" + errorCode.str() + " " +
				_errorStatus[errorCode.str()] +
				"</title>\n"
				"    <style>\n"
				"        body {\n"
				"            font-family: Arial, sans-serif;\n"
				"            display: flex;\n"
				"            justify-content: center;\n"
				"            align-items: center;\n"
				"            height: 100vh;\n"
				"            margin: 0;\n"
				"        }\n"
				"\n"
				"        .container {\n"
				"            text-align: center;\n"
				"        }\n"
				"\n"
				"        h1 {\n"
				"            font-size: 3em;\n"
				"            margin-bottom: 10px;\n"
				"        }\n"
				"\n"
				"        p {\n"
				"            font-size: 1.2em;\n"
				"            color: #888;\n"
				"        }\n"
				"    </style>\n"
				"</head>\n"
				"\n"
				"<body>\n"
				"    <div class=\"container\">\n"
				"        <h1>" + errorCode.str() + " " +
				_errorStatus[errorCode.str()] +
				"</h1>\n"
				"        <p>The server has been deserted for a while.<br>Please be patient or try again later.</p>\n"
				"    </div>\n"
				"</body>\n"
				"\n"
				"</html>";
	
	createHttpResponse:
		std::stringstream contentSize;
		contentSize << htmlCode.size();
		response.append("HTTP/1.1 " + errorCode.str() + " " + _errorStatus[errorCode.str()] + CRLF);
		response.append(std::string("Content-Type: text/html") + CRLF);
		response.append("Content-Length: " + contentSize.str() + CRLF);
		response.append(std::string("Server: Webserv (Unix)") + CRLF);
		response.append(CRLF);
		response.append(htmlCode);
		response.append(CRLF);
		return (response);
}

std::string Response::buildRedirectResponse(Connection& connection,
											const std::pair<std::string,
											std::string>& redirect)
{
	initializeRedirStatus();
	if (_redirStatus.empty())
		return (Response::buildErrorResponse(connection, 500));

	std::string response;
	response.append("HTTP/1.1 " + redirect.first + " " + _redirStatus[redirect.first] + CRLF);
	response.append("Location: " + redirect.second + CRLF);
	response.append("Content-Length: 0" CRLF);
	response.append(CRLF);
	return (response);
}
