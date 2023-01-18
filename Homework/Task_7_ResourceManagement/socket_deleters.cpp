#include <iostream>
#include <memory>

#include "sys/socket.h"
#include <unistd.h>

using socket_t = int;

struct FunctorSocketDeleter {
  void operator()(socket_t* sock) const {
    std::cout << "Socket " << *sock << " closed" << std::endl;
    close(*sock);
    delete sock;
  }
};

void DeleteSocket(socket_t* sock) {
  std::cout << "Socket " << *sock << " closed" << std::endl;
  close(*sock);
  delete sock;
}

int main() {

  std::unique_ptr<socket_t, FunctorSocketDeleter>
      sock1(new socket_t(socket(AF_INET, SOCK_STREAM, 0)));

  auto lambda_socket_deleter = [](socket_t* sock) {
    std::cout << "Socket " << *sock << " closed" << std::endl;
    close(*sock);
    delete sock;
  };
  std::unique_ptr<socket_t, decltype(lambda_socket_deleter)>
      sock2(new socket_t(socket(AF_INET, SOCK_STREAM, 0)),
            lambda_socket_deleter);

  std::unique_ptr<socket_t, void (*)(socket_t*)>
      sock3(new socket_t(socket(AF_INET, SOCK_STREAM, 0)),
            DeleteSocket);
  return 0;
}