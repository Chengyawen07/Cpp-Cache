cmake_minimum_required(VERSION 3.10)
project(CppCacheTests)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加测试目录
enable_testing()
add_subdirectory(test)
