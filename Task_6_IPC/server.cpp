#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <fstream>
#include <filesystem>

const std::string kDirectoryPath{"./downloads/"};
const size_t kBufferSize = 102400;

class Session : public std::enable_shared_from_this<Session> {
 public:

  explicit Session(boost::asio::ip::tcp::socket&& socket)
      : socket_(std::move(socket)), buffer_(kBufferSize) {
    this->client_ip_address_ =
        this->socket_.remote_endpoint().address().to_string();
    std::cout << this->client_ip_address_ << " - Connection established"
              << std::endl;
  }

  void ReceiveName() {
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_.data(),
                                                buffer_.size()),
                            [self](std::error_code ec, std::size_t length) {
                              if (!ec) {
                                std::string name
                                    (self->buffer_.begin(),
                                     self->buffer_.begin() + length);
                                self->received_file_.open(kDirectoryPath + name,
                                                          std::ios::out
                                                              | std::ios::binary);

                                std::string msg = "Ready";
                                self->socket_
                                    .async_send(
                                        boost::asio::buffer(msg.data(),
                                                            msg.size()),
                                        [](std::error_code ec,
                                           std::size_t length) {});
                                self->ReceiveFile();
                              }
                            }
    );

  }

  void ReceiveFile() {
    auto self = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_.data(),
                                                buffer_.size()),
                            [self](std::error_code ec, std::size_t length) {
                              if (!ec) {
                                self->received_file_
                                    .write(self->buffer_.data(), length);
                                self->ReceiveFile();
                              }
                            }
    );
  }

  ~Session() {
    std::cout << client_ip_address_ << " - File received. Connection closed"
              << std::endl;
    received_file_.close();
  }

 private:
  std::string client_ip_address_;
  std::ofstream received_file_;
  boost::asio::ip::tcp::socket socket_;
  std::vector<char> buffer_;
};

class server {
 public:
  server(boost::asio::io_context& io_context, int port)
      : acceptor_(io_context,
                  boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                 port)) {
    accept();
  }

 private:
  void accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket) {
          if (!ec) {
            std::make_shared<Session>(std::move(socket))->ReceiveName();
          }

          accept();
        });
  }

  boost::asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr
          << "Program should have 1 arguments: Port.\n"
          << "Example: " << argv[0] << " 55555" << std::endl;
      return EXIT_FAILURE;
    }
    const int kPort = std::atoi(argv[1]);

    std::filesystem::path path(kDirectoryPath);
    if (!std::filesystem::is_directory(path)) {
      std::cerr << "Incorrect directory path" << std::endl;
      return EXIT_FAILURE;
    }

    boost::asio::io_context context;
    server srv(context, kPort);
    context.run();
  } catch (const std::exception& exception) {
    std::cerr << exception.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}