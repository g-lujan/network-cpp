#include "tcp/tcp.hpp"
#ifdef _WIN32
#include <conio.h>
#endif // _WIN32

#include <iostream>

constexpr int MAX_MESSAGE_LEN = 1000;

void read(IP::TCP::Client &client)
{
  char data[MAX_MESSAGE_LEN] = {0};
  MutableBuffer<char> mb(data, MAX_MESSAGE_LEN);
  while (1) {
    if (client.read(mb)) {
      std::cout << std::string(mb.data()) << '\n';
    }
  }
}

void write(IP::TCP::Client &client, std::string& username)
{
  while (1) {
    std::string user_input;
    std::getline(std::cin, user_input);
    std::string message = username + ": " + user_input;
    if (message.length() <= MAX_MESSAGE_LEN) {
      const FixedBuffer<char> write_buffer(message.c_str(), message.length());
      if (!client.write(write_buffer)) {
        std::cerr << "Failed to send message!\n";
      }
    }
    else {
      std::cerr << "Message too long! Max message length:" << MAX_MESSAGE_LEN << "\n";
    }
  }
}

int main(int argc, const char *argv[])
{
  if (argc != 4) {
    std::cout << "usage: tcp_client username host port\n";
    return 0;
  }

  IP::TCP::Client client(argv[2], argv[3]);
  std::thread read_thread(read, std::ref(client));
  std::string port = std::string(argv[1]);
  std::thread write_thread(write, std::ref(client), std::ref(port));
  read_thread.join();
  write_thread.join();

  return 0;
}
