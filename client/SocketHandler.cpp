#include "SocketHandler.h"
#include <iostream>

SocketHandler::SocketHandler(std::string a, std::string p): address(a), port(p){
}

SocketHandler::~SocketHandler(){
}


bool SocketHandler::connect() {
    try {
        io_context = new boost::asio::io_context;
        resolver = new tcp::resolver(*io_context);
        socket = new tcp::socket(*io_context);
        boost::asio::connect(*socket, resolver->resolve(address, port));
        socket->non_blocking(false);
        connected = true;
    }
    catch (const std::exception &e) {
        connected = false;
    }
    return connected;
}

void SocketHandler::close() {
        try{
            if (socket != nullptr)
                socket->close();
        }
        catch (const std::exception &e) {}
        delete io_context;
        delete resolver;
        delete socket;
        connected = false;
}

bool SocketHandler::check_connection(){
    if (socket == nullptr || !connected)
        return false;
    return true;
}

bool SocketHandler::receive(uint8_t* const buffer, const size_t size){
    if(check_connection() || buffer == nullptr || size == 0)
        return false;

    size_t bytesLeft = size;
    uint8_t* ptr     = buffer;
    while (bytesLeft > 0)
    {
        uint8_t tempBuffer[PACKET_SIZE] = { 0 };

        boost::system::error_code errorCode; // read() will not throw exception when error_code is passed as argument.

        size_t bytesRead = read(*socket, boost::asio::buffer(tempBuffer, PACKET_SIZE), errorCode);
        if (bytesRead == 0)
            return false;     // Error. Failed receiving and shouldn't use buffer.

//        if (_bigEndian)  // It's required to convert from little endian to big endian.
//        {
//            swapBytes(tempBuffer, bytesRead);
//        }

        const size_t bytesToCopy = (bytesLeft > bytesRead) ? bytesRead : bytesLeft;  // prevent buffer overflow.
        memcpy(ptr, tempBuffer, bytesToCopy);
        ptr       += bytesToCopy;
        bytesLeft = (bytesLeft < bytesToCopy) ? 0 : (bytesLeft - bytesToCopy);  // unsigned protection.
    }

    return true;
}





bool SocketHandler::send(const uint8_t* buffer, const size_t size){
    if(check_connection() || buffer == nullptr || size == 0)
        return false;
    size_t bytesLeft   = size;
    const uint8_t* ptr = buffer;
    while (bytesLeft > 0)
    {
        boost::system::error_code errorCode; // write() will not throw exception when error_code is passed as argument.
        uint8_t tempBuffer[PACKET_SIZE] = { 0 };
        const size_t bytesToSend = (bytesLeft > PACKET_SIZE) ? PACKET_SIZE : bytesLeft;

        memcpy(tempBuffer, ptr, bytesToSend);

//        if (_bigEndian)  // It's required to convert from big endian to little endian.
//        {
//            swapBytes(tempBuffer, bytesToSend);
//        }

        const size_t bytesWritten = write(*socket, boost::asio::buffer(tempBuffer, PACKET_SIZE), errorCode);
        if (bytesWritten == 0)
            return false;

        ptr += bytesWritten;
        bytesLeft = (bytesLeft < bytesWritten) ? 0 : (bytesLeft - bytesWritten);  // unsigned protection.
    }
    return true;
}

//bool SocketHandler::check_big_endian(){
//    uint32_t i;
//    uint8_t c[sizeof(uint32_t)];
//    if
//}
// swap bytes


bool SocketHandler::send_and_receive(const uint8_t* send_data, const size_t size, uint8_t* const response, const size_t response_size)
{
    if (!connect())
    {
        return false;
    }
    if (!send(send_data, size))
    {
        close();
        return false;
    }
    if (!receive(response, response_size))
    {
        close();
        return false;
    }
    close();
    return true;
}