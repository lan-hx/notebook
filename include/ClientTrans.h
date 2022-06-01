//
// Created by lan on 2022/3/22.
//

#ifndef NOTEBOOK_SRC_CLIENTTRANS_H_
#define NOTEBOOK_SRC_CLIENTTRANS_H_

#include <string>
#include <vector>
#include <map>

/**
 * 将相应函数翻译为字节码送给Client
 * 本应是一个抽象类，但是没必要
 */
class ClientTrans {
public:
    //static void login(std::string username, std::string password);
    static uint32_t add(const std::string &topic, const std::string &content);
    static void update(uint32_t id, const std::string &topic, const std::string &content);
    static void del(uint32_t id);
    static std::vector<uint32_t> search(const std::string &word);
    static std::vector<uint32_t> list(char type);
    static std::pair<std::string, std::string> get(uint32_t id);
    static std::map<uint32_t, std::string> get_topic(const std::vector<uint32_t> ids);
    //static void logout();
};


#endif //NOTEBOOK_SRC_CLIENTTRANS_H_
