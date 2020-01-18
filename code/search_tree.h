#ifndef _SEARCH_TREE_H
#define _SEARCH_TREE_H

#include "_tool.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
using namespace std;

template <typename K, typename V>
class search_tree;
template <typename K, typename V>
class entry {
private:
    K _key;
    V _value;
    void set_key(const K & k) { _key = k; }
public:
    entry(const K & k = K(), const V & v = V())
         : _key(k), _value(v) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class search_tree<K, V>;
};
template <typename K, typename V>
class search_tree {
private:
    struct node {
        entry<K, V> ent;
        node * par;
        node * left;
        node * right;
        node() : ent(), par(NULL), left(NULL), right(NULL) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        entry<K, V> & operator*() { return p->ent; }
        entry<K, V> & operator*() const { return p->ent; }
        entry<K, V> * operator->() { return &p->ent; }
        entry<K, V> * operator->() const { return &p->ent; }
        iterator left() const { return iterator(p->left); }
        iterator right() const { return iterator(p->right); }
        iterator parent() const { return iterator(p->par); }
        bool is_root() const { return p->par == NULL; }
        bool is_external() const
        { return p->left == NULL && p->right == NULL; }
        bool is_internal() const { return !is_external(); }
        bool operator==(const iterator & _p) const
        { return p == _p.p; }
        bool operator!=(const iterator & _p) const
        { return p != _p.p; }
        iterator & operator++() {
            node * w = p->right;
            if (w->left != NULL || w->right != NULL) {
                do { p = w; w = w->left; }
                while (w->left != NULL || w->right != NULL);
            } else {
                w = p->par;
                while (p == w->right)
                { p = w; w = w->par; }
                p = w;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator i = *this;
            ++*this;
            return i;
        }
        friend class search_tree<K, V>;
    };
public:
    search_tree() : _root(NULL), n(0)
    { add_root(); expand_external(_root); }
    search_tree(const search_tree<K, V> & st)
               : _root(clone(st._root, NULL)), n(st.n) { }
    ~search_tree() { clear(); make_empty(_root); }
    search_tree<K, V> & operator=(const search_tree<K, V> & st) {
        clear();
        make_empty(_root);
        _root = clone(st._root, NULL);
        n = st.n;
        return *this;
    }
    void clear() {
        /*n = 0;
        make_empty(_root->left->left);
        make_empty(_root->left->right);*/
        while (!empty())
            eraser(root());
    }
    iterator find(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) return v;
        else return end();
    }
    V & operator[](const K & k) {
        iterator v = finder(k, root());
        if (v.is_external()) {
            expand_external(v);
            v->set_key(k);
            n++;
        }
        return v->_value;
    }
    iterator insert(const K & k, const V & x)
    { return inserter(k, x); }
    void erase(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) erase(v);
    }
    void erase(const iterator & p)
    { eraser(p); }
    iterator begin() {
        iterator v = root();
        while (v.is_internal()) v = v.left();
        return v.parent();
    }
    iterator end() { return _root; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    iterator root() const { return iterator(_root->left); }
private:
    void add_root() { _root = new node; }
    void expand_external(const iterator & p) {
        node * v = p.p;
        v->left = new node;
        v->left->par = v;
        v->right = new node;
        v->right->par = v;
    }
    iterator remove_above_external(const iterator & p) {
        node * w = p.p; node * v = w->par;
        node * sib = (w == v->left ? v->right : v->left);
        if (v == _root) {
            _root = sib;
            sib->par = NULL;
        } else {
            node * gpar = v->par;
            if (v == gpar->left) gpar->left = sib;
            else gpar->right = sib;
            sib->par = gpar;
        }
        delete w; delete v;
        return iterator(sib);
    }
    iterator finder(const K & k, const iterator & v) {
        //if (v.is_external()) return v;
        iterator u = v;
        while (u.is_internal())
            if (k < u->key())
                u = u.left();
            else if (u->key() < k)
                u = u.right();
            else break;
        return u;
        /*if (k < v->key()) return finder(k, v.left());
        else if (v->key() < k) return finder(k, v.right());
        else return v;*/
    }
    iterator inserter(const K & k, const V & x) {
        iterator v = finder(k, root());
        if (v.is_internal()) return end();
        expand_external(v);
        v->set_key(k); v->set_value(x);
        n++;
        return v;
    }
    iterator eraser(const iterator & v) {
        iterator w;
        if (v.left().is_external()) w = v.left();
        else if (v.right().is_external()) w = v.right();
        else {
            w = v.right();
            do { w = w.left(); } while (w.is_internal());
            iterator u = w.parent();
            v->set_key(u->key()); v->set_value(u->value());
        }
        n--;
        return remove_above_external(w);
    }
    void make_empty(node * & p) {
        if (p != NULL) {
            make_empty(p->left);
            make_empty(p->right);
            delete p;
        }
        p = NULL;
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        node * r = new node;
        r->ent = t->ent;
        r->par = p;
        r->left = clone(t->left, r);
        r->right = clone(t->right, r);
        return r;
    }
private:
    node * _root;
    int n;
};

template <typename K, typename V>
class splay_tree;
template <typename K, typename V>
class splay_entry {
private:
    K _key;
    V _value;
    void set_key(const K & k) { _key = k; }
public:
    splay_entry(const K & k = K(), const V & v = V())
         : _key(k), _value(v) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class splay_tree<K, V>;
};
template <typename K, typename V>
class splay_tree {
private:
    struct node {
        splay_entry<K, V> ent;
        node * left;
        node * right;
        node * par;
        node(const K & k = K(), const V & v = V(), node * lt = NULL, node * rt = NULL, node * p = NULL)
            : ent(k, v), left(lt), right(rt), par(p) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        splay_entry<K, V> & operator*() { return p->ent; }
        splay_entry<K, V> & operator*() const { return p->ent; }
        splay_entry<K, V> * operator->() { return &p->ent; }
        splay_entry<K, V> * operator->() const { return &p->ent; }
        iterator left() const { return iterator(p->left); }
        iterator right() const { return iterator(p->right); }
        iterator parent() const { return iterator(p->par); }
        bool is_root() const { return p->par == NULL; }
        bool is_external() const
        { return p->left == NULL && p->right == NULL; }
        bool is_internal() const { return !is_external(); }
        bool operator==(const iterator & _p) const
        { return p == _p.p; }
        bool operator!=(const iterator & _p) const
        { return p != _p.p; }
        iterator & operator++() {
            node * w = p->right;
            if (w->left != NULL || w->right != NULL) {
                do { p = w; w = w->left; }
                while (w->left != NULL || w->right != NULL);
            } else {
                w = p->par;
                while (p == w->right)
                { p = w; w = w->par; }
                p = w;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator i = *this;
            ++*this;
            return i;
        }
        friend class splay_tree<K, V>;
    };
public:
    splay_tree() : _root(NULL), n(0) {
        add_root();
        expand_external(_root);
    }
    splay_tree(const splay_tree<K, V> & rhs)
    : _root(clone(rhs._root, NULL)), n(rhs.n) { }
    ~splay_tree() { clear(); make_empty(_root); }
    splay_tree<K, V> & operator=(const splay_tree<K, V> & rhs) {
        splay_tree<K, V> copy = rhs;
        swap(*this, copy);
        return *this;
    }
    V & operator[](const K & k) {
        iterator v = finder(k, root());
        if (v.is_external()) {
            expand_external(v);
            v->set_key(k);
            splay(v, iterator(_root));
            n++;
        }
        return v->_value;
    }
    iterator find(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) {
            splay(v, iterator(_root));
            return v;
        } else return end();
    }
    iterator insert(const K & k, const V & v) {
        iterator u = inserter(k, v);
        splay(u, iterator(_root));
        return u;
    }
    void erase(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) erase(v);
    }
    void erase(const iterator & u) {
        iterator v = eraser(u);
        splay(v.parent(), iterator(_root));
    }
    void clear() {
        while (!empty()) {
            find_max();
            erase(root());
        }
    }
    iterator begin() {
        iterator v = root();
        while (v.is_internal()) v = v.left();
        return v.parent();
    }
    iterator end() { return _root; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    iterator root() const { return iterator(_root->left); }
private:
    const K & find_max() {
        iterator ptr = root();
        while (ptr.is_internal())
            ptr = ptr.right();
        splay(ptr.parent(), iterator(_root));
        return ptr.parent()->key();
    }
    void rotate_with_left_child(const iterator & u) {
        node * k2 = u.p;
        node * k1 = k2->left;
        node * p = k2->par;
        if (p->left == k2) p->left = k1;
        else p->right = k1;
        k1->par = p;
        k2->left = k1->right;
        k2->left->par = k2;
        k1->right = k2;
        k2->par = k1;
    }
    void rotate_with_right_child(const iterator & u) {
        node * k1 = u.p;
        node * k2 = k1->right;
        node * p = k1->par;
        if (p->left == k1) p->left = k2;
        else p->right = k2;
        k2->par = p;
        k1->right = k2->left;
        k1->right->par = k1;
        k2->left = k1;
        k1->par = k2;
    }
    void splay(const iterator & u, const iterator & r) {
        if (u == r) return;
        while (u != r.left()) {
            if (u.parent() == r.left())
                if (r.left().left() == u)
                    rotate_with_left_child(r.left());
                else rotate_with_right_child(r.left());
            else {
                iterator p = u.parent();
                iterator g = p.parent();
                if (u == p.left() && p == g.left()) {
                    rotate_with_left_child(g);
                    rotate_with_left_child(p);
                } else if (u == p.right() && p == g.right()) {
                    rotate_with_right_child(g);
                    rotate_with_right_child(p);
                } else if (u == p.left() && p == g.right()) {
                    rotate_with_left_child(p);
                    rotate_with_right_child(g);
                } else {
                    rotate_with_right_child(p);
                    rotate_with_left_child(g);
                }
            }
        }
    }
private:
    void add_root() { _root = new node; }
    void expand_external(const iterator & p) {
        node * v = p.p;
        v->left = new node;
        v->left->par = v;
        v->right = new node;
        v->right->par = v;
    }
    iterator remove_above_external(const iterator & p) {
        node * w = p.p; node * v = w->par;
        node * sib = (w == v->left ? v->right : v->left);
        if (v == _root) {
            _root = sib;
            sib->par = NULL;
        } else {
            node * gpar = v->par;
            if (v == gpar->left) gpar->left = sib;
            else gpar->right = sib;
            sib->par = gpar;
        }
        delete w; delete v;
        return iterator(sib);
    }
    iterator finder(const K & k, const iterator & v) {
        iterator u = v;
        while (u.is_internal())
            if (k < u->key())
                u = u.left();
            else if (u->key() < k)
                u = u.right();
            else break;
        return u;
    }
    iterator inserter(const K & k, const V & x) {
        iterator v = finder(k, root());
        if (v.is_internal()) return end();
        expand_external(v);
        v->set_key(k); v->set_value(x);
        n++;
        return v;
    }
    iterator eraser(const iterator & v) {
        iterator w;
        if (v.left().is_external()) w = v.left();
        else if (v.right().is_external()) w = v.right();
        else {
            w = v.right();
            do { w = w.left(); } while (w.is_internal());
            iterator u = w.parent();
            v->set_key(u->key()); v->set_value(u->value());
        }
        n--;
        return remove_above_external(w);
    }
    void make_empty(node * & p) {
        if (p != NULL) {
            make_empty(p->left);
            make_empty(p->right);
            delete p;
        }
        p = NULL;
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        node * r = new node;
        r->ent = t->ent;
        r->par = p;
        r->left = clone(t->left, r);
        r->right = clone(t->right, r);
        return r;
    }
private:
    node * _root;
    int n;
};

template <typename K, typename V>
class treap;
template <typename K, typename V>
class treap_entry {
private:
    K _key;
    V _value;
    int _priority;
    void set_key(const K & k) { _key = k; }
    int priority() const { return _priority; }
    void set_priority(int pr) { _priority = pr; }
public:
    treap_entry(const K & k = K(), const V & v = V(), int pr = INT_MAX)
         : _key(k), _value(v), _priority(pr) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class treap<K, V>;
};
template <typename K, typename V>
class treap {
private:
    struct node {
        treap_entry<K, V> ent;
        node * left;
        node * right;
        node * par;
        node(const K & k = K(), const V & v = V(), node * lt = NULL, node * rt = NULL, node * p = NULL, int pr = INT_MAX)
            : ent(k, v, pr), left(lt), right(rt), par(p) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        treap_entry<K, V> & operator*() { return p->ent; }
        treap_entry<K, V> & operator*() const { return p->ent; }
        treap_entry<K, V> * operator->() { return &p->ent; }
        treap_entry<K, V> * operator->() const { return &p->ent; }
        iterator left() const { return iterator(p->left); }
        iterator right() const { return iterator(p->right); }
        iterator parent() const { return iterator(p->par); }
        bool is_root() const { return p->par == NULL; }
        bool is_external() const
        { return p->left == NULL && p->right == NULL; }
        bool is_internal() const { return !is_external(); }
        bool operator==(const iterator & _p) const
        { return p == _p.p; }
        bool operator!=(const iterator & _p) const
        { return p != _p.p; }
        iterator & operator++() {
            node * w = p->right;
            if (w->left != NULL || w->right != NULL) {
                do { p = w; w = w->left; }
                while (w->left != NULL || w->right != NULL);
            } else {
                w = p->par;
                while (p == w->right)
                { p = w; w = w->par; }
                p = w;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator i = *this;
            ++*this;
            return i;
        }
        friend class treap<K, V>;
    };
public:
    treap() : _root(NULL), n(0) {
        srand(time(NULL));
        add_root();
        expand_external(_root);
    }
    treap(const treap<K, V> & rhs)
         : _root(clone(rhs._root, NULL)), n(rhs.n) {
        srand(time(NULL));
    }
    ~treap() { make_empty(_root); }
    treap<K, V> & operator=(const treap<K, V> & rhs) {
        make_empty(_root);
        _root = clone(rhs._root, NULL);
        n = rhs.n;
        return *this;
    }
    V & operator[](const K & k)
    { return find(k, root()); }
    iterator find(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) return v;
        else return end();
    }
    iterator insert(const K & k, const V & v)
    { return inserter(k, v, root()); }
    void erase(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) erase(v);
    }
    void erase(const iterator & p)
    { eraser(p); }
    void clear() {
        n = 0;
        make_empty(_root->left->left);
        make_empty(_root->left->right);
    }
    /*treap & operator=(const treap & rhs) {
        treap copy = rhs;
        swap(*this, copy);
        return *this;
    }*/
    iterator begin() {
        iterator v = root();
        while (v.is_internal()) v = v.left();
        return v.parent();
    }
    iterator end() { return _root; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    iterator root() const { return iterator(_root->left); }
private:
    iterator finder(const K & k, const iterator & v) {
        if (v.is_external()) return v;
        if (k < v->key()) return finder(k, v.left());
        else if (v->key() < k) return finder(k, v.right());
        else return v;
    }
    V & find(const K & k, const iterator & u) {
        if (u.is_external()) {
            expand_external(u);
            u->set_key(k);
            u->set_priority(rand());
            n++;
            return u->_value;
        } else if (k < u->key()) {
            V & r = find(k, u.left());
            if (u.left()->priority() < u->priority())
                rotate_with_left_child(u);
            return r;
        } else if (u->key() < k) {
            V & r = find(k, u.right());
            if (u.right()->priority() < u->priority())
                rotate_with_right_child(u);
            return r;
        } else return u->_value;
    }
    iterator inserter(const K & k, const V & v, const iterator & u) {
        if (u.is_external()) {
            expand_external(u);
            u->set_key(k);
            u->set_value(v);
            u->set_priority(rand());
            n++;
            return u;
        } else if (k < u->key()) {
            iterator r = inserter(k, v, u.left());
            if (u.left()->priority() < u->priority())
                rotate_with_left_child(u);
            return r;
        } else if (u->key() < k) {
            iterator r = inserter(k, v, u.right());
            if (u.right()->priority() < u->priority())
                rotate_with_right_child(u);
            return r;
        } else return end();
    }
    iterator eraser(const iterator & v) {
        iterator w;
        if (v.left().is_external()) w = v.left();
        else if (v.right().is_external()) w = v.right();
        else {
            w = v.right();
            do { w = w.left(); } while (w.is_internal());
            iterator u = w.parent();
            v->set_key(u->key()); v->set_value(u->value());
        }
        n--;
        return remove_above_external(w);
    }
    void rotate_with_left_child(const iterator & u) {
        node * k2 = u.p;
        node * k1 = k2->left;
        node * p = k2->par;
        if (p->left == k2) p->left = k1;
        else p->right = k1;
        k1->par = p;
        k2->left = k1->right;
        k2->left->par = k2;
        k1->right = k2;
        k2->par = k1;
    }
    void rotate_with_right_child(const iterator & u) {
        node * k1 = u.p;
        node * k2 = k1->right;
        node * p = k1->par;
        if (p->left == k1) p->left = k2;
        else p->right = k2;
        k2->par = p;
        k1->right = k2->left;
        k1->right->par = k1;
        k2->left = k1;
        k1->par = k2;
    }
private:
    void add_root() { _root = new node; }
    void expand_external(const iterator & p) {
        node * v = p.p;
        v->left = new node;
        v->left->par = v;
        v->right = new node;
        v->right->par = v;
    }
    iterator remove_above_external(const iterator & p) {
        node * w = p.p; node * v = w->par;
        node * sib = (w == v->left ? v->right : v->left);
        if (v == _root) {
            _root = sib;
            sib->par = NULL;
        } else {
            node * gpar = v->par;
            if (v == gpar->left) gpar->left = sib;
            else gpar->right = sib;
            sib->par = gpar;
        }
        delete w; delete v;
        return iterator(sib);
    }
    void make_empty(node * & p) {
        if (p != NULL) {
            make_empty(p->left);
            make_empty(p->right);
            delete p;
        }
        p = NULL;
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        node * r = new node;
        r->ent = t->ent;
        r->par = p;
        r->left = clone(t->left, r);
        r->right = clone(t->right, r);
        return r;
    }
private:
    node * _root;
    int n;
};

/**
 * AVL Tree
 * Any binary search tree T that satisfies the Height-Balance Property
 * is said to be an AVL tree, named after the initials of its inventors,
 * Adel'son-Vel'skii and Landis.
 * Height-Balance Property: For every internal node v of T, the heights
 * of the children of v differ by at most 1.
 */
template <typename K, typename V>
class avl_tree;
template <typename K, typename V>
class avl_entry {
private:
    K _key;
    V _value;
    int ht;
    void set_key(const K & k) { _key = k; }
    int height() const { return ht; }
    void set_height(int h) { ht = h; }
public:
    avl_entry(const K & k = K(), const V & v = V())
            : _key(k), _value(v), ht(0) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class avl_tree<K, V>;
};
template <typename K, typename V>
class avl_tree {
private:
    struct node {
        avl_entry<K, V> ent;
        node * par;
        node * left;
        node * right;
        node() : ent(), par(NULL), left(NULL), right(NULL) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        avl_entry<K, V> & operator*() { return p->ent; }
        avl_entry<K, V> & operator*() const { return p->ent; }
        avl_entry<K, V> * operator->() { return &p->ent; }
        avl_entry<K, V> * operator->() const { return &p->ent; }
        iterator left() const { return iterator(p->left); }
        iterator right() const { return iterator(p->right); }
        iterator parent() const { return iterator(p->par); }
        bool is_root() const { return p->par == NULL; }
        bool is_external() const
        { return p->left == NULL && p->right == NULL; }
        bool is_internal() const { return !is_external(); }
        bool operator==(const iterator & _p) const
        { return p == _p.p; }
        bool operator!=(const iterator & _p) const
        { return p != _p.p; }
        iterator & operator++() {
            node * w = p->right;
            if (w->left != NULL || w->right != NULL) {
                do { p = w; w = w->left; }
                while (w->left != NULL || w->right != NULL);
            } else {
                w = p->par;
                while (p == w->right)
                { p = w; w = w->par; }
                p = w;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator i = *this;
            ++*this;
            return i;
        }
        friend class avl_tree<K, V>;
    };
public:
    avl_tree() : _root(NULL), n(0)
    { add_root(); expand_external(_root); }
    avl_tree(const avl_tree<K, V> & st)
            : _root(clone(st._root, NULL)), n(st.n) { }
    ~avl_tree() { make_empty(_root); }
    avl_tree<K, V> & operator=(const avl_tree<K, V> & st) {
        make_empty(_root);
        _root = clone(st._root, NULL);
        n = st.n;
        return *this;
    }
    void clear() {
        n = 0;
        make_empty(_root->left->left);
        make_empty(_root->left->right);
    }
    iterator find(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) return v;
        else return end();
    }
    V & operator[](const K & k) {
        iterator v = finder(k, root());
        if (v.is_external()) {
            expand_external(v);
            v->set_key(k);
            n++;
            set_height(v);
            rebalance(v);
        }
        return v->_value;
    }
    iterator insert(const K & k, const V & x) {
        iterator v = inserter(k, x);
        if (v != end()) {
            set_height(v);
            rebalance(v);
        }
        return v;
    }
    void erase(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal())
            erase(v);
    }
    void erase(const iterator & v) {
        iterator w = eraser(v);
        rebalance(w);
    }
    iterator begin() {
        iterator v = root();
        while (v.is_internal()) v = v.left();
        return v.parent();
    }
    iterator end() { return _root; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    iterator root() const { return iterator(_root->left); }
private:
    void add_root() { _root = new node; }
    void expand_external(const iterator & p) {
        node * v = p.p;
        v->left = new node;
        v->left->par = v;
        v->right = new node;
        v->right->par = v;
    }
    iterator remove_above_external(const iterator & p) {
        node * w = p.p; node * v = w->par;
        node * sib = (w == v->left ? v->right : v->left);
        if (v == _root) {
            _root = sib;
            sib->par = NULL;
        } else {
            node * gpar = v->par;
            if (v == gpar->left) gpar->left = sib;
            else gpar->right = sib;
            sib->par = gpar;
        }
        delete w; delete v;
        return iterator(sib);
    }
    iterator finder(const K & k, const iterator & v) {
        if (v.is_external()) return v;
        if (k < v->key()) return finder(k, v.left());
        else if (v->key() < k) return finder(k, v.right());
        else return v;
    }
    iterator inserter(const K & k, const V & x) {
        iterator v = finder(k, root());
        if (v.is_internal()) return end();
        expand_external(v);
        v->set_key(k); v->set_value(x);
        n++;
        return v;
    }
    iterator eraser(const iterator & v) {
        iterator w;
        if (v.left().is_external()) w = v.left();
        else if (v.right().is_external()) w = v.right();
        else {
            w = v.right();
            do { w = w.left(); } while (w.is_internal());
            iterator u = w.parent();
            v->set_key(u->key()); v->set_value(u->value());
        }
        n--;
        return remove_above_external(w);
    }
private:
    int height(const iterator & v) const
    { return (v.is_external() ? 0 : v->height()); }
    void set_height(const iterator & v) {
        int hl = height(v.left());
        int hr = height(v.right());
        v->set_height(1 + max(hl, hr));
    }
    bool is_balanced(const iterator & v) const {
        int bal = height(v.left()) - height(v.right());
        return ((-1 <= bal) && (bal <= 1));
    }
    iterator tall_grandchild(const iterator & z) const {
        iterator zl = z.left();
        iterator zr = z.right();
        if (height(zl) >= height(zr))
            if (height(zl.left()) >= height(zl.right()))
                return zl.left();
            else return zl.right();
        else
            if (height(zr.right()) >= height(zr.left()))
                return zr.right();
            else return zr.left();
    }
    iterator restructure(const iterator & p) {
        node * x = p.p, * y = x->par, * z = y->par;
        node * a, * b, * c, * T0, * T1, * T2, * T3;
        if (x == y->left && y == z->left) {
            a = x;
            b = y;
            c = z;
            T0 = a->left;
            T1 = a->right;
            T2 = b->right;
            T3 = c->right;
        }
        else if (x == y->left && y == z->right) {
            a = z;
            b = x;
            c = y;
            T0 = a->left;
            T1 = b->left;
            T2 = b->right;
            T3 = c->right;
        }
        else if (x == y->right && y == z->left) {
            a = y;
            b = x;
            c = z;
            T0 = a->left;
            T1 = b->left;
            T2 = b->right;
            T3 = c->right;
        }
        else {
            a = z;
            b = y;
            c = x;
            T0 = a->left;
            T1 = b->left;
            T2 = c->left;
            T3 = c->right;
        }
        if (z->par->left == z) z->par->left = b;
        else z->par->right = b;
        b->par = z->par;
        b->left = a;
        a->par = b;
        a->left = T0;
        T0->par = a;
        a->right = T1;
        T1->par = a;
        b->right = c;
        c->par = b;
        c->left = T2;
        T2->par = c;
        c->right = T3;
        T3->par = c;
        return iterator(b);
    }
    void rebalance(const iterator & v) {
        iterator z = v;
        while (z != root()) {
            z = z.parent();
            set_height(z);
            if (!is_balanced(z)) {
                iterator x = tall_grandchild(z);
                z = restructure(x);
                set_height(z.left());
                set_height(z.right());
                set_height(z);
            }
        }
    }
    void make_empty(node * & p) {
        if (p != NULL) {
            make_empty(p->left);
            make_empty(p->right);
            delete p;
        }
        p = NULL;
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        node * r = new node;
        r->ent = t->ent;
        r->par = p;
        r->left = clone(t->left, r);
        r->right = clone(t->right, r);
        return r;
    }
private:
    node * _root;
    int n;
};


/**
 * Red-Black Tree
 * A Red-Black Tree is a binary search tree with nodes colored
 * red and black in a way that satisfies the following properties:
 * * Root Property: The root is black.
 * * External Property: Every external node is black.
 * * Internal Property: The children of a red node are black.
 * * Depth Property: All the external nodes have the same Black Depth,
 *   defined as the number of black ancestors minus one. (A node is an
 *   ancestor of itself.)
 */
enum COLOR { RED, BLACK };
template <typename K, typename V>
class rb_tree;
template <typename K, typename V>
class rb_entry {
private:
    K _key;
    V _value;
    COLOR col;
    void set_key(const K & k) { _key = k; }
    COLOR color() const { return col; }
    bool is_red() const { return col == RED; }
    bool is_black() const { return col == BLACK; }
    void set_color(COLOR c) { col = c; }
public:
    rb_entry(const K & k = K(), const V & v = V())
         : _key(k), _value(v), col(BLACK) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class rb_tree<K, V>;
};
template <typename K, typename V>
class rb_tree {
private:
    struct node {
        rb_entry<K, V> ent;
        node * par;
        node * left;
        node * right;
        node() : ent(), par(NULL), left(NULL), right(NULL) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        rb_entry<K, V> & operator*() { return p->ent; }
        rb_entry<K, V> & operator*() const { return p->ent; }
        rb_entry<K, V> * operator->() { return &p->ent; }
        rb_entry<K, V> * operator->() const { return &p->ent; }
        iterator left() const { return iterator(p->left); }
        iterator right() const { return iterator(p->right); }
        iterator parent() const { return iterator(p->par); }
        bool is_root() const { return p->par == NULL; }
        bool is_external() const
        { return p->left == NULL && p->right == NULL; }
        bool is_internal() const { return !is_external(); }
        bool operator==(const iterator & _p) const
        { return p == _p.p; }
        bool operator!=(const iterator & _p) const
        { return p != _p.p; }
        iterator & operator++() {
            node * w = p->right;
            if (w->left != NULL || w->right != NULL) {
                do { p = w; w = w->left; }
                while (w->left != NULL || w->right != NULL);
            } else {
                w = p->par;
                while (p == w->right)
                { p = w; w = w->par; }
                p = w;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator i = *this;
            ++*this;
            return i;
        }
        friend class rb_tree<K, V>;
    };
public:
    rb_tree() : _root(NULL), n(0)
    { add_root(); expand_external(_root); }
    rb_tree(const rb_tree<K, V> & st)
           : _root(clone(st._root, NULL)), n(st.n) { }
    ~rb_tree() { make_empty(_root); }
    rb_tree & operator=(const rb_tree<K, V> & st) {
        copy(_root->left, st._root->left);
        n = st.n;
        return *this;
    }
    void clear() {
        n = 0;
        make_empty(_root->left->left);
        make_empty(_root->left->right);
    }
    V & operator[](const K & k) {
        iterator v = finder(k, root());
        if (v.is_external()) {
            expand_external(v);
            v->set_key(k);
            n++;
            if (v == root())
                set_black(v);
            else {
                set_red(v);
                remedy_double_red(v);
            }
        }
        return v->_value;
    }
    iterator find(const K & k) {
        iterator v = finder(k, root());
        if (v.is_internal()) return v;
        else return end();
    }
    iterator insert(const K & k, const V & x) {
        iterator v = inserter(k, x);
        if (v != end())
            if (v == root())
                set_black(v);
            else {
                set_red(v);
                remedy_double_red(v);
            }
        return v;
    }
    void erase(const K & k) {
        iterator u = finder(k, root());
        if (u.is_internal())
            erase(u);
    }
    void erase(const iterator & u) {
        COLOR par_col;
        iterator r = eraser(u, par_col);
        if (r == root() || r->is_red() || par_col == RED)
            set_black(r);
        else remedy_double_black(r);
    }
    iterator begin() {
        iterator v = root();
        while (v.is_internal()) v = v.left();
        return v.parent();
    }
    iterator end() { return _root; }
    int size() const { return n; }
    bool empty() const { return n == 0; }
    iterator root() const { return iterator(_root->left); }
private:
    void add_root() { _root = new node; }
    void expand_external(const iterator & p) {
        node * v = p.p;
        v->left = new node;
        v->left->par = v;
        v->right = new node;
        v->right->par = v;
    }
    iterator remove_above_external(const iterator & p, COLOR & par_col) {
        node * w = p.p; node * v = w->par;
        node * sib = (w == v->left ? v->right : v->left);
        if (v == _root) {
            _root = sib;
            sib->par = NULL;
        } else {
            node * gpar = v->par;
            if (v == gpar->left) gpar->left = sib;
            else gpar->right = sib;
            sib->par = gpar;
        }
        par_col = v->ent.color();
        delete w; delete v;
        return iterator(sib);
    }
    iterator finder(const K & k, const iterator & v) {
        if (v.is_external()) return v;
        if (k < v->key()) return finder(k, v.left());
        else if (v->key() < k) return finder(k, v.right());
        else return v;
    }
    iterator inserter(const K & k, const V & x) {
        iterator v = finder(k, root());
        if (v.is_internal()) return end();
        expand_external(v);
        v->set_key(k); v->set_value(x);
        n++;
        return v;
    }
    iterator eraser(const iterator & v, COLOR & par_col) {
        iterator w;
        if (v.left().is_external()) w = v.left();
        else if (v.right().is_external()) w = v.right();
        else {
            w = v.right();
            do { w = w.left(); } while (w.is_internal());
            iterator u = w.parent();
            v->set_key(u->key()); v->set_value(u->value());
        }
        n--;
        return remove_above_external(w, par_col);
    }
    iterator restructure(const iterator & p) {
        node * x = p.p, * y = x->par, * z = y->par;
        node * a, * b, * c, * T0, * T1, * T2, * T3;
        if (x == y->left && y == z->left) {
            a = x;
            b = y;
            c = z;
            T0 = a->left;
            T1 = a->right;
            T2 = b->right;
            T3 = c->right;
        }
        else if (x == y->left && y == z->right) {
            a = z;
            b = x;
            c = y;
            T0 = a->left;
            T1 = b->left;
            T2 = b->right;
            T3 = c->right;
        }
        else if (x == y->right && y == z->left) {
            a = y;
            b = x;
            c = z;
            T0 = a->left;
            T1 = b->left;
            T2 = b->right;
            T3 = c->right;
        }
        else {
            a = z;
            b = y;
            c = x;
            T0 = a->left;
            T1 = b->left;
            T2 = c->left;
            T3 = c->right;
        }
        if (z->par->left == z) z->par->left = b;
        else z->par->right = b;
        b->par = z->par;
        b->left = a;
        a->par = b;
        a->left = T0;
        T0->par = a;
        a->right = T1;
        T1->par = a;
        b->right = c;
        c->par = b;
        c->left = T2;
        T2->par = c;
        c->right = T3;
        T3->par = c;
        return iterator(b);
    }
    iterator sibling(const iterator & v) const
    { return v == v.parent().left() ? v.parent().right() : v.parent().left(); }
    void set_black(const iterator & v)
    { v->set_color(BLACK); }
    void set_red(const iterator & v)
    { v->set_color(RED); }
    void set_color(const iterator & v, COLOR c)
    { v->set_color(c); }
    void remedy_double_red(const iterator & z) {
        iterator v = z.parent();
        if (v == root() || v->is_black()) return;
        if (sibling(v)->is_black()) {
            v = restructure(z);
            set_black(v);
            set_red(v.left()); set_red(v.right());
        } else {
            set_black(v); set_black(sibling(v));
            iterator u = v.parent();
            if (u == root()) return;
            set_red(u);
            remedy_double_red(u);
        }
    }
    void remedy_double_black(const iterator & r) {
        iterator x = r.parent();
        iterator y = sibling(r);
        if (y->is_black()) {
            if (y.left()->is_red() || y.right()->is_red()) {
                iterator z = (y.left()->is_red() ? y.left() : y.right());
                COLOR top_color = x->color();
                z = restructure(z);
                set_color(z, top_color);
                set_black(r);
                set_black(z.left()); set_black(z.right());
            } else {
                set_black(r); set_red(y);
                if (x->is_black() && x != root())
                    remedy_double_black(x);
                set_black(x);
            }
        } else {
            iterator z = (y == x.right() ? y.right() : y.left());
            restructure(z);
            set_black(y); set_red(x);
            remedy_double_black(r);
        }
    }
    void make_empty(node * & p) const {
        if (p != NULL) {
            make_empty(p->left);
            make_empty(p->right);
            delete p;
            p = NULL;
        }
    }
    node * clone(node * t, node * p) const {
        if (t == NULL) return NULL;
        node * r = new node;
        r->ent = t->ent;
        r->par = p;
        r->left = clone(t->left, r);
        r->right = clone(t->right, r);
        return r;
    }
    void copy(node * t, node * src) const {
        t->ent = src->ent;
        if (src->left == NULL)
            make_empty(t->left);
        else
        if (t->left == NULL)
            t->left = clone(src->left, t);
        else
            copy(t->left, src->left);
        if (src->right == NULL)
            make_empty(t->right);
        else
        if (t->right == NULL)
            t->right = clone(src->right, t);
        else
            copy(t->right, src->right);
    }
private:
    node * _root;
    int n;
};

#endif // _SEARCH_TREE_H
