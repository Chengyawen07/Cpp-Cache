#pragma once

#include <cmath>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "KICachePolicy.h"


namespace KamaCache{

// 1. LFU类里用到的频率list
template<typename Key, typename Value> 
class FreqList {

private:
    struct Node {
        int freq;  // 访问频率
        Key key;
        Value value;
        std::shared_ptr<Node> pre;
        std::shared_ptr<Node> next;

        // 默认构造函数，初始化频率为1
        Node() : freq(1), pre(nullptr), next(nullptr) {}
        // 带参数构造函数，初始化key，并设置频率为1
        Node(Key key, Value value) :
        freq(1), key(key), value(value), pre(nullptr), next(nullptr) {}
        };

    using NodePtr = std::shared_ptr<Node>;
    int freq_;  // 表示当前链表管理的访问频率
    NodePtr head_;
    NodePtr tail_;

public:





};



// 2. LFU类
template<typename Key, typename Value> 
class LfuCache : public KICachePolicy<Key, Value> {

};



} // using namespace KamaCache

