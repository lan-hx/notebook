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
    static int add(const std::string &topic, const std::string &content);
    static void update(int id, const std::string &topic, const std::string &content);
    static void del(int id);
    static std::vector<int> search(const std::string &word);
    static std::vector<int> list(int type);
    static std::map<int, std::string> get_topic(const std::vector<int> ids);
    static std::string get(int id);
    /**
     * 将字节码转换为函数调用，返回值转化为编码
     * @param data Server类接收到的数据
     * @return DBTrans返回的数据串行化
     */
    static std::string translate(std::string data);
};

#endif //NOTEBOOK_SRC_DBTRANS_H_
