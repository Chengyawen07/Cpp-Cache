find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})


# 1. 测试 LruCache的类
add_executable(test_LruCache test_LruCache.cpp)
# 链接 GTest 和线程库
target_link_libraries(test_LruCache GTest::GTest GTest::Main pthread)
# 注册测试
add_test(NAME LruCacheTest COMMAND test_LruCache)


# 2. 测试 LruKCache
add_executable(test_LruKCache test_LruKCache.cpp)
target_link_libraries(test_LruKCache GTest::GTest GTest::Main pthread)
add_test(NAME LruKCacheTest COMMAND test_LruKCache)
