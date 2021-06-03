#include "../lib/Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int port): serverSocket(AF_INET, SOCK_STREAM, 0) {
    serverSocket.SetAddress(AF_INET, port, "0.0.0.0");
}

int Server::Start(int maxConnections) {
    std::cout << "Trying to start...\n";
    if (!serverSocket.IsValid()) return 1;

    if (serverSocket.Bind() < 0) return 1;

    if (serverSocket.Listen(maxConnections) != 0) return 1;

    std::cout << "Starting server!\n";

    stopServer = false;
    mainThread = new std::thread (&Server::WaitForConnection, this);

    return 0;
}

void Server::Stop() {
    std::cout << "Stopping...\n";
    stopServer = true;

    (*mainThread).join();
}

void Server::WaitForConnection() {
    std::cout << "Waiting for connections...\n";
    while (!stopServer) {
        int client = serverSocket.Accept();

        if (client == -1) continue;

        stopClientThread[client] = false;
        clientThread[client] = new std::thread (&Server::Interact, this, client);
    }

    std::cout << "Closing connections\n";

    CloseAllConnections();

    serverSocket.Close();
}

void Server::Interact(int client) {
    std::size_t clientRoom;
    std::string clientName;

    std::cout << "Client " << client << " connected!\n";

    while (!stopClientThread[client]) {
        std::string message = serverSocket.ReceiveString(client);

        int divider = message.find(":");
        std::string cmd = message.substr(0, message.find(":"));
        std::string text = message.substr(divider+1, message.size());

        if (cmd == "login") {
            clientName = text;
        } else if (cmd == "room") {
            clientRoom = strHash(text);
            rooms[clientRoom].insert(client);
        } else if (cmd == "msg") {
            SendToRoom(clientRoom, client, clientName + ": " + text);
        } else if (cmd == "exit"){
            stopClientThread[client] = true;
        }
    }  

    std::cout << "Disconnected " << client << "\n";
    close(client);
}

void Server::SendToRoom(std::size_t room, int client, std::string message) {
    for (auto it : rooms[room]){
        if (it != client) {
            serverSocket.SendString(it, "msg: " + message, 0);
        }
    }
}

void Server::CloseAllConnections() {
    for (auto it : clientThread) {
        stopClientThread[it.first] = true;
    }

    for (auto it : clientThread) {
        (*it.second).join();
    }
}