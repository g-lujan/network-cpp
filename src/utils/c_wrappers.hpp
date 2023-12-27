#pragma once

#ifndef _C_WRAPPERS_HPP__
#define _C_WRAPPERS_HPP__

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include "constants.hpp"

namespace C_WRAPPER {
    namespace Socket {
        bool init();
        void cleanup();
        bool valid(SOCKET socket);
        void close(SOCKET socket);
        int errorno();
        bool is_socket_in_set(const FD_SET *set, SOCKET s);
    }
}

#endif