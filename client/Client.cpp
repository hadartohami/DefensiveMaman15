#include "Client.h"
#include <iostream>
#include <fstream>
#include "SocketHandler.cpp"
#include "RequestResponseHandler.h"
#include "Protocol.h"

#pragma once

Client::Client(std::string n): name(n){
}

Client::~Client(){
}

bool Client::start_client(){
    std::cout << "[ DEBUG ] start_client" << std::endl;
    bool client_info = get_client_info();
    if( client_info == false){
        bool transfer_info = get_transfer_info();
        if (transfer_info == true){
            SocketHandler* socket_handler = new SocketHandler(this->address, this->port);
            RequestResponseHandler* req_res_handler = new RequestResponseHandler(socket_handler);
            req_res_handler->registration_request(this->name);
        }else{
            // TODO: exit(1) with error for no configurations
        }
    }
    //transfer = Client:get_transfer_info();
}


// Checking whether the file is open. if open gets address and port from TRANSFER_INFO file
bool Client::get_transfer_info() {
    std::fstream transfer;
    transfer.open(TRANSFER_INFO, std::ios::in);
    if (transfer.is_open()) {
        std::string line;
        std::string info;
        std::getline(transfer, info);
        int pos = info.find(":");
        info.substr(0,pos);
        const auto address = info.substr(0,pos);
        this->address = address;
        const auto port = info.substr(pos + 1);
        this->port = port;
        std::getline(transfer, info);
        this->name = info;
        std::getline(transfer, info);
        this->files_to_send.push_back(info);
        std::cout << "[ + ] Calling server at " << address << ":" << port << "..." << std::endl;
        transfer.close();
        return true;
    }
    else {
        std::cout << "[ X ] Couldn't Open file: " << TRANSFER_INFO << std::endl;
        return false;
    }
}

// Checking whether the file is open. if open gets client info from CLIENT_INFO file
bool Client::get_client_info() {
    std::fstream client;
    client.open(CLIENT_INFO, std::ios::in);
    if (client.is_open()) {
        std::string line;
        std::string info;
        std::getline(client, info);
        const auto client_name = info;
        this->name = client_name;
        std::getline(client, info);
        const auto uuid = info;
        this->uuid = uuid;
        std::getline(client, info);
        const auto private_key = info;
        this->private_key = private_key;
        std::cout << "[ DEBUG ] Client Name: " << client_name << std::endl;
        std::cout << "[ DEBUG ] Client UUID: " << uuid << std::endl;
        std::cout << "[ DEBUG ] Client Private Key: " << private_key << std::endl;
        client.close();
        return true;
    }
    else {
        std::cout << "[ X ] Couldn't Open file: " << CLIENT_INFO << std::endl;
        return false;
    }
}

void Client::set_client_info(){
}

void Client::print_client_info(){
    std::cout << this->name << std::endl;
}

