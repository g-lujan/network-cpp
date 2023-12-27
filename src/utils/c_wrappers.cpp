#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <errno.h>
#include <sys/socket.h>
#endif

#include "c_wrappers.hpp"
#include <iostream>

bool C_WRAPPER::Socket::init()
{
#if defined(_WIN32)
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    std::cerr << "Failed to initialize.\n";
    return false;
  }
#endif
  return true;
}

void C_WRAPPER::Socket::cleanup()
{
#if defined(_WIN32)
  WSACleanup();
#endif
}

bool C_WRAPPER::Socket::valid(SOCKET socket)
{
#if defined(_WIN32)
  return socket != INVALID_SOCKET;
#else
  return socket >= 0;
#endif
}

void C_WRAPPER::Socket::close(SOCKET socket)
{
#if defined(_WIN32)
  closesocket(socket);
#else
  ::close(socket);
#endif
}

int C_WRAPPER::Socket::errorno()
{
#if defined(_WIN32)
  return WSAGetLastError();
#else
  return ::errno;
#endif
}

bool C_WRAPPER::Socket::is_socket_in_set(const FD_SET *set, SOCKET s) { return set ? FD_ISSET(s, set) : false; }
