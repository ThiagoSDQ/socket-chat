#include "../lib/Address.hpp"
#include <string.h>
#include <iostream>

Address::Address(){
}

Address::Address(sa_family_t sin_family, int port, const char* addr){
    address.sin_family = sin_family;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);
    memset(&address.sin_zero,0,sizeof(address.sin_zero));
}

sockaddr_in* Address::GetSockAddr() {
    return &address;
}