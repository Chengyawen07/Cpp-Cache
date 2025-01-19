#include <iostream>
#include "LruCache.h"

int main() {
    using namespace KamaCache;

    // 创建一个容量为 3 的 LRU 缓存
    LruCache<int, std::string> cache(3);

    // 缓存中现在有：[1: "One", 2: "Two", 3: "Three"]
    cache.put(1, "One");
    std::cout << "Put Key 1 -> One" << std::endl;

    cache.put(2, "Two");
    std::cout << "Put Key 2 -> Two" << std::endl;

    cache.put(3, "Three");
    std::cout << "Put Key 3 -> Three" << std::endl;


    // 测试 Key 1
    std::string value;
    if (cache.get(1, value)) {
        std::cout << "Key 1 -> " << value << std::endl;
    } else {
        std::cout << "Key 1 not found!" << std::endl;
    }

    // 插入key 4 （新的数据加入到链表右侧）
    // 缓存容量已满（3 个），按照 LRU 策略，淘汰最久未使用的键 Key 2
    // 缓存更新为：[3: "Three", 1: "One", 4: "Four"]
    cache.put(4, "Four"); // Key 2 should be evicted
    if (!cache.get(2, value)) std::cout << "Key 2 evicted!" << std::endl;

    return 0;
}
