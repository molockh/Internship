#include <iostream>
#include <string>

#include "sock_utils.h"

int main() {
  const std::string kServerIpAddress = "127.0.0.1";
  const int kServerPort = 55555;

  Socket sock(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in servaddr{};
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(kServerPort);
  servaddr.sin_addr.s_addr = inet_addr(kServerIpAddress.c_str());

  std::cout << "Trying to connect to "
            << kServerIpAddress << ":" << kServerPort
            << "..." << std::endl;

  sock.Connect(reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr));

  std::string message;
  while (true) {
    auto [bytes_received, received_message] = sock.Receive();

    if (bytes_received == 0 || received_message == "Connection lost") {
      std::cout << "Connection closed" << std::endl;
      exit(EXIT_SUCCESS);
    }
    std::cout << "Received: " << received_message << std::endl;

    std::cout << "Enter your message: ";
    std::getline(std::cin, message);
    sock.Write(message);
  }
}