#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <functional>
using namespace std;

template <typename T, typename CMP = less<T> >
class binary_heap {
private:
    int n;
    vector<T> array;
    CMP cmp;
    void down(int hole) {
        int child;
        T tmp = array[hole];
        for ( ; (hole << 1) <= n; hole = child)
        {
            child = hole << 1;
            if (child != n && cmp(array[child | 1], array[child]))
                ++child;
            if (cmp(array[child], tmp))
                array[hole] = array[child];
            else break;
        }
        array[hole] = tmp;
    }
public:
    explicit binary_heap(int capacity = 100)
                        : n(0), array(capacity) { }
    explicit binary_heap(const vector<T> & items)
    : array(items.size() + 10), n(items.size()) {
        for (int i = 0; i < items.size(); ++i)
            array[i + 1] = items[i];
        build_heap();
    }
    void build_heap() {
        for (int i = n / 2; i > 0; --i)
            down(i);
    }
    bool empty() const { return n == 0; }
    void push(const T & x) {
        if (n == array.size() - 1)
            array.resize(array.size() << 1);
        int hole = ++n;
        array[0] = x;
        for ( ; cmp(x, array[hole >> 1]); hole >>= 1)
            array[hole] = array[hole >> 1];
        array[hole] = array[0];
    }
    const T & top() const { return array[1]; }
    void pop() {
        array[1] = array[n--];
        down(1);
    }
};

template <typename T>
class leftist_heap {
private:
    struct node {
        T element;
        node * left;
        node * right;
        int npl;
        node(const T & e, node * lt = NULL, node * rt = NULL, int np = 0)
        : element(e), left(lt), right(rt), npl(np) { }
    };
public:
    leftist_heap() : n(0), root(NULL) { }
    leftist_heap(const leftist_heap<T> & rhs)
    : n(rhs.n), root(clone(rhs.root)) { }
    ~leftist_heap() { make_empty(root); }
    leftist_heap<T> & operator=(const leftist_heap<T> & rhs) {
        leftist_heap<T> copy = rhs;
        swap(*this, copy);
        return *this;
    }
    void merge(leftist_heap<T> & rhs) {
        root = merge(root, rhs.root);
        n += rhs.n;
        rhs.root = NULL;
        rhs.n = 0;
    }
    bool empty() const { return n == 0; }
    void clear() { make_empty(root); n = 0; }
    const T & top() const { return root->element; }
    void push(const T & x) { root = merge(new node(x), root); n++; }
    void pop() {
        if (empty()) return;
        node * old = root;
        root = merge(root->left, root->right);
        delete old;
        n--;
    }
private:
    node * merge(node * h1, node * h2) {
        if (h1 == NULL) return h2;
        if (h2 == NULL) return h1;
        if (h1->element < h2->element)
            return merge1(h1, h2);
        else return merge1(h2, h1);
    }
    node * merge1(node * h1, node * h2) {
        if (h1->left == NULL) h1->left = h2;
        else {
            h1->right = merge(h1->right, h2);
            if (h1->left->npl < h1->right->npl)
                swap_children(h1);
            h1->npl = h1->right->npl + 1;
        }
        return h1;
    }
    void swap_children(node * t) {
        node * tmp = t->left;
        t->left = t->right;
        t->right = tmp;
    }
    void make_empty(node * & t) {
        if (t != NULL) {
            make_empty(t->left);
            make_empty(t->right);
            delete t;
            t = NULL;
        }
    }
    node * clone(node * t) const {
        if (t == NULL) return NULL;
        return new node(t->element, clone(t->left), clone(t->right), t->npl);
    }
private:
    int n;
    node * root;
};

template <typename T>
class pairing_heap {
private:
    struct node {
        T element;
        node * left;
        node * next;
        node * prev;
        node(const T & e)
        : element(e), left(NULL), next(NULL), prev(NULL) { }
    };
public:
    typedef node * position;
    pairing_heap() : n(0), root(NULL) { }
    pairing_heap(const pairing_heap<T> & rhs)
    : n(rhs.n), root(clone(rhs.root)) { }
    ~pairing_heap() { make_empty(root); }
    pairing_heap<T> & operator=(const pairing_heap<T> & rhs) {
        pairing_heap<T> copy = rhs;
        swap(*this, copy);
        return *this;
    }
    position push(const T & x) {
        n++;
        node * t = new node(x);
        if (root == NULL) root = t;
        else compare_and_link(root, t);
        return t;
    }
    const T & top() const {
        return root->element;
    }
    void pop() {
        if (empty()) return;
        n--;
        node * old = root;
        if (root->left == NULL) root = NULL;
        else root = combine_siblings(root->left);
        delete old;
    }
    void decrease_key(position p, const T & new_val) {
        if (p->element < new_val) return;
        p->element = new_val;
        if (p != root) {
            if (p->next != NULL)
                p->next->prev = p->prev;
            if (p->prev->left == p)
                p->prev->left = p->next;
            else p->prev->next = p->next;
            p->next = NULL;
            compare_and_link(root, p);
        }
    }
    void merge(pairing_heap<T> & rhs) {
        compare_and_link(root, rhs.root);
        n += rhs.n;
        rhs.root = NULL;
        rhs.n = 0;
    }
    bool empty() const { return n == 0; }
    void clear() { make_empty(root); n = 0; }
private:
    void compare_and_link(node * & first, node * second) {
        if (second == NULL) return;
        if (second->element < first->element) {
            second->prev = first->prev;
            first->prev = second;
            first->next = second->left;
            if (first->next != NULL)
                first->next->prev = first;
            second->left = first;
            first = second;
        } else {
            second->prev = first;
            first->next = second->next;
            if (first->next != NULL)
                first->next->prev = first;
            second->next = first->left;
            if (second->next != NULL)
                second->next->prev = second;
            first->left = second;
        }
    }
    node * combine_siblings(node * first_sibling) {
        if (first_sibling->next == NULL)
            return first_sibling;
        static vector<node *> tree_arr(5);
        int num_siblings = 0;
        for ( ; first_sibling != NULL; ++num_siblings) {
            if (num_siblings == tree_arr.size())
                tree_arr.resize(num_siblings * 2);
            tree_arr[num_siblings] = first_sibling;
            first_sibling->prev->next = NULL;
            first_sibling = first_sibling->next;
        }
        if (num_siblings == tree_arr.size())
            tree_arr.resize(num_siblings + 1);
        tree_arr[num_siblings] = NULL;
        int i = 0;
        for ( ; i + 1 < num_siblings; i += 2)
            compare_and_link(tree_arr[i], tree_arr[i + 1]);
        int j = i - 2;
        if (j == num_siblings - 3)
            compare_and_link(tree_arr[j], tree_arr[j + 2]);
        for ( ; j >= 2; j -= 2)
            compare_and_link(tree_arr[j - 2], tree_arr[j]);
        return tree_arr[0];
    }
    void make_empty(node * & t) {
        if (t != NULL) {
            make_empty(t->left);
            make_empty(t->next);
            delete t;
            t = NULL;
        }
    }
    node * clone(node * t) {
        if (t == NULL) return NULL;
        node * p = new node(t->element);
        if ((p->left = clone(t->left)) != NULL)
            p->left->prev = p;
        if ((p->next = clone(t->next)) != NULL)
            p->next->prev = p;
        return p;
    }
private:
    int n;
    node * root;
};

#endif // _HEAP_H
