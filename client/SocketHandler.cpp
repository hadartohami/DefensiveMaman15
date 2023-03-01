#include "SocketHandler.h"
#include <iostream>

SocketHandler::SocketHandler(std::string address, std::string port): address(address), port(port){
    union   // checking endianness
	{
		uint32_t i;
		uint8_t c[sizeof(uint32_t)];
    }tester{ 1 };
    _bigEndian = (tester.c[0] == 0);    
}

SocketHandler::~SocketHandler(){
}

/**
 * Handle Endianness.
 */
void SocketHandler::swapBytes(uint8_t* const buffer, size_t size) const
{
    std::cout << "handling endiniess" << std::endl;
	if (buffer == nullptr || size < sizeof(uint32_t))
		return; 

	size -= (size % sizeof(uint32_t));
	uint32_t* const ptr = reinterpret_cast<uint32_t* const>(buffer);
	for (size_t i = 0; i < size; ++i)
	{
		const uint32_t tmp = ((buffer[i] << 8) & 0xFF00FF00) | ((buffer[i] >> 8) & 0xFF00FF);
		buffer[i] = (tmp << 16) | (tmp >> 16);
	}
		
}

bool SocketHandler::connect() {
    try {

        // Get IO ctx
        this->io_context = new boost::asio::io_context;

        // Set ctx resolving
        this->resolver = new tcp::resolver(*this->io_context);

        // Connect to socket
        this->socket = new tcp::socket(*this->io_context);
        boost::asio::connect(*this->socket, this->resolver->resolve(address, port));

        // Set non-blocking
        this->socket->non_blocking(false);

        // Mark for us
        this->connected = true;
    }
    catch (const std::exception &e) {
        std::cerr << "EXCEPTED IN sockethandler::connect";
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

bool SocketHandler::is_connected(){
    if (this->socket == nullptr || !this->connected)
        return false;
    return true;
}

bool SocketHandler::receive(uint8_t* const buffer, const size_t size){
    if(is_connected() || buffer == nullptr || size == 0)
        return false;

    size_t num_of_bytes_to_send = size;
    uint8_t* buffer_ptr     = buffer;
    while (num_of_bytes_to_send > 0)
    {
        uint8_t tempBuffer[PACKET_SIZE] = { 0 };

        boost::system::error_code errorCode; // read() will not throw exception when error_code is passed as argument.

        size_t bytesRead = read(*socket, boost::asio::buffer(tempBuffer, PACKET_SIZE), errorCode);
        if (bytesRead == 0)
            return false;     // Error. Failed receiving and shouldn't use buffer.
		if (_bigEndian)  // It's required to convert from little endian to big endian.
		{
			swapBytes(tempBuffer, bytesRead);   
		}

        const size_t bytesToCopy = (num_of_bytes_to_send > bytesRead) ? bytesRead : num_of_bytes_to_send;  // prevent buffer overflow.
        memcpy(buffer_ptr, tempBuffer, bytesToCopy);
        buffer_ptr       += bytesToCopy;
        num_of_bytes_to_send = (num_of_bytes_to_send < bytesToCopy) ? 0 : (num_of_bytes_to_send - bytesToCopy);  // unsigned protection.
    }

    return true;
}


bool SocketHandler::send(const uint8_t* buffer, const size_t size){
    if(!is_connected() || buffer == nullptr || size == 0)
        return false;
    size_t num_of_bytes_to_send = size;
    const uint8_t* buffer_ptr = buffer;
    boost::system::error_code errorCode;
    while (num_of_bytes_to_send > 0)
    {
        uint8_t tempBuffer[PACKET_SIZE] = { 0 }; // PACKET_SIZE = 1024; this inits an array with 0s
        const size_t bytesToSend = (num_of_bytes_to_send > PACKET_SIZE) ? PACKET_SIZE : num_of_bytes_to_send; // Fitting the amount of bytes left to send

        // TODO: make this safe copy omg
        memcpy(tempBuffer, buffer_ptr, bytesToSend);

        if (_bigEndian)  // It's required to convert from big endian to little endian.
        {
            swapBytes(tempBuffer, bytesToSend);
        }

        const size_t bytesWritten = write(*socket, boost::asio::buffer(tempBuffer, PACKET_SIZE), errorCode); // write() will not throw exception when error_code is passed as argument.
        if (bytesWritten == 0){
            std::cout << "Wrote 0 bytes in socket handler";
            return false;
        }
        // TODO: check if errorCode is not null
        // if (errorCode != NULL){
        //     std::cout << "NULL WTF?! in send" << std::endl;
        // }

        buffer_ptr += bytesWritten;
        num_of_bytes_to_send = (num_of_bytes_to_send < bytesWritten) ? 0 : (num_of_bytes_to_send - bytesWritten);  // unsigned protection.
    }
    std::cout << "Wrote all bytes in socket handler, returning";
    return true;
}


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