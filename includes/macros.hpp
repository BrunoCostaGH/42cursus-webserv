/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricard <maricard@student.porto.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 15:56:15 by bsilva-c          #+#    #+#             */
/*   Updated: 2024/01/20 19:04:08 by bsilva-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iomanip>
#include <netdb.h>
#include <cstring>
#include <ctime>

#define HERE printf("HERE\n")

# define DEBUG true
# define MAX_CONNECTIONS 1024

// colors foreground
# define F_BLACK    "\033[1;30m"
# define F_RED    "\033[1;31m"
# define F_GREEN  "\033[1;32m"
# define F_YELLOW "\033[1;33m"
# define F_BLUE "\033[1;34m"
# define F_MAGENTA  "\033[1;35m"
# define F_CYAN  "\033[1;36m"
# define F_WHITE  "\033[1;37m"

// colors background
# define B_BLACK    "\033[1;40m"
# define B_RED    "\033[1;41m"
# define B_GREEN  "\033[1;42m"
# define B_YELLOW "\033[1;43m"
# define B_BLUE "\033[1;44m"
# define B_MAGENTA  "\033[1;45m"
# define B_CYAN  "\033[1;46m"
# define B_WHITE  "\033[1;47m"

# define RESET  "\033[0m"

# define ERROR 1
# define WARNING 2
# define INFORMATION 3
# define REQUEST 4
# define SUCCESS 5

# define CRLF "\r\n"

# define GET 1
# define POST 2
# define DELETE 4
# define CGI 8
# define DIR_LIST 16
# define REDIR 32

# define MESSAGE(message, level) {\
    time_t current = std::time(0);\
    tm* time = std::localtime(&current);\
    if (level == ERROR)\
    {\
        std::cerr << B_RED "[" << std::setw(2) << std::setfill('0') <<\
        time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
        time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
        time->tm_sec << "]" << RESET F_RED;\
    }\
    else if (level == WARNING)\
    {\
        std::cerr << B_YELLOW "[" << std::setw(2) << std::setfill('0') <<\
        time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
        time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
        time->tm_sec << "]" << RESET F_YELLOW;\
    }\
    else if (level == INFORMATION)\
    {\
        std::cerr << B_BLUE "[" << std::setw(2) << std::setfill('0') <<\
        time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
        time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
        time->tm_sec << "]" << RESET F_BLUE;\
    }\
    std::cerr << " " << message << RESET << std::endl;\
}

# define LOG(id, message, level) {\
    if (DEBUG)\
    {\
        time_t current = std::time(0);\
        tm* time = std::localtime(&current);\
        if (level == ERROR)\
        {\
            std::cerr << B_RED "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_RED " [" << id << "]";\
        }\
        else if (level == WARNING || level == REQUEST)\
        {\
            std::cerr << B_YELLOW "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_YELLOW " [" << id << "]";\
        }\
        else if (level == INFORMATION)\
        {\
            std::cerr << B_BLUE "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_BLUE " [" << id << "]";\
        }\
        else if (level == SUCCESS)\
        {\
            std::cerr << B_BLUE "[" << std::setw(2) << std::setfill('0') <<\
            time->tm_hour << ":"  << std::setw(2) << std::setfill('0') <<\
            time->tm_min << ":" << std::setw(2) << std::setfill('0') <<\
            time->tm_sec << "]" << RESET F_GREEN " [" << id << "]";\
        }\
        std::cerr << " " << message << RESET << std::endl;\
    }\
}
