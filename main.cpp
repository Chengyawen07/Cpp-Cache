#include <iostream>
#include "LfuCache.h"

using namespace KamaCache;

int main() {
    // 创建一个LfuCache，容量为3，最大平均访问频次为5
    LfuCache<int, std::string> cache(3, 5);

    // 插入几个键值对
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");

    // 测试get操作
    std::string value;
    if (cache.get(1, value)) {
        std::cout << "Key 1: " << value << std::endl;
    } else {
        std::cout << "Key 1 not found!" << std::endl;
    }

    // 访问key 2两次，增加其访问频次
    cache.get(2, value);
    cache.get(2, value);

    // 插入新的键值对，触发淘汰
    cache.put(4, "four");

    // 检查淘汰情况
    if (cache.get(3, value)) {
        std::cout << "Key 3: " << value << std::endl;
    } else {
        std::cout << "Key 3 not found (evicted)!" << std::endl;
    }

    if (cache.get(4, value)) {
        std::cout << "Key 4: " << value << std::endl;
    } else {
        std::cout << "Key 4 not found!" << std::endl;
    }

    // 清空缓存
    cache.purge();
    std::cout << "Cache purged. Checking keys..." << std::endl;

    if (cache.get(1, value)) {
        std::cout << "Key 1: " << value << std::endl;
    } else {
        std::cout << "Key 1 not found!" << std::endl;
    }

    if (cache.get(4, value)) {
        std::cout << "Key 4: " << value << std::endl;
    } else {
        std::cout << "Key 4 not found!" << std::endl;
    }

    return 0;
}
