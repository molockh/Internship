# Networking
The server supports multiple clients at the same time with the help of select() function.
Also was implemented a high-level wrapper over the socket, which monitors its state and closes the connection when the destructor is called.

### Source
* server.cpp - server implementation
* client.cpp - client implementation
* sock_utils.h - general header-only ‘library’ which provides socket wrapper

## Building
### Server & Client
```bash
make all
```
### Server
```bash
make server
```
### Client
```bash
make client
```
### Clear build directory
```bash
make clean
```