//
// Created by lan on 2022/3/21.
//

#include <sqlite3.h>
#include "DB.h"

DB *DB::instance = nullptr;
std::string DB::path = "data.db";

DB &DB::get_ins() {
    if (!instance) {
        instance = new DB;
        if(instance) instance->open();
    }
    return *instance;
}
//todo: not done
void DB::change_db_path(const std::string &new_path) noexcept {

}
//todo: not done
void DB::open() {

}
//todo: not done
void DB::close() {
    delete instance;
    instance = nullptr;
}


