//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_CLIENTCON_H_
#define NOTEBOOK_SRC_CLIENTCON_H_

#include <map>
class ClientCon {
public:
    /**
     * 读取所需的设置
     */
    static void get_config();
    /**
     * 控制台主函数，负责与用户交互
     * @param argc 参数个数
     * @param argv 参数列表
     * @return 程序返回值
     */
    int operator()(int argc, char **argv);
    // todo: needed data
    static std::map<std::string, std::string> config;
};

#endif //NOTEBOOK_SRC_CLIENTCON_H_
