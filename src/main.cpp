#include <iostream>
#include "ClientCon.h"
#include "ServerCon.h"
#include <thread>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    ClientCon::get_config();
    if(ClientCon::config["type"] == "client") {
        if(ClientCon::config["address"] == "localhost") {
            std::thread(ServerCon(), argc, argv).detach();
        }
        return ClientCon()(argc, argv);
    }
    else if(ClientCon::config["type"] == "server") {
        return ServerCon()(argc, argv);
    }
    else {
        cerr << "fatal error: type error" << endl;
    }
}
