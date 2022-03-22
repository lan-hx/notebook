//
// Created by lan on 2022/3/21.
//

#include "Server.h"

Server *Server::instance = nullptr;

Server &Server::get_ins() {
    if (!instance) {
        instance = new Server;
    }
    return *instance;
}
