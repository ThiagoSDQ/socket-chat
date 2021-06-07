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

Socket::Socket() {
}

Socket::Socket(int domain, int type, int protocol) {
    socket_ = socket(domain, type, protocol);
}

int Socket::GetSocketId(){
    return socket_;
}

void Socket::SetSocketId(int id){
    socket_ = id;
}

bool Socket::IsValid() {
    return socket_ != -1;
}

void Socket::SetAddress(sa_family_t sin_family, int port, const char* addr) {
    address = Address(sin_family, port, addr);
}

void Socket::SetAddress(Address addr) {
    address = addr;
}

//Bind's the socket to the socket address
int Socket::Bind() {
    return bind(socket_, (sockaddr*)&address, sizeof(sockaddr));
}

//Start listening for connections.
//Returns 0 for sucess and -1 for errors.
int Socket::Listen(int connections)  {
    return listen(socket_, connections);
}

//Accept a connection, if there is any, and make the new socket non-blocking.
//Returns the new socket.
Socket Socket::Accept() {
    Socket newSocket;
    sockaddr_in newSocketAddr;
    int addrlen = sizeof(newSocketAddr);

    int newSocketId = accept(socket_, (sockaddr *)&newSocketAddr, (socklen_t *)&addrlen);

    Address newAddress;
    newAddress.SetAddress(newSocketAddr);
    
    newSocket.SetSocketId(newSocketId);
    newSocket.SetAddress(newAddress);


    newSocket.MakeSocketNonBlocking();

    return newSocket;
}

//Open connection with given address
//Returns 0 for sucess and -1 for errors.
int Socket::Connect(Address addr) {
    if (connect(socket_, (sockaddr*)addr.GetSockAddr(), sizeof(*addr.GetSockAddr())) == -1) {
        if (errno != EINPROGRESS) {
            return -1;
        }
    }

    return 0;
}

//Send a string to the socket
int Socket::SendString(std::string message, int flags) {
    return send(socket_, message.c_str(), message.size()*sizeof(char), flags);
}

//Receives a string from the socket
std::string Socket::ReceiveString() {
    char buf[400];
    memset(buf, 0, sizeof(buf));

    if (recv(socket_, buf, 399, 0) == -1) {
        //If the recv would block, it meand there was no message waiting to be received
        if (errno == EWOULDBLOCK) return "";

        //If the error was any other, return error
        return "error:";
    } 

    std::string message = buf;

    return message;
}

void Socket::Close() {
    close(socket_);
}

//Make socket actions(send, recv, accept,...) non blocking
//Return 0 on sucess and -1 for errors
int Socket::MakeSocketNonBlocking(){
    int flags = fcntl(socket_, F_GETFL);
    if (flags != -1) {
        int setNonBlocking = fcntl(socket_, F_SETFL, flags | O_NONBLOCK);
        if (setNonBlocking != -1){
            return 0;
        }
    }

    return -1;
}