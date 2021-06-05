#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>
#include <set>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Socket.hpp"

class Client {
    public:
        Client();
        int Start(int maxConnections);
        int ConnectToServer();
        void SetServerAddress(Address addr);
        void Start();
        void WaitForStop();

    private:
        void Receive();
        void ReadInput();
        void Stop();

        Socket clientSocket;
        Address serverAddress;
        std::thread* receiveThread;
        std::atomic_bool stopReceiveThread;    
        std::thread* readInputThread;
        std::atomic_bool stopReadInputThread;
        std::mutex stopMutex;
        std::condition_variable stop;
        bool stopClient;
};

#endif  