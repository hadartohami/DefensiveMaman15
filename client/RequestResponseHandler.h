#ifndef REQUESTRESPONSEHANDLER_H_
#define REQUESTRESPONSEHANDLER_H_
#include <iostream>
#include "SocketHandler.h"
#include "RSAWrapper.cpp"
#include "Protocol.h"

class RequestResponseHandler{
private:
    SocketHandler* socket_handler;
    RSAPrivateWrapper* rsa_private_wrapper;
public:
    RequestResponseHandler(SocketHandler *s);
    ~RequestResponseHandler();
    bool check_name(std::string name);
    bool registration_request(std::string name);
    bool send_public_key_request(std::string name);
};
#endif
