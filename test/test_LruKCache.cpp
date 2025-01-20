#include <gtest/gtest.h>
#include "/home/ubuntu/Cpp-Cache/LruKCache.h"

using namespace KamaCache;

// 测试 LruKCache 的基本操作
TEST(LruKCacheTest, BasicOperations) {
    LruKCache<int, std::string> cache(2, 3, 2); // 主缓存容量为 2，历史记录容量为 3，k=2

    // 插入数据到历史记录
    cache.put(1, "One");
    cache.put(2, "Two");

    std::string value;
    EXPECT_FALSE(cache.get(1, value)); // 尚未满足 k 次访问，主缓存中没有数据

    // 多次访问 key=1，满足 k 次访问条件
    cache.get(1);
    cache.get(1);

    // 调试信息
    std::cout << "After accessing key 1 twice, cache state: \n";
    EXPECT_TRUE(cache.get(1, value));
    std::cout << "Key 1 value: " << value << "\n";

    // 插入更多数据，触发缓存淘汰策略
    cache.put(3, "Three");
    EXPECT_TRUE(cache.get(3, value));  // 添加调试信息
    std::cout << "Key 3 value: " << value << "\n";
}

// 测试历史记录的容量限制
TEST(LruKCacheTest, HistoryCapacityLimit) {
    LruKCache<int, std::string> cache(2, 2, 2); // 主缓存容量为 2，历史记录容量为 2，k=2

    // 插入数据到历史记录
    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three"); // 插入第 3 个数据，触发历史记录淘汰

    // key=1 应该已经被淘汰，无法记录访问
    std::string value;
    EXPECT_FALSE(cache.get(1, value));
}
