//
// Created by lan on 2022/3/21.
//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include "ClientCon.h"
#include "ClientTrans.h"
#include "Editor.h"

std::map<std::string, std::string> config;

/**
* 读取类似以下文件格式的配置文件
* #server ip port
* server.ip = 127.0.0.1
* server.port = 8888
* #open_with
* open_with = notepad
*/
void ClientCon::get_config() {
    std::ifstream ifs("config.txt");
    if (!ifs) {
        std::cout << "config.txt not found" << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        //去除空字符
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        std::stringstream ss(line);
        std::string key;
        std::string value;
        std::getline(ss, key, '=');
        std::getline(ss, value);
        config[key] = value;
    }
}

int ClientCon::operator()(int argc, char **argv) {
    Editor editor;
    if(config.find("Client.ip")==config.end()&&strcmp(argv[1],"login")!=0){
        std::cout << "请先登录,登录命令格式为: login <username> <password>" << std::endl;
        return 1;
    }
    if(strcmp(argv[1],"login")==0) {
        return 0;
    }else if(argv[1]=="add"){
        std::string topic = argv[2];
        //判断是否已有该标题
        if(ClientTrans::search(argv[2]).size()>0){
            std::cout<<"该标题已存在"<<std::endl;
            return 1;
        }
        //使用editor编辑内容
        std::string content;
        editor(topic, content);
        int id = ClientTrans::add(topic, content);
        std::cout<<"添加成功,id为"<<id<<std::endl;
    }else if(strcmp(argv[1],"update")==0){
        std::string topic = argv[3];
        std::vector <int> ids = ClientTrans::search(argv[2]);
        if(ids.size()==0||ClientTrans::get(ids[0])!=argv[3]){
            std::cout<<"该笔记不存在"<<std::endl;
            return 1;
        }
        int id = ids[0];
        Editor editor;
        std::string content;
        editor(topic, content);
        ClientTrans::update(id, topic, content);
        std::cout<<"更新成功"<<std::endl;
    }else if(strcmp(argv[1],"del")==0){
        std::vector <int> ids = ClientTrans::search(argv[2]);
        if(ids.size()==0 ||ClientTrans::get(ids[0])!=argv[3]){
            std::cout<<"该笔记不存在"<<std::endl;
            return 1;
        }
        int id = ids[0];
        ClientTrans::del(id);
        std::cout<<"删除成功"<<std::endl;
    }else if(strcmp(argv[1],"search")==0){
        std::vector <int> ids = ClientTrans::search(argv[2]);
        if(ids.size()==0){
            std::cout<<"没有找到相关笔记"<<std::endl;
            return 1;
        }
        std::cout<<"找到"<<ids.size()<<"个相关笔记"<<std::endl;
        for(int i=0;i<ids.size();i++){
            std::cout<<"id:"<<ids[i]<<"\t"<<ClientTrans::get(ids[i])<<std::endl;
        }
    }else if(strcmp(argv[1],"get")==0){
        std::vector <int> ids = ClientTrans::list(atoi(argv[2]));
        if(ids.size()==0){
            std::cout<<"没有笔记"<<std::endl;
            return 1;
        }
        std::cout<<"共有"<<ids.size()<<"篇笔记"<<std::endl;
        for(int i=0;i<ids.size();i++){
            std::cout<<"id:"<<ids[i]<<"\t"<<ClientTrans::get(ids[i])<<std::endl;
        }
    }else if(strcmp(argv[1],"list")==0){
        ClientTrans::logout();
        std::cout<<"退出成功"<<std::endl;
    }else if(strcmp(argv[1],"logout")==0){
        std::string open_with = argv[2];
        config["open_with"] = open_with;
        editor.change_open_with(open_with);
        //修改配置文件(不会)
        std::cout<<"修改成功"<<std::endl;
    }else{
        std::cout << "请使用如下格式输入命令: " << " <command> [args...]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "添加笔记: add <topic>" << std::endl;
        std::cout << "更新笔记: update <old_topic> <new_topic>" << std::endl;
        std::cout << "删除笔记: del <topic>" << std::endl;
        std::cout << "查找笔记: search <word>" << std::endl;
        std::cout << "查看所有笔记: list <type>" << std::endl;
        std::cout << "退出登录: logout" << std::endl;
        std::cout << "更改默认编辑器: change_open_with <editor>" << std::endl;
    }
    return 0;
}
