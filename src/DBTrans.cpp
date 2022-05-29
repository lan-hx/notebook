//
// Created by lan on 2022/3/21.
//

#include <string>
#include <stdexcept>
#include <cstring>
#include "DBTrans.h"
#include "TransCode.h"
#include "DB.h"
#include "macro.h"
using namespace std;

//todo: not done
uint32_t DBTrans::add(const std::string &topic, const std::string &content) {
    sqlite3 *db = DB::get_ins().db;
    int ret = -1;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK)
            throw exception();
        sqlite3_prepare_v2(db, "insert into notebook (topic, content) values (?, ?)", -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, topic.c_str(), -1, nullptr);
        sqlite3_bind_text(stmt, 2, content.c_str(), -1, nullptr);
        if (sqlite3_step(stmt) != SQLITE_DONE) throw exception();
        if (sqlite3_finalize(stmt) != SQLITE_OK) throw exception();

        sqlite3_prepare_v2(db, "select last_insert_rowid()", -1, &stmt, nullptr);
        if (sqlite3_step(stmt) != SQLITE_ROW) throw exception();
        ret = sqlite3_column_int(stmt, 0);
        if (sqlite3_finalize(stmt) != SQLITE_OK) throw exception();

        if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK)
            throw exception();

    } catch (exception &e) {
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        throw runtime_error(sqlite3_errmsg(db));
    }
    return ret;
}
//todo: not done
void DBTrans::update(uint32_t id, const std::string &topic, const std::string &content) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, "select id from notebook where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_ROW) throw runtime_error("note not exist.");
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
    sqlite3_prepare_v2(db, "update notebook set topic = ?, content = ? where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, topic.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 3, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) throw runtime_error(sqlite3_errmsg(db));
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
}
//todo: not done
void DBTrans::del(uint32_t id) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, "delete from notebook where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) throw runtime_error(sqlite3_errmsg(db));
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
}
//todo: not done
std::vector<uint32_t> DBTrans::search(const std::string &word) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt = nullptr;
    std::vector<uint32_t> ret;
    sqlite3_prepare_v2(db, "select id from notebook where topic like ? or content like ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ("%" + word + "%").c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, ("%" + word + "%").c_str(), -1, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ret.push_back(sqlite3_column_int(stmt, 0));
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
    return ret;
    //throw runtime_error("search not implemented.");
    //return std::vector<int>();
}
//todo: not done
std::vector<uint32_t> DBTrans::list(uint32_t type) {
    vector<uint32_t> v;
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, "select id from notebook where type = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, type);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        v.push_back(sqlite3_column_int(stmt, 0));
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
    return v;
}
//todo: not done
std::map<uint32_t, std::string> DBTrans::get_topic(const vector<uint32_t> ids) {
    map<uint32_t, string> m;
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt = nullptr;
    string query = "select id, topic from notebook where id = ";
    for (auto &id : ids) {
        query += to_string(id);
        query += " or ";
    }
    query.pop_back();
    query.pop_back();
    query.pop_back();
    sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        m.insert(make_pair(sqlite3_column_int(stmt, 0), string((const char *)sqlite3_column_text(stmt, 1))));
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
    return m;
}

std::string DBTrans::get(uint32_t id) {
    sqlite3 *db = DB::get_ins().db;
    string ret;

    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, "select content from notebook where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_ROW) throw runtime_error("note not exist.");
    ret = (const char *)sqlite3_column_text(stmt, 0);
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));

    return ret;
}
//todo: not done
std::string DBTrans::translate(std::string data) {
    const char &ins = data[0];
    try {
        // not to use switch-case here intentionally
        if(ins == OP_GET) {
            auto s = get(READ_UINT32(&data[1]));
            string ret(5 + s.length(), RES_GET);
            WRITE_UINT32(ret.data()+1) = s.length();
            memcpy(ret.data()+5, s.data(), s.length());
            return ret;
        }
        else if(ins == OP_GET_TOPIC) {
            uint32_t num = READ_UINT32(&data[1]);
            vector<uint32_t> v;
            v.resize(num);
            memcpy(&v[0], &data[5], num*sizeof(uint32_t));
            auto m = get_topic(v);
            string s(5, RES_GET_TOPIC);
            WRITE_UINT32(&s[1]) = num;
            for(const auto &i:m) {
                string tmp(i.second.length() + 2 * sizeof(uint32_t), 0);
                WRITE_UINT32(&tmp[0]) = i.first;
                WRITE_UINT32(&tmp[4]) = i.second.length();
                memcpy(tmp.data()+8, i.second.data(), i.second.length());
                // slow code, need to rewrite here
                s += tmp;
            }
            return s;
        }
        else if(ins == OP_ADD) {
            uint32_t topic_len = READ_UINT32(&data[1]);
            uint32_t content_len = READ_UINT32(&data[5+topic_len]);
            string topic = data.substr(5, topic_len);
            string content = data.substr(9+topic_len, content_len);
            string ret(5, RES_ADD);
            WRITE_UINT32(&ret[1]) = add(topic, content);
            return ret;
        }
        else if(ins == OP_DEL) {
            uint32_t id = READ_UINT32(&data[1]);
            del(id);
            return string(1, RES_DEL);
        }
        else if(ins == OP_UPD) {
            uint32_t id = READ_UINT32(&data[1]);
            uint32_t topic_len = READ_UINT32(&data[5]);
            uint32_t content_len = READ_UINT32(&data[9+topic_len]);
            string topic = data.substr(9, topic_len);
            string content = data.substr(13+topic_len, content_len);
            update(id, topic, content);
            return string(1, RES_UPD);
        }
        else if(ins == OP_SEARCH) {
            uint32_t len = READ_UINT32(&data[1]);
            string word = data.substr(5, len);
            auto v = search(word);
            string ret(5 + 4 * v.size(), RES_SEARCH);
            for(size_t i = 0; i < v.size(); i++) {
                WRITE_UINT32(ret.data()+5+4*i) = v[i];
            }
            return ret;
        }
        else if(ins == OP_LST) {
            uint32_t type = READ_UINT32(&data[1]);
            auto v = list(type);
            string ret(5 + 4 * v.size(), RES_LST);
            for(size_t i = 0; i < v.size(); i++) {
                WRITE_UINT32(ret.data()+5+4*i) = v[i];
            }
            return ret;
        }
        else {
            throw runtime_error("Illegal instruction.");
        }
    }
    catch(exception &e) {
        string ret(6 + strlen(e.what()), RES_ERR);
        ret[1] = data[0];
        WRITE_UINT32(ret.data()+2) = strlen(e.what());
        strcpy(ret.data()+6, e.what());
        return ret;
    }
    catch(...) {}
    return string() + RES_ERR + data[0] + "Unknown error.";
}
