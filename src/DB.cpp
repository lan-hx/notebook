//
// Created by lan on 2022/3/21.
//

#include <sqlite3.h>
#include <exception>
#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include "DB.h"

using namespace std;

DB *DB::instance = nullptr;
string DB::path;

DB &DB::get_ins() {
    if (!instance) {
        change_db_path("data.db");
        instance = new DB;
        //if (instance) instance->open();
    }
    return *instance;
}
//todo: not done
void DB::change_db_path(const std::string &new_path) {
    if (instance && instance->is_open()) {
        throw std::runtime_error("change path failed: database opened.");
    }
    path = new_path;
}
//todo: not done
void DB::open() {
    if (is_open()) throw std::runtime_error("open failed: database opened.");
    if (sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db));
    }
}
//todo: not done
void DB::close() {
    if (!is_open()) throw std::runtime_error("close failed: database not opened.");
    sqlite3_close_v2(db);
    db = nullptr;
}
bool DB::validate() noexcept {
    if (!is_open()) return false;
    try {
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, "PRAGMA table_info(notebook)", -1, &stmt, nullptr);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            // insert
            sqlite3_finalize(stmt);
            if (sqlite3_exec(db, "create table notebook (\n"
                                 "    id integer primary key autoincrement not null ,\n"
                                 "    topic text not null,\n"
                                 "    content text not null,\n"
                                 "    type integer not null default 0);", nullptr, nullptr, nullptr) != SQLITE_OK) {
                return false;
            }
            return true;
        } else {
            if (
                strcmp((const char *)sqlite3_column_text(stmt, 1), "id")
                    || strcmp((const char *)sqlite3_column_text(stmt, 2), "INTEGER")
                    || sqlite3_column_int(stmt, 3) != 1
                    || sqlite3_column_int(stmt, 5) != 1)
                return false;
            if (sqlite3_step(stmt) != SQLITE_ROW) return false;
            if (
                strcmp((const char *)sqlite3_column_text(stmt, 1), "topic")
                    || strcmp((const char *)sqlite3_column_text(stmt, 2), "TEXT")
                    || sqlite3_column_int(stmt, 3) != 1
                    || sqlite3_column_int(stmt, 5) != 0)
                return false;
            if (sqlite3_step(stmt) != SQLITE_ROW) return false;
            if (
                strcmp((const char *)sqlite3_column_text(stmt, 1), "content")
                    || strcmp((const char *)sqlite3_column_text(stmt, 2), "TEXT")
                    || sqlite3_column_int(stmt, 3) != 1
                    || sqlite3_column_int(stmt, 5) != 0)
                return false;
            if (sqlite3_step(stmt) != SQLITE_ROW) return false;
            if (
                strcmp((const char *)sqlite3_column_text(stmt, 1), "type")
                    || strcmp((const char *)sqlite3_column_text(stmt, 2), "INTEGER")
                    || sqlite3_column_int(stmt, 3) != 1
                    || sqlite3_column_type(stmt, 4) == SQLITE_NULL
                    || sqlite3_column_int(stmt, 4) != 0
                    || sqlite3_column_int(stmt, 5) != 0)
                return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}


