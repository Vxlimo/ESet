#include <cstddef>
#include <ctime>
#include <iostream>

template <class Key, class Compare = std::less<Key>>
class RBTree {
private:
    struct Node {
        Key* val; /* the value of the node */
        size_t size; /* the size of the subtree rooted at the node */
        Node *left, *right; /* the left and right child of the node */
        Node* parent; /* the parent of the node */
        enum Color { RED,
            BLACK } color; /* the color of the node */
        Node()
            : val(nullptr)
            , size(0)
            , left(nullptr)
            , right(nullptr)
            , parent(nullptr)
            , color(BLACK)
        {
        }
        Node(const Key& _val, Node* _left, Node* _right, Node* _parent, Color _color = RED)
            : val(new Key(_val))
            , size(1)
            , left(_left)
            , right(_right)
            , parent(_parent)
            , color(_color)
        {
        }
        Node(const Node& other)
            : val(nullptr)
            , size(other.size)
            , color(other.color)
        {
            if (other.val != nullptr)
                val = new Key(*(other.val));
        }
        ~Node()
        {
            if (val != nullptr)
                delete val;
        }
    };
    Compare less = Compare(); /* the comparator */
    Node* root; /* the root of the treap */
    Node* end_p; /* the node of end() */
    /**
     * copy the treap rooted at the other
     */
    Node* copy(Node* other)
    {
        Node* cur = new Node(*other);
        if (other->left->val != nullptr) {
            cur->left = copy(other->left);
            cur->left->parent = cur;
        } else
            cur->left = end_p;
        if (other->right->val != nullptr) {
            cur->right = copy(other->right);
            cur->right->parent = cur;
        } else
            cur->right = end_p;
        cur->parent = end_p;
        return cur;
    }
    /**
     * destroy the tree
     */
    void destroy(Node* cur)
    {
        if (cur == end_p)
            return;
        destroy(cur->left);
        destroy(cur->right);
        delete cur;
        return;
    }
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
        if (cur == end_p)
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
        if (cur == end_p)
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
        while (cur->right != end_p)
            cur = cur->right;
        return cur;
    }
    /**
     * find the rank of the key in the treap
     */
    size_t rank(Node* cur, const Key& key) const
    {
        if (cur == end_p)
            return 0;
        if (lessequal(*(cur->val), key))
            return 1 + cur->left->size + rank(cur->right, key);
        else
            return rank(cur->left, key);
    }
    /**
     * pushup the size of the node
     */
    void pushup(Node* cur)
    {
        if (cur == end_p)
            return;
        cur->size = 1 + cur->left->size + cur->right->size;
        pushup(cur->parent);
        return;
    }
    /**
     * rotate the node to the left
     */
    void rotate_left(Node* cur)
    {
        Node* parent = cur->parent;
        Node* child = cur->right;
        cur->right = child->left;
        if (child->left != end_p)
            child->left->parent = cur;
        child->left = cur;
        cur->parent = child;
        child->parent = parent;
        if (parent != end_p) {
            if (parent->left == cur)
                parent->left = child;
            else
                parent->right = child;
        } else
            root = child;
        pushup(cur);
        return;
    }
    /**
     * rotate the node to the right
     */
    void rotate_right(Node* cur)
    {
        Node* parent = cur->parent;
        Node* child = cur->left;
        cur->left = child->right;
        if (child->right != end_p)
            child->right->parent = cur;
        child->right = cur;
        cur->parent = child;
        child->parent = parent;
        if (parent != end_p) {
            if (parent->left == cur)
                parent->left = child;
            else
                parent->right = child;
        } else
            root = child;
        pushup(cur);
        return;
    }
    /**
     * insert the node into the treap
     */
    void insert(Node* cur)
    {
        Node* parent = end_p;
        Node* p = root;
        while (p != end_p) {
            parent = p;
            if (less(*(cur->val), *(p->val)))
                p = p->left;
            else
                p = p->right;
        }
        cur->parent = parent;
        if (parent == end_p)
            root = cur;
        else if (less(*(cur->val), *(parent->val)))
            parent->left = cur;
        else
            parent->right = cur;
        cur->left = cur->right = end_p;
        cur->color = Node::RED;
        pushup(parent);
        insert_fix(cur);
        return;
    }
    /**
     * fix the treap after insertion
     */
    void insert_fix(Node* cur)
    {
        while (cur->parent->color == Node::RED) {
            if (cur->parent == cur->parent->parent->left) {
                Node* uncle = cur->parent->parent->right;
                /**
                 * case 1: uncle is red
                 */
                if (uncle->color == Node::RED) {
                    cur->parent->color = uncle->color = Node::BLACK;
                    cur->parent->parent->color = Node::RED;
                    cur = cur->parent->parent;
                } else {
                    /**
                     * case 2: uncle is black and cur is right child
                     */
                    if (cur == cur->parent->right) {
                        cur = cur->parent;
                        rotate_left(cur);
                    }
                    /**
                     * case 3: uncle is black and cur is left child
                     */
                    cur->parent->color = Node::BLACK;
                    cur->parent->parent->color = Node::RED;
                    rotate_right(cur->parent->parent);
                }
            } else {
                Node* uncle = cur->parent->parent->left;
                /**
                 * case 1: uncle is red
                 */
                if (uncle->color == Node::RED) {
                    cur->parent->color = uncle->color = Node::BLACK;
                    cur->parent->parent->color = Node::RED;
                    cur = cur->parent->parent;
                } else {
                    /**
                     * case 2: uncle is black and cur is left child
                     */
                    if (cur == cur->parent->left) {
                        cur = cur->parent;
                        rotate_right(cur);
                    }
                    /**
                     * case 3: uncle is black and cur is right child
                     */
                    cur->parent->color = Node::BLACK;
                    cur->parent->parent->color = Node::RED;
                    rotate_left(cur->parent->parent);
                }
            }
        }
        root->color = Node::BLACK;
        return;
    }
    /**
     * transplant the child to the parent
     */
    void transplant(Node* parent, Node* child)
    {
        std::swap(parent->size, child->size);
        std::swap(parent->color, child->color);
        if (child->parent == parent) {
            if (parent->left == child) {
                std::swap(parent->right, child->right);
                parent->left = child->left;
                child->left = parent;
                if (child->right != end_p)
                    child->right->parent = child;
            } else {
                std::swap(parent->left, child->left);
                parent->right = child->right;
                child->right = parent;
                if (child->left != end_p)
                    child->left->parent = child;
            }
            if (parent->parent == end_p)
                root = child;
            else if (parent->parent->left == parent)
                parent->parent->left = child;
            else
                parent->parent->right = child;
            child->parent = parent->parent;
            parent->parent = child;
            if (parent->left != end_p)
                parent->left->parent = parent;
            if (parent->right != end_p)
                parent->right->parent = parent;
        } else {
            std::swap(parent->left, child->left);
            std::swap(parent->right, child->right);
            std::swap(parent->parent, child->parent);
            if (child->left != end_p)
                child->left->parent = child;
            if (child->right != end_p)
                child->right->parent = child;
            if (child->parent != end_p) {
                if (child->parent->left == parent)
                    child->parent->left = child;
                else
                    child->parent->right = child;
            } else
                root = child;
            if (parent->left != end_p)
                parent->left->parent = parent;
            if (parent->right != end_p)
                parent->right->parent = parent;
            if (parent->parent != end_p) {
                if (parent->parent->left == child)
                    parent->parent->left = parent;
                else
                    parent->parent->right = parent;
            } else
                root = parent;
        }
        return;
    }
    /**
     * erase the node from the treap
     */
    void erase(Node* cur)
    {
        if (cur->left != end_p && cur->right != end_p) {
            Node* temp = cur->left;
            while (temp->right != end_p)
                temp = temp->right;
            transplant(cur, temp);
        }
        typename Node::Color color = cur->color;
        Node *parent = cur->parent, *child = end_p;
        child->parent = parent;
        if (cur->left == end_p) {
            child = cur->right;
            if (cur->parent != end_p) {
                if (cur->parent->left == cur)
                    cur->parent->left = child;
                if (cur->parent->right == cur)
                    cur->parent->right = child;
            } else
                root = child;
            child->parent = cur->parent;
        } else if (cur->right == end_p) {
            child = cur->left;
            if (cur->parent != end_p) {
                if (cur->parent->left == cur)
                    cur->parent->left = child;
                if (cur->parent->right == cur)
                    cur->parent->right = child;
            } else
                root = child;
            child->parent = cur->parent;
        }
        delete cur;
        pushup(parent);
        if (color == Node::BLACK) {
            if (child->color == Node::RED)
                child->color = Node::BLACK;
            else
                erase_fix(child);
        }
        return;
    }
    /**
     * fix the treap after erasing
     */
    void erase_fix(Node* cur)
    {
        Node* parent = cur->parent;
        while (cur != root && cur->color == Node::BLACK) {
            if (cur == parent->left) {
                Node* brother = parent->right;
                /**
                 * case 1: brother is red
                 */
                if (brother->color == Node::RED) {
                    brother->color = Node::BLACK;
                    parent->color = Node::RED;
                    rotate_left(parent);
                    brother = parent->right;
                }
                /**
                 * case 2: brother is black and both children of brother are black
                 */
                if (brother->left->color == Node::BLACK && brother->right->color == Node::BLACK) {
                    /**
                     * case 2.1: father is black
                     */
                    if (parent->color == Node::BLACK) {
                        brother->color = Node::RED;
                        cur = parent;
                        parent = cur->parent;
                    } else {
                        /**
                         * case 2.2: father is red
                         */
                        brother->color = Node::RED;
                        parent->color = Node::BLACK;
                        break;
                    }
                } else {
                    /**
                     * case 3: brother is black and left child of brother is red and right child of brother is black
                     */
                    if (brother->right->color == Node::BLACK) {
                        brother->left->color = Node::BLACK;
                        brother->color = Node::RED;
                        rotate_right(brother);
                        brother = parent->right;
                    }
                    /**
                     * case 4: brother is black and right child of brother is red
                     */
                    brother->color = parent->color;
                    parent->color = Node::BLACK;
                    brother->right->color = Node::BLACK;
                    rotate_left(parent);
                    break;
                }
            } else {
                Node* brother = parent->left;
                /**
                 * case 1: brother is red
                 */
                if (brother->color == Node::RED) {
                    brother->color = Node::BLACK;
                    parent->color = Node::RED;
                    rotate_right(parent);
                    brother = parent->left;
                }
                /**
                 * case 2: brother is black and both children of brother are black
                 */
                if (brother->left->color == Node::BLACK && brother->right->color == Node::BLACK) {
                    /**
                     * case 2.1: father is black
                     */
                    if (parent->color == Node::BLACK) {
                        brother->color = Node::RED;
                        cur = parent;
                        parent = cur->parent;
                    } else {
                        /**
                         * case 2.2: father is red
                         */
                        brother->color = Node::RED;
                        parent->color = Node::BLACK;
                        break;
                    }
                } else {
                    /**
                     * case 3: brother is black and right child of brother is red and left child of brother is black
                     */
                    if (brother->left->color == Node::BLACK) {
                        brother->right->color = Node::BLACK;
                        brother->color = Node::RED;
                        rotate_left(brother);
                        brother = parent->left;
                    }
                    /**
                     * case 4: brother is black and left child of brother is red
                     */
                    brother->color = parent->color;
                    parent->color = Node::BLACK;
                    brother->left->color = Node::BLACK;
                    rotate_right(parent);
                    break;
                }
            }
        }
        root->color = Node::BLACK;
        return;
    }

public:
    RBTree()
    {
        end_p = new Node();
        root = end_p;
    }
    RBTree(const RBTree& other)
    {
        end_p = new Node();
        if (other.root != other.end_p)
            root = copy(other.root);
        else
            root = end_p;
    }
    RBTree(RBTree&& other)
    {
        end_p = other.end_p;
        root = other.root;
        other.root = other.end_p = nullptr;
    }
    RBTree& operator=(const RBTree& other)
    {
        if (this == &other)
            return *this;
        destroy(root);
        delete end_p;
        end_p = new Node();
        if (other.root != other.end_p)
            root = copy(other.root);
        else
            root = end_p;
        return *this;
    }
    RBTree& operator=(RBTree&& other) noexcept
    {
        if (this == &other)
            return *this;
        destroy(root);
        delete end_p;
        root = other.root;
        end_p = other.end_p;
        other.root = other.end_p = nullptr;
        return *this;
    }
    ~RBTree()
    {
        if (root != nullptr)
            destroy(root);
        if (end_p != nullptr)
            delete end_p;
    }
    void clear()
    {
        if (root != nullptr)
            destroy(root);
        if (end_p != nullptr)
            delete end_p;
        end_p = new Node();
        root = end_p;
    }
    class iterator {
        friend class RBTree;

    private:
        Node* p; /* the pointer to the node */
        const RBTree* base; /* the base treap */
        iterator(Node* _p, const RBTree* _base)
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
        while (cur != end_p) {
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
        Node* cur = new Node(key, end_p, end_p, end_p);
        insert(cur);
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
        erase(it.p);
        return true;
    }
    /**
     * return the iterator to the first element
     */
    iterator begin() const
    {
        if (root == end_p)
            return iterator(end_p, this);
        Node* cur = root;
        while (cur->left != end_p)
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
        return root->size;
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