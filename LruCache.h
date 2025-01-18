
/*
这段代码实现了一个LRU缓存策略，并进行了多种优化，比如支持访问计数（LFU的基础）和分片以提升并发性能。
以下是详细的讲解和学习这段代码的顺序逻辑，同时结合一些C++编程技巧和方法：
*/

// make_shared()
#include <memory>
#include <unordered_map>
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
    using LruNodeType = LruNode(Key, Value);


    // 1. 构造函数
    LruCache(int capacity) : capacity_(capacity) {
        initializeList();
    }

    ~LruCache() override = default;


    void initializeList(){
        // 创建首尾的虚拟节点
        dummyHead_ = std::make_shared<LruNodeType>(Key(), Value());
        dummyTail_ = std::make_shared<LruNodeType>(Key(), Value());


    }


private:
    int capacity_;
    std:shared_ptr<LruNodeType> dummyHead_;
    std:shared_ptr<LruNodeType> dummyTail_;


};





} // namespace KamaCache

