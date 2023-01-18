#include <vector>
#include <algorithm>
#include <sstream>

#include "sock_utils.h"

int main() {
  const int kPort = 55555;
  const size_t kMaxClients = 30;
  const std::string kDisconnectMessage = "disconnect";

  std::vector<Socket> client_sockets(kMaxClients);
  fd_set readfds;

  Socket main_socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(kPort);

  socklen_t addrlen = sizeof(address);

  main_socket.Bind(reinterpret_cast<sockaddr*>(&address), sizeof(address));
  main_socket.Listen(5);

  std::cout << "Listening on port " << kPort << "..." << std::endl;

  while (true) {
    FD_ZERO(&readfds); // Clear socket set

    FD_SET(main_socket, &readfds); // Add main socket to the set
    socket_t max_descriptor = main_socket;

    // Add clients sockets to the set
    for (const auto& client_sock : client_sockets) {
      if (client_sock.Initialized()) {
        FD_SET(client_sock, &readfds);
      }
      if (client_sock > max_descriptor) {
        max_descriptor = client_sock;
      }
    }

    // Wait for the network activity
    int activity = select(max_descriptor + 1, &readfds, nullptr, nullptr, nullptr);

    if ((activity < 0) && (errno != EINTR)) {
      std::cerr << "Select error " << errno << std::endl;
    }

    // Handle incoming connection
    if (FD_ISSET(main_socket, &readfds)) {
      auto it = std::find_if(client_sockets.begin(),
                             client_sockets.end(),
                             [](auto& sock) {
                               return !sock.Initialized();
                             });

      if (it != client_sockets.end()) {
        *it = std::move(main_socket
                            .Accept(reinterpret_cast<sockaddr*>(&address),
                                    &addrlen));

        it->Send("Connection established!");

        std::cout << inet_ntoa(address.sin_addr) << ":"
                  << ntohs(address.sin_port) << " - Connection established"
                  << std::endl;

      }
    }

    // I/O socket operations
    for (auto& client_sock : client_sockets) {
      if (FD_ISSET(client_sock, &readfds)) {
        auto [bytes, received_message] = client_sock.Receive();

        std::string full_address = client_sock.GetFullAddress();

        if (bytes == 0) {
          std::cout << full_address << " - Client disconnected" << std::endl;
          client_sock.Deinitialize();
        } else { // Process client message
          std::cout << full_address << " - Message received: "
                    << received_message << std::endl;

          if (received_message == "hello") {
            client_sock.Send("world");
            continue;
          }
          if (received_message == kDisconnectMessage) {
            std::cout << full_address
                      << " - Client sent disconnect request. Processing..."
                      << std::endl;

            client_sock.Send("Connection lost");
            client_sock.Deinitialize();
            std::cout << full_address
                      << " - Disconnected" << std::endl;

            continue;
          }

          // Send message back
          client_sock.Send(received_message);
        }
      }
    }
  }
}