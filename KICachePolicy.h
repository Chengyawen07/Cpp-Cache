#pragma once

/*
KICachePolicy 是一个模板基类，定义了缓存策略的接口。
任何缓存策略（如 LRU、LFU）都可以继承这个基类，并实现其虚函数。
这种设计符合 面向接口编程 的原则，可以方便地实现多态并扩展不同的缓存策略。
*/

namespace KamaCache {

template <typename Key, typename Value>
class KICachePolicy {

public:

    virtual ~KICachePolicy() {};

    // 虚函数： 继承这个类的后续的LRU，LFU，都必须override这个虚函数
    // 插入键值对到缓存中
    virtual void put(Key key, Value value) = 0;

    // 查找缓存中的键
    virtual bool get(Key key, Value& value) = 0;

    // 返回键对应的值，适用于不需要区分缓存命中与否的情况：
    // 如果未命中，派生类可以选择抛出异常或返回默认值。
    virtual Value get(Key key) = 0;

};

} // namespace KameCache



