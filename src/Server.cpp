#include "../lib/Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

//Defines the server's socket to TCP and the socket's address to the given port and addr 
Server::Server(int port, const char *addr): serverSocket(AF_INET, SOCK_STREAM, 0) {
    serverSocket.SetAddress(AF_INET, port, addr);
}

//Start the server with a maximum number of connections;
int Server::Start(int maxConnections) {
    std::cout << "Trying to start...\n";
    if (!serverSocket.IsValid()) return 1;

    std::cout << "Biding...\n";
    if (serverSocket.Bind() < 0) return 1;

    std::cout << "Listening...\n";
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

//Wait for client connections 
void Server::WaitForConnection() {
    std::cout << "Waiting for connections...\n";
    while (!stopServer) {
        int client = serverSocket.Accept();

        if (client == -1) continue;

        //If conection was accepted sucesscifully, starts a new thread to interact wih client
        stopClientThread[client] = false;
        clientThread[client] = new std::thread (&Server::Interact, this, client);
    }

    std::cout << "Closing connections\n";

    CloseAllConnections();

    serverSocket.Close();
}

//Starts receiving messages from the client and acting accordingly
void Server::Interact(int client) {
    //Client starts in a default room
    std::size_t clientRoom = strHash("default");
    rooms[clientRoom].insert(client);
    std::string clientName = "Someone";

    std::cout << "Client " << client << " connected!\n";

    while (!stopClientThread[client]) {
        std::string message = serverSocket.ReceiveString(client);
    
        //Separates command and text from client's message
        int divider = message.find(":");
        std::string cmd = message.substr(0, message.find(":"));
        std::string text = message.substr(divider+1, message.size());

        if (cmd == "login") {
            //Changes client's name
            clientName = text;
        } else if (cmd == "room") {
            //Removes client from current room and add it to the new one
            rooms[clientRoom].erase(client);
            clientRoom = strHash(text);
            rooms[clientRoom].insert(client);
        } else if (cmd == "msg") {
            //Redirect client's message to other client in the same room
            SendToRoom(clientRoom, client, clientName + ": " + text);
        } else if (cmd == "exit"){ 
            //Stops interacting with the client
            stopClientThread[client] = true;
        }
    }  

    close(client);
}

//Send "message" to every client in "room", except "client".
void Server::SendToRoom(std::size_t room, int client, std::string message) {
    for (auto it : rooms[room]){
        if (it != client) {
            serverSocket.SendString(it, "msg:" + message, 0);
        }
    }
}

//Closes connection with every client
void Server::CloseAllConnections() {
    for (auto it : clientThread) {
        serverSocket.SendString(it.first, "stop:", 0);
        stopClientThread[it.first] = true;
    }

    for (auto it : clientThread) {
        (*it.second).join();
    }

    serverSocket.Close();
}