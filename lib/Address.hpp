#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <arpa/inet.h>

class Address {
    public:
        Address();
        Address(sa_family_t sin_family, int port, const char* addr);
        sockaddr_in* GetSockAddr();

    private:
        sockaddr_in address; 
};

#endif