#ifndef CLIENT_H_
#define CLIENT_H_
#include <iostream>
#include <list>
#include <boost/asio.hpp>

const char* CLIENT_INFO = "/client/me.info"; // this doesn't work to wotk with transfer.info
const char* TRANSFER_INFO = "../client/transfer.info";

// Class that represents a Client
class Client{
private:
    std::string name;
    std::string uuid;
    std::string private_key;
    std::string address;
    std::string port;
    std::list<std::string> files_to_send;
public:
    Client (std::string name);
    ~Client();
    bool get_transfer_info();
    bool get_client_info();
    void set_client_info();
    bool start_client();
    void print_client_info();
    void start_session();
};
#endif
