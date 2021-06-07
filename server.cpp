#include <iostream>
#include <string>
#include <string.h>
#include "lib/Server.hpp"

int main() {
    Server myServer(4321, "0.0.0.0");//Same as INADDR_ANY

    myServer.Start(5);

    getchar();

    myServer.Stop();

    return 0;
}