#include <iostream>
#include "LruKCache.h" // 假设你的代码保存为 LruKCache.h

// run:
// g++ -std=c++17 -pthread main.cpp -o lruk_cache
// ./lruk_cache


int main() {
    using namespace KamaCache;

    // 创建一个 LRU-k 缓存
    // 主缓存容量为 3，历史访问记录容量为 5，阈值 k = 2
    LruKCache<int, std::string> cache(3, 5, 2);

    // 插入一些数据
    std::cout << "Inserting data into history list:" << std::endl;
    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");

    // 访问数据，未达到阈值 k 的数据不会进入主缓存
    std::cout << "\nAccessing data:" << std::endl;
    std::cout << "Get key 1 -> " << cache.get(1) << std::endl; // 增加历史访问记录
    std::cout << "Get key 1 again -> " << cache.get(1) << std::endl; // 达到阈值后进入主缓存

    // 插入更多数据，触发淘汰逻辑
    std::cout << "\nInserting data into main cache (with threshold checks):" << std::endl;
    cache.put(4, "Four");
    cache.put(5, "Five");

    // 验证缓存状态
    std::cout << "\nFinal cache state (direct access):" << std::endl;
    std::cout << "Get key 1 -> " << cache.get(1) << std::endl; // Key 1 应该在主缓存中
    std::cout << "Get key 4 -> " << cache.get(4) << std::endl; // Key 4 应该在主缓存中
    std::cout << "Get key 2 -> " << cache.get(2) << std::endl; // Key 2 应该被淘汰
    std::cout << "Get key 5 -> " << cache.get(5) << std::endl; // Key 5 应该在主缓存中

    return 0;
}
