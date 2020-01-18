#ifndef _TREE_H
#define _TREE_H

#include "_tool.h"

#include <cstdlib>
#include <cstring>
#include <climits>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class avl_tree {
private:
    struct node {
        T element;
        node * left;
        node * right;
        int size;
        int height;
        node(const T & ele, node * lt = NULL, node * rt = NULL, int n = 1, int h = 0)
            : element(ele), left(lt), right(rt), size(n), height(h) { }
    };
    node * root;
public:
    avl_tree() : root(NULL) { }
    avl_tree(const avl_tree & rhs)
    { root = clone(rhs.root); }
    ~avl_tree() { clear(); }
    void get_kth(int k, T & x) const {
        if (k <= 0 || k > root->size) return;
        x = get_kth(k, root);
    }
    int rank(const T & x) const
    { return rank(x, root); }
    bool contain(const T & x) const
    { return contain(x, root); }
    void insert(const T & x)
    { insert(x, root); }
    void erase(const T & x)
    { erase(x, root); }
    bool empty() const { return root == NULL; }
    void clear() { make_empty(root); }
    avl_tree & operator=(const avl_tree & rhs) {
        avl_tree copy = rhs;
        swap(*this, copy);
        return *this;
    }
private:
    const T & get_kth(int k, node * t) const {
        while (size(t->left) >= k)
            t = t->left;
        if (k == size(t->left) + 1) return t->element;
        return get_kth(k - 1 - size(t->left), t->right);
    }
    int rank(const T & x, node * t) const {
        if (x < t->element) return rank(x, t->left);
        else if (t->element < x) return size(t->left) + 1 + rank(x, t->right);
        else return size(t->left) + 1;
    }
    bool contain(const T & x, node * t) const {
        while (t != NULL)
            if (x < t->element)
                t = t->left;
            else if (t->element < x)
                t = t->right;
            else return true;
        return false;
    }
    void insert(const T & x, node * & t) {
        if (t == NULL)
            t = new node(x);
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
        balance(t);
    }
    void erase(const T & x, node * & t) {
        if (t == NULL) return;
        if (x < t->element)
            erase(x, t->left);
        else if (t->element < x)
            erase(x, t->right);
        else if (t->left != NULL && t->right != NULL) {
            node * p = t->right;
            while (p->left != NULL)
                p = p->left;
            t->element = p->element;
            erase(t->element, t->right);
        } else {
            node * old = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete old;
        }
        balance(t);
    }
    void balance(node * & t) {
        if (t == NULL) return;
        if (height(t->left) - height(t->right) > 1)
            if (height(t->left->left) >= height(t->left->right))
                rotate_with_left_child(t);
            else double_with_left_child(t);
        else
        if (height(t->right) - height(t->left) > 1)
            if (height(t->right->right) >= height(t->right->left))
                rotate_with_right_child(t);
            else double_with_right_child(t);
        t->size = size(t->left) + size(t->right) + 1;
        t->height = max(height(t->left), height(t->right)) + 1;
    }
    void make_empty(node * & t) {
        if (t != NULL) {
            make_empty(t->left);
            make_empty(t->right);
            delete t;
        }
        t = NULL;
    }
    node * clone(node * t) const {
        if (t == NULL) return NULL;
        else return new node(t->element, clone(t->left), clone(t->right), t->size, t->height);
    }
    int height(node * t) const
    { return t == NULL ? -1 : t->height; }
    int size(node * t) const
    { return t == NULL ? 0 : t->size; }
    void rotate_with_left_child(node * & k2) {
        node * k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->size = size(k2->left) + size(k2->right) + 1;
        k1->size = size(k1->left) + size(k1->right) + 1;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }
    void rotate_with_right_child(node * & k1) {
        node * k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->size = size(k1->left) + size(k1->right) + 1;
        k2->size = size(k2->left) + size(k2->right) + 1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }
    void double_with_left_child(node * & k3) {
        rotate_with_right_child(k3->left);
        rotate_with_left_child(k3);
    }
    void double_with_right_child(node * & k1) {
        rotate_with_left_child(k1->right);
        rotate_with_right_child(k1);
    }
};

template <typename T>
class treap {
private:
    struct node {
        T element;
        node * left;
        node * right;
        int size;
        int priority;
        node(const T & ele, node * lt = NULL, node * rt = NULL, int n = 1, int pr = INT_MAX)
            : element(ele), left(lt), right(rt), size(n), priority(pr) { }
    };
    node * root;
public:
    treap() : root(NULL) {
        srand(time(NULL));
    }
    treap(const treap & rhs) {
        srand(time(NULL));
        root = clone(rhs.root);
    }
    ~treap() { clear(); }
    void get_kth(int k, T & x) const {
        if (k <= 0 || k > root->size) return;
        x = get_kth(k, root);
    }
    int rank(const T & x) const
    { return rank(x, root); }
    bool contain(const T & x) const
    { return contain(x, root); }
    void insert(const T & x)
    { insert(x, root); }
    void erase(const T & x)
    { erase(x, root); }
    bool empty() const { return root == NULL; }
    void clear() { make_empty(root); }
    treap & operator=(const treap & rhs) {
        treap copy = rhs;
        swap(*this, copy);
        return *this;
    }
private:
    const T & get_kth(int k, node * t) const {
        while (size(t->left) >= k)
            t = t->left;
        if (k == size(t->left) + 1) return t->element;
        return get_kth(k - 1 - size(t->left), t->right);
    }
    int rank(const T & x, node * t) const {
        if (x < t->element) return rank(x, t->left);
        else if (t->element < x) return size(t->left) + 1 + rank(x, t->right);
        else return size(t->left) + 1;
    }
    bool contain(const T & x, node * t) const {
        while (t != NULL)
            if (x < t->element)
                t = t->left;
            else if (t->element < x)
                t = t->right;
            else return true;
        return false;
    }
    void insert(const T & x, node * & t) {
        if (t == NULL)
            t = new node(x, NULL, NULL, 1, rand());
        else if (x < t->element) {
            insert(x, t->left);
            if (priority(t->left) < priority(t))
                rotate_with_left_child(t);
        }
        else if (t->element < x) {
            insert(x, t->right);
            if (priority(t->right) < priority(t))
                rotate_with_right_child(t);
        }
        t->size = size(t->left) + size(t->right) + 1;
    }
    /*void erase(const T & x, node * & t) {
        if (t == NULL) return;
        if (x < t->element)
            erase(x, t->left);
        else if (t->element < x)
            erase(x, t->right);
        else if (t->left != NULL && t->right != NULL) {
            if (priority(t->left) < priority(t->right))
                rotate_with_left_child(t);
            else rotate_with_right_child(t);
            erase(x, t);
        }
        else {
            node * old = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete old;
        }
        if (t != NULL)
            t->size = size(t->left) + size(t->right) + 1;
    }*/
    void erase(const T & x, node * & t) {
        if (t == NULL) return;
        if (x < t->element)
            erase(x, t->left);
        else if (t->element < x)
            erase(x, t->right);
        else if (t->left != NULL && t->right != NULL) {
            node * p = t->right;
            while (p->left != NULL)
                p = p->left;
            t->element = p->element;
            erase(t->element, t->right);
        } else {
            node * old = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete old;
        }
        if (t != NULL)
            t->size = size(t->left) + size(t->right) + 1;
    }
    void make_empty(node * & t) {
        if (t != NULL) {
            make_empty(t->left);
            make_empty(t->right);
            delete t;
        }
        t = NULL;
    }
    node * clone(node * t) const {
        if (t == NULL) return NULL;
        else return new node(t->element, clone(t->left), clone(t->right), t->size, t->priority);
    }
    int priority(node * t) const
    { return t == NULL ? INT_MAX : t->priority; }
    int size(node * t) const
    { return t == NULL ? 0 : t->size; }
    void rotate_with_left_child(node * & k2) {
        node * k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->size = size(k2->left) + size(k2->right) + 1;
        k1->size = size(k1->left) + size(k1->right) + 1;
        k2 = k1;
    }
    void rotate_with_right_child(node * & k1) {
        node * k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->size = size(k1->left) + size(k1->right) + 1;
        k2->size = size(k2->left) + size(k2->right) + 1;
        k1 = k2;
    }
};

template <typename T>
class splay_tree {
private:
    struct node {
        T element;
        node * left;
        node * right;
        node * parent;
        T sum;
        T lazy;
        int size;
        node(const T & ele, node * lt = NULL, node * rt = NULL, node * par = NULL, int n = 1)
            : element(ele), left(lt), right(rt), parent(par), sum(ele), lazy(0), size(n) { }
    };
    node * root;
    node * min_node;
    node * max_node;
public:
    splay_tree() {
        min_node = new node(0);
        max_node = new node(0);
        max_node->left = min_node;
        min_node->parent = max_node;
        max_node->size = 2;
        root = max_node;
    }
    splay_tree(const splay_tree & rhs) = delete;
//    : root(clone(rhs.root, NULL)) { }
    ~splay_tree() { make_empty(root); }
    /**
     * k should be in the range [1, n];
     */
    bool get_kth(int k, T & x) {
        if (k < 1 || k > root->size - 2) return false;
        else {
            get_kth(k);
            x = root->element;
            return true;
        }
    }
//    void merge(splay_tree & rhs) {
//        if (rhs.empty()) return;
//        if (empty()) {
//            root = rhs.root;
//            rhs.root = NULL;
//            return;
//        }
//        find_max();
//        rhs.find_max();
//        if (root->element < rhs.root->element) {
//            root->right = rhs.root;
//            root->right->parent = root;
//            rhs.root = NULL;
//        } else {
//            rhs.root->right = root;
//            root->parent = rhs.root;
//            root = rhs.root;
//            rhs.root = NULL;
//        }
//    }
    /**
     * l and r should be in the range [1, n];
     */
    T query(int l, int r) {
        if (l < 1 || l > root->size - 2 || r < 1 || r > root->size - 2 || l > r) return 0;
        splay(get_kth(l, root), root);
        splay(get_kth(r + 2, root), root->right);
        return root->right->left->sum;
    }
    /**
     * l and r should be in the range [1, n];
     */
    void update(int l, int r, const T & add) {
        if (l < 1 || l > root->size - 2 || r < 1 || r > root->size - 2 || l > r) return;
        splay(get_kth(l, root), root);
        splay(get_kth(r + 2, root), root->right);
        root->right->left->element += add;
        root->right->left->lazy += add;
        root->right->left->sum += (r - l + 1) * add;
        root->right->sum += (r - l + 1) * add;
        root->sum += (r - l + 1) * add;
    }
    /**
     * k should be in the range [1, n + 1];
     */
    void insert(int k, const T & x) {
        if (k < 1 || k > root->size - 1) return;
        else {
            get_kth(k);
            push_down(root);
            node * t = new node(x, root->left, root, NULL, root->size + 1);
            t->left->parent = t;
            t->sum = x + root->sum;
            root->left = NULL;
            root->parent = t;
            root->sum = root->element + sum(root->right);
            root->size = 1 + size(root->right);
            root = t;
        }
    }
    /**
     * k should be in the range [1, n];
     */
    void erase(int k) {
        if (k < 1 || k > root->size - 2) return;
        get_kth(k);
        push_down(root);
        splay(find_max(root->left), root->left);
        push_down(root->left);
        node * new_tree = root->left;
        new_tree->right = root->right;
        new_tree->right->parent = new_tree;
        delete root;
        root = new_tree;
        root->sum = root->element + sum(root->left) + sum(root->right);
        root->size = 1 + size(root->left) + size(root->right);
    }
    int size() const { return root->size - 2; }
    bool empty() const { return size() == 0; }
//    void clear() {
//        while (!empty()) {
//            find_max();
//            erase(root->element);
//        }
//    }
    splay_tree & operator=(const splay_tree & rhs) = delete;
//    {
//        splay_tree copy = rhs;
//        swap(*this, copy);
//        return *this;
//    }
    const T & find_max() {
        splay(max_node, root);
        node * ptr = find_max(max_node->left);
        splay(ptr, root);
        return ptr->element;
    }
    void walk_tree() const {
        walk_tree(root);
        cout << endl;
    }
private:
    void walk_tree(node * t) const {
        if (t) {
            walk_tree(t->left);
            cout << t->element << ' ' << t->sum << ' ' << t->lazy << endl;
            walk_tree(t->right);
        }
    }
    node * find_max(node * r) {
        node * ptr = r;
        while (ptr->right != NULL)
            ptr = ptr->right;
        return ptr;
    }
    /**
     * k should be in the range [1, n + 1];
     */
    void get_kth(int k) {
        k++;
        node * p = get_kth(k, root);
        splay(p, root);
    }
    node * get_kth(int k, node * t) const {
        while (size(t->left) >= k)
            t = t->left;
        if (k == size(t->left) + 1) return t;
        return get_kth(k - 1 - size(t->left), t->right);
    }
//    void merge(node * & r1, node * & r2) {
//        if (r2 == NULL) return;
//        if (r1 == NULL) {
//            r1 = r2;
//            r2 = NULL;
//            return;
//        }
//        find_max(r1);
//        find_max(r2);
//        if ()
//    }
    void make_empty(node * & t) {
        if (t != NULL) {
            make_empty(t->left);
            make_empty(t->right);
            delete t;
        }
        t = NULL;
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        else {
            node * r = new node(t->element);
            r->left = clone(t->left, r);
            r->right = clone(r->right, r);
            r->parent = p;
            r->size = t->size;
            return r;
        }
    }
    int size(node * t) const
    { return t == NULL ? 0 : t->size; }
    T sum(node * t) const
    { return t == NULL ? 0 : t->sum; }
    T lazy(node * t) const
    { return t == NULL ? 0 : t->lazy; }
    void push_down(node * t) {
        if (t->lazy != 0) {
            if (t->left != NULL) {
                t->left->lazy += t->lazy;
                t->left->element += t->lazy;
                t->left->sum += t->left->size * t->lazy;
            }
            if (t->right != NULL) {
                t->right->lazy += t->lazy;
                t->right->element += t->lazy;
                t->right->sum += t->right->size * t->lazy;
            }
            t->lazy = 0;
        }
    }
    void rotate_with_left_child(node * k2) {
        node * p = k2->parent;
        node * k1 = k2->left;
        push_down(k2); push_down(k1);
        k2->sum = k2->element + sum(k1->right) + sum(k2->right);
        k1->sum = k1->element + sum(k2) + sum(k1->left);
        k2->left = k1->right;
        if (k2->left != NULL)
            k2->left->parent = k2;
        k1->parent = p;
        if (p != NULL)
            if (p->left == k2) p->left = k1;
            else p->right = k1;
        k1->right = k2;
        k2->parent = k1;
        k2->size = size(k2->left) + size(k2->right) + 1;
        k1->size = size(k1->left) + size(k1->right) + 1;
    }
    void rotate_with_right_child(node * k1) {
        node * p = k1->parent;
        node * k2 = k1->right;
        push_down(k1); push_down(k2);
        k1->sum = k1->element + sum(k1->left) + sum(k2->left);
        k2->sum = k2->element + sum(k1) + sum(k2->right);
        k1->right = k2->left;
        if (k1->right != NULL)
            k1->right->parent = k1;
        k2->parent = p;
        if (p != NULL)
            if (p->left == k1) p->left = k2;
            else p->right = k2;
        k2->left = k1;
        k1->parent = k2;
        k1->size = size(k1->left) + size(k1->right) + 1;
        k2->size = size(k2->left) + size(k2->right) + 1;
    }
    void splay(node * t, node * & r) {
        node * rp = r->parent;
        while (t->parent != rp) {
            node * p = t->parent;
            if (p == r)
                if (t == r->left)
                    rotate_with_left_child(p);
                else rotate_with_right_child(p);
            else {
                node * g = p->parent;
                if (t == p->left && p == g->left) {
                    rotate_with_left_child(g);
                    rotate_with_left_child(p);
                } else if (t == p->right && p == g->right) {
                    rotate_with_right_child(g);
                    rotate_with_right_child(p);
                } else if (t == p->left && p == g->right) {
                    rotate_with_left_child(p);
                    rotate_with_right_child(g);
                } else {
                    rotate_with_right_child(p);
                    rotate_with_left_child(g);
                }
            }
        }
        r = t;
    }
    /*void splay(const K & k, node * & t) {
        node * left_tree_max, * right_tree_min;
        static node header;
        header.left = header.right = null;
        left_tree_max = right_tree_min = &header;
        null->element.key = k;
        for ( ; ; )
            if (k < t->element.key) {
                if (k < t->left->element.key)
                    rotate_with_left_child(t);
                if (t->left == null) break;
                right_tree_min->left = t;
                right_tree_min = t;
                t = t->left;
            }
            else if (t->element.key < k) {
                if (t->right->element.key < k)
                    rotate_with_right_child(t);
                if (t->right == null) break;
                left_tree_max->right = t;
                left_tree_max = t;
                t = t->right;
            }
            else break;
        left_tree_max->right = t->left;
        right_tree_min->left = t->right;
        t->left = header.right;
        t->right = header.left;
    }*/
};

const int CHAR_SET = 256;
template <class T>
class trie {
private:
    struct node {
        int id;
        T value;
        node * fail;
        node * children[CHAR_SET];
        node() : id(-1), value(), fail(NULL) { memset(children, 0, sizeof(children)); }
    };
public:
    trie() : n(0) { root = new node; }
    ~trie() { make_free(root); }
    T & operator[](const char * str) {
        node * p = root;
        while (true)
        {
            if (*str == 0) {
                if (p->id == -1)
                    p->id = n++;
                break;
            }
            int k = get_key(*str);
            if (p->children[k] == NULL)
                p->children[k] = new node;
            p = p->children[k];
            ++str;
        }
        return p->value;
    }
    void build_trie() {
        queue<node *> q;
        q.push(root);
        while (!q.empty()) {
            node * u = q.front();
            q.pop();
            for (int i = 0; i < CHAR_SET; i++) {
                if (u->children[i] != NULL) {
                    if (u == root)
                        u->children[i]->fail = root;
                    else {
                        node * v = u->fail;
                        while (v != NULL) {
                            if (v->children[i] != NULL) {
                                u->children[i]->fail = v->children[i];
                                break;
                            }
                            v = v->fail;
                        }
                        if (v == NULL)
                            u->children[i]->fail = root;
                    }
                    q.push(u->children[i]);
                }
            }
        }
    }
    vector<pair<int, int> > aho_corasick_algorithm(const char * str) const {
        vector<pair<int, int> > v;
        int len = strlen(str);
        node * p = root;
        for (int i = 0; i < len; i++) {
            int k = get_key(str[i]);
            while (p->children[k] == NULL && p != root)
                p = p->fail;
            if (p->children[k] == NULL)
                continue;
            p = p->children[k];
            node * t = p;
            while (t != root) {
                if (t->id != -1)
                    v.push_back(make_pair(t->id, i));
                t = t->fail;
            }
        }
        return v;
    }
private:
    node * root;
    int n;
    int get_key(char c) const { return c; }
    void make_free(node * & p) {
        if (p == NULL) return;
        for (int i = 0; i < CHAR_SET; i++)
            make_free(p->children[i]);
        delete p;
        p = NULL;
    }
};

const int N = 100000;

class palindrome_automaton {
private:
    vector<vector<int> > ch;
    vector<int> fail, len, chr, sz;
    int last, n_chr, n_node;
    int new_node(int l) {
        ch[n_node].assign(CHAR_SET, 0);
        len[n_node] = l;
        return n_node++;
    }
    int get_fail(int x) const {
        while (chr[n_chr - len[x] - 1] != chr[n_chr])
            x = fail[x];
        return x;
    }
    int get_chr(int c) const { return c; }
public:
    explicit palindrome_automaton(int _n = N)
    : ch(_n, vector<int>(CHAR_SET)), fail(_n),
      len(_n), chr(_n), sz(_n) { clear(); }
    void clear() {
        n_node = 0; new_node(0); new_node(-1);
        fail[0] = 1; last = 0;
        n_chr = 0; chr[n_chr] = -1;
        sz.assign(sz.size(), 0);
    }
    bool add(int c) {
        c = get_chr(c);
        chr[++n_chr] = c;
        int u = get_fail(last);
        bool b = false;
        if (!ch[u][c]) {
            int v = new_node(len[u] + 2);
            fail[v] = ch[get_fail(fail[u])][c];
            ch[u][c] = v;
            b = true;
        }
        sz[last = ch[u][c]]++;
        return b;
    }
    int size() const { return n_node; }
    const vector<int> & calc_size() {
        for (int i = n_node; i >= 0; i--)
            sz[fail[i]] += sz[i];
        return sz;
    }
    /**
     * Tip for matching:
     *
     *  --------- str[0] = -1, str[1..n], p = 1 ---------
     *
        while (p != 1 && (!ch[p][get_chr(str[i])] || str[i - len[p] - 1] != str[i])) p = fail[p];
        if (!ch[p][get_chr(str[i])]) { p = 1; continue; }
        p = ch[p][get_chr(str[i])];
     *
     */
};

class suffix_automaton {
private:
    vector<vector<int> > ch;
    vector<int> link, len, sz, cnt, rk;
    int n, last;
    int get_chr(int c) const { return c; }
public:
    suffix_automaton(int _n = N)
    : ch(_n, vector<int>(CHAR_SET)),
      link(_n), len(_n), sz(_n),
      cnt(_n), rk(_n), n(1), last(1) { }
    int extend(int c) {
        c = get_chr(c);
        int p = last, np = ++n;
        len[np] = len[p] + 1;
        for ( ; p && !ch[p][c]; p = link[p])
            ch[p][c] = np;
        if (!p) link[np] = 1;
        else {
            int q = ch[p][c];
            if (len[q] == len[p] + 1)
                link[np] = q;
            else {
                int nq = ++n;
                ch[nq] = ch[q];
                link[nq] = link[q];
                len[nq] = len[p] + 1;
                link[q] = link[np] = nq;
                for ( ; p && ch[p][c] == q; p = link[p])
                    ch[p][c] = nq;
            }
        }
        sz[last = np]++;
        return len[last] - len[link[last]];
    }
    int size() const { return n; }
    const vector<int> & calc_size() {
        for (int i = 1; i <= n; i++) cnt[len[i]]++;
        for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
        for (int i = 1; i <= n; i++) rk[cnt[len[i]]--] = i;
        for (int i = n; i >= 1; i--) sz[link[rk[i]]] += sz[rk[i]];
        return sz;
    }
    /**
     * Tip for matching:
     *
     *  --------- str[0..n-1], p = 1 ---------
     *
        if (ch[p][get_chr(str[i])]) { // Accepted!
            p = ch[p][get_chr(str[i])];
        } else { // Failed!
            while (p != 1 && !ch[p][get_chr(str[i])])
                p = link[p];
            if (!ch[p][get_chr(str[i])]) { continue; }
            p = ch[p][get_chr(str[i])];
        }
     *
     */
};

namespace ext_suffix_automaton {

const int M = N * 2;
const int MAXN = M * 27;
const int CHAR_SET = 26;
int rt[M], ls[MAXN], rs[MAXN], val[MAXN], pos[MAXN], n_node, m;
void push_up(int o) {
    val[o] = max(val[ls[o]], val[rs[o]]);
    if (val[o]) {
        if (val[o] == val[ls[o]]) pos[o] = pos[ls[o]];
        else pos[o] = pos[rs[o]];
    } else pos[o] = 0;
}
int merge(int u, int v, int l, int r) {
    if (!u) return v;
    if (!v) return u;
    int o = ++n_node;
    if (l == r) {
        val[o] = val[u] + val[v];
        if (val[o]) pos[o] = l;
        else pos[o] = 0;
        return o;
    }
    int mid = (l + r) >> 1;
    ls[o] = merge(ls[u], ls[v], l, mid);
    rs[o] = merge(rs[u], rs[v], mid + 1, r);
    push_up(o);
    return o;
}
void add(int & o, int l, int r, int x, int y) {
    if (!o) o = ++n_node;
    if (l == r) {
        val[o] += y;
        if (val[o]) pos[o] = l;
        else pos[o] = 0;
        return;
    }
    int mid = (l + r) >> 1;
    if (x <= mid) add(ls[o], l, mid, x, y);
    else add(rs[o], mid + 1, r, x, y);
    push_up(o);
}
pair<int, int> get(int o, int l, int r, int x, int y) {
    if (x <= l && y >= r) return {pos[o], val[o]};
    int mid = (l + r) >> 1;
    if (y <= mid) return get(ls[o], l, mid, x, y);
    else if (x > mid) return get(rs[o], mid + 1, r, x, y);
    else {
        auto res1 = get(ls[o], l, mid, x, y);
        auto res2 = get(rs[o], mid + 1, r, x, y);
        if (res1.second < res2.second) return res2;
        else return res1;
    }
}
class suffix_automaton {
private:
    int ch[M][CHAR_SET], link[M], len[M], cnt[M], rk[M], n, last;
    int pos[N], anc[M][23];
    vector<int> g[M];
    int get_chr(int c) const { return c - 'a'; }
    int extend(int c, int id) {
        c = get_chr(c);
        int p = last;
        if (!ch[p][c]) {
            int np = ++n;
            len[np] = len[p] + 1;
            for ( ; p && !ch[p][c]; p = link[p])
                ch[p][c] = np;
            if (!p) link[np] = 1;
            else {
                int q = ch[p][c];
                if (len[q] == len[p] + 1)
                    link[np] = q;
                else {
                    int nq = ++n;
                    for (int i = 0; i < CHAR_SET; i++)
                        ch[nq][i] = ch[q][i];
                    link[nq] = link[q];
                    len[nq] = len[p] + 1;
                    link[q] = link[np] = nq;
                    for ( ; p && ch[p][c] == q; p = link[p])
                        ch[p][c] = nq;
                }
            }
            last = np;
        } else if (len[ch[p][c]] != len[p] + 1) {
            int q = ch[p][c], nq = ++n;
            for (int i = 0; i < CHAR_SET; i++)
                ch[nq][i] = ch[q][i];
            link[nq] = link[q];
            len[nq] = len[p] + 1;
            link[q] = nq;
            for ( ; p && ch[p][c] == q; p = link[p])
                ch[p][c] = nq;
            last = nq;
        } else last = ch[p][c];
        if (id) add(rt[last], 1, m, id, 1);
        return last;
    }
    void dfs(int u, int p) {
        anc[u][0] = p;
        for (int i = 1; i < 23; i++)
            anc[u][i] = anc[anc[u][i - 1]][i - 1];
        for (auto v : g[u]) dfs(v, u);
    }
public:
    suffix_automaton(int m = 0) : n(1), last(1) { }
    void insert(const string & s, int id) {
        last = 1;
        for (int i = 0; i < s.length(); i++) {
            if (!id) pos[i + 1] = extend(s[i], id);
            else extend(s[i], id);
        }
    }
    /**
     * Tips for construction:
     * You can also construct S.A. on a tree such as a trie:
     * ------------------------------------------------------------
        // build(tree_root);
        void build(int u) {
            par[u] = extend(s[u]);
            for (auto v : g[u]) {
                last = par[u];
                build(v);
            }
        }
     * ------------------------------------------------------------
     */
    void build() {
        for (int i = 2; i <= n; i++) g[link[i]].push_back(i);
        dfs(1, 1);
        for (int i = 1; i <= n; i++) cnt[len[i]]++;
        for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
        for (int i = 1; i <= n; i++) rk[cnt[len[i]]--] = i;
        for (int i = n; i >= 1; i--)
            rt[link[rk[i]]] = merge(rt[link[rk[i]]], rt[rk[i]], 1, m);
    }
    pair<int, int> query(int x, int y, int l, int r) {
        l = r - l + 1; r = pos[r];
        for (int i = 22; i >= 0; i--)
            if (len[anc[r][i]] >= l) r = anc[r][i];
        return get(rt[r], 1, m, x, y);
    }
    int size() const { return n; }
};

}

class trie_2 {
private:
    vector<vector<int> > ch;
    vector<int> id, sz, fail, q;
    int n, cnt, front, rear;
    int get_key(char c) const { return c; }
public:
    explicit trie_2(int _n = N)
    : ch(_n, vector<int>(CHAR_SET)),
      id(_n), sz(_n), fail(_n),
      q(_n), n(1), cnt(1) { }
    void insert(const char * str) {
        int u = 1;
        while (true) {
            sz[u]++;
            if (*str == 0) {
                if (!id[u]) id[u] = n++;
                break;
            }
            int v = get_key(*str);
            if (!ch[u][v]) ch[u][v] = ++cnt;
            u = ch[u][v];
            ++str;
        }
    }
    int find(const char * str) const {
        int u = 1;
        while (true) {
            if (*str == 0)  return sz[u];
            int v = get_key(*str);
            if (!ch[u][v]) return 0;
            u = ch[u][v];
            ++str;
        }
        return 0;
    }
    void build_trie_2() {
        front = 0; rear = 0;
        for (int i = 0; i < CHAR_SET; i++)
            if (ch[1][i]) {
                fail[ch[1][i]] = 1;
                q[rear++] = ch[1][i];
            } else ch[1][i] = 1;
        while (front != rear) {
            int u = q[front++];
            for (int i = 0; i < CHAR_SET; i++)
                if (ch[u][i]) {
                    fail[ch[u][i]] = ch[fail[u]][i];
                    q[rear++] = ch[u][i];
                } else ch[u][i] = ch[fail[u]][i];
        }
    }
    vector<pair<int, int> > aho_corasick_2(const char * str) const {
        vector<pair<int, int> > r;
        int len = strlen(str), u = 1;
        for (int i = 0; i < len; i++) {
            int v = get_key(str[i]);
            v = u = ch[u][v];
            while (v != 1) {
                if (id[v]) r.push_back(make_pair(id[v], i));
                v = fail[v];
            }
        }
        return r;
    }
    void build_trie() {
        q[0] = 1; front = 0; rear = 1;
        while (front != rear) {
            int u = q[front++];
            for (int i = 0; i < CHAR_SET; i++) {
                if (ch[u][i]) {
                    if (u == 1) fail[ch[u][i]] = 1;
                    else {
                        int v = fail[u];
                        while (v) {
                            if (ch[v][i]) {
                                fail[ch[u][i]] = ch[v][i];
                                break;
                            }
                            v = fail[v];
                        }
                        if (!v) fail[ch[u][i]] = 1;
                    }
                    q[rear++] = ch[u][i];
                }
            }
        }
    }
    vector<pair<int, int> > aho_corasick(const char * str) const {
        vector<pair<int, int> > r;
        int len = strlen(str), u = 1;
        for (int i = 0; i < len; i++) {
            int v = get_key(str[i]);
            while (!ch[u][v] && u != 1) u = fail[u];
            if (!ch[u][v]) continue;
            v = u = ch[u][v];
            while (v != 1) {
                if (id[v]) r.push_back(make_pair(id[v], i));
                v = fail[v];
            }
        }
        return r;
    }
};

class trie_01 {
private:
    vector<vector<int> > ch;
    vector<int> sz;
    int cnt;
public:
    explicit trie_01(int n = N)
    : ch(n, vector<int>(2)), sz(n), cnt(1) { }
    void insert(int x) {
        int u = 1;
        for (int i = 30; i >= 0; i--) {
            int v = (x >> i) & 1;
            if (!ch[u][v]) ch[u][v] = ++cnt;
            sz[u]++;
            u = ch[u][v];
        }
        sz[u]++;
    }
    void erase(int x) {
        int u = 1;
        for (int i = 30; i >= 0; i--) {
            int v = (x >> i) & 1;
            if (!ch[u][v]) return;
            sz[u]--;
            u = ch[u][v];
        }
        sz[u]--;
    }
    int find(int x) const {
        int u = 1;
        for (int i = 30; i >= 0; i--) {
            int v = (x >> i) & 1;
            if (!ch[u][v]) return 0;
            u = ch[u][v];
        }
        return sz[u];
    }
};

template <class T>
class binary_indexed_tree {
private:
    int N;
    vector<T> val; /* Used to store data. */
    int lowbit(int x) const { return x & -x; }
public:
    explicit binary_indexed_tree(int n) : N(n + 1), val(N) { }
    /**
     * Gets the sum of the array within the range [1, n].
     */
    T query(int n) const {
        T r = 0;
        while (n > 0) {
            r += val[n];
            n -= lowbit(n);
        }
        return r;
    }
    /**
     * Updates the element indexed i.
     */
    void update(int i, const T & add) {
        while (i < N) {
            val[i] += add;
            i += lowbit(i);
        }
    }
};

template <class T>
class binary_indexed_tree_2 {
private:
    binary_indexed_tree<T> bit0;
    binary_indexed_tree<T> bit1;
    T query_sum(int n) const
    { return bit1.query(n) * n + bit0.query(n); }
public:
    explicit binary_indexed_tree_2(int n)
    : bit0(n), bit1(n) { }
    T query(int l, int r) const
    { return query_sum(r) - query_sum(l - 1); }
    void update(int l, int r, const T & add) {
        bit0.update(l, -add * (l - 1)); bit0.update(r + 1, add * r);
        bit1.update(l, add);            bit1.update(r + 1, -add);
    }
};

/**
 * Segment Tree
 */
template <class T>
class segment_tree {
private:
    int N;
    vector<T> val;  /* Used to store values. */
    vector<T> lazy; /* Used to store values to be updated. */
    void push_down(int root, int istart, int iend) {
        if (lazy[root] != 0) {
            lazy[root << 1] += lazy[root];
            lazy[root << 1 | 1] += lazy[root];
            int mid = (istart + iend) >> 1;
            val[root << 1] += (mid - istart + 1) * lazy[root];
            val[root << 1 | 1] += (iend - mid) * lazy[root];
            lazy[root] = 0;
        }
    }
    /**
     * Builds segment tree.
     * root: the index of the current root.
     * (ATTENTION: root starts from 1, NOT 0)
     * arr:  the original data array.
     * (ATTENTION: the zero-th position should NOT be used)
     * [istart, iend]
     */
    void build(int root, const vector<T> & arr, int istart, int iend) {
        lazy[root] = 0;
        if (istart == iend) val[root] = arr[istart];
        else {
            int mid = (istart + iend) >> 1;
            build(root << 1, arr, istart, mid);
            build(root << 1 | 1, arr, mid + 1, iend);
            val[root] = val[root << 1] + val[root << 1 | 1];
        }
    }
    /**
     * Queries data within range [qstart, qend].
     */
    T query(int root, int istart, int iend, int qstart, int qend) {
        if (qstart > iend || qend < istart) return 0;
        if (qstart <= istart && qend >= iend) return val[root];
        push_down(root, istart, iend);
        int mid = (istart + iend) >> 1;
        return query(root << 1, istart, mid, qstart, qend) + query(root << 1 | 1, mid + 1, iend, qstart, qend);
    }
    /**
     * Updates data within range [ustart, uend].
     */
    void update(int root, int istart, int iend, int ustart, int uend, const T & add) {
        if (ustart > iend || uend < istart) return;
        if (ustart <= istart && uend >= iend) {
            lazy[root] += add;
            val[root] += (iend - istart + 1) * add;
            return;
        }
        push_down(root, istart, iend);
        int mid = (istart + iend) >> 1;
        update(root << 1, istart, mid, ustart, uend, add);
        update(root << 1 | 1, mid + 1, iend, ustart, uend, add);
        val[root] = val[root << 1] + val[root << 1 | 1];
    }
public:
    /**
     * (ATTENTION: the zero-th position should NOT be used)
     */
    segment_tree(const vector<T> & arr)
    : N(arr.size() - 1), val(N << 2), lazy(N << 2)
    { build(1, arr, 1, N); }
    T query(int l, int r) { return query(1, 1, N, l, r); }
    void update(int l, int r, const T & add) { update(1, 1, N, l, r, add); }
};

const int INF = 1000000;

/**
 * Segment Tree
 */
template <class T>
class segment_tree_2 {
private:
    T val[N << 2];  /* Used to store values. */
    T lazy[N << 2]; /* Used to store values to be updated. */
    void push_down(int root) {
        if (lazy[root] != 0) {
            lazy[root << 1] += lazy[root];
            lazy[root << 1 | 1] += lazy[root];
            val[root << 1] += lazy[root];
            val[root << 1 | 1] += lazy[root];
            lazy[root] = 0;
        }
    }
public:
    /**
     * Builds segment tree.
     * root: the index of the current root.
     * (ATTENTION: root starts from 1, NOT 0)
     * arr:  the original data array.
     * (ATTENTION: the zero-th position should NOT be used)
     * [istart, iend]
     */
    void build(int root, T arr[], int istart, int iend) {
        lazy[root] = 0;
        if (istart == iend) val[root] = arr[istart];
        else {
            int mid = (istart + iend) >> 1;
            build(root << 1, arr, istart, mid);
            build(root << 1 | 1, arr, mid + 1, iend);
            val[root] = min(val[root << 1], val[root << 1 | 1]);
        }
    }
    /**
     * Queries data within range [qstart, qend].
     */
    T query(int root, int istart, int iend, int qstart, int qend) {
        if (qstart > iend || qend < istart) return INF;
        if (qstart <= istart && qend >= iend) return val[root];
        push_down(root);
        int mid = (istart + iend) >> 1;
        return min(query(root << 1, istart, mid, qstart, qend), query(root << 1 | 1, mid + 1, iend, qstart, qend));
    }
    /**
     * Updates data within range [ustart, uend].
     */
    void update(int root, int istart, int iend, int ustart, int uend, T add) {
        if (ustart > iend || uend < istart) return;
        if (ustart <= istart && uend >= iend) {
            lazy[root] += add;
            val[root] += add;
            return;
        }
        push_down(root);
        int mid = (istart + iend) >> 1;
        update(root << 1, istart, mid, ustart, uend, add);
        update(root << 1 | 1, mid + 1, iend, ustart, uend, add);
        val[root] = min(val[root << 1], val[root << 1 | 1]);
    }
};

class persistent_segment_tree {
private:
    int n;
    const int M, N, MAXN;
    vector<int> root, left, right, sz;
    void build(int & rt, int l, int r) {
        rt = n++;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(left[rt], l, mid);
        build(right[rt], mid + 1, r);
    }
    void insert(int & rt, int pre, int l, int r, int x) {
        rt = n++;
        left[rt] = left[pre];
        right[rt] = right[pre];
        sz[rt] = sz[pre] + 1;
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (x <= mid) insert(left[rt], left[pre], l, mid, x);
        else insert(right[rt], right[pre], mid + 1, r, x);
    }
    int query(int u, int v, int l, int r, int k) const {
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int x = sz[left[v]] - sz[left[u]];
        if (k <= x) return query(left[u], left[v], l, mid, k);
        else return query(right[u], right[v], mid + 1, r, k - x);
    }
public:
    /**
     * ATTENTION:
     * For any 1 <= i <= n,
     * arr[i] should be between [1, max_val],
     * where n is the length of arr.
     * The zero-th position should NOT be used!
     */
    persistent_segment_tree(const vector<int> & arr, int max_val)
    : n(0), M(arr.size()), N(max_val),
      MAXN((N << 2) + M * ((int)log2(N) + 5)),
      root(M), left(MAXN), right(MAXN), sz(MAXN) {
        build(root[0], 1, N);
        for (int i = 1; i < M; i++)
            insert(root[i], root[i - 1], 1, N, arr[i]);
    }
    int query(int l, int r, int k) const {
        return query(root[l - 1], root[r], 1, N, k);
    }
};

class persistent_segment_tree_2 {
private:
    int n;
    const int M, N, MAXN;
    vector<int> root, tree, left, right, sz, val, u, v;
    inline int low_bit(int x) const { return x & -x; }
    int sum(const vector<int> & v, int x) const {
        int r = 0;
        for ( ; x; x -= low_bit(x))
            r += sz[left[v[x]]];
        return r;
    }
    void build(int & rt, int l, int r) {
        rt = n++;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(left[rt], l, mid);
        build(right[rt], mid + 1, r);
    }
    void insert(int & rt, int pre, int l, int r, int x, int y) {
        rt = n++;
        left[rt] = left[pre];
        right[rt] = right[pre];
        sz[rt] = sz[pre] + y;
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (x <= mid) insert(left[rt], left[pre], l, mid, x, y);
        else insert(right[rt], right[pre], mid + 1, r, x, y);
    }
    int query(int low, int high, int rt1, int rt2, int l, int r, int k) {
        if (l == r) return l;
        int mid = (l + r) >> 1;
        int x = sz[left[rt2]] - sz[left[rt1]] + sum(v, high) - sum(u, low - 1);
        if (k <= x) {
            for (int i = low - 1; i; i -= low_bit(i)) u[i] = left[u[i]];
            for (int i = high; i; i -= low_bit(i)) v[i] = left[v[i]];
            return query(low, high, left[rt1], left[rt2], l, mid, k);
        } else {
            for (int i = low - 1; i; i -= low_bit(i)) u[i] = right[u[i]];
            for (int i = high; i; i -= low_bit(i)) v[i] = right[v[i]];
            return query(low, high, right[rt1], right[rt2], mid + 1, r, k - x);
        }
    }
public:
    persistent_segment_tree_2(const vector<int> & arr, int max_val, int num_update)
    : n(0), M(arr.size()), N(max_val),
      MAXN((N << 2) + (M + (num_update << 1) * (int)log2(M)) * ((int)log2(N) + 5)),
      root(M), tree(M), left(MAXN), right(MAXN), sz(MAXN), val(M), u(M), v(M) {
        build(root[0], 1, N);
        for (int i = 1; i < M; i++) {
            tree[i] = root[0];
            insert(root[i], root[i - 1], 1, N, val[i] = arr[i], 1);
        }
    }
    void update(int x, int y) {
        for (int i = x; i < M; i += low_bit(i)) {
            insert(tree[i], tree[i], 1, N, val[x], -1);
            insert(tree[i], tree[i], 1, N, y, 1);
        }
        val[x] = y;
    }
    int query(int l, int r, int k) {
        for (int i = l - 1; i; i -= low_bit(i)) u[i] = tree[i];
        for (int i = r; i; i -= low_bit(i)) v[i] = tree[i];
        return query(l, r, root[l - 1], root[r], 1, N, k);
    }
};

namespace TREAP {
class treap {
    int ls[N], rs[N], sz[N], pri[N], val[N], cnt[N], rt, n, s[N], top;
    int new_node() {
        if (n < N) return n++;
        return s[--top];
    }
    void free_node(int x) {
        s[top++] = x;
    }
    void rotate_left(int & o) {
        int k = ls[o];
        ls[o] = rs[k];
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
        rs[k] = o;
        o = k;
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
    }
    void rotate_right(int & o) {
        int k = rs[o];
        rs[o] = ls[k];
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
        ls[k] = o;
        o = k;
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
    }
    void _insert(int & o, int x) {
        if (!o) {
            o = new_node();
            ls[o] = rs[o] = 0;
            sz[o] = 1;
            pri[o] = rand();
            val[o] = x;
            cnt[o] = 1;
            return;
        }
        if (x < val[o]) _insert(ls[o], x);
        else if (val[o] < x) _insert(rs[o], x);
        else cnt[o]++;
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
        if (pri[ls[o]] < pri[o]) rotate_left(o);
        if (pri[rs[o]] < pri[o]) rotate_right(o);
    }
    void _erase(int & o, int x) {
        if (!o) return;
        if (x < val[o]) _erase(ls[o], x);
        else if (val[o] < x) _erase(rs[o], x);
        else if (cnt[o] > 1) cnt[o]--;
        else if (ls[o] && rs[o]) {
            int ptr = ls[o];
            while (rs[ptr]) ptr = rs[ptr];
            val[o] = val[ptr];
            cnt[o] = cnt[ptr];
            cnt[ptr] = 1;
            _erase(ls[o], val[o]);
        } else {
            int t = o;
            o = ls[o] ? ls[o] : rs[o];
            free_node(t);
        }
        sz[o] = sz[ls[o]] + cnt[o] + sz[rs[o]];
    }
    int _get_kth(int o, int k) {
        if (!o) return -1;
        if (k <= sz[ls[o]]) return _get_kth(ls[o], k);
        else if (k <= sz[ls[o]] + cnt[o]) return val[o];
        else return _get_kth(rs[o], k - sz[ls[o]] - cnt[o]);
    }
    void walk_tree(int o) const {
        if (!o) return;
        walk_tree(ls[o]);
        for (int i = 0; i < cnt[o]; i++)
            cout << val[o] << ' ';
        walk_tree(rs[o]);
    }
public:
    void init() {
        pri[0] = INT_MAX;
        top = 0;
        n = 1;
        rt = 0;
    }
    void insert(int x) { _insert(rt, x); }
    void erase(int x) { _erase(rt, x); }
    int get_kth(int k) { return _get_kth(rt, k); }
    void walk_tree() const { walk_tree(rt); cout << endl; }
};
}

namespace heavy_light_decomposition {

struct edge_link { int to, next; } e[N << 4];
int depth[N], head[N], cnt = 0;
void add(int u, int v)
{
    e[cnt] = (edge_link){v, head[u]};
    head[u] = cnt++;
}
void operate(int u, int v) { }

// dfs1(1, 0, 0); dfs2(1, 1);
// Build linear data structure with: arr[rk[i]]
int fa[N], son[N], sz[N];
void dfs1(int u, int p, int d) {
    depth[u] = d; fa[u] = p; sz[u] = 1;
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to; if (v == p) continue;
        dfs1(v, u, d + 1); sz[u] += sz[v];
        if (son[u] == -1 || sz[v] > sz[son[u]])
            son[u] = v; }
}
int top[N], id[N], rk[N], pos = 1;
void dfs2(int u, int t) {
    top[u] = t; id[u] = pos; rk[pos++] = u;
    if (son[u] == -1) return; dfs2(son[u], t);
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v != son[u] && v != fa[u])
            dfs2(v, v); }
}
void operate_path(int u, int v) {
    int fu = top[u], fv = top[v];
    while (fu != fv) {
        if (depth[fu] >= depth[fv]) {
            operate(id[fu], id[u]); u = fa[fu];
        } else { operate(id[fv], id[v]); v = fa[fv]; }
        fu = top[u]; fv = top[v];
    } if (u != v) {
        if (id[u] < id[v]) operate(id[u], id[v]);
        else operate(id[v], id[u]);
    } else operate(id[u], id[v]);
}

} // heavy_light_decomposition

namespace centroid_decomposition_of_tree {

struct edge { int to, next; } e[N << 1];
int head[N], cnt, sz[N], msz[N], tree_size, centroid;
bool is_centroid[N];
void add(int u, int v) {
    e[cnt] = (edge){v, head[u]};
    head[u] = cnt++;
}
void add_edge(int u, int v) {
    add(u, v);
    add(v, u);
}
void get_centroid(int u, int p) {
    sz[u] = 1; msz[u] = 0;
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v == p || is_centroid[v]) continue;
        get_centroid(v, u);
        sz[u] += sz[v];
        msz[u] = max(msz[u], sz[v]);
    }
    msz[u] = max(msz[u], tree_size - sz[u]);
    if (centroid == -1 || msz[u] < msz[centroid])
        centroid = u;
}
// tree_size = n;
// get_centroid(1, centroid = -1);
// solve(centroid);
void solve(int u) {
    get_centroid(u, centroid = -1);
    is_centroid[u] = true;
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (is_centroid[v]) continue;
        tree_size = sz[v];
        get_centroid(v, centroid = -1);
        solve(centroid);
    }
}

} // centroid_decomposition_of_tree

namespace dsu_on_tree {

typedef long long ll;

struct edge {int to, next;} e[N << 1];
int head[N], cnt;
void add_edge(int u, int v) {
    e[cnt] = {v, head[u]};
    head[u] = cnt++;
}

int sz[N], son[N];
void dfs1(int u, int p) {
    sz[u] = 1;
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to; if (v == p) continue;
        dfs1(v, u); sz[u] += sz[v];
        if (son[u] == -1 || sz[v] > sz[son[u]])
            son[u] = v; }
}
bool big[N];
int num[N], col[N];
ll ans[N], sum, mx;
void add(int u, int p, int x) {
    num[col[u]] += x;
    if (x > 0) {
        if (num[col[u]] > mx) {
            mx = num[col[u]];
            sum = col[u];
        } else if (num[col[u]] == mx)
            sum += col[u];
    }
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v != p && !big[v])
            add(v, u, x); }
}
// dfs1(1, -1);
// dfs2(1, -1, false);
void dfs2(int u, int p, bool keep) {
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v != p && v != son[u])
            dfs2(v, u, false); }
    if (~son[u]) {
        dfs2(son[u], u, true);
        big[son[u]] = true; }
    add(u, p, 1);
    ans[u] = sum;
    if (~son[u]) big[son[u]] = false;
    if (!keep) {
        add(u, p, -1);
        sum = mx = 0; }
}

} // dsu_on_tree

namespace auxiliary_tree {

int l[N], r[N];

namespace original_tree {

struct edge {int to, next;} e[N << 1];
const int C = (int)log2(N) + 5;
int head[N], cnt, dep[N], anc[N][C + 1];
void add(int u, int v) {
    e[cnt] = {v, head[u]};
    head[u] = cnt++;
}
void add_edge(int u, int v) {
    add(u, v);
    add(v, u);
}
// dfs(0, root, root, k);
void dfs(int d, int u, int p, int & k) {
    l[u] = k++;
    dep[u] = d; anc[u][0] = p;
    for (int i = 1; i <= C; i++)
        anc[u][i] = anc[anc[u][i - 1]][i - 1];
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v == p) continue;
        dfs(d + 1, v, u, k);
    }
    r[u] = k++;
}
int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = C; i >= 0; i--)
        if (dep[v] <= dep[anc[u][i]])
            u = anc[u][i];
    if (u == v) return u;
    for (int i = C; i >= 0; i--) {
        if (anc[u][i] != anc[v][i]) {
            u = anc[u][i];
            v = anc[v][i];
        }
    }
    return anc[u][0] == anc[v][0] ? anc[u][0] : -1;
}

}
namespace auxiliary_tree {

struct edge {int to, next;} e[N];
int head[N], cnt, a[N << 1], n, b[N], s[N], top;
void add(int u, int v) {
    e[cnt] = {v, head[u]};
    head[u] = cnt++;
}
void build() {
    auto cmp = [] (int i, int j) { return l[i] < l[j]; };
    sort(a, a + n, cmp);
    for (int k = n, i = 0; i < k - 1; i++)
        a[n++] = original_tree::lca(a[i], a[i + 1]);
    sort(a, a + n, cmp);
    n = unique(a, a + n) - a;
    top = 0;
    s[top++] = a[0];
    for (int i = 1; i < n; i++) {
        while (r[s[top - 1]] < l[a[i]]) top--;
        add(s[top - 1], a[i]);
        s[top++] = a[i];
    }
}
void dfs(int u) {
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to; dfs(v);
    }
}
void solve() {
    // Input: n, a
    for (int i = 0; i < n; i++) b[a[i]] = 1;
    build(); dfs(a[0]); // tree DP dfs
    // Output: e.g. dp[a[0]]
    for (int i = 0; i < n; i++) {
        head[a[i]] = -1;
        b[a[i]] = 0;
    }
    cnt = 0;
}

}

} // auxiliary_tree

#endif // _TREE_H
