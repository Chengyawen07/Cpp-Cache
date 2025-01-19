
/*
这段代码实现了一个LRU缓存策略，并进行了多种优化，比如支持访问计数（LFU的基础）和分片以提升并发性能。
以下是详细的讲解和学习这段代码的顺序逻辑，同时结合一些C++编程技巧和方法：
*/

// make_shared()
#include <memory>
#include <unordered_map>
#include <mutex>
#include <thread>
using namespace std;


namespace KamaCache 
{

// 1. 定义Cache 节点类
template <typename Key, typename Value>
class LruNode{

private:
    Key key_;
    Value value_;
    size_t accessCount_;
    // 创建两个只能指针，用做双向链表的指针
    std::shared_ptr<LruNode<Key, Value>> prev_;
    std::shared_ptr<LruNode<Key, Value>> next_;


public:
    // 1. 类的构造函数，与类名一样
    LruNode(Key key, Value value):
    key_(key),
    value_(value),
    accessCount_(1),
    prev_(nullptr),
    next_(nullptr)
    {}

    // 2. 提供必要的访问器： 为了确保成员变量的可读性
    Key getKey() const {return key_;}
    Value getValue() const {return value_;}
    void setValue(const Value& value) {value_ = value;}
    size_t getAccessCount() const {return accessCount_;}
    void increaseAccessCount() {++accessCount_;}

    friend class LruCache<Key, Value>;
};

template<typename Key, typename Value>
class LruCache
{
public:

    // 用using取别名，简化后面的代码书写
    // 这里的NodeType就是一个链表节点Node，包含Key 和 Value
    using LruNodeType = LruNode<Key, Value>;
    // 这个是指向链表的指针
    using NodePtr = std::shared_ptr<LruNodeType>;
    // 定义一个关联容器unordered map，里面有Key和Value，Value是一个链表指针类型
    using NodeMap = std::unordered_map<Key, NodePtr>;


    // 1. 构造函数
    LruCache(int capacity) : capacity_(capacity) {
        // 用来创建虚拟链表
        initializeList();
    }

    ~LruCache() override = default;


    // 2. 业务逻辑：插入数据
    void put(Key key, Value value){
        // 如果缓存为0，直接返回，不执行操作
        if(capacity_ <= 0) return;

        // 使用 std::lock_guard，自动加锁，代替了手动加锁
        std::lock_guard<std::mutex> lock(mutex_);
        // 在哈希表中找key
        auto it = nodeMap_.find(key);

        // 如果找到了key，就更新节点，并将节点移动到链表头部，标记为最近使用
        if(it != nodeMap_.end()){
            updateExistingNode(it->second, value);
            return;
        }
        
    }

    // 3. 获取数据
    bool get(Key key, Value& value) override {

        // 1. 加锁保护共享资源（如 nodeMap_ 和链表）不被多个线程同时修改
        std::lock_guard<std::mutex> lock(mutex_);

        // 在哈希表 nodeMap_ 中查找键 key 是否存在，返回迭代器it
        auto it = nodeMap_.find(key);

        // 如果找到了key
        if (it != nodeMap_.end()) {
            moveToMostRecent(it->second); // 将节点移动到链表头部
            value = it->second->getValue(); // 通过引用参数返回值
            return true; // 返回成功
        }
        return false;

    }

    // 第二个get函数
    Value get(Key key) override {
        Value value{}; // 构造函数，设置初始，比如0或空字符串" "
        get(key, value);
        return value;
    }


    // 4. 删除数据
    void remove(Key key){
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = nodeMap_.find(key);
        if(it != nodeMap_.end()){
            removeNode(it->second); // 删除链表的node
            nodeMap_.erase(it); // 删除哈希表的key
        }

    }

// 上述公共函数里，调用的一些具体删除操作，是写在Private函数里的
private:
    void initializeList(){
        // 创建首尾的虚拟节点
        dummyHead_ = std::make_shared<LruNodeType>(Key(), Value());
        dummyTail_ = std::make_shared<LruNodeType>(Key(), Value());
        dummyHead_->next_ = dummyTail_;
        dummyTail_->prev_ = dummyHead_;
    }

    // 作用是，插入一个新的Node，要更新Node到链表头部
    void updateExistingNode(NodePtr node, const Value& value){
        node->setValue(value);
        moveToMostRecent(node);
    }

    // 添加新节点
    void addNewNode(const Key& key, const Value& value){
        // 1. 先检查缓存，如果缓存已满，就删除最久未使用的
        if(nodeMap_.size() >= capacity_){
            evictLeastRecent(); //  删除掉最久远的（在Head）
        }
        // 新增节点
        NodePtr newNode = std::make_shared<LruNodeType>(key, value);
        insertNode(newNode);  // 插入链表尾部
        nodeMap_[key] = newNode; //哈希表加入新节点
    }


    void moveToMostRecent(NodePtr node){
        removeNode(node);  // 从链表中移除当前节点(先找到对应node，删除)
        insertNode(node);  // 将节点插入到链表头部（再把这个node添加到尾部）
    }

    //removeNode 函数中的操作逻辑是 基于双向链表的，
    // 删除节点时，必须同时更新 prev 和 next。
    // A->next_ 指向 C
    // C->prev_ 指向 A
    void removeNode(NodePtr node) 
    {
        node->prev_->next_ = node->next_; // 前节点的 next 指针指向当前节点的后节点
        node->next_->prev_ = node->prev_; // 后节点的 prev 指针指向当前节点的前节点
    }


    // 因为是双向链表，所以要操作两次
    // 从尾部插入节点（尾部是新节点）
    void insertNode(NodePtr node){
        node->next_ = dummyTail_;
        node->prev_ = dummyTail_->prev_;
        dummyTail_->prev_->next_ = node;
        dummyTail_->prev_ = node;
    }

    // 淘汰使用最少的node（头部是最少用的）
    void evictLeastRecent() 
    {
        NodePtr leastRecent = dummyHead_->next_; // 最久未访问的数据是链表尾部的节点
        removeNode(leastRecent);                 // 从链表中移除
        nodeMap_.erase(leastRecent->getKey());   // 从哈希表中删除
    }

private:
    int capacity_;  
    std::mutex  mutex_;
    // NodePtr = std::shared_ptr<LruNodeType>
    NodePtr dummyHead_;
    NodePtr dummyTail_;
    NodeMap nodeMap_; 
};




} // namespace KamaCache

