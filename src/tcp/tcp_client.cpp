#include "tcp_client.hpp"
#include "../buffer/buffer.hpp"
#include "../utils/c_wrappers.hpp"

#include <chrono>
#include <ranges>

using namespace std::chrono_literals;

IP::TCP::Client::Client(const std::string &hostname, const std::string &port) : _hostname{hostname}, _port{port}
{
  C_WRAPPER::Socket::init();
  _socket.connect(hostname, port);
  _socket_manager.add_socket(_socket.descriptor());
}

IP::TCP::Client::~Client() { C_WRAPPER::Socket::cleanup(); }

bool IP::TCP::Client::read(MutableBuffer<char> &read_buffer)
{
  int bytes_received = 0;
  for (auto const &socket : _socket_manager.get(QUERY_SOCKET::READABLE, 1s) | std::ranges::views::take(1)) {
    bytes_received = recv(socket, read_buffer.data(), read_buffer.size(), 0);
  }
  return bytes_received > 0;
}

bool IP::TCP::Client::write(const FixedBuffer<char> &write_buffer)
{
  int result = 0;
  for (auto const &socket : _socket_manager.get(QUERY_SOCKET::WRITEABLE, 1s) | std::ranges::views::take(1)) {
    result = send(socket, write_buffer.data(), write_buffer.size(), 0);
  }
  return result != -1;
}
