//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_CLIENT_H_
#define NOTEBOOK_SRC_CLIENT_H_

#include <string>
//#include <boost/beast/core.hpp>
//#include <boost/beast/websocket.hpp>
//#include <boost/asio/connect.hpp>
//#include <boost/asio/ip/tcp.hpp>

/**
 * @brief client side, 负责与server进行通信，将结果转换为string，并转换为相应函数调用
 * @note 注意传出的string可能包含\0,该类的函数均可能抛出异常
 * @note 单例模式
 */
class Client {
public:
    /**
     * 向服务端发送数据
     * @param data 数据，注意其中很可能包含\0
     */
    void send(const std::string &data);
    /**
     * 从服务端接收数据
     * @return 数据，注意其中很可能包含\0
     * @note 同步函数，若没有数据将阻塞
     */
    [[nodiscard]] std::string get() const noexcept;
    /**
     * 设置网络参数
     * @param host 服务端地址
     */
    static void set(const std::string &host, int port, const std::string &path = "/") noexcept;
    /**
     * 获取Client对象
     * @return Client对象
     */
    static Client &get_ins();
    void _client_send(const std::string &data, int times = 1);
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;
private:
    Client() = default;
    ~Client() = default;
    static Client *instance;
    // todo: 存储网络相关变量、缓冲区等
    static std::string host;
    static std::string path;
    static int port;
};

#endif //NOTEBOOK_SRC_CLIENT_H_
