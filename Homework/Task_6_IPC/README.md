# Sending file through the network

Implemented with the help of Boost.Asio.

## Using

### Starting server

```bash
./server 55555
```

### Sending file to the server

```bash
./client 127.0.0.1 55555 /Users/user/image.png
```

## Building

Tested on OS X.\
Maybe Boost library should be added to the 
CMakeLists.txt on your host system in another way.

### Server & Client

```bash
mkdir build
cd build
cmake ..
make
```