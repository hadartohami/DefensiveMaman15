#include <iostream>
#include <string>
#include <pthread.h>
#include "Client.cpp"

int main() {
    std::string name = "Hadar";
    Client *c = new Client(name);
    c->print_client_info();
    std::cout << "[ + ] Client started" << std::endl;
    c->start_client();
    return 0;
}

