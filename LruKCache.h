#pragma once

#include <cstring>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "KICachePolicy.h"
#include "LruCache.h"

namespace KamaCache{

// LRU优化：Lru-k版本, 通过继承的方式进行再优化
template<typename Key, typename Value>
class LruKCache : public LruCache<Key, Value> {

public:
    // 构造函数
    LruKCache(int capacity, int historyCapacity, int k) 
        : LruCache<Key, Value>(capacity),
        // 初始化 historyList_: historyList_ 是一个指向 LruCache<Key, size_t> 的智能指针。
          historyList_(std::make_shared<LruCache<Key, size_t>>(historyCapacity)), // 初始化历史访问记录
          k_(k) 
    {}

    // 获取数据 (支持返回布尔值和传出参数)
    bool get(Key key, Value& value) {
        int historyCount = historyList_->get(key);
        historyList_->put(key, ++historyCount);

        // 调用基类的 get 函数
        return LruCache<Key, Value>::get(key, value);
    }

    // 从缓存中获取指定键 key 对应的值，并更新访问记录。
    Value get(Key key) {
        // 获取历史访问次数
        // historyList_ 是一个指向 LruCache<Key, size_t> 的智能指针。
        // 所以这个智能指针可以直接访问LruCache对象的函数 get
        int historyCount = historyList_->get(key);
        std::cout << "Key " << key << " history access count: " << historyCount << "\n";

        // 更新历史访问记录的访问次数
        historyList_->put(key, ++historyCount);

        // 尝试从主缓存中获取数据: 因为get是主缓存的函数
        return LruCache<Key, Value>::get(key);
    }


    // 插入数据
    void put(Key key, Value value) {
        // 首先更新历史访问记录中的访问次数
        int historyCount = historyList_->get(key);
        historyList_->put(key, ++historyCount);

        // 如果历史访问次数达到阈值 k，则将数据加入主缓存
        if (historyCount >= k_) {
            // 从历史访问记录中移除该键
            historyList_->remove(key);

            // 将数据存入主缓存
            LruCache<Key, Value>::put(key, value);
        }
    }

private:
    int k_;  // 定义一个k，只有数据的访问次数超过k才会被存入缓存区
    std::shared_ptr<LruCache<Key, size_t>> historyList_; // 历史访问记录(value为访问次数)
};

} // namespace KamaCache
