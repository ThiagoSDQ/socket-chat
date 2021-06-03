//TEMPORARY CLIENT
#include <iostream>
#include <string>
#include <thread>
#include "lib/Socket.hpp"

void receive(Socket mySocket) {
    while (1) {
        std::string msg = mySocket.ReceiveString(mySocket.GetSocket());
        if (msg != "") std::cout << msg << "\n";
    }
}

int main() {
    Socket mySocket = Socket(AF_INET, SOCK_STREAM, 0);

    if (!mySocket.IsValid()) {
        std::cout << "error on socket creation.\n";
        return 1;
    }

    Address server = Address(AF_INET, 6660, "127.0.0.1");

    if (mySocket.Connect(server) != 0)  {
        std::cout << " error on connect.\n";
        //return 1;
    }

    std::thread readThread(receive, mySocket);

    while(1){
        std::string msg;
        std::getline(std::cin, msg);

        mySocket.SendString(mySocket.GetSocket(), msg, 0);
    }

    return 0;
}