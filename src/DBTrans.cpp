//
// Created by lan on 2022/3/21.
//

#include "DBTrans.h"
#include "TransCode.h"
#include "DB.h"
#include <string>
#include <stdexcept>
using namespace std;

//todo: not done
int DBTrans::add(const std::string &topic, const std::string &content) {
    sqlite3 *db = DB::get_ins().db;
    int ret = -1;
    sqlite3_stmt *stmt;
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
void DBTrans::update(int id, const std::string &topic, const std::string &content) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt;
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
void DBTrans::del(int id) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "delete from notebook where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) throw runtime_error(sqlite3_errmsg(db));
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
}
//todo: not done
std::vector<int> DBTrans::search(const std::string &word) {
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt;
    std::vector<int> ret;
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
std::vector<int> DBTrans::list(int type) {
    vector<int> v;
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "select id from notebook where type = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, type);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        v.push_back(sqlite3_column_int(stmt, 0));
    }
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));
    return v;
}
//todo: not done
std::map<int, std::string> DBTrans::get_topic(const std::vector<int> ids) {
    map<int, string> m;
    sqlite3 *db = DB::get_ins().db;
    sqlite3_stmt *stmt;
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

std::string DBTrans::get(int id) {
    sqlite3 *db = DB::get_ins().db;
    string ret;

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "select content from notebook where id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_ROW) throw runtime_error("note not exist.");
    ret = (const char *)sqlite3_column_text(stmt, 0);
    if (sqlite3_finalize(stmt) != SQLITE_OK) throw runtime_error(sqlite3_errmsg(db));

    return ret;
}
//todo: not done
std::string DBTrans::translate(std::string data) {
    return std::string();
}
