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
    size_t len = topic.size() + content.size() + 9;
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

string _update_send(uint32_t id, const std::string &topic, const std::string &content) {
    string str;
    size_t len = topic.size() + content.size() + 13;
    str.resize(len);
    str[0] = OP_UPD;
    *reinterpret_cast<uint32_t *>(str.data()+1) = id;
    *reinterpret_cast<uint32_t *>(str.data()+5) = topic.length();
    memcpy(str.data()+9, topic.data(), topic.length());
    *reinterpret_cast<uint32_t *>(str.data()+9+topic.length()) = content.length();
    memcpy(str.data()+13+topic.length(), content.data(), content.length());
    return str;
}
void _update_recv(const string &data) {
    ASSERT(data[0] == RES_UPD, "unexpected data from server");
}
//todo: not done
void ClientTrans::update(uint32_t id, const std::string &topic, const std::string &content) {
    auto data = _update_send(id, topic, content);
    auto &inst = Client::get_ins();
    inst.send(data);
    data = inst.get();
    _update_recv(data);
}

string _del_send(uint32_t id) {
    string str;
    str.resize(5);
    str[0] = OP_DEL;
    *reinterpret_cast<uint32_t *>(str.data()+1) = id;
    return str;
}
void _del_recv(const string &data) {
    ASSERT(data[0] == RES_DEL, "unexpected data from server");
}
//todo: not done
void ClientTrans::del(uint32_t id) {
    auto data = _del_send(id);
    auto &inst = Client::get_ins();
    inst.send(data);
    data = inst.get();
    _del_recv(data);
}

string _search_send(const std::string &word) {
    string str;
    size_t len = word.size() + 5;
    str.resize(len);
    str[0] = OP_SEARCH;
    *reinterpret_cast<uint32_t *>(str.data()+1) = word.length();
    memcpy(str.data()+5, word.data(), word.length());
    return str;
}
vector<uint32_t> _search_recv(const string &data) {
    ASSERT(data[0] == RES_SEARCH, "unexpected data from server");
    vector<uint32_t> ids;
    uint32_t num = *reinterpret_cast<const uint32_t *>(data.data()+1);
    uint32_t id = 0;
    for (size_t i = 5; i < num; i += 4) {
        id = *reinterpret_cast<const uint32_t *>(data.data() + i);
        ids.push_back(id);
    }
    return ids;
} 
//todo: not done
vector<uint32_t> ClientTrans::search(const std::string &word) {
    auto data = _search_send(word);
    auto &inst = Client::get_ins();
    inst.send(data);
    data = inst.get();
    return _search_recv(data);
}

string _list_send (char type) {
    string str;
    str.resize(2);
    str[0] = OP_LST;
    str[1] = type;
    return str;
}
vector<uint32_t> _list_recv(const string &data) {
    ASSERT(data[0] == RES_LST, "unexpected data from server");
    vector<uint32_t> ids;
    uint32_t num = *reinterpret_cast<const uint32_t *>(data.data()+1);
    uint32_t id = 0;
    for (size_t i = 5; i < num; i += 4) {
        id = *reinterpret_cast<const uint32_t *>(data.data() + i);
        ids.push_back(id);
    }
    return ids;
}
//todo: not done
std::vector<uint32_t> ClientTrans::list(char type) {
    auto &inst = Client::get_ins();
    inst.send(_list_send(type));
    return _list_recv(inst.get());
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
string _get_topic_send(const vector<uint32_t> ids) {
    string str;
    str.resize(5 + ids.size() * 4);
    str[0] = OP_GET_TOPIC;
    *reinterpret_cast<uint32_t *>(str.data()+1) = ids.size();
    for (size_t i = 0; i < ids.size(); i++) {
        *reinterpret_cast<uint32_t *>(str.data()+5+i*4) = ids[i];
    }
    return str;
}
map <uint32_t, string> _get_topic_recv(const string &data) {
    ASSERT(data[0] == RES_GET_TOPIC, "unexpected data from server");
    map <uint32_t, string> topics;
    uint32_t id = 0;
    uint32_t topic_len = 0;
    for (size_t i = 1; i < data.size(); i += 8 + topic_len) {
        id = *reinterpret_cast<const uint32_t *>(data.data() + i);
        topic_len = *reinterpret_cast<const uint32_t *>(data.data() + i + 4);
        string topic;
        topic.resize(topic_len);
        memcpy(topic.data(), data.data() + i + 8, topic_len);
        topics[id] = topic;
    }
    return topics;
}
//todo: not done
map <uint32_t, string> ClientTrans::get_topic(const vector<uint32_t> ids) {
    auto &inst = Client::get_ins();
    inst.send(_get_topic_send(ids));
    return _get_topic_recv(inst.get());
}
//todo: not done
//void ClientTrans::logout() {
//    std::string data;
//    data = OP_LOGOUT;
//    Client::get_ins().send(data);
//}
