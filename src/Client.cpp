#include "../lib/Client.hpp"
#include <iostream>

Client::Client(): clientSocket(AF_INET, SOCK_STREAM, 0) {
}

void Client::SetServerAddress(Address addr) {
    serverAddress = addr;
}

int Client::ConnectToServer() {
    clientSocket.Connect(serverAddress);
    if (errno != EINPROGRESS)  {
        return 1;
    }

    return 0;
}

void Client::Start() {
    stopClient = false;
    stopReceiveThread = false;
    stopReadInputThread = false;

    receiveThread = new std::thread (&Client::Receive, this);
    readInputThread = new std::thread (&Client::ReadInput, this);
}

void Client::ReadInput() {
    while(!stopReadInputThread){
        std::string message;
        std::getline(std::cin, message);

        clientSocket.SendString(clientSocket.GetSocketId(), message, 0);

        if (message == "exit:"){
            Stop();
            break;
        }
    }
}

void Client::Receive() {
    while(!stopReceiveThread){
        std::string message = clientSocket.ReceiveString(clientSocket.GetSocketId());

        int divider = message.find(":");
        std::string cmd = message.substr(0, message.find(":"));
        std::string text = message.substr(divider+1, message.size());
        
        if (cmd == "msg") {
            std::cout << text << "\n";
        } else if (cmd == "stop") {
            std::cout << "Server stopped!\n";
            Stop();
            break;
        }
    }
}

void Client::Stop() {
    std::unique_lock<std::mutex> lock(stopMutex);
    std::cout << "Stopping...\n";
    stopClient = true;
    lock.unlock();
    stop.notify_one();
}

void Client::WaitForStop() {
    std::unique_lock<std::mutex> lock(stopMutex);
    stop.wait(lock, [this]{return stopClient;});
    stopReceiveThread = true;
    stopReadInputThread = true;
    
    (*receiveThread).join();
    (*readInputThread).join();

    clientSocket.Close();
}
