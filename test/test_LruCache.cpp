#include <gtest/gtest.h>
#include "/home/ubuntu/Cpp-Cache/LruCache.h" // 包含你的 LruCache 头文件

using namespace KamaCache;

// 测试 LruCache 的基本插入和访问功能
TEST(LruCacheTest, BasicOperations) {
    // 创建一个容量为 2 的 LRU 缓存
    LruCache<int, std::string> cache(2);

    // 插入数据
    cache.put(1, "One");
    cache.put(2, "Two");

    // 验证数据是否正确插入
    std::string value;
    EXPECT_TRUE(cache.get(1, value)); // 验证 Key 1 是否存在
    EXPECT_EQ(value, "One");          // 验证 Key 1 的值是否正确

    EXPECT_TRUE(cache.get(2, value)); // 验证 Key 2 是否存在
    EXPECT_EQ(value, "Two");          // 验证 Key 2 的值是否正确

    // 插入新的数据，触发淘汰
    cache.put(3, "Three");

    EXPECT_FALSE(cache.get(1, value)); // Key 1 应该被淘汰
    EXPECT_TRUE(cache.get(3, value));  // Key 3 应该存在
    EXPECT_EQ(value, "Three");
}
