#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <arpa/inet.h>

class Address {
    public:
        Address();
        Address(sa_family_t sin_family, int port, const char* addr);
        sockaddr_in* GetSockAddr();
        void SetAddress(sockaddr_in addr);
        
    private:
        sockaddr_in address; 
};

#endif