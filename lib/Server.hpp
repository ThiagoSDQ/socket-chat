#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <set>
#include <thread>
#include "Socket.hpp"

class Server {
    public:
        Server(int port);
        int Start(int maxConnections);
        void Stop();

    private:
        void WaitForConnection();
        void Interact(int client);
        void CloseAllConnections();
        void SendToRoom(std::size_t room, int client, std::string message);

        Socket serverSocket;
        std::thread* mainThread;
        std::map<int, std::thread*> clientThread;
        bool stopServer;
        std::map<int, bool> stopClientThread;
        std::map<std::size_t, std::set<int>> rooms;
        std::hash<std::string> strHash;
};

#endif  