#include <iostream>
#include "LruCache.h" // 假设你的代码保存为 LruCache.h

int main() {
    using namespace KamaCache;

    // 创建一个容量为 3 的 LRU 缓存
    LruCache<int, std::string> cache(3);

    // 插入数据
    std::cout << "=== Inserting Data ===" << std::endl;
    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");

    // 验证插入是否成功
    std::string value;
    if (cache.get(1, value)) {
        std::cout << "Get key 1 -> " << value << std::endl; // 输出: Get key 1 -> One
    }

    if (cache.get(2, value)) {
        std::cout << "Get key 2 -> " << value << std::endl; // 输出: Get key 2 -> Two
    }

    // 插入更多数据，触发淘汰逻辑
    std::cout << "\n=== Inserting More Data ===" << std::endl;
    cache.put(4, "Four"); // 触发淘汰，Key 3 将被移除
    if (!cache.get(3, value)) {
        std::cout << "Key 3 evicted!" << std::endl; // 输出: Key 3 evicted!
    }

    if (cache.get(4, value)) {
        std::cout << "Get key 4 -> " << value << std::endl; // 输出: Get key 4 -> Four
    }

    // 验证最近使用逻辑
    std::cout << "\n=== Accessing Key 2 ===" << std::endl;
    if (cache.get(2, value)) {
        std::cout << "Get key 2 -> " << value << std::endl; // 输出: Get key 2 -> Two
    }

    // 再插入数据，触发淘汰逻辑
    std::cout << "\n=== Inserting Key 5 ===" << std::endl;
    cache.put(5, "Five"); // Key 1 将被淘汰
    if (!cache.get(1, value)) {
        std::cout << "Key 1 evicted!" << std::endl; // 输出: Key 1 evicted!
    }

    if (cache.get(5, value)) {
        std::cout << "Get key 5 -> " << value << std::endl; // 输出: Get key 5 -> Five
    }

    return 0;
}
