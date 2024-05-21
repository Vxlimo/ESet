#include <cstddef>
#include <ctime>
#include <iostream>
#define RBTree ESet

template <class Key, class Compare = std::less<Key>>
class RBTree {
private:
public:
    RBTree() { }
    RBTree(const RBTree& other) { }
    RBTree(RBTree&& other) { }
    RBTree& operator=(const RBTree& other) { }
    RBTree& operator=(RBTree&& other) noexcept { }
    ~RBTree() { }
    class iterator {
    private:
    public:
        iterator() { }
        iterator(const iterator& other) { }
        iterator& operator=(const iterator& other) { }
        ~iterator() { }
        const Key operator*() const { }
        const Key* operator->() const { }
        iterator& operator++() { }
        iterator operator++(int) { }
        iterator& operator--() { }
        iterator operator--(int) { }
        bool operator==(const iterator& other) { }
        bool operator!=(const iterator& other) { }
    };
    iterator find(const Key& key) const { }
    template <class... Args>
    std::pair<iterator, bool> insert(Args&&... args) { }
    bool erase(const Key& key) { }
    iterator begin() const { }
    iterator end() const { }
    size_t range(const Key& l, const Key& r) const { }
    size_t size() const { }
    iterator lower_bound(const Key& key) const { }
    iterator upper_bound(const Key& key) const { }
};