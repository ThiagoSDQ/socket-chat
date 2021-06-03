#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include "Address.hpp"
#include <string>

class Socket {
    public:
        Socket(int domain, int type, int protocol);
        int GetSocket();
        void SetAddress(sa_family_t sin_family, int port, const char* addr);
        int Bind();
        bool IsValid();
        int Listen(int connections);
        int Accept();
        int Connect(Address addr);
        int SendString(int dest, std::string message, int flags);
        std::string ReceiveString(int source);
        void Close();

    private:
        int socket_;
        Address address; 
};

#endif