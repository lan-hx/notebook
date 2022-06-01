//
// Created by lan on 2022/5/24.
//

#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include "Client.h"
#include "ClientTrans.h"
#include "TransCode.h"
using namespace std;


TEST(ClientTrans,add_and_del) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test", "this is a test");
    auto m = ClientTrans::get_topic(vector<uint32_t>{id});
    EXPECT_STREQ(m[id].c_str(), "test");
    auto c = ClientTrans::get(id);
    EXPECT_STREQ(c.first.c_str(), "test");
    EXPECT_STREQ(c.second.c_str(), "this is a test");
    ClientTrans::del(id);
    auto m1 = ClientTrans::get_topic(vector<uint32_t>{id});
    EXPECT_TRUE(m1.find(id)==m1.end());
    ClientTrans::del(id);
    string res = inst.get();
    EXPECT_STREQ(res.substr(0,2).c_str(), "ed");
}
TEST(ClientTrans,get) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test2", "this is another test");
    auto c = ClientTrans::get(id);
    EXPECT_STREQ(c.first.c_str(), "test2");
    EXPECT_STREQ(c.second.c_str(), "this is another test");
    ClientTrans::del(id);
}

TEST(ClientTrans,get_null) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test3", "this is the third test");
    ClientTrans::del(id);
    ClientTrans::get(id);
    string res = inst.get();
    EXPECT_STREQ(res.substr(0,2).c_str(), "eg");
}

TEST(ClientTrans,list) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    auto v = ClientTrans::list(0);
    cout<<"retrieved ids: ";
    for(auto i:v) {
        cout<<i<<" ";
    }
    cout<<endl;
}

TEST(ClientTrans,update) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    srand((unsigned)time(nullptr));
    string s;
    for (int i = 0; i < rand() % 100 + 100; i++) {
        s.push_back(static_cast<char>(rand() % 26) + 'a');
    }
    uint32_t id = ClientTrans::add("test4", "this is the fourth test");
    ClientTrans::update(id, "test4.1", s);
    auto c = ClientTrans::get(id);
    EXPECT_STREQ(c.first.c_str(), "test4.1");
    EXPECT_STREQ(c.second.c_str(), s.c_str());
    ClientTrans::del(id);
}

TEST(ClientTrans,update_null) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test5", "this is the fifth test");
    ClientTrans::del(id);
    ClientTrans::update(id, "test5.1", "this is the fifth test.1");
    string res = inst.get();
    EXPECT_STREQ(res.substr(0,2).c_str(), "eu");
}

TEST (ClientTrans,get_topic) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test6", "this is the sixth test");
    uint32_t id1 = ClientTrans::add("test6.1", "this is the sixth test.1");
    auto v = ClientTrans::get_topic(vector<uint32_t>{id,id1});
    EXPECT_STREQ(v[id].c_str(), "test6");
    EXPECT_STREQ(v[id1].c_str(), "test6.1");
    ClientTrans::del(id);
    ClientTrans::del(id1);
}

TEST (ClientTrans,get_topic_null) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test7", "this is the seventh test");
    ClientTrans::del(id);
    ClientTrans::get_topic(vector<uint32_t>{id});
    string res = inst.get();
    EXPECT_STREQ(res.substr(0,2).c_str(), "et");
}

TEST (ClientTrans,search) {
    auto &inst = Client::get_ins();
    ASSERT_TRUE(&inst);
    uint32_t id = ClientTrans::add("test", "this is a test for search");
    auto v = ClientTrans::search("test");
    cout << "retrieved ids: ";
    for (auto i:v) {
        cout << i << " ";
    }
    cout << endl;
    ClientTrans::del(id);
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


