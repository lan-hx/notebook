#include <iostream>
#include "ClientCon.h"

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    ClientCon::get_config();
    return ClientCon()(argc, argv);
}
