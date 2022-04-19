//
// Created by lan on 2022/3/22.
//

#include "ClientTrans.h"
#include "TransCode.h"
#include "Client.h"


//translate int to string
static std::string itos(int val) {
    std::string s;
    s.resize(4);
    *reinterpret_cast<int*>(s.data()) = val;
    return s;
}

//todo: not done
int ClientTrans::add(const std::string &topic, const std::string &content) {
    std::string data;
    data = OP_ADD + itos(topic.length()) + topic + itos(content.length()) + content;
    Client::get_ins().send(data);
    return 0;
}

//todo: not done
void ClientTrans::update(int id, const std::string &topic, const std::string &content) {
    std::string data;
    data = OP_UPD + itos(id) + itos(topic.length()) + topic + itos(content.length()) + content;
    Client::get_ins().send(data);
}
//todo: not done
void ClientTrans::del(int id) {
    std::string data;
    data = OP_DEL + itos(id);
    Client::get_ins().send(data);
}
//todo: not done
std::vector<int> ClientTrans::search(const std::string &word) {
    std::string data;
    data = OP_SEARCH + itos(word.length()) + word;
    Client::get_ins().send(data);
    return std::vector<int>();
}
//todo: not done
std::vector<int> ClientTrans::list(int type) {
    std::string data;
    if(type){
            data = OP_LST+TYPE_DELETE;
    }else{
            data = OP_LST+TYPE_NORMAL;
    }
    Client::get_ins().send(data);
    return std::vector<int>();
}
//todo: not done
std::string ClientTrans::get(int id) {
    std::string data;
    data = OP_GET + itos(id);
    Client::get_ins().send(data);
    return std::string();
}
//todo: not done
std::map<int, std::string> ClientTrans::get_topic(const std::vector<int> ids) {
    std::string data;
    data = OP_GET_TOPIC + itos(ids.size());
    for(int id : ids) {
        data += itos(id);
    }
    Client::get_ins().send(data);
    return std::map<int, std::string>();
}
//todo: not done
void ClientTrans::logout() {
    std::string data;
    data = OP_LOGOUT;
    Client::get_ins().send(data);
}
