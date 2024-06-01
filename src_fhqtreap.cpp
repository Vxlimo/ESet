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
        Node *left, *right; /* the left and right child of the node */
        Node() = default;
        Node(const Key& _val)
            : val(new Key(_val))
            , weight(rand())
            , size(1)
            , left(nullptr)
            , right(nullptr)
        {
        }
        Node(Node& other)
        {
            val = new Key(*other.val);
            weight = other.weight;
            size = other.size;
            if (other.left != nullptr)
                left = new Node(*(other.left));
            else
                left = nullptr;
            if (other.right != nullptr)
                right = new Node(*(other.right));
            else
                right = nullptr;
        }
        ~Node()
        {
            if (val != nullptr)
                delete val;
            if (left != nullptr)
                delete left;
            if (right != nullptr)
                delete right;
        }
    };
    Compare less = Compare(); /* the comparator */
    Node* root; /* the root of the treap */
    Node* end_p; /* the node of end() */
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
    /**
     * find the first node with value <= key
     */
    Node* pre(Node* cur, const Key& key) const
    {
        if (cur == nullptr)
            return nullptr;
        if (less(*(cur->val), key)) {
            auto res = pre(cur->right, key);
            if (res == nullptr)
                return cur;
            return res;
        } else
            return pre(cur->left, key);
    }
    /**
     * find the first node with value > key
     */
    Node* nxt(Node* cur, const Key& key) const
    {
        if (cur == nullptr)
            return nullptr;
        if (lessequal(*(cur->val), key))
            return nxt(cur->right, key);
        else {
            auto res = nxt(cur->left, key);
            if (res == nullptr)
                return cur;
            return res;
        }
    }
    /**
     * find the last node
     */
    Node* last() const
    {
        Node* cur = root;
        while (cur->right != nullptr)
            cur = cur->right;
        return cur;
    }
    /**
     * find the rank of the key in the treap
     */
    size_t rank(Node* cur, const Key& key) const
    {
        if (cur == nullptr)
            return 0;
        if (lessequal(*(cur->val), key))
            return 1 + (cur->left == nullptr ? 0 : cur->left->size) + rank(cur->right, key);
        else
            return rank(cur->left, key);
    }
    /**
     * pushup the size of the node
     */
    void pushup(Node* cur)
    {
        cur->size = 1;
        if (cur->left != nullptr)
            cur->size += cur->left->size;
        if (cur->right != nullptr)
            cur->size += cur->right->size;
        return;
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
            auto [l, r] = split(cur->right, key);
            cur->right = l;
            pushup(cur);
            return std::make_pair(cur, r);
        } else {
            auto [l, r] = split(cur->left, key);
            cur->left = r;
            pushup(cur);
            return std::make_pair(l, cur);
        }
    }
    /**
     * split the treap rooted at the cur into two treaps
     * the first treap contains all the nodes with value < key
     * the second treap contains all the nodes with value >= key
     */
    std::pair<Node*, Node*> split_small(Node* cur, const Key& key)
    {
        if (cur == nullptr)
            return std::make_pair(nullptr, nullptr);
        if (less(*(cur->val), key)) {
            auto [l, r] = split_small(cur->right, key);
            cur->right = l;
            pushup(cur);
            return std::make_pair(cur, r);
        } else {
            auto [l, r] = split_small(cur->left, key);
            cur->left = r;
            pushup(cur);
            return std::make_pair(l, cur);
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
            pushup(cur1);
            return cur1;
        } else {
            cur2->left = merge(cur1, cur2->left);
            pushup(cur2);
            return cur2;
        }
    }

public:
    fhqTreap()
    {
        root = nullptr;
        end_p = new Node();
    }
    fhqTreap(const fhqTreap& other)
    {
        root = new Node(*(other.root));
        end_p = new Node();
    }
    fhqTreap(fhqTreap&& other)
    {
        root = other.root;
        end_p = other.end_p;
        other.root = other.end_p = nullptr;
    }
    fhqTreap& operator=(const fhqTreap& other)
    {
        if (this == &other)
            return *this;
        delete root;
        delete end_p;
        root = new Node(*(other.root));
        end_p = new Node();
        return *this;
    }
    fhqTreap& operator=(fhqTreap&& other) noexcept
    {
        if (this == &other)
            return *this;
        delete root;
        delete end_p;
        root = other.root;
        end_p = other.end_p;
        other.root = other.end_p = nullptr;
        return *this;
    }
    ~fhqTreap()
    {
        if (root != nullptr)
            delete root;
        if (end_p != nullptr)
            delete end_p;
    }
    class iterator {
        friend class fhqTreap;

    private:
        Node* p; /* the pointer to the node */
        const fhqTreap* base; /* the base treap */
        iterator(Node* _p, const fhqTreap* _base)
            : p(_p)
            , base(_base)
        {
        }

    public:
        iterator() = default;
        iterator(const iterator& other)
        {
            p = other.p;
            base = other.base;
        }
        iterator& operator=(const iterator& other)
        {
            p = other.p;
            base = other.base;
            return *this;
        }
        ~iterator() = default;
        const Key operator*() const
        {
            if (p == nullptr || p->val == nullptr)
                throw std::out_of_range("dereference nullptr");
            return *(p->val);
        }
        const Key* operator->() const
        {
            if (p == nullptr || p->val == nullptr)
                throw std::out_of_range("dereference nullptr");
            return p->val;
        }
        /**
         * ++it, if it == end(), do nothing and return it
         */
        iterator& operator++()
        {
            if (*this != base->end()) {
                p = base->nxt(base->root, *(p->val));
                if (p == nullptr)
                    p = base->end_p;
            }
            return *this;
        }
        /**
         * it++, if it == end(), do nothing and return it
         */
        iterator operator++(int)
        {
            iterator temp = *this;
            if (*this != base->end()) {
                p = base->nxt(base->root, *(p->val));
                if (p == nullptr)
                    p = base->end_p;
            }
            return temp;
        }
        /**
         * --it, if it == begin(), do nothing and return it
         */
        iterator& operator--()
        {
            if (*this != base->begin()) {
                if (p == base->end_p)
                    p = base->last();
                else
                    p = base->pre(base->root, *(p->val));
            }
            return *this;
        }
        /**
         * it--, if it == begin(), do nothing and return it
         */
        iterator operator--(int)
        {
            iterator tmp = *this;
            if (*this != base->begin()) {
                if (p == base->end_p)
                    p = base->last();
                else
                    p = base->pre(base->root, *(p->val));
            }
            return tmp;
        }
        bool operator==(const iterator& other)
        {
            return p == other.p && base == other.base;
        }
        bool operator!=(const iterator& other)
        {
            return p != other.p || base != other.base;
        }
    };
    /**
     * find the item in the treap
     * return the iterator to the item if the item is found
     * otherwise return end()
     */
    iterator find(const Key& key) const
    {
        Node* cur = root;
        while (cur != nullptr) {
            if (equal(*(cur->val), key))
                return iterator(cur, this);
            if (less(*(cur->val), key))
                cur = cur->right;
            else
                cur = cur->left;
        }
        return end();
    }
    /**
     * insert the item into the treap
     * if the item is already in the treap, do nothing
     * return a pair whose first element is the iterator to the item
     * and the second element is whether the item is inserted
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        Key key = Key(std::forward<Args>(args)...);
        iterator it = find(key);
        if (it != end())
            return std::make_pair(it, false);
        auto [l, r] = split(root, key);
        Node* cur = new Node(key);
        root = merge(merge(l, cur), r);
        return std::make_pair(iterator(cur, this), true);
    }
    /**
     * erase the item from the treap
     * if the item is not in the treap, do nothing
     * return whether the item is erased
     */
    bool erase(const Key& key)
    {
        iterator it = find(key);
        if (it == end())
            return false;
        auto [l, r] = split(root, key);
        auto [ll, lr] = split_small(l, key);
        root = merge(ll, r);
        delete lr;
        return true;
    }
    /**
     * return the iterator to the first element
     */
    iterator begin() const
    {
        if (root == nullptr)
            return iterator(end_p, this);
        Node* cur = root;
        while (cur->left != nullptr)
            cur = cur->left;
        return iterator(cur, this);
    }
    /**
     * return the iterator to the element after the last element
     */
    iterator end() const
    {
        return iterator(end_p, this);
    }
    /**
     * return the number of elements with l<=key<=r
     */
    size_t range(const Key& l, const Key& r) const
    {
        return rank(root, r) - rank(root, l) + (find(l) != end());
    }
    /**
     * return the number of elements in the treap
     */
    size_t size() const
    {
        return root == nullptr ? 0 : root->size;
    }
    /**
     * return the iterator to the element with the smallest key >= item
     */
    iterator lower_bound(const Key& key) const
    {
        iterator it = find(key);
        if (it != end())
            return it;
        else {
            Node* p = nxt(root, key);
            if (p == nullptr)
                return end();
            return iterator(p, this);
        }
    }
    /**
     * return the iterator to the element with the smallest key > item
     */
    iterator upper_bound(const Key& key) const
    {
        Node* p = nxt(root, key);
        if (p == nullptr)
            return end();
        return iterator(p, this);
    }
};