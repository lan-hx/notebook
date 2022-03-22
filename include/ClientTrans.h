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
    static int add(const std::string &topic, const std::string &content);
    static void update(int id, const std::string &topic, const std::string &content);
    static void del(int id);
    static std::vector<int> search(const std::string &word);
    static std::vector<int> list();
    static std::map<int, std::string> get_topic(const std::vector<int> ids);
};

#endif //NOTEBOOK_SRC_CLIENTTRANS_H_
