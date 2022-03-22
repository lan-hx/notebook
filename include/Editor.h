//
// Created by lan on 2022/3/21.
//

#ifndef NOTEBOOK_SRC_EDITOR_H_
#define NOTEBOOK_SRC_EDITOR_H_

#include <string>

/**
 * @brief 编辑器类，负责修改笔记内容
 */
class Editor {
public:
    // 主函数返回值定义
    enum State { succeed = 0, not_changed, error };
    /**
     * 编辑器主函数，使用utf8编码
     * @param topic 文章标题
     * @param content 文章内容
     * @return 是否需要修改
     */
    State operator()(std::string &topic, std::string &content) noexcept;
    /**
     * 更改默认打开方式
     * @param open 打开方式
     * @note 例："code %s" or "native"
     */
    void change_open_with(const std::string &open) noexcept;
private:
    std::string open_with;
};

#endif //NOTEBOOK_SRC_EDITOR_H_
