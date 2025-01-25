# C++ Cache Strategies

Welcome to the **C++ Cache Strategies** repository! This project provides an efficient and extensible implementation of various caching strategies commonly used in software systems to optimize memory and performance. The repository includes:

- **LRU (Least Recently Used)**: Evicts the least recently accessed items.
- **LFU (Least Frequently Used)**: Evicts the least frequently accessed items.
- **ARC (Adaptive Replacement Cache)**: A hybrid approach combining the benefits of LRU and LFU.

## Features

- Written in modern C++ with support for **RAII** and **smart pointers**.
- Highly modular design for easy extensibility.
- Thread-safe implementations using `std::mutex`.
- Benchmarks for performance comparison.

---

## Caching Strategies

### **1. Least Recently Used (LRU)**
The LRU strategy maintains a list of recently used items. When the cache reaches its capacity, the least recently used item is evicted.

- **Use Case**: Suitable for systems where recent usage predicts future usage (e.g., web browsers, disk caching).

#### Key Concepts:
- Uses a **doubly linked list** to maintain the order of access.
- Uses a **hash map** for O(1) access and updates.

#### Example:
```cpp
LRUCache<int, std::string> cache(3);
cache.put(1, "one");
cache.put(2, "two");
cache.get(1); // Moves key 1 to the most recent position
cache.put(3, "three");
cache.put(4, "four"); // Evicts key 2
```

---

### **2. Least Frequently Used (LFU)**
The LFU strategy evicts items that are accessed least frequently.

- **Use Case**: Suitable for systems where frequency of access predicts importance (e.g., recommendation systems, ML model caching).

#### Key Concepts:
- Maintains a **frequency map** and a **list of keys per frequency**.
- Eviction is based on the lowest frequency, with ties broken by recency.

#### Example:
```cpp
LFUCache<int, std::string> cache(3);
cache.put(1, "one");
cache.put(2, "two");
cache.get(1); // Key 1 frequency becomes 2
cache.put(3, "three");
cache.put(4, "four"); // Evicts key 2 (lowest frequency)
```

---

### **3. Adaptive Replacement Cache (ARC)**
The ARC strategy dynamically balances between **recency** (LRU) and **frequency** (LFU) to achieve better performance.

- **Use Case**: Suitable for workloads with varying access patterns where neither LRU nor LFU alone performs well.

#### Key Concepts:
- Maintains two lists for recency and frequency tracking.
- Dynamically adjusts the size of these lists based on access patterns.

#### Example:
```cpp
ARCCache<int, std::string> cache(3);
cache.put(1, "one");
cache.put(2, "two");
cache.get(1); // Moves key 1 to the frequent list
cache.put(3, "three");
cache.put(4, "four"); // Adjusts based on recency and frequency
```

---

## Getting Started

### Prerequisites
- A modern C++ compiler (C++17 or newer).
- CMake (optional, for building).

### Installation
Clone the repository:
```bash
git clone https://github.com/Chengyawen07/Cpp-Cache.git
cd Cpp-Cache
```

Build the project:
```bash
mkdir build && cd build
cmake .. && make
```

### Running Tests
Run the unit tests to ensure the implementation is correct:
```bash
./tests
```

