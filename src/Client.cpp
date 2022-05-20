//
// Created by lan on 2022/3/21.
//

#include <iostream>
#include "Client.h"
#include "nb_net.h"
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

Client *Client::instance = nullptr;
static std::string receive_buffer;
std::string Client::host;
std::string Client::path = "/";
int Client::port;
static net::io_context ioc;
static tcp::resolver resolver{ioc};
static websocket::stream<tcp::socket> ws{ioc};

// todo: not done
void Client::send(const std::string &data) {
    _client_send(data, 1);
}

void Client::_client_send(const std::string &data, int times) {
    try {
        const auto results = resolver.resolve(host, std::to_string(port));
        auto ep = net::connect(ws.next_layer(), results);
        auto address = host + ':' + std::to_string(ep.port());
        ws.handshake(host, path);
        ws.write(net::buffer(data));
        beast::flat_buffer buffer;
        for (int i = 0; i < times - 1; i++) {
            ws.read(buffer);
            buffer.clear();
        }
        ws.read(buffer);
        ws.close(websocket::close_code::normal);
        receive_buffer = beast::buffers_to_string(buffer.data());
    }
    catch (std::exception const &e) {
        std::cerr << "Exception in Client::_client_send: " << e.what() << std::endl;
        throw e;
    }
}

// todo: not done
std::string Client::get() const noexcept {
    return receive_buffer;
}

Client &Client::get_ins() {
    if (!instance) {
        instance = new Client;
    }
    return *instance;
}
void Client::set(const std::string &host, int port, const std::string &path) noexcept {
    Client::host = host;
    Client::port = port;
    Client::path = path;
}
