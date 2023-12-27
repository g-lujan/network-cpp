#include "tcp_server.hpp"
#include "../utils/c_wrappers.hpp"

#include <chrono>
#include <ranges>
#include <iostream>

using namespace std::chrono_literals;

IP::TCP::Server::Server(const std::string &port)
{
  C_WRAPPER::Socket::init();
  _socket_listen.bind(port);
  _socket_manager.add_socket(_socket_listen.descriptor());
}

IP::TCP::Server::~Server() { C_WRAPPER::Socket::cleanup(); }

void IP::TCP::Server::start(int backlog)
{
  _socket_listen.listen(backlog);
  while (true) {
    for (auto const &socket : _socket_manager.get(QUERY_SOCKET::READABLE)) {
      if (socket == _socket_listen.descriptor()) {
        //  new connection is ready to be established -> spawns new socket
        SOCKET client = _socket_listen.accept();
        if (!C_WRAPPER::Socket::valid(socket)) {
            std::cerr << "accept() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
          continue;
        }
        _socket_manager.add_socket(client);
      }
      else {
        handler(socket);
      }
    }
  }
}

void IP::TCP::Server::handler(SOCKET handler_socket) {}

