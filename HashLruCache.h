#pragma once

#include "LruKCache.h" // 引入 KLruKCache

namespace KamaCache {

// 定义 KHashLruCache 类
template <typename Key, typename Value>
class HashLruCache : public LruKCache<Key, Value> {
    // KHashLruCache 的声明和实现
};

} // namespace KamaCache
