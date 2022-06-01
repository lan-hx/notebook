//
// Created by lan on 2022/3/22.
//

#include "ServerCon.h"
#include "Server.h"
#include <fstream>
#include <iostream>
#include <string>
#include <DBTrans.h>
#include <map>

using namespace std;

std::map<std::string, std::string> config;
constexpr const char *config_file = "config_server.conf";
std::string strip(const std::string &s);

//todo: not done
void ServerCon::get_config() {
    std::ifstream ifs(config_file);
    if (!ifs) {
        std::cerr << "fatal error: 没有找到配置文件,请修改" << config_file << std::endl;
        std::ofstream ofs(config_file);
        ofs << "# notebook config file(server side)\n"
                "version = 1\n"
                "\n"
                "# server address\n"
                "# eg: 0.0.0.0\n"
                "# address = 0.0.0.0\n"
                "\n"
                "# server port\n"
                "# eg: 8080\n"
                "# port = 8080\n";
        ofs.close();
        throw std::runtime_error("fatal error: no config file");
    }
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        auto key = strip(line.substr(0, pos));
        auto value = strip(line.substr(pos + 1));
        config[key] = value;
    }
}

//todo: not done
int ServerCon::operator()(int argc, char **argv) {
    auto &sinst = Server::get_ins();
    unsigned short port = stoul(config["port"]);
    sinst.start_server(config["address"], port, DBTrans::translate);
    return 0;
}
