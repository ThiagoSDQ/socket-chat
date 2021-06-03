#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../lib/Socket.hpp"

int makeSocketNonBlocking(int socket_){
    int flags = fcntl(socket_, F_GETFL);
    if (flags != -1) {
        int setNonBlocking = fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
        if (setNonBlocking != -1){
            return 0;
        }
    }

    return -1;
}

Socket::Socket(int domain, int type, int protocol) {
    socket_ = socket(domain, type, protocol);
    
    if (socket_ != -1) {
        int nonBlocking = makeSocketNonBlocking(socket_);
        if (nonBlocking == -1){
            close(socket_);
            socket_ = -1;
        }
    }
}

int Socket::GetSocket(){
    return socket_;
}

bool Socket::IsValid() {
    return socket_ != -1;
}

void Socket::SetAddress(sa_family_t sin_family, int port, const char* addr) {
    address = Address(sin_family, port, addr);
}

int Socket::Bind() {
    return bind(socket_, (sockaddr*)&address, sizeof(sockaddr));
}

int Socket::Listen(int connections)  {
    return listen(socket_, connections);
}

int Socket::Accept() {
    int newSocket = accept(socket_, 0, 0);

    if (newSocket == -1) return -1; 

    int nonBlocking = makeSocketNonBlocking(newSocket);

    return (nonBlocking == 0 ? newSocket : -1);
}

int Socket::Connect(Address addr) {
    return connect(socket_, (sockaddr*)addr.GetSockAddr(), sizeof(*addr.GetSockAddr()));
}

int Socket::SendString(int dest, std::string message, int flags) {
    return send(dest, message.c_str(), message.size()*sizeof(char), flags);
}
std::string Socket::ReceiveString(int source) {
    char buf[400];
    memset(buf, 0, sizeof(buf));

    if (recv(source, buf, 399, 0) == -1) {
        if (errno == EWOULDBLOCK) return "";

        return "error:";
    } 

    std::string message = buf;

    return message;
}

void Socket::Close() {
    close(socket_);
}