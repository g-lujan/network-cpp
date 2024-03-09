#pragma once

#ifndef _SOCKET_H__
#define _SOCKET_H__


#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string>
#include <vector>
#include "../utils/constants.hpp"

namespace IP::TCP
{
        // Stream Socket
        class Socket
        {
        public:
            Socket();
            ~Socket();

            /* associates a socket with a local address */
            void bind(const std::string &port);

            /* associates a socket with a remote address and initiates the TCP connection */
            void connect(const std::string &hostname, const std::string &port);

            void listen(const int backlog);

            /* blocking */
            SOCKET accept();

            /* blocking */
            int receive(std::vector<char> &buffer);

            int send(std::vector<char> &buffer);

            void close();

            inline SOCKET descriptor() const { return _socket; }

        private:
            std::string _port;
            SOCKET _socket;
        };
}

#endif