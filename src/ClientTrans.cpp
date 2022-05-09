//
// Created by lan on 2022/3/22.
//

#include "ClientTrans.h"
#include "TransCode.h"
#include "Client.h"
#include <cassert>
using namespace std;

#define ASSERT(expr, message) assert((expr) && (message))

//translate int to string
std::string itos(int val) {
    std::string s;
    s.resize(4);
    *reinterpret_cast<int *>(s.data()) = val;
    return s;
}

string _add_send(const std::string &topic, const std::string &content) {
    string str;
    size_t len = topic.size() + content.size() + 8;
    str.resize(len);
    str[0] = OP_ADD;
    *reinterpret_cast<uint32_t *>(str.data()+1) = topic.length();
    memcpy(str.data()+5, topic.data(), topic.length());
    *reinterpret_cast<uint32_t *>(str.data()+5+topic.length()) = content.length();
    memcpy(str.data()+9+topic.length(), content.data(), content.length());
    return str;
}

uint32_t _add_recv(const string &data) {
    ASSERT(data[0] == RES_ADD, "unexpected data from server");
    return *reinterpret_cast<const uint32_t *>(data.data() + 1);
}

//todo: not done
uint32_t ClientTrans::add(const std::string &topic, const std::string &content) {
    auto data = _add_send(topic, content);
    auto &inst = Client::get_ins();
    inst.send(data);
    data = inst.get();
    return _add_recv(data);
}

//todo: not done
void ClientTrans::update(uint32_t id, const std::string &topic, const std::string &content) {
    std::string data;
    data = OP_UPD + itos(id) + itos(topic.length()) + topic + itos(content.length()) + content;
    Client::get_ins().send(data);
}
//todo: not done
void ClientTrans::del(uint32_t id) {
    std::string data;
    data = OP_DEL + itos(id);
    Client::get_ins().send(data);
}
//todo: not done
std::vector<uint32_t> ClientTrans::search(const std::string &word) {
    std::string data;
    data = OP_SEARCH + itos(word.length()) + word;
    Client::get_ins().send(data);
    return std::vector<uint32_t>();
}
//todo: not done
std::vector<uint32_t> ClientTrans::list(int type) {
    std::string data;
    if (type) {
        data = OP_LST + TYPE_DELETE;
    } else {
        data = OP_LST + TYPE_NORMAL;
    }
    Client::get_ins().send(data);
    return std::vector<uint32_t>();
}

string _get_send(uint32_t id) {
    string str;
    str.resize(5);
    str[0] = OP_GET;
    *reinterpret_cast<uint32_t *>(str.data()+1) = id;
    return str;
}

pair<string, string> _get_recv(const string &data) {
    ASSERT(data[0] == RES_GET, "unexpected data from server");
    auto topic_len = *reinterpret_cast<const unsigned *>(data.data() + 1);
    auto content_len = *reinterpret_cast<const unsigned *>(data.data() + 1 + 4 + topic_len);
    string topic(topic_len, 0), content(content_len, 0);
    memcpy(topic.data(), data.data() + 1 + 4, topic_len);
    memcpy(content.data(), data.data() + 1 + 8 + topic_len, content_len);
    return make_pair(topic, content);
}

//todo: not done
pair<string, string> ClientTrans::get(uint32_t id) {
    auto &inst = Client::get_ins();
    inst.send(_get_send(id));
    return _get_recv(inst.get());
}
//todo: not done
std::map<uint32_t, std::string> ClientTrans::get_topic(const vector<uint32_t> ids) {
    std::string data;
    data = OP_GET_TOPIC + itos(ids.size());
    for (int id : ids) {
        data += itos(id);
    }
    Client::get_ins().send(data);
    return std::map<uint32_t, std::string>();
}
//todo: not done
//void ClientTrans::logout() {
//    std::string data;
//    data = OP_LOGOUT;
//    Client::get_ins().send(data);
//}
