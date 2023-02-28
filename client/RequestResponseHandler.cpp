#include "RequestResponseHandler.h"
#include "Protocol.h"

RequestResponseHandler::RequestResponseHandler(SocketHandler *s): sHandler(s){
    try{
        s->connect();
    }
    catch (const std::exception &e) {
        std::cout << "Couldn't connect to Server" << std::endl;
    }
}

RequestResponseHandler::~RequestResponseHandler(){
}

bool RequestResponseHandler::check_name(std::string name) {
    if (name.length() >= CLIENT_NAME_SIZE){
        for (auto ch : name){
            if (!std::isalnum(ch))
                return false;
        }
    }
    return true;
}


bool RequestResponseHandler::registration_request(std::string name){
    RegistrationRequest req;
    if (!check_name(name)){
        return false;
    }
    req.header.payload_size = sizeof(req.payload);
    strcpy_s(req.payload.client_name, CLIENT_NAME_SIZE, name.c_str());
    std::count << req;
    // sHandler->send_and_receive()
    return true;
}

ostream& operator << (ostream& os, const RegistrationRequest& req)
{
   return os << "header_code: " << req.header.code <<
              "version: " << req.header.version << 
              << " Payload:" << req.payload << endl;
}