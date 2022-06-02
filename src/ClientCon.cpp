//
// Created by lan on 2022/3/21.
//
#include <iostream>
#include <string>
#include <cstring> 
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include "Client.h"
#include "ClientCon.h"
#include "ClientTrans.h"
#include "macro.h"
#include "Editor.h"

std::map<std::string, std::string> ClientCon::config;
auto &cconfig = ClientCon::config;
constexpr const char *config_file = "config.conf";

std::string strip(const std::string &s) {
    auto start_it = s.begin();
    auto end_it = s.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}

/**
* 读取类似以下文件格式的配置文件
* #服务器地址
* server.address = 127.0.0.1:8888
* #使用的编辑器
* open_with = notepad
*/
void ClientCon::get_config() {
    std::ifstream ifs(config_file);
    if (!ifs) {
        std::cerr << "fatal error: 没有找到配置文件,请修改" << config_file << std::endl;
        std::ofstream ofs(config_file);
        ofs << "# notebook config file\n"
               "version = 1\n"
               "\n"
               "# client or server\n"
               "type = client\n"
               "\n"
               "# server address\n"
               "# if you want to use a local notebook, plz use localhost\n"
               "# eg: localhost / xxx.com / 192.168.1.1\n"
               "address = localhost\n"
               "\n"
               "# listen address(server)\n"
               "# eg: 0.0.0.0\n"
               "listen_address = 0.0.0.0\n"
               "\n"
               "# server port\n"
               "# eg: 8080\n"
               "port = 8080\n"
               "\n"
               "# change open with\n"
               "# eg: notepad %s\n"
               "# eg: Code.exe -g %s\n"
               "open_with = notepad\n"
               "\n"
               "# change database filename\n"
               "# eg: note.db\n"
               "db_name = note.db\n"
               "\n"
               ;
        ofs.close();
        throw std::runtime_error("fatal error: no config file");
    }
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        //去除空字符
        std::string key, value;
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            std::cerr << "parse error: " << line << std::endl;
            continue;
        }
        key = strip(line.substr(0, pos));
        value = strip(line.substr(pos + 1));
        config[key] = value;
    }
}

//输出帮助信息
void help(){
    std::cout << "请使用如下支持的命令 " << std::endl;
    std::cout <<"添加笔记: add " << std::endl;
    std::cout << "更新笔记: update " << std::endl;
    std::cout << "删除笔记: del " << std::endl;
    std::cout << "查看所有笔记: list" << std::endl;
    std::cout << "查看已删除的笔记: list_del" << std::endl;
    std::cout << "搜索笔记: search " << std::endl;
    std::cout << "退出登录: logout" << std::endl;
    std::cout << "查看帮助信息: help" << std::endl;
    std::cout << "退出系统: exit" << std::endl;
    //to be continued
}
//add操作
void add(){
    std::cout<<"请输入笔记标题:"<<std::endl;
    std::string topic; 
    // while(true){
    //     getline(std::cin,topic);
    //     if(topic.size()==0){
    //         std::cout<<"笔记标题不能为空"<<std::endl;
    //         continue;
    //     }
    //     break;
    // }
    getline(std::cin,topic);
    std::string content;
    Editor editor;
    editor.change_open_with(cconfig["open_with"]);
    editor(topic, content);
    int id = ClientTrans::add(topic, content);
    auto &inst = Client::get_ins();
    std::string res = inst.get();
    if(res.size()!=0){
        if (res[0] == 'e') {
            std::string err;
            err.resize(READ_UINT32(res.c_str()+2));
            memcpy(err.data(), res.c_str()+6, err.size());
            std::cout<<err<<std::endl;
        } else {
            std::cout << "添加成功,笔记id为:" << id << std::endl;
        }
    }
}
//update操作
void update(){
    std::cout<<"请输入笔记id(您可以通过list或者通过search获取笔记id):";
    int id;
    while(true){
        std::cin>>id;
        if(std::cin.fail()){
            std::cout<<"输入格式错误,请重新输入"<<std::endl;
            std::cin.clear();
            std::cin.ignore(1024,'\n');
            continue;
        }
        if(id<0){
            std::cout<<"笔记id不能小于0,请重新输入";
            continue;
        }
        break;
    }
    std::string topic;
    std::string content;
    auto get_res = ClientTrans::get(id);
    auto &inst = Client::get_ins();
    std::string res = inst.get();
    if(res.size()!=0){
        if (res[0] == 'e') {
            std::string err;
            err.resize(READ_UINT32(res.c_str()+2));
            memcpy(err.data(), res.c_str()+6, err.size());
            std::cout<<err<<std::endl;
        } else {
            topic = get_res.first;
            content = get_res.second;
            Editor editor;
            editor.change_open_with(cconfig["open_with"]);
            editor(topic, content);
            ClientTrans::update(id, topic, content);
            res = inst.get();
            if(res.size()!=0){
                if (res[0] == 'e') {
                    std::string err;
                    err.resize(READ_UINT32(res.c_str()+2));
                    memcpy(err.data(), res.c_str()+6, err.size());
                    std::cout<<err<<std::endl;
                } else {
                    std::cout << "更新成功" << std::endl;
                }
            }
        }
    }
}
//del操作
void del(){
    std::cout<<"请输入笔记id(您可以通过list或者通过search获取笔记id):"<<std::endl;
    int id;
    while(true){
        std::cin>>id;
        if(std::cin.fail()){
            std::cout<<"输入格式错误,请重新输入"<<std::endl;
            std::cin.clear();
            std::cin.ignore(1024,'\n');
            continue;
        }
        if(id<=0){
            std::cout<<"笔记id不能小于0"<<std::endl;
            continue;
        }
        break;
    }
    ClientTrans::del(id);
    auto &inst = Client::get_ins();
    std::string res = inst.get();
    if(res.size()!=0){
        if (res[0] == 'e') {
            std::string err;
            err.resize(READ_UINT32(res.c_str()+2));
            memcpy(err.data(), res.c_str()+6, err.size());
            std::cout<<err<<std::endl;
        } else {
            std::cout << "删除成功" << std::endl;
        }
    }
}
//list操作
void list(char type){
    //暂时先用服务器给的id
    auto m = ClientTrans::get_topic(ClientTrans::list(type));
    std::map<uint32_t, std::string>::iterator it;
    auto &inst = Client::get_ins();
    std::string res = inst.get();
    if(res.size()!=0){
        if (res[0] == 'e') {
            std::string err;
            err.resize(READ_UINT32(res.c_str()+2));
            memcpy(err.data(), res.c_str()+6, err.size());
            std::cout<<err<<std::endl;
        } else {
            std::cout<<"\t笔记id\t笔记标题"<<std::endl;
            for(it = m.begin(); it != m.end(); it++){
                std::cout<<"\t"<<it->first<<"\t"<<it->second<<std::endl;
            }
        }
    }
}
//search操作
void search(){
    std::cout<<"请输入要查找的笔记标题:";
    std::string topic;
    while(true){
        getline(std::cin,topic);
        if(topic.size()==0){
            std::cout<<"查找内容不能为空,请重新输入:";
            continue;
        }else{
            break;
        }
    }
    auto m = ClientTrans::get_topic(ClientTrans::search(topic));
    std::map<uint32_t, std::string>::iterator it;
    auto &inst = Client::get_ins();
    std::string res = inst.get();
    if(res.size()!=0){
        if (res[0] == 'e') {
            std::string err;
            err.resize(READ_UINT32(res.c_str()+2));
            memcpy(err.data(), res.c_str()+6, err.size());
            std::cout<<err<<std::endl;
        } else {
            std::cout<<"查询结果如下:"<<std::endl;
            std::cout<<"共查询到"<<m.size()<<"条笔记"<<std::endl;
            std::cout<<"\t笔记id\t笔记标题"<<std::endl;
            for(it = m.begin(); it != m.end(); it++){
                std::cout<<"\t"<<it->first<<"\t"<<it->second<<std::endl;
            }
        }
    }
}
//logout操作
void logout(){
    // ClientTrans::logout();
    std::cout<<"退出成功"<<std::endl;
}

//交互主程序
int ClientCon::operator()(int argc, char **argv) {
    if(config["port"].empty()){
        std::cout<<"未定义端口号，请修改配置文件"<<std::endl;
        return -1;
    }
    int port = stoi(config["port"]);
    Client::set(config["host"], port);
    std::cout << "欢迎使用笔记本系统" << std::endl;
    // std::cout << "请输入您的用户名(新用户将自动创建):";
    // std::string username;
    // std::cin >> username;
    // std::cout << "请输入您的密码:";
    // std::string password;
    // std::cin >> password;
    // ClientTrans::login(username, password);
    //登录部分
    std::cout<<"您可以通过输入特定的命令来操作笔记本系统"<<std::endl;
    std::cout<<"支持的命令有:"<<std::endl;
    std::cout<<"add:添加笔记"<<std::endl;
    std::cout<<"update:更新笔记,以笔记id查找待更新的笔记"<<std::endl;
    std::cout<<"del:删除笔记,"<<std::endl;
    std::cout<<"search:查找笔记"<<std::endl;
    std::cout<<"list:查看所有笔记"<<std::endl;
    std::cout<<"list_del:查看已删除的笔记"<<std::endl;
    std::cout<<"logout:退出登录"<<std::endl;
    std::cout<<"help:查看帮助信息"<<std::endl;
    std::cout<<"exit:退出系统"<<std::endl;
    std::cout<<"请输入命令:";
    std::string command;
    std::cin >> command;
    while (command.compare("exit") != 0){
        getchar();
        if(command.compare("help") == 0){
            help();
        }else if(command.compare("add") == 0){
            add();
        }else if(command.compare("update") == 0){
            update();
        }else if(command.compare("del") == 0){
            del();
        }else if(command.compare("search") == 0){
            search();
        }else if(command.compare("list") == 0){
            list(0);
        }else if(command.compare("list_del") == 0){
            list(1);
        }else if(command.compare("logout") == 0){
            // ClientTrans::logout();
        }else if(command.compare("exit") == 0){
            break;
        }else if(command.compare("help") == 0){
            help();
        }else{
            std::cout<<"您输入的命令不存在"<<std::endl;
            std::cout<<"请输入help查看有效命令"<<std::endl;
        }
        std::cout<<"请输入命令:";
        std::cin >> command;
    }
    return 0;
}