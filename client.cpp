#include <iostream>
#include <string>
#include <string.h>
#include <thread>
#include "lib/Client.hpp"

int main(int argc, char *argv[]) {
    Client myClient;

    Address server = Address(AF_INET, 4321, "127.0.0.1");

    myClient.SetServerAddress(server);

    if (myClient.ConnectToServer() == -1){
        std::cout << "Failed to connect to server!\n";
        std::cout << strerror(errno) << "\n";
        return 1;
    }

    if (myClient.Start() != 0){
        std::cout << "Failed to start client!\n";
        std::cout << strerror(errno) << "\n";
        return 1;
    }

    myClient.WaitForStop();

    return 0;
}