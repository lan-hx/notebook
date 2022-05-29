//
// Created by lan on 2022/5/24.
//

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "DBTrans.h"
#include "macro.h"
#include "TransCode.h"
using namespace std;

TEST(DBTrans, invalid_code) {
    EXPECT_EQ(memcmp(DBTrans::translate("v").data(), "ev\x14\x00\x00\x00Illegal instruction.", 26), 0);
}

TEST(DBTrans, err_sample) {
    EXPECT_EQ(memcmp(DBTrans::translate("g\xff\xff\xff\xff").data(), "eg\x0f\x00\x00\x00note not exist.", 21), 0);
}

TEST(DBTrans, sample) {
    string str;
    char code;

    str.resize(31);
    memcpy(str.data(), "a\x07\x00\x00\x00#234321\x0f\x00\x00\x00@@@@@@@@@@@@@@@", 31);
    string res = DBTrans::translate(str);
    ASSERT_EQ(res[0], RES_ADD);
    auto id = READ_UINT32(&res[1]);

    str.resize(5);
    str[0] = OP_GET;
    memcpy(str.data()+1, &id, sizeof id);
    EXPECT_EQ(memcmp(DBTrans::translate(str).data(), "g\x0f\x00\x00\x00@@@@@@@@@@@@@@@", 20), 0);

    str.resize(9);
    str[0] = OP_GET_TOPIC;
    WRITE_UINT32(str.data()+1) = 1;
    WRITE_UINT32(str.data()+5) = id;
    char rs1[] = "t\x01\x00\x00\x00\x00\x00\x00\x00\x07\x00\x00\x00#234321";
    WRITE_UINT32(&rs1[5]) = id;
    EXPECT_EQ(memcmp(DBTrans::translate(str).data(), rs1, 20), 0);

    str.resize(36);
    char rs2[] = "u\x00\x00\x00\x00\x07\x00\x00\x00#234321\x10\x00\x00\x00@@@@@@@@@@@@@@@@";
    WRITE_UINT32(&rs2[1]) = id;
    memcpy(str.data(), rs2, 36);
    code = RES_UPD;
    EXPECT_EQ(DBTrans::translate(str), &code);
    EXPECT_EQ(DBTrans::get(id), "@@@@@@@@@@@@@@@@");

    str.resize(5);
    str[0] = OP_DEL;
    WRITE_UINT32(str.data()+1) = id;
    code = RES_DEL;
    EXPECT_EQ(DBTrans::translate(str), &code);
    EXPECT_THROW(DBTrans::get(id), runtime_error);
}
