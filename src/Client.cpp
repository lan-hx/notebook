//
// Created by lan on 2022/3/21.
//

#include "Client.h"

Client *Client::instance;

// todo: not done
void Client::send(std::string data) {

}

// todo: not done
std::string Client::get() {
    return std::string();
}

Client &Client::get_ins() {
    if (!instance) {
        instance = new Client;
    }
    return *instance;
}
