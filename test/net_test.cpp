//
// Created by lan on 2022/5/9.
//

#include "Client.h"
#include "Server.h"
#include <gtest/gtest.h>
#include <string>
#include <thread>
using namespace std;

TEST(Client, echo) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    inst.set("echo.websocket.events", 80, "/");
    inst._client_send("Hello World!", 2);
    string s;
    s = inst.get();
    EXPECT_EQ(s, "Hello World!");
}

TEST(Server, echo) {
    auto echo_func = [](const string &s) {
        return s;
    };
    auto &sinst = Server::get_ins();
    ASSERT_TRUE(&sinst);
    Client::set("localhost", 13579, "/");
    auto &cinst = Client::get_ins();
    ASSERT_TRUE(&cinst);
    auto th = std::thread(&Server::start_server, &sinst, "0.0.0.0", 13579, echo_func, false);
    cinst.send("Hello World!");
    string s;
    s = cinst.get();
    th.join();
    EXPECT_EQ(s, "Hello World!");
}
