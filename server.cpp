#include <iostream>
#include <string>
#include <string.h>
#include "lib/Server.hpp"

int main() {
    Server myServer(6660);

    myServer.Start(5);

    getchar();

    myServer.Stop();

    return 0;
}