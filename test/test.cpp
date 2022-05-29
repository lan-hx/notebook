//
// Created by lan on 2022/4/17.
//
#include <gtest/gtest.h>
#include "DB.h"
using namespace std;

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    auto &inst = DB::get_ins();
    inst.open();
    return RUN_ALL_TESTS();
}