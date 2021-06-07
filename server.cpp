#include <iostream>
#include <string>
#include <string.h>
#include "lib/Server.hpp"

int main() {
    Server myServer(4321, "0.0.0.0");//Same as INADDR_ANY
    int maxConnections = 10;

    if(myServer.Start(maxConnections) == 1){
        std::cout << "Failed to start server!\n";
        std::cout << strerror(errno) << "\n";
        return 1;
    }

    getchar();

    myServer.Stop();

    return 0;
}