#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;

template <typename K, typename V>
class skip_list;
template <typename K, typename V>
class entry {
private:
    K _key;
    V _value;
public:
    entry(const K & k = K(), const V & v = V())
         : _key(k), _value(v) { }
    const K & key() const { return _key; }
    const V & value() const { return _value; }
    void set_value(const V & v) { _value = v; }
    friend class skip_list<K, V>;
    template <typename K1, typename V1, typename H>
    friend class dictionary;
};

template <typename K, typename V, typename H>
class dictionary {
private:
    struct node {
        int hash_code;
        entry<K, V> ent;
        int next;
        bool is_end;
        node() : hash_code(-1), ent(), next(-1), is_end(false) { }
    };
public:
    class iterator {
    private:
        node * p;
    public:
        iterator(node * _p = NULL) : p(_p) { }
        entry<K, V> * operator->() { return &p->ent; }
        entry<K, V> * operator->() const { return &p->ent; }
        entry<K, V> & operator*() { return p->ent; }
        entry<K, V> & operator*() const { return p->ent; }
        bool operator==(const iterator & u) const
        { return u.p == p; }
        bool operator!=(const iterator & u) const
        { return u.p != p; }
        iterator & operator++() {
            do {
                ++p;
            } while (!p->is_end && p->hash_code == -1);
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
    };
private:
    int capacity;
    int n;
    int free_list;
    int last;
    int upper;
    int * bucket;
    node * data;
    H hash;
public:
    dictionary(int cap = 3)
              : capacity(0), n(0), free_list(0), last(-1), upper(0) {
        capacity = prime(cap);
        bucket = new int [capacity];
        memset(bucket, -1, sizeof(int) * capacity);
        data = new node [capacity];
        data[last + 1].is_end = true;
    }
    dictionary(const dictionary<K, V, H> & dict)
    : capacity(dict.capacity), n(dict.n), free_list(dict.free_list), last(dict.last), upper(dict.upper) {
        bucket = new int [capacity];
        memcpy(bucket, dict.bucket, sizeof(int) * capacity);
        data = new node [capacity];
        memcpy(data, dict.data, sizeof(node) * capacity);
    }
    ~dictionary() { delete [] bucket; delete [] data; }
    dictionary<K, V, H> & operator=(const dictionary<K, V, H> & dict) {
        delete [] bucket;
        delete [] data;
        capacity = dict.capacity;
        n = dict.n;
        free_list = dict.free_list;
        last = dict.last;
        upper = dict.upper;
        bucket = new int [capacity];
        memcpy(bucket, dict.bucket, sizeof(int) * capacity);
        data = new node [capacity];
        memcpy(data, dict.data, sizeof(node) * capacity);
        return *this;
    }
    V & operator[](const K & k) {
        ensure_capacity(n + 1);
        int h = hash(k);
        int i = h % capacity;
        if (bucket[i] == -1) {
            bucket[i] = inserter(k, V(), h);
            return data[bucket[i]].ent._value;
        }
        i = bucket[i];
        int p;
        if (finder(k, h, i, p))
            return data[i].ent._value;
        int j = inserter(k, V(), h);
        data[i].next = j;
        return data[j].ent._value;
    }
    iterator find(const K & k) const {
        int h = hash(k);
        int i = h % capacity;
        if (bucket[i] == -1) return end();
        i = bucket[i];
        int p;
        if (finder(k, h, i, p))
            return iterator(data + i);
        return end();
    }
    iterator insert(const K & k, const V & v) {
        ensure_capacity(n + 1);
        int h = hash(k);
        int i = h % capacity;
        if (bucket[i] != -1) {
            i = bucket[i];
            int p;
            if (finder(k, h, i, p))
                return end();
            data[i].next = free_list;
        } else bucket[i] = free_list;
        int j = inserter(k, v, h);
        return iterator(data + j);
    }
    void erase(const iterator & u)
    { erase(u->key()); }
    void erase(const K & k) {
        int h = hash(k);
        int i = h % capacity;
        if (bucket[i] == -1) return;
        int j = bucket[i], p = -1;
        if (finder(k, h, j, p)) {
            if (p == -1)
                bucket[i] = data[j].next;
            else data[p].next = data[j].next;
            data[j].hash_code = -1;
            data[j].next = free_list;
            free_list = j;
            if (j == last) {
                data[last + 1].is_end = false;
                while (data[last].hash_code == -1)
                    last--;
                data[last + 1].is_end = true;
            }
            n--;
        }
    }
    void clear() {
        memset(bucket, -1, sizeof(int) * capacity);
        delete [] data;
        data = new node [capacity];
        n = 0;
        free_list = 0;
        last = -1;
        data[last + 1].is_end = true;
    }
    iterator begin() const {
        node * u = data;
        while (!u->is_end && u->hash_code == -1)
            u++;
        return iterator(u);
    }
    iterator end() const { return iterator(data + last + 1); }
    int size() const
    { return n; }
    bool empty() const
    { return n == 0; }
private:
    bool finder(const K & k, const int & h, int & i, int & j) const {
        while ((data[i].hash_code != h || (data[i].hash_code == h && data[i].ent._key != k))
               && data[i].next != -1) {
            j = i;
            i = data[i].next;
        }
        return data[i].hash_code == h && data[i].ent._key == k;
    }
    int inserter(const K & k, const V & v, const int & h) {
        int i = free_list;
        if (i > last) {
            data[last + 1].is_end = false;
            last = i;
            data[last + 1].is_end = true;
        }
        data[i].ent._key = k;
        data[i].ent._value = v;
        data[i].hash_code = h;
        if (data[free_list].next != -1) {
            free_list = data[free_list].next;
            data[i].next = -1;
        } else free_list = last + 1;
        n++;
        return i;
    }
private:
    bool is_prime(int num) const {
        if (num < 2) return false;
        for (int i = 2; i * i <= num; i++)
            if (num % i == 0) return false;
        return true;
    }
    int prime(int new_capacity) {
        if (is_prime(upper) && new_capacity <= upper)
            return upper;
        while (!is_prime(new_capacity))
            new_capacity++;
        return upper = new_capacity;
    }
    void ensure_capacity(int new_capacity) {
        new_capacity = prime(new_capacity);
        if (new_capacity <= capacity) return;
        int * new_bucket = new int [new_capacity];
        memset(new_bucket, -1, sizeof(int) * new_capacity);
        node * new_data = new node [new_capacity];
        memcpy(new_data, data, sizeof(node) * n);
        for (int i = 0; i < n; i++) {
            int h = new_data[i].hash_code;
            if (h != -1) {
                int j = h % new_capacity;
                new_data[i].next = new_bucket[j];
                new_bucket[j] = i;
            }
        }
        delete [] bucket;
        bucket = new_bucket;
        delete [] data;
        data = new_data;
        capacity = new_capacity;
    }
};

template <typename K, typename V>
class skip_list {
private:
    struct node {
        entry<K, V> ent;
        node * before;
        node * after;
        node * above;
        node * below;
        int height;
        node() : ent(), before(NULL), after(NULL), above(NULL), below(NULL), height(0) { }
    };
public:
    class iterator {
    public:
        iterator(node * _p = NULL) : p(_p) { }
        entry<K, V> & operator*() { return p->ent; }
        entry<K, V> & operator*() const { return p->ent; }
        entry<K, V> * operator->() { return &p->ent; }
        entry<K, V> * operator->() const { return &p->ent; }
        bool operator==(const iterator & v) const
        { return p == v.p; }
        bool operator!=(const iterator & v) const
        { return p != v.p; }
        iterator & operator++() {
            p = p->after;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        friend class skip_list<K, V>;
    private:
        node * p;
    };
public:
    skip_list() : n(0) { create(); }
    ~skip_list() { destroy(); }
    V & operator[](const K & k) {
        iterator u = finder(k);
        if (u.p == root || u->key() != k)
            return inserter(u, k, V())->_value;
        else return u->_value;
    }
    iterator find(const K & k) {
        iterator u = finder(k);
        if (u.p == root || u->key() != k) return end();
        return u;
    }
    iterator insert(const K & k, const V & v) {
        iterator u = finder(k);
        if (u.p != root && u->key() == k) return end();
        return inserter(u, k, v);
    }
    void erase(const K & k) {
        iterator u = finder(k);
        if (u.p != root && u->key() == k)
            erase(u);
    }
    void erase(const iterator & p) {
        node * u = p.p;
        do {
            node * v = u->above;
            u->before->after = u->after;
            u->after->before = u->before;
            delete u;
            u = v;
        } while (u != NULL);
        n--;
    }
    void clear() {
        destroy();
        create();
        n = 0;
    }
    iterator begin() { return iterator(root->after); }
    iterator end() { return iterator(null); }
    int size() const { return n; }
    bool empty() const { return n == 0; }
private:
    iterator finder(const K & k) {
        node * v = highest_root;
        node * u = highest_null;
        while (v->below != NULL) {
            v = v->below;
            u = u->below;
            while (v->after != u && k >= v->after->ent._key)
                v = v->after;
        }
        return iterator(v);
    }
    iterator inserter(const iterator & p, const K & k, const V & v) {
        node * u = p.p;
        node * t = NULL;
        node * r = root;
        node * w = null;
        iterator ret;
        int h = 0;
        do {
            if (h > height_max(n + 1)) break;
            h++;
            node * tt = t;
            if (h == 1) {
                t = insert_after(u, k, v);
                ret = iterator(t);
            } else t = insert_after(u, k, V());
            if (tt != NULL) {
                t->below = tt;
                tt->above = t;
            }
            while (u != r && u->above == NULL)
                u = u->before;
            if (u->above == NULL) {
                node * tr = new node;
                tr->height = h;
                tr->below = r;
                r->above = tr;
                node * tn = new node;
                tn->below = w;
                w->above = tn;
                tn->height = h;
                tr->after = tn;
                tn->before = tr;
                highest_root = tr;
                highest_null = tn;
            }
            u = u->above;
            r = r->above;
            w = w->above;
        } while (rand() % 2);
        n++;
        return ret;
    }
    node * insert_after(node * u, const K & k, const V & v) {
        node * p = u;
        node * t = new node;
        t->ent._key = k;
        t->ent._value = v;
        t->before = p;
        t->after = p->after;
        t->height = p->height;
        p->after->before = t;
        p->after = t;
        return t;
    }
    void create() {
        root = new node;
        null = new node;
        root->after = null;
        null->before = root;
        highest_root = root;
        highest_null = null;
        srand(time(NULL));
    }
    void destroy() {
        node * p = root;
        while (p != NULL) {
            node * u = p->above;
            while (p != NULL) {
                node * v = p->after;
                delete p;
                p = v;
            }
            p = u;
        }
    }
    int height_max(int i) const
    { return 3 * int(ceil(log(i) / log(2))); }
private:
    node * highest_root;
    node * highest_null;
    node * root;
    node * null;
    int n;
};

#endif // _HASH_TABLE_H
