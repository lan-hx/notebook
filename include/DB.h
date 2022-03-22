//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_DB_H_
#define NOTEBOOK_SRC_DB_H_

#include <string>

/**
 * 管理数据库实例
 */
class DB {
public:
    /**
     * 更改访问的数据库路径
     * @param new_path 路径
     */
    static void change_db_path(const std::string &new_path) noexcept;
    /**
     * 打开数据库
     */
    void open();
    /**
     * 关闭数据库
     */
    void close();
    /**
     * 获取数据库对象
     * @return 数据库对象
     */
    static DB &get_ins();
    DB(const DB &d) = delete;
    sqlite3 *h = nullptr;
private:
    static std::string path;
    static DB *instance;
    DB() = default;
    ~DB() { if (instance) close(); }
};

#endif //NOTEBOOK_SRC_DB_H_
