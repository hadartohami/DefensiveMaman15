#include <iostream>
#include <string>
#include <pthread.h>
#include "Client.cpp"
#include "rsa.h"

int main() {
    std::string name = "Hadar";
    CryptoPP::RSA::PublicKey       public_key
    Client *c = new Client(name);
    //c->print_client_info();
    std::cout << "[ + ] Client started" << std::endl;
    //c->start_client();
    return 0;
}

