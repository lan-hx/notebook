//
// Created by lan on 2022/5/24.
//

#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include "ClientTrans.h"
#include "TransCode.h"
using namespace std;

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


