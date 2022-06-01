//
// Created by lan on 2022/3/21.
//

#include "Editor.h"
#include <fstream>
#include <string>
#include <map>

// todo: not done
Editor::State Editor::operator()(std::string &topic, std::string &content) noexcept {
    std::ofstream ofs("note.txt");
    ofs << topic << "\n" << content;
    ofs << content;
    ofs.close();
    std::string cmd = open_with + " note.txt";
    if (system(cmd.c_str()) != 0) {
        return State::error;
    }
    std::ifstream ifs("note.txt");
    if (!ifs) {
        return State::error;
    }
    std::string new_topic, new_content,temp;
    std::getline(ifs, new_topic);
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
    system("del note.txt");
    return State::succeed;
}

// todo: not done
void Editor::change_open_with(const std::string &open) noexcept {
    open_with = open;
}
