//
// Created by hp on 2025/6/28.
//

#include "vector/myvector.h"
int main() {
    // 基本使用
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << std::endl;

    // 初始化列表
    MyVector<double> vec2 = {3.14, 2.71, 1.41};
    std::cout << "vec2[0]: " << vec2[0] << std::endl;

    // 迭代器遍历
    std::cout << "Iterate with iterator: ";
    for (double & it : vec2) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    // 异常处理
    try {
        std::cout << vec2.at(10) << std::endl;  // 越界访问
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}