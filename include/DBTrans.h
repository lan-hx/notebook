//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_DBTRANS_H_
#define NOTEBOOK_SRC_DBTRANS_H_

#include <string>
#include <vector>
#include <map>

/**
 * 将相应函数翻译为SQL调用
 * 本应是一个抽象类，但是没必要
 */
class DBTrans {
public:
    static uint32_t add(const std::string &topic, const std::string &content);
    static void update(uint32_t id, const std::string &topic, const std::string &content);
    static void del(uint32_t id);
    static std::vector<uint32_t> search(const std::string &word);
    static std::vector<uint32_t> list(uint32_t type);
    static std::map<uint32_t, std::string> get_topic(const std::vector<uint32_t> ids);
    static std::string get(uint32_t id);
    /**
     * 将字节码转换为函数调用，返回值转化为编码
     * @param data Server类接收到的数据
     * @return DBTrans返回的数据串行化
     */
    static std::string translate(std::string data);
};

#endif //NOTEBOOK_SRC_DBTRANS_H_
