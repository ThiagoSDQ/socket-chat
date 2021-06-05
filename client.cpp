//TEMPORARY CLIENT
#include <iostream>
#include <string>
#include <string.h>
#include <thread>
#include "lib/Client.hpp"

int main(int argc, char *argv[]) {
    Client myClient;

    Address server = Address(AF_INET, 6660, "127.0.0.1");

    myClient.SetServerAddress(server);

    myClient.ConnectToServer();

    myClient.Start();

    myClient.WaitForStop();

    return 0;
}