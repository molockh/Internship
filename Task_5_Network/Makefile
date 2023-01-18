CXX = g++
CFLAGS = -std=c++17
BUILD_DIR := ./build
SRC_DIR := ./src

all: server client

server: $(BUILD_DIR)/server.o
	$(CXX) $(CFLAGS) $(BUILD_DIR)/server.o -o server

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $(SRC_DIR)/server.cpp -o $(BUILD_DIR)/server.o

client: $(BUILD_DIR)/client.o
	$(CXX) $(CFLAGS) $(BUILD_DIR)/client.o -o client

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $(SRC_DIR)/client.cpp -o $(BUILD_DIR)/client.o

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o