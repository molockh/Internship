#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

int main(int argc, char* argv[]) {
  try {
    if (argc != 4) {
      std::cerr
          << "Program should have 3 arguments: Server IP, Port, FilePath.\n"
          << "Example: " << argv[0]
          << "127.0.0.1 55555 /Users/user/Documents/image.png"
          << std::endl;
      return EXIT_FAILURE;
    }

    const std::string kIpAddress = argv[1];
    const int kPort = std::stoi(argv[2]);
    const std::filesystem::path kFilePath(argv[3]);

    if (!std::filesystem::exists(kFilePath)) {
      std::cerr << "Incorrect path" << std::endl;
      return EXIT_FAILURE;
    }

    boost::system::error_code error_code;
    boost::asio::io_context context;

    boost::asio::ip::tcp::endpoint
        endpoint(boost::asio::ip::make_address(kIpAddress), kPort);

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, error_code);

    if (!error_code) {
      std::cout << "Connected!" << std::endl;
    } else {
      std::cerr << "Connection failed: " << error_code.message();
      return EXIT_FAILURE;
    }

    socket.send(boost::asio::buffer(kFilePath.filename().string().data(),
                                    kFilePath.filename().string().size()));

    socket.wait(socket.wait_read);
    std::size_t bytes = socket.available();

    if (bytes > 0) {
      std::string buff(bytes, 0);

      socket
          .read_some(boost::asio::buffer(buff.data(), bytes), error_code);

      if (error_code) {
        std::cerr << error_code.message() << std::endl;
        return EXIT_FAILURE;
      }
      if (buff != "Ready") {
        std::cerr << "Client should receive 'Ready' message" << std::endl;
        return EXIT_FAILURE;
      }
    }

    std::ifstream file(kFilePath, std::ios::binary);
    std::string buff(std::istreambuf_iterator<char>(file), {});

    std::cout << "Sending..." << std::endl;
    socket.send(boost::asio::buffer(buff.data(), buff.size()));

    file.close();
    std::cout << "Success" << std::endl;
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}