#pragma once

#ifndef _SOCKET_MANAGER_H__
#define _SOCKET_MANAGER_H__

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <sys/select.h>
#endif

#include <chrono>
#include <unordered_set>
#include <vector>

enum QUERY_SOCKET {
  READABLE = 1 /*<< 0*/,
  WRITEABLE = 1 << 1,
  ERRORED = 1 << 2,
};

class SocketManager {
public:
  SocketManager();
  
  void add_socket(SOCKET sockets);
  void add_sockets(const std::initializer_list<SOCKET> &sockets);
  void remove_sockets(const std::initializer_list<SOCKET> &sockets);

  /* blocks until at least one of the sockets is ready to be read/write from/to, or has returned an error.  */
  std::unordered_set<SOCKET> get(const QUERY_SOCKET query,
                                 const std::chrono::system_clock::duration &timeout = std::chrono::system_clock::duration::zero());

private:
  fd_set _file_descriptors;
  int _max_socket; // necessary to conform to select API
};

#endif