//
// Created by lan on 2022/3/22.
//

#include "ServerCon.h"
#include "Server.h"
#include "DB.h"
#include <fstream>
#include <iostream>
#include <string>
#include <DBTrans.h>
#include <map>

using namespace std;

std::map<std::string, std::string> ServerCon::config;
auto &sconfig = ServerCon::config;
constexpr const char *config_file = "config.conf";
std::string strip(const std::string &s);

//todo: not done
void ServerCon::get_config() {
    std::ifstream ifs(config_file);
    if (!ifs) {
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
        sconfig[key] = value;
    }
}

//todo: not done
int ServerCon::operator()(int argc, char **argv) {
    get_config();
    DB::change_db_path(sconfig["db_path"]);
    auto &db = DB::get_ins();
    db.open();
    if(!db.validate()) {
        cerr<<"fatal error: 数据库结构错误"<<endl;
    }
    auto &sinst = Server::get_ins();
    unsigned short port = stoul(sconfig["port"]);
    sinst.start_server(sconfig["listen_address"], port, DBTrans::translate);
    return 0;
}
