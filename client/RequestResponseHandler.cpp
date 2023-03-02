#include "RequestResponseHandler.h"
#include "Protocol.h"
using namespace std;

RequestResponseHandler::RequestResponseHandler(SocketHandler *s): socket_handler(s){
    // TODO: this should happen before emitting requests
    try{
        s->connect();
    }
    catch (const std::exception &e) {
        std::cerr << "Couldn't connect to Server" << std::endl;
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
    std::cout << "[ DEBUG ] in registration request" << std::endl;
    if (!check_name(name)){
        return false;
    }
    RequestRegistration request;
    ResponseRegistration response;
    std::string client_name = name;
    std::strcpy(reinterpret_cast<char*>(request.payload.client_name.name), client_name.c_str());
    request.header.payload_size = sizeof(request.payload);
    if(!socket_handler->send_and_receive(reinterpret_cast<uint8_t*>(&request), sizeof(request), reinterpret_cast<uint8_t*>(&response), sizeof(response))){
        return false;
    }
    std::cout << response.header.code << std::endl;
    return true;
    
}