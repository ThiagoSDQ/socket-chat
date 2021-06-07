#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <set>
#include <thread>
#include <atomic>
#include "Socket.hpp"

class Server {
    public:
        Server(int port, const char *addr);
        int Start(int maxConnections);
        void Stop();

    private:
        void WaitForConnection();
        void Interact(Socket client);
        void CloseAllConnections();
        void SendToRoom(std::size_t room, int clientId, std::string message);

        Socket serverSocket;
        std::thread* mainThread;
        std::map<int, std::thread*> clientThread;
        std::map<int, Socket> clientSocket;
        std::atomic_bool stopServer;
        std::map<int, std::atomic_bool> stopClientThread;
        std::map<std::size_t, std::set<int>> rooms;
        std::hash<std::string> strHash;
};

#endif  