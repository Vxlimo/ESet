#include <cstddef>
#include <ctime>
#include <iostream>
#include <vector>
#define SkipList ESet

template <class Key, class Compare = std::less<Key>>
class SkipList {
private:
    struct Node {
        Key* val; /* the value of the node */
        Node *pre, next; /* the previous and next node in the same level */
        Node* down; /* the node in the lower level */
        size_t sz; /* the number of nodes in the subtree rooted at the node */
        Node(Key* _value, Node* _pre, Node* _next, Node* _down, size_t _sz)
            : val(_value)
            , pre(_pre)
            , next(_next)
            , down(_down)
            , sz(_sz)
        {
            return;
        }
        ~Node()
        {
            if (val != nullptr)
                delete val;
            return;
        }
    };
    Compare cmp = Compare(); /* the comparator */
    Node* top; /* the top node of the skiplist */
    Node* head; /* the head node of the bottom level */
    Node* tail; /* the tail node of the bottom level */
    size_t sz; /* the number of elements in the skiplist */
    /**
     * build a empty skiplist
     */
    void build()
    {
        srand(time(0));
        top = head = new Node(nullptr, nullptr, nullptr, nullptr, 0);
        tail = new Node(nullptr, head, nullptr, nullptr, 0);
        head->next = tail;
        sz = 0;
        return;
    }
    /**
     * destroy the whole skiplist
     */
    void destroy()
    {
        Node* cur = top;
        while (cur != nullptr) {
            Node* temp = cur->next;
            while (temp != nullptr) {
                Node* temp2 = temp->next;
                delete temp;
                temp = temp2;
            }
            temp = cur->down;
            delete cur;
            cur = temp;
        }
        sz = 0;
        return;
    }
    /**
     * return a random boolean value
     */
    bool rand() const
    {
        return std::rand() & 1;
    }
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
     * generate the road to the item
     * from the top to the bottom
     */
    std::vector<Node*> genroad(const Key& item) const
    {
        Node* cur = top;
        std::vector<Node*> road;
        while (true) {
            while (cur->next != nullptr && cur->next->val != nullptr && lessequal(*(cur->next->val), item))
                cur = cur->next;
            road.push_back(cur);
            if (cur->down != nullptr)
                cur = cur->down;
            else
                break;
        }
        return road;
    }
    /**
     * update the sz of the node
     */
    void update(Node* cur)
    {
        if (cur->down == nullptr)
            return;
        Node *temp = cur->down, *target;
        if (cur->next != nullptr)
            target = cur->next->down;
        else
            target = nullptr;
        cur->sz = 0;
        while (temp != target) {
            cur->sz += temp->sz;
            temp = temp->next;
        }
        return;
    }
    /**
     * return rank of the iterator
     */
    size_t rank(const Key& item) const
    {
        Node* cur = top;
        size_t sz = 0;
        while (cur != nullptr) {
            while (cur->next != nullptr && cur->next->val != nullptr && lessequal(*(cur->next->val), item)) {
                sz += cur->sz;
                cur = cur->next;
            }
            if (cur->down != nullptr)
                cur = cur->down;
            else
                break;
        }
        return sz;
    }

public:
    SkipList()
    {
        build();
        return;
    }
    SkipList(const SkipList& other)
    {
        build();
        for (iterator it = other.begin(); it != other.end(); it++)
            emplace(*it);
        return;
    }
    SkipList(SkipList&& other)
    {
        if (this == &other)
            return;
        srand(time(0));
        top = other.top;
        head = other.head;
        tail = other.tail;
        sz = other.sz;
        other.build();
        return;
    }
    SkipList& operator=(const SkipList& other)
    {
        if (this == &other)
            return *this;
        destroy();
        build();
        for (iterator it = other.begin(); it != other.end(); it++)
            emplace(*it);
        return *this;
    }
    SkipList& operator=(SkipList&& other) noexcept
    {
        if (this == &other)
            return *this;
        destroy();
        top = other.top;
        head = other.head;
        tail = other.tail;
        sz = other.sz;
        other.build();
        return *this;
    }
    ~SkipList()
    {
        destroy();
        return;
    }
    class iterator {
    private:
        Node* p; /* the pointer to the node */
        const SkipList* base; /* the base skiplist */

    public:
        iterator() = default;
        iterator(Node* p, const SkipList* base)
            : p(p)
            , base(base)
        {
        }
        iterator(const iterator& rhs)
            : p(rhs.p)
            , base(rhs.base)
        {
        }
        iterator operator=(const iterator& rhs)
        {
            p = rhs.p;
            base = rhs.base;
            return *this;
        }
        ~iterator() = default;
        const Key operator*() const
        {
            if (p->val == nullptr)
                throw std::out_of_range("out of range");
            return *(p->val);
        }
        const Key* operator->() const
        {
            return p->val;
        }
        /**
         * ++it, if it == end(), do nothing and return it
         */
        iterator& operator++()
        {
            if (*this != base->end())
                p = p->next;
            return *this;
        }
        /**
         * it++, if it == end(), do nothing and return it
         */
        iterator operator++(int)
        {
            iterator temp = *this;
            if (*this != base->end())
                p = p->next;
            return temp;
        }
        /**
         * --it, if it == begin(), do nothing and return it
         */
        iterator& operator--()
        {
            if (*this != base->begin())
                p = p->pre;
            return *this;
        }
        /**
         * it--, if it == begin(), do nothing and return it
         */
        iterator operator--(int)
        {
            iterator temp = *this;
            if (*this != base->begin())
                p = p->pre;
            return temp;
        }
        bool operator==(const iterator& rhs) const
        {
            return p == rhs.p;
        }
        bool operator!=(const iterator& rhs) const
        {
            return p != rhs.p;
        }
    };
    /**
     * find the item in the skiplist
     * return the iterator to the item if the item is found
     * otherwise return end()
     */
    iterator find(const Key& item) const
    {
        std::vector<Node*> road = genroad(item);
        if (road.back()->val != nullptr && equal(*(road.back()->val), item))
            return iterator(road.back(), this);
        else
            return end();
    }
    /**
     * insert the item into the skiplist
     * if the item is already in the skiplist, do nothing
     * return a pair whose first element is the iterator to the item
     * and the second element is whether the item is inserted
     */
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        Key item(std::forward<Args>(args)...);
        std::vector<Node*> road = genroad(item);
        if (road.back()->val != nullptr && equal(*(road.back()->val), item))
            return { iterator(road.back(), this), false };
        sz++;
        Node *down = nullptr, *cur = nullptr;
        iterator it;
        bool build_flag = true;
        for (int i = road.size() - 1;; i--) {
            if (i == road.size() - 1 || (build_flag && rand())) {
                if (i >= 0) {
                    Key* val = new Key(item);
                    cur = new Node(val, road[i], road[i]->next, down, 1);
                    if (road[i]->next != nullptr)
                        road[i]->next->pre = cur;
                    road[i]->next = cur;
                } else {
                    top = new Node(nullptr, nullptr, nullptr, top, 0);
                    Key* val = new Key(item);
                    cur = new Node(val, top, nullptr, down, 1);
                    top->next = cur;
                }
                if (i == road.size() - 1)
                    it = iterator(cur, this);
                update(cur);
                update(cur->pre);
                down = cur;
            } else {
                build_flag = false;
                if (i >= 0)
                    update(road[i]);
                else
                    break;
            }
        }
        return { it, true };
    }
    /**
     * erase the item from the skiplist
     * if the item is not in the skiplist, do nothing
     * return whether the item is erased
     */
    bool erase(const Key& item)
    {
        std::vector<Node*> road = genroad(item);
        if (road.back()->val == nullptr || !equal(*(road.back()->val), item))
            return false;
        sz--;
        Node* cur = nullptr;
        bool erase_flag = true;
        for (int i = road.size() - 1; i >= 0; i--) {
            cur = road[i];
            if (erase_flag) {
                if (cur->next != nullptr)
                    cur->next->pre = cur->pre;
                if (cur->pre != nullptr) {
                    cur->pre->next = cur->next;
                    update(cur->pre);
                }
                if (i && road[i - 1]->down != cur)
                    erase_flag = false;
                delete cur;
            } else
                update(cur);
        }
        while (top->next == nullptr && top->down != nullptr) {
            Node* temp = top->down;
            delete top;
            top = temp;
        }
        return true;
    }
    /**
     * return the iterator to the first element
     */
    iterator begin() const noexcept
    {
        return iterator(head->next, this);
    }
    /**
     * return the iterator to the element after the last element
     */
    iterator end() const noexcept
    {
        return iterator(tail, this);
    }
    /**
     * return the number of elements with l<=key<=r
     */
    size_t range(const Key& l, const Key& r) const
    {
        return rank(r) - rank(l) + (find(l) != end());
    }
    /**
     * return the number of elements in the skiplist
     */
    size_t size() const noexcept
    {
        return sz;
    }
    /**
     * return the iterator to the element with the smallest key >= item
     */
    iterator lower_bound(const Key& key) const
    {
        std::vector<Node*> road = genroad(key);
        if (road.back()->val == nullptr || !equal(*(road.back()->val), key))
            return iterator(road.back()->next, this);
        return iterator(road.back(), this);
    }
    /**
     * return the iterator to the element with the smallest key > item
     */
    iterator upper_bound(const Key& key) const
    {
        std::vector<Node*> road = genroad(key);
        return iterator(road.back()->next, this);
    }
};