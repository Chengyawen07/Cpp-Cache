
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
    Key getKey() const {return key_};
    Value getValue() const {return value_};
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
    bool get(Key key, Value value){
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


    }



    // 4. 删除数据
    void remove(){

    }



private:
    void initializeList(){
        // 创建首尾的虚拟节点
        dummyHead_ = std::make_shared<LruNodeType>(Key(), Value());
        dummyTail_ = std::make_shared<LruNodeType>(Key(), Value());
        dummyHead_->next_ = dummyTail_;
        dummyTail_->prev_ = dummyHead_;
    }


private:
    int capacity_;
    std::shared_ptr<LruNodeType> dummyHead_;
    std::shared_ptr<LruNodeType> dummyTail_;
    std::mutex  mutex_;
    NodeMap nodeMap_; 
};





} // namespace KamaCache

