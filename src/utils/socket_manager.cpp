#include "socket_manager.hpp"
#include "c_wrappers.hpp"

using C_WRAPPER::Socket::is_socket_in_set;

/****************************************************
 * Private utilitary functions forward declarations *
 ****************************************************/

timeval to_timeval(const std::chrono::system_clock::duration &timeout);

/**********************************
 * Public methods implementations *
 **********************************/

SocketManager::SocketManager() : _max_socket{0} { FD_ZERO(&_file_descriptors); }

void SocketManager::add_socket(SOCKET socket)
{
  FD_SET(socket, &_file_descriptors);
  _max_socket = _max_socket > socket ? _max_socket : socket;
}

void SocketManager::add_sockets(const std::initializer_list<SOCKET> &sockets)
{
  for (SOCKET socket : sockets) {
    add_socket(socket);
  }
}

void SocketManager::remove_sockets(const std::initializer_list<SOCKET> &sockets)
{
  for (SOCKET socket : sockets) {
    FD_CLR(socket, &_file_descriptors);
    C_WRAPPER::Socket::close(socket);
    // how to change the _max_socket?
  }
}

std::unordered_set<SOCKET> SocketManager::get(const QUERY_SOCKET query, const std::chrono::system_clock::duration &timeout)
{
  fd_set copy_file_descriptors = _file_descriptors; // copy to not change the original fd_set when running the selects below
  fd_set *readable_set = query & QUERY_SOCKET::READABLE ? &copy_file_descriptors : nullptr;
  fd_set *writeable_set = query & QUERY_SOCKET::WRITEABLE ? &copy_file_descriptors : nullptr;
  fd_set *errored_set = query & QUERY_SOCKET::ERRORED ? &copy_file_descriptors : nullptr;

  if (timeout == std::chrono::system_clock::duration::zero()) {
    select(_max_socket + 1, readable_set, writeable_set, errored_set, nullptr);
  }
  else {
    timeval tv_timeout = to_timeval(timeout);
    select(_max_socket + 1, readable_set, writeable_set, errored_set, &tv_timeout);
  }

  std::unordered_set<SOCKET> filtered;
  for (SOCKET socket = 1; socket <= _max_socket; ++socket) {
    if (is_socket_in_set(readable_set, socket) || is_socket_in_set(writeable_set, socket) || is_socket_in_set(errored_set, socket)) {
      filtered.insert(socket);
    }
  }
  return filtered;
}

/***********************************************
 * Private utilitary functions implementations *
 ***********************************************/

timeval to_timeval(const std::chrono::system_clock::duration &timeout)
{
  timeval tv;
  std::chrono::seconds const sec = std::chrono::duration_cast<std::chrono::seconds>(timeout);
  tv.tv_sec = sec.count();
  tv.tv_usec = std::chrono::duration_cast<std::chrono::microseconds>(timeout - sec).count();
  return tv;
}