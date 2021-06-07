#include "../lib/Client.hpp"
#include <iostream>

Client::Client(): clientSocket(AF_INET, SOCK_STREAM, 0) {
}

void Client::SetServerAddress(Address addr) {
    serverAddress = addr;
}

//Attempts to connect to server
int Client::ConnectToServer() {
    return clientSocket.Connect(serverAddress);
}

//Starts the client
int Client::Start() {
    if (clientSocket.MakeSocketNonBlocking() != 0) return 1;

    stopClient = false;
    stopReceiveThread = false;
    stopReadInputThread = false;

    //Starts two threads
    //One for receiving messages from the server
    receiveThread = new std::thread (&Client::Receive, this);
    //And one for reading input from the user
    readInputThread = new std::thread (&Client::ReadInput, this);

    return 0;
}

//Reads input from the user
void Client::ReadInput() {
    while(!stopReadInputThread){
        std::string message;
        std::getline(std::cin, message);

        clientSocket.SendString(message, 0);

        if (message == "exit:"){
            Stop();
            break;
        }
    }
}

//Receive messages from the server
void Client::Receive() {
    while(!stopReceiveThread){
        std::string message = clientSocket.ReceiveString();

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

//Sets stopClient to true, and notifies the condition variable stop
void Client::Stop() {
    std::unique_lock<std::mutex> lock(stopMutex);
    std::cout << "Stopping...\n";
    stopClient = true;
    lock.unlock();
    stop.notify_one();
}

//Blocks the thread until the client receive a command to stop,
//from the user or from the server
void Client::WaitForStop() {
    std::unique_lock<std::mutex> lock(stopMutex);
    //Blocks the thread until stopClient is set to true
    stop.wait(lock, [this]{return stopClient;});

    //Marks both threads to stop
    stopReceiveThread = true;
    stopReadInputThread = true;
    
    //Waits both threads to join
    (*receiveThread).join();
    (*readInputThread).join();

    clientSocket.Close();
}
