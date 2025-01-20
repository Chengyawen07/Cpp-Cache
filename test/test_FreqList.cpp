#include <gtest/gtest.h>
#include "/home/ubuntu/Cpp-Cache/LfuCache.h"  // 头文件路径

using namespace KamaCache;

TEST(FreqListTest, BasicOperations) {
    // 创建一个频率列表，频率为1
    FreqList<int, std::string> freqList(1);

    // 检查链表是否为空
    EXPECT_TRUE(freqList.isEmpty());

    // 创建节点并添加到链表
    auto node1 = std::make_shared<FreqList<int, std::string>::Node>(1, "Node1");
    freqList.addNode(node1);
    EXPECT_FALSE(freqList.isEmpty()); // 添加节点后链表不应为空

    // 检查链表中的第一个节点
    auto firstNode = freqList.getFirstNode();
    EXPECT_EQ(firstNode, node1);          // 第一个节点应为 node1
    EXPECT_EQ(firstNode->key, 1);         // key 应该为 1
    EXPECT_EQ(firstNode->value, "Node1"); // value 应该为 "Node1"

    // 移除节点后链表应为空
    freqList.removeNode(node1);
    EXPECT_TRUE(freqList.isEmpty());
}
