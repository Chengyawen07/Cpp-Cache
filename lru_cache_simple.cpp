#include <iostream>
#include <unordered_map>
#include <list>

class LRUCache {
    int capacity;
    std::list<int> keys;
    std::unordered_map<int, std::pair<int, std::list<int>::iterator>> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        keys.splice(keys.begin(), keys, cache[key].second);  // 更新最近使用顺序
        return cache[key].first;
    }

    void put(int key, int value) {
        if (capacity == 0) return;  // 防止访问 `keys.back()` 造成错误

        if (cache.find(key) != cache.end()) {
            keys.erase(cache[key].second);
        } else if (keys.size() >= capacity) {
            cache.erase(keys.back());
            keys.pop_back();
        }
        keys.push_front(key);
        cache[key] = {value, keys.begin()};
    }
};

int main() {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    std::cout << "Get key 1: " << cache.get(1) << std::endl; // 10
    cache.put(3, 30); // 淘汰 key 2
    std::cout << "Get key 2: " << cache.get(2) << std::endl; // -1
    return 0;
}
