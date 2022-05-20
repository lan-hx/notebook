//
// Created by lan on 2022/3/21.
//

#include <boost/beast/core.hpp>
#include <utility>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include "Server.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

Server *Server::instance = nullptr;
net::ip::address addr;
unsigned short _port;
net::io_context ioc{1};

Server &Server::get_ins() {
    if (!instance) {
        instance = new Server;
    }
    return *instance;
}

void do_session(tcp::socket socket, Server::exec_t &exec) {
    try {
        websocket::stream<tcp::socket> ws{std::move(socket)};
        ws.accept();
        while(true) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string data = beast::buffers_to_string(buffer.data());
            ws.write(net::buffer(exec(data)));
        }
    }
    catch(const beast::system_error &se)
    {
        // This indicates that the session was closed
        if(se.code() != websocket::error::closed) {
            std::cerr << "Session Error: " << se.code().message() << std::endl;
            throw se;
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << "Exception in Server::do_session: " << e.what() << std::endl;
        throw e;
    }
}

[[noreturn]] void Server::start_server(const std::string &address, unsigned short port, exec_t func, bool loop) {
    addr = net::ip::make_address(address);
    _port = port;
    exec = std::move(func);

    try {
        tcp::acceptor acceptor{ioc, {addr, port}};
        do {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            do_session(std::move(socket), exec);
        } while(loop);
    } catch(const std::exception &e) {
        std::cerr << "Exception in Server::start_server: " << e.what() << std::endl;
        throw e;
    }
}

