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
    sHandler->send_and_receive(reinterpret_cast<const uint8_t* const>(&req), sizeof(req),
                                reinterpret_cast<uint8_t* const>(&response), sizeof(response)))
}