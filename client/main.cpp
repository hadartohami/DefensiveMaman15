#include <iostream>
#include <string>
#include <pthread.h>
#include "Client.cpp"


int main() {
    std::string name = "Hadar";
    std::cout << "[ + ] Client started" << std::endl;
    Client *c = new Client(name);
    //c->print_client_info();
    c->start_client();
    return 0;
}

