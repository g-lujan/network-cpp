#if defined(_WIN32)
#include <ws2tcpip.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include "../utils/c_wrappers.hpp"
#include "socket.hpp"
#include <iostream>

IP::TCP::Socket::Socket() {}

IP::TCP::Socket::~Socket()
{
#if defined(_WIN32)
  closesocket(_socket);
#else
  close(_socket);
#endif
}

void IP::TCP::Socket::bind(const std::string &port)
{
  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  addrinfo *bind_address;
  if (getaddrinfo(0, port.c_str(), &hints, &bind_address)) {
    std::cerr << "getaddrinfo() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
    return;
  }

  // file descriptor to an unbound socket
  _socket = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

  // For dual IPv6/IPv4, after the call to socket() and before the call to bind(), we must clear the
  // IPV6_V6ONLY flag (enabled by default) on the socket by setting it to zero.
  int option = 0;
  if (setsockopt(_socket, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<char *>(&option), sizeof(option))) {
    std::cerr << "setsockopt() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
    return;
  }

  //  assigns a local protocol address to a socket: IPv4/IPv6 address (32-bit/128-bit) + a 16 bit TCP port number
  if (::bind(_socket, bind_address->ai_addr, bind_address->ai_addrlen)) {
    std::cerr << "bind() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
  }

  freeaddrinfo(bind_address);
  _port = port;
}

void IP::TCP::Socket::connect(const std::string &hostname, const std::string &port)
{
  addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  addrinfo *peer_address;
  if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &peer_address)) {
    std::cerr << "getaddrinfo() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
    return;
  }

  _socket = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
  if (!C_WRAPPER::Socket::valid(_socket)) {
    std::cerr << "socket() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
    return;
  }
  printf("Connecting...\n");
  if (::connect(_socket, peer_address->ai_addr, peer_address->ai_addrlen)) {
    std::cerr << "connect() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
    return;
  }

  freeaddrinfo(peer_address);
  _port = port;
}

void IP::TCP::Socket::listen(const int backlog)
{
  if (::listen(_socket, backlog) < 0) {
    std::cerr << "listen() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
  }
}

SOCKET IP::TCP::Socket::accept()
{
  sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  SOCKET socket_client = ::accept(_socket, reinterpret_cast<sockaddr *>(& client_address), &client_len);
  if (!C_WRAPPER::Socket::valid(_socket)) {
    std::cerr << "accept() failed. Error number: " << C_WRAPPER::Socket::errorno() << std::endl;
  }
  return socket_client;
}

int IP::TCP::Socket::receive(MutableBuffer<char> &buffer) { return recv(_socket, buffer.data(), buffer.size(), 0); }

int IP::TCP::Socket::send(FixedBuffer<char> &buffer) { return ::send(_socket, buffer.data(), buffer.size(), 0); }

void IP::TCP::Socket::close() { C_WRAPPER::Socket::close(_socket); }
