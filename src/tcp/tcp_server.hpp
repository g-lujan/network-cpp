#pragma once

#ifndef _TCP_SERVER_H__
#define _TCP_SERVER_H__

#include "../utils/socket_manager.hpp"
#include "socket.hpp"
#include <string>

namespace IP::TCP {
  class Server {
  public:
    Server(const std::string &port);
    ~Server();
    void start(int backlog = 10);

  private:
    virtual void handler(SOCKET handler_socket);

  protected:
    Socket _socket_listen;
    SocketManager _socket_manager;
  };
} // namespace IP::TCP

#endif
