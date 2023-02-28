#ifndef SOCKETHANDLER_H_
#define SOCKETHANDLER_H_
#include <iostream>
using boost::asio::ip::tcp;

constexpr size_t PACKET_SIZE = 1024;

class SocketHandler{
private:
    std::string address;
    std::string port;
    boost::asio::io_context*  io_context;
    tcp::resolver* resolver;
    tcp::socket*   socket;
    bool connected;
public:
    SocketHandler (std::string address, std::string port);
    ~SocketHandler();
    bool connect();
    void close();
    bool check_connection();
    bool receive(uint8_t* const buffer, const size_t size);
    bool send(const uint8_t* const buffer, const size_t size);
    bool send_and_receive(const uint8_t* send_data, const size_t size, uint8_t* const response, const size_t response_size);
};
#endif
