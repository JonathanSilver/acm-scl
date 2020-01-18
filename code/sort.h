#ifndef _SORT_H
#define _SORT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
void insertion_sort(vector<T> & v, int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        T t = v[i];
        int j = i - 1;
        while (j >= lo && t < v[j]) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = t;
    }
}

template <typename T>
void heap_down(vector<T> & v, int i, int n, int lo) {
    int j;
    T t = v[lo + i - 1];
    for ( ; (i << 1) <= n; i = j) {
        j = i << 1;
        if (j < n && v[lo + j - 1] < v[lo + j])
            ++j;
        if (t < v[lo + j - 1])
            v[lo + i - 1] = v[lo + j - 1];
        else break;
    }
    v[lo + i - 1] = t;
}
template <typename T>
void heap_sort(vector<T> & v, int lo, int hi) {
    int n = hi - lo + 1;
    for (int i = n >> 1; i >= 1; i--)
        heap_down(v, i, n, lo);
    for (int i = n; i > 1; i--) {
        swap(v[lo], v[lo + i - 1]);
        heap_down(v, 1, i - 1, lo);
    }
}

template <typename T>
void swap_if_greater(vector<T> & v, int a, int b) {
    if (a != b && !(v[a] < v[b]))
        swap(v[a], v[b]);
}
int floor_log_2(int n) {
    int c = 0;
    while (n >= 1) {
        c++;
        n >>= 2;
    }
    return c;
}
template <typename T>
int pick_pivot_and_partition(vector<T> & v, int lo, int hi) {
    int mid = lo + ((hi - lo) >> 1);
    swap_if_greater(v, lo, mid);
    swap_if_greater(v, lo, hi);
    swap_if_greater(v, mid, hi);
    swap(v[mid], v[hi - 1]);
    const T & t = v[hi - 1];
    int i = lo;
    int j = hi - 1;
    while (i < j) {
        while (v[++i] < t);
        while (t < v[--j]);
        if (i >= j) break;
        swap(v[i], v[j]);
    }
    swap(v[i], v[hi - 1]);
    return i;
}
template <typename T>
void intro_sort(vector<T> & v, int lo, int hi, int dep_lim) {
    while (lo < hi) {
        int n = hi - lo + 1;
        if (n <= 16) {
            if (n == 1) return;
            if (n == 2) {
                swap_if_greater(v, lo, hi);
                return;
            }
            if (n == 3) {
                swap_if_greater(v, lo, hi - 1);
                swap_if_greater(v, lo, hi);
                swap_if_greater(v, hi - 1, hi);
                return;
            }
            insertion_sort(v, lo, hi);
            return;
        } else {
            if (dep_lim == 0) {
                heap_sort(v, lo, hi);
                return;
            }
            --dep_lim;
            int pivot = pick_pivot_and_partition(v, lo, hi);
            intro_sort(v, pivot + 1, hi, dep_lim);
            hi = pivot - 1;
        }
    }
}
template <typename T>
void introspective_sort(vector<T> & v) {
    intro_sort(v, 0, v.size() - 1, floor_log_2(v.size()) << 1);
}

template <typename T>
const T & median3(vector<T> & v, int left, int right) {
    int mid = (left + right) >> 1;
    if (v[mid] < v[left])
        swap(v[left], v[mid]);
    if (v[right] < v[left])
        swap(v[left], v[right]);
    if (v[right] < v[mid])
        swap(v[mid], v[right]);
    swap(v[mid], v[right - 1]);
    return v[right - 1];
}
template <typename T>
void quick_sort(vector<T> & v, int left, int right) {
    if (left + 10 <= right) {
        const T & pivot = median3(v, left, right);
        int i = left, j = right - 1;
        for ( ; ; ) {
            while (v[++i] < pivot);
            while (pivot < v[--j]);
            if (i < j) swap(v[i], v[j]);
            else break;
        }
        swap(v[i], v[right - 1]);
        quick_sort(v, left, i - 1);
        quick_sort(v, i + 1, right);
    } else insertion_sort(v, left, right);
}
template <typename T>
void quick_sort(vector<T> & v) {
    if (v.size() <= 1) return;
    quick_sort(v, 0, v.size() - 1);
}

template <typename T>
void quick_select(vector<T> & v, int left, int right, int k) {
    if (left + 10 <= right) {
        const T & pivot = median3(v, left, right);
        int i = left, j = right - 1;
        for ( ; ; ) {
            while (v[++i] < pivot);
            while (pivot < v[--j]);
            if (i < j) swap(v[i], v[j]);
            else break;
        }
        swap(v[i], v[right - 1]);
        if (k <= i) quick_select(v, left, i - 1, k);
        else if (k > i + 1) quick_select(v, i + 1, right, k);
    } else insertion_sort(v, left, right);
}
/**
 * Places the k-th smallest item in v[k - 1].
 * k is the desired rank which starts from 1.
 */
template <typename T>
void quick_select(vector<T> & v, int k) {
    quick_select(v, 0, v.size() - 1, k);
}

template <typename T>
void merge_2(vector<T> & a, vector<T> & tmp, int left_pos, int right_pos, int right_end, long long & cnt) {
    int left_end = right_pos - 1;
    int tmp_pos = left_pos;
    int num = right_end - left_pos + 1;
    while (left_pos <= left_end && right_pos <= right_end) {
        if (!(a[right_pos] < a[left_pos]))
            tmp[tmp_pos++] = a[left_pos++];
        else {
            tmp[tmp_pos++] = a[right_pos++];
            cnt += left_end - left_pos + 1;
        }
    }
    while (left_pos <= left_end)
        tmp[tmp_pos++] = a[left_pos++];
    while (right_pos <= right_end)
        tmp[tmp_pos++] = a[right_pos++];
    for (int i = 0; i < num; ++i, --right_end)
        a[right_end] = tmp[right_end];
}
template <typename T>
void merge_sort_2(vector<T> & v, vector<T> & tmp, int left, int right, long long & cnt) {
    if (left < right) {
        int mid = (left + right) >> 1;
        merge_sort_2(v, tmp, left, mid, cnt);
        merge_sort_2(v, tmp, mid + 1, right, cnt);
        merge_2(v, tmp, left, mid + 1, right, cnt);
    }
}
template <typename T>
long long merge_sort_2(vector<T> & v) {
    long long cnt = 0;
    vector<T> tmp(v.size());
    merge_sort_2(v, tmp, 0, v.size() - 1, cnt);
    return cnt;
}

template <typename T>
void merge(vector<T> & in, vector<T> & out, int m, int l) {
    int i = l;
    int j = l + m;
    int n = in.size();
    int e1 = min(l + m, n);
    int e2 = min(l + (m << 1), n);
    int k = l;
    while (i < e1 && j < e2) {
        if (!(in[j] < in[i]))
            out[k++] = in[i++];
        else
            out[k++] = in[j++];
    }
    while (i < e1)
        out[k++] = in[i++];
    while (j < e2)
        out[k++] = in[j++];
}
template <typename T>
void merge_sort(vector<T> & v) {
    typedef vector<T> vect;
    int n = v.size();
    vect v1(v); vect * in = &v1;
    vect v2(n); vect * out = &v2;
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; j += (i << 1))
            merge(*in, *out, i, j);
        swap(in, out);
    }
    v = *in;
}

void radix_sort_1(vector<string> & v, int str_len) {
    const int BUCKETS = 256;
    vector<vector<string> > buckets(BUCKETS);
    for (int pos = str_len - 1; pos >= 0; --pos) {
        for (string & s : v)
            buckets[s[pos]].push_back(move(s));
        int idx = 0;
        for (auto & this_bucket : buckets) {
            for (string & s : this_bucket)
                v[idx++] = move(s);
            this_bucket.clear();
        }
    }
}

void radix_sort_2(vector<string> & v, int max_len) {
    const int BUCKETS = 256;
    vector<vector<string> > words_by_len(max_len + 1);
    vector<vector<string> > buckets(BUCKETS);
    for (string & s : v)
        words_by_len[s.size()].push_back(move(s));
    int idx = 0;
    for (auto & word_list : words_by_len)
        for (string & s : word_list)
            v[idx++] = move(s);
    int starting_idx = v.size();
    for (int pos = max_len - 1; pos >= 0; --pos) {
        starting_idx -= words_by_len[pos + 1].size();
        for (int i = starting_idx; i < v.size(); ++i)
            buckets[v[i][pos]].push_back(move(v[i]));
        idx = starting_idx;
        for (auto & this_bucket : buckets) {
            for (string & s : this_bucket)
                v[idx++] = move(s);
            this_bucket.clear();
        }
    }
}

void counting_radix_sort(vector<string> & v, int str_len) {
    const int BUCKETS = 256;
    int N = v.size();
    vector<string> buffer(N);
    vector<string> * in = &v;
    vector<string> * out = &buffer;
    for (int pos = str_len - 1; pos >= 0; --pos) {
        vector<int> count(BUCKETS + 1);
        for (int i = 0; i < N; ++i)
            ++count[(*in)[i][pos] + 1];
        for (int b = 1; b <= BUCKETS; ++b)
            count[b] += count[b - 1];
        for (int i = 0; i < N; ++i)
            (*out)[count[(*in)[i][pos]]++] = move((*in)[i]);
        swap(in, out);
    }
    if (str_len & 1)
        for (int i = 0; i < N; ++i)
            (*out)[i] = move((*in)[i]);
}

#endif // _SORT_H
