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
#include "ClientCon.h"
#include "ClientTrans.h"
#include "Editor.h"

std::map<std::string, std::string> config;

/**
* 读取类似以下文件格式的配置文件
* #服务器地址
* server.address = 127.0.0.1:8888
* #使用的编辑器
* open_with = notepad
*/
void ClientCon::get_config() {
    std::ifstream ifs("config.txt");
    if (!ifs) {
        std::cout << "没有找到标准配置文件,自动创建标准配置文件" << std::endl;
        std::cout<<"请输入服务器地址:";
        std::string address;
        std::cin >> address;
        std::cout<<"请输入使用的编辑器:";
        std::string editor;
        std::cin >> editor;
        std::ofstream ofs("config.txt");
        ofs << "#服务器地址" << std::endl;
        ofs << "server.address = " << address << std::endl;
        ofs << "#使用的编辑器" << std::endl;
        ofs << "open_with = " << editor << std::endl;
        ofs.close();
        ifs = std::ifstream("config.txt");
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

bool is_repeat(std::string topic){
        std::map m = ClientTrans::get_topic(ClientTrans::search(topic));
        std::map<int, std::string>::iterator it;
        for(it = m.begin(); it != m.end(); it++){
            if(it->second == topic){
                return true;
            }
        }
        return false;
}

//输出帮助信息
void help(){
    std::cout << "请使用如下格式输入命令: " << " <command> [args...]" << std::endl;
    std::cout <<"添加笔记: add <topic>" << std::endl;
    std::cout << "更新笔记: update <topic>" << std::endl;
    std::cout << "删除笔记: del <id>" << std::endl;
    std::cout << "查看所有笔记: list" << std::endl;
    std::cout << "查看已删除的笔记: list_del" << std::endl;
    std::cout << "退出登录: logout" << std::endl;
    std::cout << "查看帮助信息: help" << std::endl;
    std::cout << "退出系统: exit" << std::endl;
    //to be continued
}
//add操作
void add(){
    std::cout<<"请输入笔记标题:"<<std::endl;
    std::string topic;
    while(true){
        getline(std::cin,topic);
        if(topic.size()==0){
            std::cout<<"笔记标题不能为空"<<std::endl;
            continue;
        }
    }
    std::string content;
    Editor editor;
    editor(topic, content);
    //判断是否有重复标题
    while(is_repeat(topic)){
        std::cout<<"该笔记已存在"<<std::endl;
        std::cout<<"请重新输入笔记标题:"<<std::endl;
        editor(topic, content);
    }
    int id = ClientTrans::add(topic, content);
    if (id == -1) {
        std::cout << "添加失败" << std::endl;
    } else {
        std::cout << "添加成功,笔记id为:" << id << std::endl;
    }
}
//update操作
void update(){
    std::cout<<"请输入笔记id:"<<std::endl;
    std::cout<<"您可以通过list或者通过search获取笔记id"<<std::endl;
    int id;
    while(true){
        std::cin>>id;
        if(id<0){
            std::cout<<"笔记id不能为负数"<<std::endl;
            continue;
        }
        //判断是否存在(后面再改)
        if(ClientTrans::get(id)==""){
            std::cout<<"笔记不存在"<<std::endl;
            continue;
        }
        break;
    }
    std::string topic;
    std::string content;
    Editor editor;
    //获取笔记内容(后面再改)
    editor(topic, content);
    //判断是否有重复标题
    while(is_repeat(topic)){
        std::cout<<"该笔记已存在"<<std::endl;
        std::cout<<"请重新输入笔记标题:"<<std::endl;
        editor(topic, content);
    }
    ClientTrans::update(id, topic, content);
    //判断是否成功(后面再写)
    std::cout<<"更新成功"<<std::endl;
}
//del操作
void del(){
    std::cout<<"请输入笔记id:"<<std::endl;
    std::cout<<"您可以通过list或者通过search获取笔记id"<<std::endl;
    int id;
    while(true){
        std::cin>>id;
        if(id<0){
            std::cout<<"笔记id不能为负数"<<std::endl;
            continue;
        }
        //判断是否存在(后面再改)
        if(ClientTrans::get(id)==""){
            std::cout<<"笔记不存在"<<std::endl;
            continue;
        }
        break;
    }
    ClientTrans::del(id);
    //判断是否成功(后面再写)
    std::cout<<"删除成功"<<std::endl;
}
//list操作
void list(int type){
    //暂时先用服务器给的id
    std::map m = ClientTrans::get_topic(ClientTrans::list(type));
    std::map<int, std::string>::iterator it;
    for(it = m.begin(); it != m.end(); it++){
        std::cout<<"笔记id:"<<it->first<<"\t笔记标题:"<<it->second<<std::endl;
    }
}
//search操作
void search(){
    std::cout<<"请输入笔记标题:"<<std::endl;
    std::string topic;
    while(true){
        getline(std::cin,topic);
        if(topic.size()==0){
            std::cout<<"笔记标题不能为空"<<std::endl;
            continue;
        }
    }
    std::map m = ClientTrans::get_topic(ClientTrans::search(topic));
    std::map<int, std::string>::iterator it;
    std::cout<<"查询结果如下:"<<std::endl;
    std::cout<<"共查询到"<<m.size()<<"条笔记"<<std::endl;
    for(it = m.begin(); it != m.end(); it++){
        std::cout<<"笔记id:"<<it->first<<"\t笔记标题:"<<it->second<<std::endl;
    }
}
//logout操作
void logout(){
    ClientTrans::logout();
    //判断是否成功(后面再写)
    std::cout<<"退出成功"<<std::endl;
}

//交互主程序
int ClientCon::operator()(int argc, char **argv) {
    std::cout << "欢迎使用笔记本系统" << std::endl;
    std::cout << "请输入您的用户名(新用户将自动创建):";
    std::string username;
    std::cin >> username;
    std::cout << "请输入您的密码:";
    std::string password;
    std::cin >> password;
    // ClientTrans::login(username, password);
    //登录部分不太会
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
    std::string command;
    std::cin >> command;
    while (command.compare("exit") != 0){
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
            ClientTrans::logout();
        }else if(command.compare("exit") == 0){
            break;
        }else{
            std::cout<<"您输入的命令不存在"<<std::endl;
            std::cout<<"请输入help查看帮助信息"<<std::endl;
        }
    }
}