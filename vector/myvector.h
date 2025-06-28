//
// Created by hp on 2025/6/28.
//

#ifndef C__CODE_MYVECTOR_H
#define C__CODE_MYVECTOR_H


#include <iostream>
#include <cstddef>  // for size_t
#include <stdexcept>  // for exception
#include <initializer_list>  // for initializer_list

// 前向声明迭代器
template <typename T>
class MyVector;

template <typename T>
class MyVectorIterator {
private:
    T* ptr;
public:
    MyVectorIterator(T* p) : ptr(p) {}

    // 迭代器基本操作（++、--、*、->、==、!=）
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    MyVectorIterator& operator++() { ptr++; return *this; }
    MyVectorIterator operator++(int) { MyVectorIterator temp = *this; ptr++; return temp; }
    bool operator==(const MyVectorIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const MyVectorIterator& other) const { return ptr != other.ptr; }

    // 友元类访问私有成员
    friend class MyVector<T>;
};

template <typename T>
class MyVector {
private:
    T* data;          // 数据指针
    size_t size_;     // 元素数量
    size_t capacity_; // 总容量

    // 内存分配器
    T* allocate(size_t n) {
        return static_cast<T*>(operator new(n * sizeof(T)));
    }

    // 内存释放
    void deallocate(T* ptr) {
        operator delete(ptr);
    }

    // 元素构造与析构
    void construct(T* ptr, const T& value) {
        new (ptr) T(value);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }

    // 扩容逻辑
    void resize_capacity(size_t new_cap) {
        if (new_cap <= capacity_) return;

        T* new_data = allocate(new_cap);
        for (size_t i = 0; i < size_; ++i) {
            construct(&new_data[i], data[i]);
            destroy(&data[i]);
        }
        deallocate(data);
        data = new_data;
        capacity_ = new_cap;
    }

public:
    // 类型定义（模仿STL的traits）
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = MyVectorIterator<T>;
    using const_iterator = MyVectorIterator<const T>;
    using size_type = size_t;

    // 默认构造函数
    MyVector() : data(nullptr), size_(0), capacity_(0) {
        resize_capacity(1);  // 初始分配1个元素空间
    }

    // 带初始容量的构造函数
    explicit MyVector(size_t n) : size_(n), capacity_(n) {
        data = allocate(n);
        for (size_t i = 0; i < n; ++i) {
            construct(&data[i], T());
        }
    }

    // 初始化列表构造（C++11特性）
    MyVector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()) {
        data = allocate(init.size());
        size_t i = 0;
        for (const auto& val : init) {
            construct(&data[i], val);
            i++;
        }
    }

    // 析构函数
    ~MyVector() {
        for (size_t i = 0; i < size_; ++i) {
            destroy(&data[i]);
        }
        deallocate(data);
    }

    // 拷贝构造函数
    MyVector(const MyVector& other) : size_(other.size_), capacity_(other.capacity_) {
        data = allocate(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            construct(&data[i], other.data[i]);
        }
    }

    // 拷贝赋值运算符
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            // 先释放当前资源
            for (size_t i = 0; i < size_; ++i) {
                destroy(&data[i]);
            }
            deallocate(data);

            // 分配新资源并复制
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = allocate(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                construct(&data[i], other.data[i]);
            }
        }
        return *this;
    }

    // 移动构造与赋值（C++11，优化性能）
    MyVector(MyVector&& other) noexcept
            : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            // 释放当前资源
            for (size_t i = 0; i < size_; ++i) {
                destroy(&data[i]);
            }
            deallocate(data);

            // 接管资源
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    // 元素访问
    reference at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data[pos];
    }

    reference operator[](size_t pos) {
        return data[pos];
    }

    reference front() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return data[0];
    }

    reference back() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return data[size_ - 1];
    }

    // 迭代器相关
    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size_); }
    const_iterator begin() const { return const_iterator(data); }
    const_iterator end() const { return const_iterator(data + size_); }

    // 容量相关
    bool empty() const { return size_ == 0; }
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            resize_capacity(new_cap);
        }
    }

    // 修改器
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            destroy(&data[i]);
        }
        size_ = 0;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize_capacity(capacity_ == 0 ? 1 : capacity_ * 2);  // 类似STL的扩容策略
        }
        construct(&data[size_], value);
        size_++;
    }

    void push_back(T&& value) {  // 右值引用优化
        if (size_ == capacity_) {
            resize_capacity(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        construct(&data[size_], std::move(value));
        size_++;
    }

    void pop_back() {
        if (size_ > 0) {
            destroy(&data[size_ - 1]);
            size_--;
        }
    }

    iterator erase(iterator pos) {
        if (pos == end() || size_ == 0) {
            throw std::out_of_range("Invalid iterator");
        }

        size_t index = pos.ptr - data;
        destroy(&data[index]);

        // 移动后续元素
        for (size_t i = index; i < size_ - 1; ++i) {
            data[i] = std::move(data[i + 1]);  // 使用移动语义
            destroy(&data[i + 1]);
        }
        size_--;
        return iterator(data + index);
    }
};


#endif //C__CODE_MYVECTOR_H
