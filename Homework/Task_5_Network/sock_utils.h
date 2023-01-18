#ifndef TASK_5_NETWORK__SOCK_UTILS_H_
#define TASK_5_NETWORK__SOCK_UTILS_H_

#include <iostream>
#include <string>
#include <utility>
#include <sstream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>

#define NOT_INITIALIZED 0
#define BUFFER_SIZE 1024

using socket_t = int;

class Socket final {
 public:
  Socket() = default;
  explicit Socket(socket_t sock);
  Socket(int domain, int type, int protocol);
  Socket(Socket&& sock) noexcept;
  Socket& operator=(Socket&& sock) noexcept;
  Socket& operator=(Socket& sock) = delete;

  void Deinitialize();

  void Connect(const sockaddr* address, socklen_t size);
  void Bind(const sockaddr* address, socklen_t size);
  void Listen(int connection_count);

  Socket Accept(sockaddr* address, socklen_t* size);

  bool Write(const std::string& message);
  bool Send(const std::string& message);
  std::pair<size_t, std::string> Receive();

  std::string GetFullAddress();

  bool Initialized() const;

  operator socket_t() const;

  void Close();
  ~Socket();

 private:
  socket_t sock_ = NOT_INITIALIZED;
  char buffer_[BUFFER_SIZE] = {0};

  void CleanBuffer();
};

Socket::Socket(socket_t sock) : sock_(sock) {
  if (sock < 0) {
    throw std::runtime_error("Incorrect socket");
  }
}

Socket::Socket(int domain, int type, int protocol) {
  socket_t sockfd = socket(domain, type, protocol);

  if (sockfd == -1) {
    throw std::runtime_error("Socket creation failed");
  }

  sock_ = sockfd;
}

Socket::Socket(Socket&& sock) noexcept: sock_(sock.sock_) {
  sock.sock_ = NOT_INITIALIZED;
}

Socket& Socket::operator=(Socket&& sock) noexcept {
  if (sock_ != sock.sock_) {
    Close();
  }

  sock_ = sock.sock_;
  sock.sock_ = NOT_INITIALIZED;

  return *this;
}

void Socket::Deinitialize() {
  Close();
  sock_ = NOT_INITIALIZED;
}

void Socket::Connect(const sockaddr* address, socklen_t size) {
  if (!Initialized()) {
    throw std::runtime_error("Socket is not initialized");
  }

  int connection_result = connect(sock_, address, size);

  if (connection_result == -1) {
    std::cerr << "Connecting error " << errno << std::endl;
    throw std::runtime_error("Connection failed");
  }
}

void Socket::Bind(const sockaddr* address, socklen_t size) {
  if (!Initialized()) {
    throw std::runtime_error("Socket is not initialized");
  }

  int bind_result = bind(sock_, address, size);

  if (bind_result == -1) {
    std::cerr << "Binding error " << errno << std::endl;
    throw std::runtime_error("Binding failed");
  }
}

void Socket::Listen(int connection_count) {
  if (!Initialized()) {
    throw std::runtime_error("Socket is not initialized");
  }

  listen(sock_, connection_count);
}

Socket Socket::Accept(sockaddr* address, socklen_t* size) {
  return Socket(accept(sock_, address, size));
}

bool Socket::Write(const std::string& message) {
  CleanBuffer();
  std::copy(message.begin(), message.end(), buffer_);
  size_t write_result = write(sock_, message.c_str(), sizeof(buffer_));

  if (write_result != sizeof(buffer_)) {
    std::cerr << "Writing error " << errno << std::endl;
    return false;
  }

  return true;
}

bool Socket::Send(const std::string& message) {
  CleanBuffer();
  std::copy(message.begin(), message.end(), buffer_);
  size_t send_result = send(sock_, message.c_str(), sizeof(buffer_), 0);

  if (send_result != sizeof(buffer_)) {
    std::cerr << "Sending error " << errno << std::endl;
    return false;
  }

  return true;
}

std::pair<size_t, std::string> Socket::Receive() {
  CleanBuffer();
  size_t bytes_received = read(sock_, buffer_, sizeof(buffer_));

  return {bytes_received, buffer_};
}

void Socket::CleanBuffer() {
  memset(buffer_, '\0', sizeof(char) * BUFFER_SIZE);
}

bool Socket::Initialized() const {
  return sock_ != NOT_INITIALIZED;
}

Socket::operator socket_t() const {
  return sock_;
}

void Socket::Close() {
  if (Initialized()) {
    close(sock_);
    sock_ = NOT_INITIALIZED;
  }
}

Socket::~Socket() {
  Close();
}

std::string Socket::GetFullAddress() {
  struct sockaddr_in address{};
  socklen_t addrlen = sizeof(address);

  getpeername(sock_,
              (struct sockaddr*) &address,
              &addrlen);

  std::stringstream full_addr;
  full_addr << inet_ntoa(address.sin_addr)
            << ":" << ntohs(address.sin_port);

  return full_addr.str();
}

#endif //TASK_5_NETWORK__SOCK_UTILS_H_
