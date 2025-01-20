#pragma once

#include <cmath>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "KICachePolicy.h"

namespace KamaCache {

// 1. LFU类里用到的频率list
template <typename Key, typename Value>
class FreqList {

public:
    // Node 的作用是封装一个缓存节点
    struct Node {
        int freq; // 访问频率
        Key key;
        Value value;
        std::shared_ptr<Node> pre;
        std::shared_ptr<Node> next;

        // 默认构造函数，初始化频率为1
        Node() : freq(1), pre(nullptr), next(nullptr) {}

        // 带参数构造函数，初始化key，并设置频率为1
        Node(Key key, Value value) : freq(1), key(key), value(value), pre(nullptr), next(nullptr) {}
    };

    using NodePtr = std::shared_ptr<Node>;

private:
    int freq_;   // 表示当前链表管理的访问频率
    NodePtr head_;
    NodePtr tail_;

public:
    // 构造函数
    explicit FreqList(int n) : freq_(n) {
        // 初始化双向链表
        head_ = std::make_shared<Node>();
        tail_ = std::make_shared<Node>();
        head_->next = tail_;
        tail_->pre = head_;
    }

    // 检查链表是否为空
    bool isEmpty() const {
        return head_->next == tail_;
    }

    // 新增node，将节点插入链表尾部
    void addNode(NodePtr node) {
        if (!node || !head_ || !tail_) {
            throw std::invalid_argument("Invalid node or uninitialized head/tail.");
        }
        if (node->pre || node->next) {
            throw std::runtime_error("Node is already in a list.");
        }

        node->pre = tail_->pre;
        node->next = tail_;
        tail_->pre->next = node;
        tail_->pre = node;
    }

    // 从链表中移除节点
    void removeNode(NodePtr node) {
        if (!node || !head_ || !tail_) {
            throw std::invalid_argument("Invalid node or uninitialized head/tail.");
        }
        if (!node->pre || !node->next) {
            throw std::runtime_error("Node is not in a list.");
        }

        // 调整node前后节点，让他们相连
        node->pre->next = node->next;
        node->next->pre = node->pre;

        // 清空node指针
        node->pre = nullptr;
        node->next = nullptr;
    }

    // 返回链表中的第一个有效节点
    NodePtr getFirstNode() const {
        return head_->next == tail_ ? nullptr : head_->next;
    }

    // 返回链表的尾节点
    NodePtr getTail() const {
        return tail_;
    }

    template <typename K, typename V>
    friend class LfuCache; // 修正 friend 声明
};

// 2. LFU类
template <typename Key, typename Value>
class LfuCache : public KICachePolicy<Key, Value> {
    // 实现 LFU 缓存的逻辑...
};

} // namespace KamaCache
