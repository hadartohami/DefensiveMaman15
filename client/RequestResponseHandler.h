#ifndef REQUESTRESPONSEHANDLER_H_
#define REQUESTRESPONSEHANDLER_H_
#include <iostream>
#include "SocketHandler.h"

class RequestResponseHandler{
private:
    SocketHandler* socket_handler;

public:
    RequestResponseHandler(SocketHandler *s);
    ~RequestResponseHandler();
    bool check_name(std::string name);
    bool registration_request(std::string name);
};
#endif
