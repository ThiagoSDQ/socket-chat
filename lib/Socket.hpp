#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include "Address.hpp"
#include <string>

class Socket {
    public:
        Socket();
        Socket(int domain, int type, int protocol);
        int GetSocketId();
        void SetSocketId(int id);
        void SetAddress(sa_family_t sin_family, int port, const char* addr);
        void SetAddress(Address addr);
        int Bind();
        bool IsValid();
        int Listen(int connections);
        Socket Accept();
        int Connect(Address addr);
        int MakeSocketNonBlocking();
        int SendString(std::string message, int flags);
        std::string ReceiveString();
        void Close();

    private:
        int socket_;
        Address address; 
};

#endif