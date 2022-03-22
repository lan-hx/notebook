//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_SERVER_H_
#define NOTEBOOK_SRC_SERVER_H_

#include <string>

/**
 * @brief client side, 负责与server进行通信，将结果转换为string
 * @note 注意传出的string可能包含\0,该类的函数均可能抛出异常
 * @note 单例模式
 */
class Server {
public:
    /**
     * 向客户端发送数据
     * @param data 数据，注意其中很可能包含\0
     */
    void send(std::string data);
    /**
     * 从客户端接收数据
     * @return 数据，注意其中很可能包含\0
     * @note 同步函数，若没有数据将阻塞
     */
    std::string get();
    // todo: 设置工作模式、网络参数
    static void set() noexcept;
    /**
     * 获取Client对象
     * @return Client对象
     */
    static Server &get_ins();
    Server(const Server &) = delete;
private:
    Server() = default;
    static Server *instance;
    // todo: 存储网络相关变量、缓冲区等
};

#endif //NOTEBOOK_SRC_SERVER_H_
