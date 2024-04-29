#ifndef __REDIRECT_HH__
#define __REDIRECT_HH__
#include <string>

struct RedirectInfo {
    /**
     * @brief 转发的目标文件
     * 
     */
    std::string target;
    /**
     * @brief 文件的内容
     * 
     * 如果目标文件不存在，将会使用此数据创建文件。
     * 
     */
    std::string content;
    int cur = 0;
    /**
     * @brief 开始点
     * 
     * 区间范围：[start, end)
     * 
     */
    int start;
    int end;
};

#endif