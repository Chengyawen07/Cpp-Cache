#include <iostream>
#include "LfuCache.h" // 包含 FreqList 的头文件，路径根据实际情况调整

using namespace KamaCache;

int main() {
    // 创建一个频率列表，频率为1
    FreqList<int, std::string> freqList(1);

    // 检查链表是否为空
    if (freqList.isEmpty()) {
        std::cout << "Initial list is empty." << std::endl;
    } else {
        std::cout << "Initial list is not empty." << std::endl;
    }

    // 创建节点并添加到链表
    auto node1 = std::make_shared<FreqList<int, std::string>::Node>(1, "Node1");
    freqList.addNode(node1);

    if (!freqList.isEmpty()) {
        std::cout << "Node added successfully. List is no longer empty." << std::endl;
    }

    // 检查链表中的第一个节点
    auto firstNode = freqList.getFirstNode();
    if (firstNode == node1) {
        std::cout << "First node matches: key = " << firstNode->key
                  << ", value = " << firstNode->value << std::endl;
    } else {
        std::cout << "Error: First node does not match expected node." << std::endl;
    }

    // 删除节点
    freqList.removeNode(node1);
    if (freqList.isEmpty()) {
        std::cout << "Node removed successfully. List is empty again." << std::endl;
    } else {
        std::cout << "Error: Node removal failed. List is not empty." << std::endl;
    }

    return 0;
}
