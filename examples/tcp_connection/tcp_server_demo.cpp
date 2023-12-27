#include "tcp/tcp.hpp"
#include <iostream>

class ChatServer : public IP::TCP::Server {
public:
  ChatServer(const std::string &port) : IP::TCP::Server(port) {}

private:
  void handler(SOCKET handler_socket)
  {
    char read[1024] = {0};
    int bytes_received = recv(handler_socket, read, 1024, 0);
    if (bytes_received < 1) {
      _socket_manager.remove_sockets({handler_socket});
      return;
    }

    for (auto const &others : _socket_manager.get(QUERY_SOCKET::WRITEABLE)) {
      if (others == _socket_listen.descriptor() || others == handler_socket) {
        continue;
      }
      else {
        send(others, read, bytes_received, 0);
      }
    }
  }
};

int main(int argc, const char *argv[])
{
  if (argc != 2) {
    std::cout << "usage: port\n";
    return 0;
  }

  ChatServer server(argv[1]);
  server.start();

  return 0;
}