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

    if (serverSocket.Bind() < 0) return 1;

    if (serverSocket.Listen(maxConnections) != 0) return 1;

    if (serverSocket.MakeSocketNonBlocking() != 0) return 1;

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
        Socket client = serverSocket.Accept();

        if (client.GetSocketId() == -1) continue;

        //If conection was accepted sucesscifully, starts a new thread to interact wih client
        clientSocket[client.GetSocketId()] = client;
        stopClientThread[client.GetSocketId()] = false;
        clientThread[client.GetSocketId()] = new std::thread (&Server::Interact, this, client);
    }

    std::cout << "Closing connections\n";

    CloseAllConnections();

    serverSocket.Close();
}

//Starts receiving messages from the client and acting accordingly
void Server::Interact(Socket client) {
    //Client starts in a default room
    std::size_t clientRoom = strHash("default");
    rooms[clientRoom].insert(client.GetSocketId());
    std::string clientName = "Someone";

    std::cout << "Client " << client.GetSocketId() << " connected!\n";

    while (!stopClientThread[client.GetSocketId()]) {
        std::string message = client.ReceiveString();
    
        //Separates command and text from client's message
        int divider = message.find(":");
        std::string cmd = message.substr(0, divider);
        std::string text = message.substr(divider+1, message.size());

        if (cmd == "login") {
            //Changes client's name
            clientName = text;
        } else if (cmd == "room") {
            //Removes client from current room and add it to the new one
            rooms[clientRoom].erase(client.GetSocketId());
            clientRoom = strHash(text);
            rooms[clientRoom].insert(client.GetSocketId());
        } else if (cmd == "msg") {
            //Redirect client's message to other client in the same room
            SendToRoom(clientRoom, client.GetSocketId(), clientName + ": " + text);
        } else if (cmd == "exit"){ 
            //Stops interacting with the client
            stopClientThread[client.GetSocketId()] = true;
        } else if (cmd == "error"){
            client.SendString("msg:#Server: Failed to receive last message.", 0);
        }
    }  

    client.Close();
}

//Send "message" to every client in "room", except "client".
void Server::SendToRoom(std::size_t room, int clientId, std::string message) {
    for (auto it : rooms[room]){
        if (it != clientId) {
            clientSocket[it].SendString("msg:" + message, 0);
        }
    }
}

//Closes connection with every client
void Server::CloseAllConnections() {
    for (auto it : clientThread) {
        clientSocket[it.first].SendString("stop:", 0);
        clientSocket[it.first].Close();
        stopClientThread[clientSocket[it.first].GetSocketId()] = true;
    }

    for (auto it : clientThread) {
        (*it.second).join();
    }

    serverSocket.Close();
}