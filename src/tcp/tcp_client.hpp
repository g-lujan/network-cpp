#pragma once

#ifndef _TCP_CLIENT_H__
#define _TCP_CLIENT_H__

#include "../utils/socket_manager.hpp"
#include "socket.hpp"
#include <string>
#include <vector>

namespace IP::TCP {
  class Client {
  public:
    Client(const std::string &hostname, const std::string &port);
    ~Client();
    bool read(std::vector<char> &read_buffer);
    bool write(const std::vector<char> &write_buffer);

  private:
    std::string _hostname;
    std::string _port;
    Socket _socket;
    SocketManager _socket_manager;
  };
} // namespace IP::TCP

#endif
