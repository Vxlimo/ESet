#include "src_fhqtreap.hpp"
#include "src_rbtree.hpp"
#include "src_skiplist.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
using std::cin, std::cout, std::endl;
int read()
{
    int f = 1, x = 0;
    char ch;
    do {
        ch = getchar();
        if (ch == '-')
            f = -1;
    } while (ch < '0' || ch > '9');
    do {
        x = x * 10 + ch - '0';
        ch = getchar();
    } while (ch >= '0' && ch <= '9');
    return f * x;
}

fhqTreap<int, std::less<int>> treap;
RBTree<int, std::less<int>> rbtree;
SkipList<int, std::less<int>> skiplist;
std::set<int, std::less<int>> set;
std::vector<int> operations;

int main()
{
    srand(time(0));
    time_t start, end;
    std::cout << "insert 10000 elements to set, skiplist, treap, rbtree" << std::endl;
    for (int i = 1; i <= 10000; i++) {
        int x = rand();
        set.emplace(x);
        skiplist.emplace(x);
        treap.emplace(x);
        rbtree.emplace(x);
    }

    std::cout << "random insert 1000 elements from set, skiplist, treap, rbtree" << std::endl;
    for (int i = 1; i <= 1000; i++)
        operations.push_back(rand());
    start = clock();
    for (int i = 1; i <= 1000; i++)
        set.emplace(operations[i]);
    end = clock();
    std::cout << "set insert time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        skiplist.emplace(operations[i]);
    end = clock();
    std::cout << "skiplist insert time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        treap.emplace(operations[i]);
    end = clock();
    std::cout << "treap insert time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        rbtree.emplace(operations[i]);
    end = clock();
    std::cout << "rbtree insert time: " << end - start << std::endl;

    std::cout << "random erase 1000 elements from set, skiplist, treap, rbtree" << std::endl;
    operations.clear();
    for (int i = 1; i <= 1000; i++)
        operations.push_back(rand());
    start = clock();
    for (int i = 1; i <= 1000; i++)
        set.erase(operations[i]);
    end = clock();
    std::cout << "set erase time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        skiplist.erase(operations[i]);
    end = clock();
    std::cout << "skiplist erase time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        treap.erase(operations[i]);
    end = clock();
    std::cout << "treap erase time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        rbtree.erase(operations[i]);
    end = clock();
    std::cout << "rbtree erase time: " << end - start << std::endl;

    std::cout << "random find 1000 elements from set, skiplist, treap, rbtree" << std::endl;
    operations.clear();
    for (int i = 1; i <= 1000; i++)
        operations.push_back(rand());
    start = clock();
    for (int i = 1; i <= 1000; i++)
        set.find(operations[i]);
    end = clock();
    std::cout << "set find time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        skiplist.find(operations[i]);
    end = clock();
    std::cout << "skiplist find time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        treap.find(operations[i]);
    end = clock();
    std::cout << "treap find time: " << end - start << std::endl;
    start = clock();
    for (int i = 1; i <= 1000; i++)
        rbtree.find(operations[i]);
    end = clock();
    std::cout << "rbtree find time: " << end - start << std::endl;

    std::cout << "iterator operation to set, skiplist, treap, rbtree" << std::endl;
    start = clock();
    for (auto it = set.begin(); it != set.end(); it++)
        ;
    end = clock();
    std::cout << "set iterator time: " << end - start << std::endl;
    start = clock();
    for (auto it = skiplist.begin(); it != skiplist.end(); it++)
        ;
    end = clock();
    std::cout << "skiplist iterator time: " << end - start << std::endl;
    start = clock();
    for (auto it = treap.begin(); it != treap.end(); it++)
        ;
    end = clock();
    std::cout << "treap iterator time: " << end - start << std::endl;
    start = clock();
    for (auto it = rbtree.begin(); it != rbtree.end(); it++)
        ;
    end = clock();
    std::cout << "rbtree iterator time: " << end - start << std::endl;

    std::cout << "copy operation to set, skiplist, treap, rbtree" << std::endl;
    start = clock();
    std::set<int, std::less<int>> set2 = set;
    end = clock();
    std::cout << "set copy time: " << end - start << std::endl;
    start = clock();
    SkipList<int, std::less<int>> skiplist2 = skiplist;
    end = clock();
    std::cout << "skiplist copy time: " << end - start << std::endl;
    start = clock();
    fhqTreap<int, std::less<int>> treap2 = treap;
    end = clock();
    std::cout << "treap copy time: " << end - start << std::endl;
    start = clock();
    RBTree<int, std::less<int>> rbtree2 = rbtree;
    end = clock();
    std::cout << "rbtree copy time: " << end - start << std::endl;

    std::cout << "clear operation to set, skiplist, treap, rbtree" << std::endl;
    start = clock();
    set.clear();
    end = clock();
    std::cout << "set clear time: " << end - start << std::endl;
    start = clock();
    skiplist.clear();
    end = clock();
    std::cout << "skiplist clear time: " << end - start << std::endl;
    start = clock();
    treap.clear();
    end = clock();
    std::cout << "treap clear time: " << end - start << std::endl;
    start = clock();
    rbtree.clear();
    end = clock();
    std::cout << "rbtree clear time: " << end - start << std::endl;
    return 0;
}
