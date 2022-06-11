//
// Created by lan on 2022/3/21.
//

#include "Editor.h"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>
namespace fs = std::filesystem;

// todo: not done
Editor::State Editor::operator()(std::string &topic, std::string &content) noexcept {
    std::string filename = tmpnam(nullptr);
    filename += ".txt";
    if(fs::exists(filename)) {
        std::cerr << filename << "已被占用" << std::endl;
        return error;
    }
    std::ofstream ofs(filename);
    ofs << topic << "\n" << content;
    ofs.close();
    std::string cmd = open_with.replace(open_with.find("%s"), 2, filename);
    if (system(cmd.c_str()) != 0) {
        return State::error;
    }
    std::ifstream ifs(filename);
    if (!ifs) {
        return State::error;
    }
    std::string new_topic, new_content,temp;
    std::getline(ifs, new_topic);
    getchar(); // 注意这里要把换行符去掉
    while (std::getline(ifs, temp)) {
        new_content += temp + "\n";
        if(ifs.eof()) {
            break;
        }
    }
    if (new_topic == topic && new_content == content) {
        return State::not_changed;
    }
    topic = new_topic;
    content = new_content;
    ifs.close();
    fs::remove(filename);
    return State::succeed;
}

// todo: not done
bool Editor::change_open_with(const std::string &open) noexcept {
    if(open.find("%s") == std::string::npos) {
        std::cerr << "打开方式格式错误" << std::endl;
        return false;
    }
    open_with = open;
    return true;
}
