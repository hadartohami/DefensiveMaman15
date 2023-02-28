#include <iostream>
#include <string>
#include <pthread.h>
#include "Client.cpp"


int main() {
    std::string name = "hadar";
    Client *c = new Client(name);
    c->get_transfer_info();
    c->get_client_info();
    c->print_client_info();
    c->start_client();
    return 0;
}

