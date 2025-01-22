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
public:
    using Node = typename FreqList<Key, Value>::Node; // 定义频率链表 FreqList 中的节点类型
    using NodePtr = std::shared_ptr<Node>; // 指向Node的指针
    using NodeMap = std::unordered_map<Key, NodePtr>; // 定义哈希表，用于将键 Key 映射到对应的缓存节点

    // 构造函数: 目的 为 LFU 缓存的运行提供初始化参数
    KLfuCache(int capacity, int maxAverageNum = 10)
    : capacity_(capacity),  // 初始化缓存容量
    minFreq_(INT8_MAX),  // 初始化为一个极大值，用于追踪当前最小访问频次
    maxAverageNum_(maxAverageNum),
    curAverageNum_(0), 
    curTotalNum_(0) // 分别用于管理和记录访问频次
    {}

    // 析构
    ~LfuCache() override = default;

    // Put: 将键值对存入缓存，如果键已存在，则更新对应值
    void put(Key key, Value value) override {
        // 1. 容量检查：如果容量为 0，直接返回。
        // 2. 线程安全：使用 std::lock_guard<std::mutex> 对缓存操作加锁。
        // 3. 查找键：如果存在，就调用getInternal 更新。如果不存在，就用putInternal 添加新缓存

        if(capacity_ == 0){
            return;
        }

        // 更新缓存值时，需要加锁
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = nodeMap_.find(key);

        // 如果it不为空，说明找到了key值. 则更新key对应的值（也就是频率）
        if(it != nodeMap_.end()){
            //解释：it->second 的作用是访问哈希表 nodeMap_ 中，键对应的缓存节点指针 NodePtr
            it->second->value = value;  
            getInternal(it->second, value);
            return;
        }

        putInternal(key, value);

    }

    // 用于直接判断键是否存在，并通过引用参数返回值。
    bool get(Key key, Value& value) override {
        // 思路：查找键 key 是否存在于缓存中
        // 如果在，存入Value，更新频率，并返回true
        // 如果不在，返回false
        
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = nodeMap_.find(key);
        if(it != nodeMap_.end()){
            getInternal(it->second, value);
            return true;
        }

        return false;
    }

    // 通过键直接返回对应的值
    Value get(Key key) override {
        Value = value;
        get(key, value);
        return value;

    }

    // 清空缓存，回收资源
    void purge(Key key) override {
        // .clear() 是 C++中容器的清除函数，如map, set, string, vector, list 等
        nodeMap_.clear(); // 清空键值对
        freqToFreqList_.clear();  // 清空映射

    }


// 私有成员变量
private:
    int capacity_;
    int minFreq_;  // 当前最小访问频次，用于找到需要淘汰的节点
    int maxAverageNum_;
    int curAverageNum_;
    int curTotalNum_;
    std::mutex mutex_;
    NodeMap nodeMap_; //存储键到缓存节点的映射
    // 存储 频率到频率链表的映射: key是访问频次，Value是指向一个 FreqList 对象的指针
    std::unordered_map<int, FreqList<Key, Value>*> freqToFreqList_;
    

// 私有方法声明 
private:
    void putInternal(Key key, Value value); // 添加缓存
    void getInternal(NodePtr node, Value& value); // 获取缓存
    void kickOut(); // 移除缓存中的过期数据
    void removeFromFreqList(NodePtr node); // 从频率列表中移除节点
    void addToFreqList(NodePtr node); // 添加到频率列表
    void addFreqNum(); // 增加平均访问等频率
    void decreaseFreqNum(int num); // 减少平均访问等频率
    void handleOverMaxAverageNum(); // 处理当前平均访问频率超过上限的情况
    void updateMinFreq();


};

// 处理 缓存读取（get） 操作：根据提供的 node，返回对应的 value
template<typename Key, typename Value>
void LfuCache<Key, Value>::getInternal(NodePtr node, Value& value){

    value = node->value;
    removeFromFreqList(node);
    node->freq++;
    addToFreqList(node);
    if (node->freq - 1 == minFreq_ && freqToFreqList_[node->freq - 1]->isEmpty())
    minFreq_++;
    addFreqNum();
}

template<typename Key, typename Value>
void LfuCache<Key, Value>::putInternal(Key key, Value value) {
    if(nodeMap_.size() == capacity_){
        // 如果缓存已满，调用 kickOut() 函数移除最不常访问的节点
        kickOut();
    }
    // 构造新节点，包含 key 和 value，并将其加入缓存的 nodeMap_
    NodePtr node = std::make_shared<Node>(key, value);
    nodeMap_[key] = node;

    addToFreqList(node);
    addFreqNum();
    // 对于新插入的节点，最小频率设置为 1
    minFreq_ = std::min(minFreq_, 1);

}

// 根据 minFreq_ 找到访问频率最低的节点并删除
template<typename Key, typename Value>
void LfuCache<Key, Value>::kickOut() {
    NodePtr node = freqToFreqList_[minFreq_]->getFirstNode();
    removeFromFreqList(node);
    nodeMap_.erase(node->key);
    decreaseFreqNum(node->freq);

}

template<typename Key, typename Value>
void LfuCache<Key, Value>::removeFromFreqList(NodePtr node)
{
    if(!node){
        return;
    }

    auto freq = node->freq;
    freqToFreqList_[freq]->removeNode(node);
}
// 管理节点在访问频率链表中的添加和移除。
template<typename Key, typename Value>
void LfuCache<Key, Value>::addToFreqList(NodePtr node)
{
    // 检查结点是否为空
    if (!node) 
        return;

    // 添加进入相应的频次链表前需要判断该频次链表是否存在
    auto freq = node->freq;
    if (freqToFreqList_.find(node->freq) == freqToFreqList_.end())
    {
        // 不存在则创建
        freqToFreqList_[node->freq] = new FreqList<Key, Value>(node->freq);
    }

    freqToFreqList_[freq]->addNode(node);
}

// 更新总访问频次和平均访问频次，用于统计和优化。
template<typename Key, typename Value>
void LfuCache<Key, Value>::addFreqNum()
{
    curTotalNum_++;
    if (nodeMap_.empty())
        curAverageNum_ = 0;
    else
        curAverageNum_ = curTotalNum_ / nodeMap_.size();

    if (curAverageNum_ > maxAverageNum_)
    {
       handleOverMaxAverageNum();
    }
}

template<typename Key, typename Value>
void LfuCache<Key, Value>::decreaseFreqNum(int num)
{
    // 减少平均访问频次和总访问频次
    curTotalNum_ -= num;
    if (nodeMap_.empty())
        curAverageNum_ = 0;
    else
        curAverageNum_ = curTotalNum_ / nodeMap_.size();
}





} // namespace KamaCache
