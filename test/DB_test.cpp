//
// Created by lan on 2022/5/24.
//

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "DB.h"
#include "DBTrans.h"
using namespace std;

TEST(DB, valid) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    EXPECT_TRUE(inst.validate());
}

TEST(DB, insert_and_del) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    uint32_t id = DBTrans::add("123", "12345678好好学习天天向上");
    auto m = DBTrans::get_topic(vector<uint32_t>{id});
    cout << "retrieved topic: " << m[id] << endl;
    EXPECT_STREQ(m[id].c_str(), "123");
    auto c = DBTrans::get(id);
    cout << "retrieved content: " << c << endl;
    EXPECT_STREQ(c.c_str(), "12345678好好学习天天向上");
    DBTrans::del(id);
    auto m1 = DBTrans::get_topic(vector<uint32_t>{id});
    EXPECT_TRUE(m1.find(id)==m1.end());
    EXPECT_THROW(DBTrans::get(id), runtime_error);
}

TEST(DB, get) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    uint32_t id = DBTrans::add("1", "23");
    EXPECT_STREQ(DBTrans::get(id).c_str(), "23");
}

TEST(DB, get_null) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    uint32_t id = DBTrans::add("1", "23");
    DBTrans::del(id);
    EXPECT_THROW(DBTrans::get(id), runtime_error);
}

TEST(DB, add_null) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    uint32_t id = DBTrans::add("", "");
    auto m = DBTrans::get_topic(vector<uint32_t>{id});
    EXPECT_STREQ(m[id].c_str(), "");
    auto c = DBTrans::get(id);
    EXPECT_STREQ(c.c_str(), "");
    DBTrans::del(id);
}

//TEST(DB, del_null) {
//    auto &inst = DB::get_ins();
//    ASSERT_TRUE(&inst);
//    ASSERT_TRUE(inst.is_open());
//    EXPECT_THROW(DBTrans::del(0), runtime_error);
//}

TEST(DB, list) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    auto v = DBTrans::list(0);
    cout << "retrieved ids: ";
    for (auto &i : v) {
        cout << i << ' ';
    }
    cout << endl;
}

TEST(DB, update) {
    srand((unsigned)time(nullptr));
    string s;
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    for (int i = 0; i < rand() % 100 + 100; i++) {
        s.push_back(static_cast<char>(rand() % 26) + 'a');
    }
    cout<<"updated value: "<<s<<endl;
    DBTrans::update(3, "test 3", s);
    auto ss = DBTrans::get(3);
    EXPECT_EQ(s, ss);
}

TEST(DB, update_null) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    uint32_t id = DBTrans::add("1", "23");
    DBTrans::del(id);
    EXPECT_THROW(DBTrans::update(id, "000", "0000000"), runtime_error);
}

TEST(DB, search) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    auto v = DBTrans::search("学习");
    cout << "retrieved ids: ";
    for (auto &i : v) {
        cout << i << ' ';
    }
    cout << endl;
}

