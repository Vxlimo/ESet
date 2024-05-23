#include <cstddef>
#include <ctime>
#include <iostream>
#define RBTree ESet

template <class Key, class Compare = std::less<Key>>
class RBTree {
private:
    Compare less = Compare(); /* the comparator */
    /**
     * operator = for Key
     */
    bool equal(const Key& a, const Key& b) const
    {
        return !(less(a, b) || less(b, a));
    }
    /**
     * operator <= for Key
     */
    bool lessequal(const Key& a, const Key& b) const
    {
        return !(less(b, a));
    }

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
        /**
         * ++it, if it == end(), do nothing and return it
         */
        iterator& operator++() { }
        /**
         * it++, if it == end(), do nothing and return it
         */
        iterator operator++(int) { }
        /**
         * --it, if it == begin(), do nothing and return it
         */
        iterator& operator--() { }
        /**
         * it--, if it == begin(), do nothing and return it
         */
        iterator operator--(int) { }
        bool operator==(const iterator& other) { }
        bool operator!=(const iterator& other) { }
    };
    /**
     * find the item in the treap
     * return the iterator to the item if the item is found
     * otherwise return end()
     */
    iterator find(const Key& key) const { }
    /**
     * insert the item into the treap
     * if the item is already in the treap, do nothing
     * return a pair whose first element is the iterator to the item
     * and the second element is whether the item is inserted
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) { }
    /**
     * erase the item from the treap
     * if the item is not in the treap, do nothing
     * return whether the item is erased
     */
    bool erase(const Key& key) { }
    /**
     * return the iterator to the first element
     */
    iterator begin() const { }
    /**
     * return the iterator to the element after the last element
     */
    iterator end() const { }
    /**
     * return the number of elements with l<=key<=r
     */
    size_t range(const Key& l, const Key& r) const { }
    /**
     * return the number of elements in the treap
     */
    size_t size() const { }
    /**
     * return the iterator to the element with the smallest key >= item
     */
    iterator lower_bound(const Key& key) const { }
    /**
     * return the iterator to the element with the smallest key > item
     */
    iterator upper_bound(const Key& key) const { }
};