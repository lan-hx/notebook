//
// Created by lan on 2022/4/17.
//
#include <gtest/gtest.h>
#include "DBTrans.h"
#include "DB.h"
#include "ClientTrans.h"
#include "Client.h"
#include "TransCode.h"
#include <vector>
#include <cstring>
using namespace std;

TEST(DB, valid) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    EXPECT_TRUE(inst.validate());
}

TEST(DB, get) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    EXPECT_STREQ(DBTrans::get(2).c_str(), "23");
}

TEST(DB, get_null) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    EXPECT_THROW(DBTrans::get(0), runtime_error);
}

TEST(DB, insert_and_del) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    int id = DBTrans::add("123", "12345678好好学习天天向上");
    auto m = DBTrans::get_topic(vector<int>{id});
    cout << "retrieved topic: " << m[id] << endl;
    EXPECT_STREQ(m[id].c_str(), "123");
    auto c = DBTrans::get(id);
    cout << "retrieved content: " << c << endl;
    EXPECT_STREQ(c.c_str(), "12345678好好学习天天向上");
    DBTrans::del(id);
    auto m1 = DBTrans::get_topic(vector<int>{id});
    EXPECT_TRUE(m1.find(id)==m1.end());
    EXPECT_THROW(DBTrans::get(id), runtime_error);
}

TEST(DB, add_null) {
    auto &inst = DB::get_ins();
    ASSERT_TRUE(&inst);
    ASSERT_TRUE(inst.is_open());
    int id = DBTrans::add("", "");
    auto m = DBTrans::get_topic(vector<int>{id});
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
        s.push_back(rand() % 26 + 'a');
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
    EXPECT_THROW(DBTrans::update(0, "000", "0000000"), runtime_error);
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
//TEST(ClientTrans, logout) {
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::logout();
//    string s;
//    s=OP_LOGOUT;
//
//}
TEST(ClientTrans,add) {
    string s = OP_ADD+itos(strlen("123"))+"123"+itos(strlen("12345678好好学习天天向上"))+"12345678好好学习天天向上";
    const char *p = "a\x03\0\0\000123\x18\0\0\00012345678好好学习天天向上";
    EXPECT_TRUE(memcmp(p, s.c_str(), s.size()) == 0);
}
//
//TEST(ClientTrans,del) {
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::del(3);
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    string s;
//    s=OP_DEL+itos(3);
//    EXPECT_EQ(inst.get(),s);
//}
//
//TEST (ClientTrans,update) {
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    string s,y;
//    for (int i = 0; i < rand() % 100 + 100; i++) {
//        s.push_back(rand() % 26 + 'a');
//    }
//    cout<<"updated value: "<<s<<endl;
//    ClientTrans::update(3, "test 3", s);
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    y=OP_UPD+itos(3)+itos(strlen("test 3"))+"test 3"+itos(s.length())+s;
//    EXPECT_EQ(inst.get(),s);
//}
//
//TEST(ClientTrans,search) {
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::search("123");
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    string s;
//    s=OP_SEARCH+itos(strlen("123"))+"123";
//    EXPECT_EQ(inst.get(),s);
//}
//
//TEST (ClientTrans,list) {
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::list(0);
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    string s;
//    s=OP_LST+itos(0);
//    EXPECT_EQ(inst.get(),s);
//}
//
//TEST (ClientTrans,get){
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::get(3);
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    string s;
//    s=OP_GET+itos(3);
//    EXPECT_EQ(inst.get(),s);
//}
//
//TEST (ClientTrans,get_topic){
//    auto &inst = Client::get_ins();
//    ASSERT_TRUE(&inst);
//    ClientTrans::get_topic(vector<int>{1,2,3});
//    cout<<"retrieved Byte-code: "<<inst.get()<<endl;
//    string s;
//    s=OP_GET_TOPIC+itos(3)+itos(1)+itos(2)+itos(3);
//    EXPECT_EQ(inst.get(),s);
//}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    auto &inst = DB::get_ins();
    inst.open();
    return RUN_ALL_TESTS();
}