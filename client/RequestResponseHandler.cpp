#include "RequestResponseHandler.h"
#include "Protocol.h"
using namespace std;

RequestResponseHandler::RequestResponseHandler(SocketHandler *s): socketHandler(s){
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

// ostream& operator<<(ostream& os, const RegistrationRequest& req)
// {
//    return os << "payload: " << "ddcbfghfhf" << endl;
// }

bool RequestResponseHandler::registration_request(std::string name){
    std::cout << "[ DEBUG ] in registration request" << std::endl;
    //RegistrationRequest req(REGISTRATION_CODE);
    //req.header.code = REGISTRATION_CODE);
    if (!check_name(name)){
        return false;
    }
    //req.header.payload_size = sizeof(req.payload);
    //strcpy(treq.payload.client_name, CLIENT_NAME_SIZE, name.c_str());
    //std::cout << req;
    // socketHandler->send_and_receive()
    return true;
    
}