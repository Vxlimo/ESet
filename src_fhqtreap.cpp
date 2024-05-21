#include <cstddef>
#include <ctime>
#include <iostream>
#define fhqTreap ESet

template <class Key, class Compare = std::less<Key>>
class fhqTreap {
private:
    struct Node {
        Key* val; /* the value of the node */
        size_t weight; /* the weight of the node */
        size_t size; /* the size of the subtree rooted at the node */
        Node *left, right; /* the left and right child of the node */
        Node(const Key& _val)
            : val(_val)
            , weight(rand())
            , size(1)
            , left(nullptr)
            , right(nullptr)
        {
        }
        ~Node()
        {
            delete left;
            delete right;
        }
    };
    Compare cmp = Compare(); /* the comparator */
    Node* root; /* the root of the treap */
    /**
     * operator = for Key
     */
    bool equal(const Key& a, const Key& b) const
    {
        return !(cmp(a, b) || cmp(b, a));
    }
    /**
     * operator <= for Key
     */
    bool lessequal(const Key& a, const Key& b) const
    {
        return !(cmp(b, a));
    }
    /**
     * split the treap rooted at the cur into two treaps
     * the first treap contains all the nodes with value <= key
     * the second treap contains all the nodes with value > key
     */
    std::pair<Node*, Node*> split(Node* cur, const Key& key)
    {
        if (cur == nullptr)
            return std::make_pair(nullptr, nullptr);
        if (lessequal(*(cur->val), key)) {
            auto res = split(cur->right, key);
            cur->right = res.first;
            return std::make_pair(cur, res.second);
        } else {
            auto res = split(cur->left, key);
            cur->left = res.second;
            return std::make_pair(res.first, cur);
        }
    }
    /**
     * merge two treaps into one treap
     */
    Node* merge(Node* cur1, Node* cur2)
    {
        if (cur1 == nullptr)
            return cur2;
        if (cur2 == nullptr)
            return cur1;
        if (cur1->weight < cur2->weight) {
            cur1->right = merge(cur1->right, cur2);
            return cur1;
        } else {
            cur2->left = merge(cur1, cur2->left);
            return cur2;
        }
    }

public:
    fhqTreap() { }
    fhqTreap(const fhqTreap& other) { }
    fhqTreap(fhqTreap&& other) { }
    fhqTreap& operator=(const fhqTreap& other) { }
    fhqTreap& operator=(fhqTreap&& other) noexcept { }
    ~fhqTreap() { }
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