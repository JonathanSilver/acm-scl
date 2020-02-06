#ifndef _MISC_H
#define _MISC_H

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <complex>

using namespace std;

template <class T>
int search_lower(vector<T> & v, T x) {
    int l = 0, r = v.size() - 1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (v[mid] < x)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return l;
}

template <class T>
int search_upper(vector<T> & v, T x) {
    int l = 0, r = v.size() - 1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (x < v[mid])
            r = mid - 1;
        else
            l = mid + 1;
    }
    return r;
}

vector<int> compute_fail_function(const string & pattern) {
    vector<int> fail(pattern.size());
    fail[0] = 0;
    int m = pattern.size();
    int i = 1, j = 0;
    while (i < m) {
        if (pattern[j] == pattern[i]) {
            fail[i] = j + 1;
            i++; j++;
        } else if (j > 0) j = fail[j - 1];
        else {
            fail[i] = 0;
            i++;
        }
    }
    return fail;
}
vector<int> KMP_match(const string & text, const string & pattern) {
    vector<int> v;
    int n = text.size();
    int m = pattern.size();
    vector<int> fail = compute_fail_function(pattern);
    int i = 0, j = 0;
    while (i < n) {
        if (pattern[j] == text[i]) {
            if (j == m - 1) {
                v.push_back(i - m + 1);
                j = fail[j];
            } else j++;
            i++;
        } else if (j > 0) j = fail[j - 1];
        else i++;
    }
    return v;
}

vector<int> get_next(const string & pattern) {
    int m = pattern.size();
    vector<int> next(m);
    next[0] = m;
    int i = 0;
    while (i + 1 < m && pattern[i] == pattern[i + 1])
        i++;
    next[1] = i;
    int k = 1;
    for (i = 2; i < m; i++) {
        if (next[i - k] + i < next[k] + k)
            next[i] = next[i - k];
        else {
            int j = next[k] + k - i;
            if (j < 0) j = 0;
            while (i + j < m && pattern[j] == pattern[i + j])
                j++;
            next[i] = j;
            k = i;
        }
    }
    return next;
}
vector<int> ext_KMP(const string & text, const string & pattern) {
    int n = text.size();
    int m = pattern.size();
    vector<int> extend(n);
    vector<int> next = get_next(pattern);
    int i = 0;
    while (i < n && i < m && pattern[i] == text[i])
        i++;
    extend[0] = i;
    int k = 0;
    for (i = 1; i < n; i++) {
        if (next[i - k] + i < extend[k] + k)
            extend[i] = next[i - k];
        else {
            int j = extend[k] + k - i;
            if (j < 0) j = 0;
            while (i + j < n && j < m && pattern[j] == text[i + j])
                j++;
            extend[i] = j;
            k = i;
        }
    }
    return extend;
}

int manacher(const string & text, char delimiter = 1, char begin_letter = 2, char end_letter = 3) {
    int m = text.size();
    int n = (m << 1) + 3;
    string str(n, delimiter);
    str[0] = begin_letter;
    str[n - 1] = end_letter;
    for (int i = 0; i < m; i++)
        str[(i << 1) + 2] = text[i];
    int p = 0, q = 0, r = 0;
    vector<int> len(n, 1);
    for (int i = 1; i < n - 1; i++) {
        if (q > i) len[i] = min(q - i, len[(p << 1) - i]);
        while (str[i - len[i]] == str[i + len[i]])
            len[i]++;
        if (i + len[i] > q) {
            q = i + len[i];
            p = i;
        }
        r = max(r, len[i]);
    }
    return r - 1;
}

vector<int> get_SG(const vector<int> & v, int n) {
    vector<int> SG(n);
    for (int i = 1; i < n; i++) {
        set<int> s;
        for (const auto & x : v)
            if (x <= i) s.insert(SG[i - x]);
        for (int j = 0; ; j++)
            if (!s.count(j)) {
                SG[i] = j;
                break;
            }
    }
    return SG;
}

int rev(int idx, int n) {
    int r = 0;
    for (int i = 0; (1 << i) < n; i++) {
        r <<= 1;
        if (idx & (1 << i))
            r |= 1;
    }
    return r;
}
/**
 * FFT - Fast Fourier Transform
 * Complexity: O(n lg n)
 *
 * Polynomial: A(x) = Sum(a[i] x^i, i = 0 .. n - 1)
 * y = (y[0], y[1], y[n - 1]), where y[k] = A(x[k])
 * a = (a[0], a[1], a[n - 1])
 * y = DFT(a)  (DFT - Discrete Fourier Transform)
 *
 * Convolution Theorem:
 * For any two vectors a and b of length n, where n is a power of 2,
 * a (*) b = DFT'(DFT(a) dot DFT(b))
 *
 * Parameters:
 * The size of a must be a power of 2.
 * If op ==  1, DFT,
 * or op == -1, DFT'.
 */
vector<complex<double> > FFT(const vector<complex<double> > & a, int op) {
    int n = a.size();
    vector<complex<double> > v(n);
    for (int i = 0; i < n; i++)
        v[rev(i, n)] = a[i];
    for (int s = 1; (1 << s) <= n; s++)
    {
        int m = (1 << s);
        complex<double> wm = complex<double>(cos(op * 2 * acos(-1) / m), sin(op * 2 * acos(-1) / m));
        for (int k = 0; k < n; k += m)
        {
            complex<double> w = complex<double>(1, 0);
            for (int j = 0; j < (m >> 1); j++)
            {
                complex<double> t = w * v[k + j + (m >> 1)];
                complex<double> u = v[k + j];
                v[k + j] = u + t;
                v[k + j + (m >> 1)] = u - t;
                w = w * wm;
            }
        }
    }
    if (op == -1)
        for (int i = 0; i < n; i++)
            v[i] = complex<double>(v[i].real() / n, v[i].imag() / n);
    return v;
}

/**
 * str: the input string
 * SA:  an existing array to place the suffix array
 * LCP: an existing array to place the LCP (Longest Common Prefix) information
 * The size of SA and LCP should be as large as that of str
 * Complexity: O(N^2 log N)
 */
void create_suffix_array_slow(const string & str, vector<int> & SA, vector<int> & LCP) {
    size_t N = str.size();
    const char * cstr = str.c_str();
    vector<const char *> suffixes(N);
    for (int i = 0; i < N; i++)
        suffixes[i] = cstr + i;
    sort(suffixes.begin(), suffixes.end(), [] (const char * s1, const char * s2) { return strcmp(s1, s2) < 0; });
    for (int i = 0; i < N; i++)
        SA[i] = suffixes[i] - cstr;
    LCP[0] = 0;
    for (int i = 1; i < N; i++) {
        int j = 0;
        while (j < strlen(suffixes[i - 1]) && j < strlen(suffixes[i]) && suffixes[i - 1][j] == suffixes[i][j])
            j++;
        LCP[i] = j;
    }
}

void radix_pass(const vector<int> & in, vector<int> & out, const vector<int> & s, int offset, int n, int k) {
    vector<int> count(k + 2);
    for (int i = 0; i < n; i++)
        count[s[in[i] + offset] + 1]++;
    for (int i = 1; i <= k + 1; i++)
        count[i] += count[i - 1];
    for (int i = 0; i < n; i++)
        out[count[s[in[i] + offset]]++] = in[i];
}
void radix_pass(const vector<int> & in, vector<int> & out, const vector<int> & s, int n, int k)
{ radix_pass(in, out, s, 0, n, k); }
int assign_names(const vector<int> & s, vector<int> & s12, vector<int> & sa12, int n0, int n12, int k) {
    radix_pass(s12, sa12, s, 2, n12, k);
    radix_pass(sa12, s12, s, 1, n12, k);
    radix_pass(s12, sa12, s, 0, n12, k);
    int name = 0;
    int c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n12; i++) {
        if (s[sa12[i]] != c0 || s[sa12[i] + 1] != c1 || s[sa12[i] + 2] != c2) {
            ++name;
            c0 = s[sa12[i]];
            c1 = s[sa12[i] + 1];
            c2 = s[sa12[i] + 2];
        }
        if (sa12[i] % 3 == 1)
            s12[sa12[i] / 3] = name;
        else
            s12[sa12[i] / 3 + n0] = name;
    }
    return name;
}
void make_suffix_array(const vector<int> & s, vector<int> & sa, int n, int k);
void compute_s12(vector<int> & s12, vector<int> & sa12, int n12, int k12) {
    if (k12 == n12)
        for (int i = 0; i < n12; i++)
            sa12[s12[i] - 1] = i;
    else {
        make_suffix_array(s12, sa12, n12, k12);
        for (int i = 0; i < n12; i++)
            s12[sa12[i]] = i + 1;
    }
}
void compute_s0(const vector<int> & s, vector<int> & s0, vector<int> & sa0, const vector<int> & sa12, int n0, int n12, int k) {
    for (int i = 0, j = 0; i < n12; i++)
        if (sa12[i] < n0)
            s0[j++] = 3 * sa12[i];
    radix_pass(s0, sa0, s, n0, k);
}
int get_index_into_s(const vector<int> & sa12, int t, int n0) {
    if (sa12[t] < n0)
        return sa12[t] * 3 + 1;
    else
        return (sa12[t] - n0) * 3 + 2;
}
bool leq(int a1, int a2, int b1, int b2)
{ return a1 < b1 || a1 == b1 && a2 <= b2; }
bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{ return a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3); }
bool suffix12_is_smaller(const vector<int> & s, const vector<int> & s12, const vector<int> & sa12, int n0, int i, int j, int t) {
    if (sa12[t] < n0)
        return leq(s[i], s12[sa12[t] + n0], s[j], s12[j / 3]);
    else
        return leq(s[i], s[i + 1], s12[sa12[t] - n0 + 1], s[j], s[j + 1], s12[j / 3 + n0]);
}
void merge(const vector<int> & s, const vector<int> & s12, vector<int> & sa, const vector<int> & sa0, const vector<int> & sa12, int n, int n0, int n12, int t) {
    int p = 0, k = 0;
    while (t != n12 && p != n0) {
        int i = get_index_into_s(sa12, t, n0);
        int j = sa0[p];
        if (suffix12_is_smaller(s, s12, sa12, n0, i, j, t)) {
            sa[k++] = i;
            t++;
        } else {
            sa[k++] = j;
            p++;
        }
    }
    while (p < n0)
        sa[k++] = sa0[p++];
    while (t < n12)
        sa[k++] = get_index_into_s(sa12, t++, n0);
}
void make_suffix_array(const vector<int> & s, vector<int> & sa, int n, int k) {
    int n0 = (n + 2) / 3;
    int n1 = (n + 1) / 3;
    int n2 = n / 3;
    int t = n0 - n1;
    int n12 = n1 + n2 + t;
    vector<int> s12(n12 + 3);
    vector<int> sa12(n12 + 3);
    vector<int> s0(n0);
    vector<int> sa0(n0);
    for (int i = 0, j = 0; i < n + t; i++)
        if (i % 3 != 0)
            s12[j++] = i;
    int k12 = assign_names(s, s12, sa12, n0, n12, k);
    compute_s12(s12, sa12, n12, k12);
    compute_s0(s, s0, sa0, sa12, n0, n12, k);
    merge(s, s12, sa, sa0, sa12, n, n0, n12, t);
}
void make_LCP_array(vector<int> & s, const vector<int> & sa, vector<int> & LCP) {
    int N = sa.size();
    vector<int> rank(N);
    s[N] = -1;
    for (int i = 0; i < N; i++)
        rank[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < N; i++)
        if (rank[i] > 0) {
            int j = sa[rank[i] - 1];
            while (s[i + h] == s[j + h])
                h++;
            LCP[rank[i]] = h;
            if (h > 0)
                h--;
        }
}
/**
 * Linear-Time Construction of Suffix Arrays
 */
void create_suffix_array(const string & str, vector<int> & SA, vector<int> & LCP) {
    size_t N = str.size();
    vector<int> s(N + 3);
    vector<int> sa(N + 3);
    for (int i = 0; i < N; i++)
        s[i] = str[i];
    make_suffix_array(s, sa, N, 250);
    for (int i = 0; i < N; i++)
        SA[i] = sa[i];
    make_LCP_array(s, SA, LCP);
}

/**
 * Constructs Suffix Array sa for s (whose size is n,
 * and all of whose elements must be non-negative numbers),
 * where sa[0] will always be n (empty),
 * and the size of sa is n + 1.
 * Complexity: O(n log^2(n))
 */
vector<int> construct_sa(const vector<int> & s) {
    int n = s.size();
    vector<int> sa(n + 1);
    vector<int> rk(n + 1), tmp(n + 1);
    for (int i = 0; i <= n; i++) {
        sa[i] = i;
        rk[i] = i < n ? s[i] : -1;
    }
    for (int k = 1; k <= n; k <<= 1) {
        auto compare_sa = [&] (auto & i, auto & j) {
            if (rk[i] == rk[j]) {
                int ri = i + k <= n ? rk[i + k] : -1;
                int rj = j + k <= n ? rk[j + k] : -1;
                return ri < rj;
            } return rk[i] < rk[j];
        };
        sort(sa.begin(), sa.end(), compare_sa);
        tmp[sa[0]] = 0;
        for (int i = 1; i <= n; i++)
            tmp[sa[i]] = tmp[sa[i - 1]] + (compare_sa(sa[i - 1], sa[i]) ? 1 : 0);
        rk.swap(tmp);
    }
    return sa;
}
/**
 * Constructs Longest Common Prefix Array lcp for s
 * based on the Suffix Array sa of s,
 * where lcp[0] will always be 0.
 * The size of lcp is n (the size of s).
 * Let s[k...] be s[k], s[k + 1], ... , s[n].
 * lcp[i] is the length of the Longest Common Prefix
 * of s[sa[i]...] and s[sa[i + 1]...].
 * Complexity: O(n)
 */
vector<int> construct_lcp(const vector<int> & s, const vector<int> & sa) {
    int n = s.size();
    vector<int> lcp(n);
    vector<int> rk(n + 1);
    for (int i = 0; i <= n; i++) rk[sa[i]] = i;
    int h = 0;
    lcp[0] = 0;
    for (int i = 0; i < n; i++) {
        int j = sa[rk[i] - 1];
        if (h > 0) h--;
        for ( ; i + h < n && j + h < n && s[i + h] == s[j + h]; h++);
        lcp[rk[i] - 1] = h;
    }
    return lcp;
}

pair<vector<int>, vector<int> > construct_sa_lcp(const vector<int> & s) {
    const int n = s.size();
    vector<int> sa(n), lcp(n), a(n), b(n);
    vector<int> * x = &a, * y = &b;
    int m = 0;
    for (int i = 0; i < n; i++) {
        sa[i] = i;
        m = max(m, s[i]);
    }
    vector<int> cnt(max(m, n) + 5);
    for (int i = 0; i < n; i++) cnt[(*x)[i] = s[i]]++;
    for (int i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
    for (int i = n - 1; i >= 0; i--) sa[--cnt[(*x)[i]]] = i;
    for (int k = 1; k <= n; k <<= 1) {
        auto cmp = [&] (int i, int j) {
            if ((*y)[i] == (*y)[j]) {
                int ri = i + k < n ? (*y)[i + k] : -1;
                int rj = j + k < n ? (*y)[j + k] : -1;
                return ri < rj;
            } else return (*y)[i] < (*y)[j];
        };
        int p = 0;
        for (int i = n - k; i < n; i++) (*y)[p++] = i;
        for (int i = 0; i < n; i++)
            if (sa[i] >= k) (*y)[p++] = sa[i] - k;
        for (int i = 0; i <= m; i++) cnt[i] = 0;
        for (int i = 0; i < n; i++) cnt[(*x)[(*y)[i]]]++;
        for (int i = 1; i <= m; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sa[--cnt[(*x)[(*y)[i]]]] = (*y)[i];
        swap(x, y);
        (*x)[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            (*x)[sa[i]] = (*x)[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
        m = (*x)[sa[n - 1]];
    }
    for (int i = 0, h = 0; i < n; i++) {
        if ((*x)[i]) {
            int j = sa[(*x)[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcp[(*x)[i]] = h;
            if (h) h--;
        }
    }
    return make_pair(sa, lcp);
}

void radix(int n, const vector<int> & r, const vector<int> & s,
           vector<int> & t, vector<int> & cnt) {
    cnt.assign(cnt.size(), 0);
    for (int i = 0; i <= n; i++) cnt[r[s[i]] + 1]++;
    for (int i = 1; i < cnt.size(); i++) cnt[i] += cnt[i - 1];
    for (int i = n; i >= 0; i--) t[--cnt[r[s[i]] + 1]] = s[i];
}
pair<vector<int>, vector<int> > construct_sa_lcp_slow(const vector<int> & s) {
    int n = s.size();
    vector<int> sa(n + 1), lcp(n);
    if (!n) return make_pair(sa, lcp);
    vector<int> rk(n + 1), a(n + 1), b(n + 1);
    int high = -1;
    for (int i = 0; i <= n; i++) {
        sa[i] = i;
        rk[i] = i < n ? s[i] : -1;
        high = max(high, rk[i]);
    }
    vector<int> cnt(max(high + 2, n) + 5);
    auto sa_cmp = [&] (int i, int j) {
        return a[i] == a[j] ? b[i] < b[j] : a[i] < a[j];
    };
    for (int k = 1; k <= n; k <<= 1) {
        for (int i = 0; i <= n; i++) {
            a[i] = rk[i];
            b[i] = i + k <= n ? rk[i + k] : -1;
        }
        radix(n, b, sa, rk, cnt);
        radix(n, a, rk, sa, cnt);
        rk[sa[0]] = 0;
        for (int i = 1; i <= n; i++)
            rk[sa[i]] = rk[sa[i - 1]] + sa_cmp(sa[i - 1], sa[i]);
    }
    int h = 0;
    for (int i = 0; i < n; i++) {
        int j = sa[rk[i] - 1];
        if (h) h--;
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
        lcp[rk[i] - 1] = h;
    }
    return make_pair(sa, lcp);
}

/**
 * Disjoint set class.
 */
class disj_sets {
private:
    vector<int> s;
public:
    explicit disj_sets(int n) : s(n, -1) { }
    int find(int x) { return s[x] < 0 ? x : s[x] = find(s[x]); }
    void union_sets(int x, int y) {
        int root1 = find(x), root2 = find(y);
        if (root1 == root2) return;
        if (s[root2] < s[root1])
            s[root1] = root2;
        else {
            if (s[root1] == s[root2])
                --s[root1];
            s[root2] = root1;
        }
    }
};

const int N = 100005;

namespace SA_IS {
int sa[N], rk[N], lcp[N], s[N << 1], t[N << 1], p[N], cnt[N], cur[N];
#define pushS(x) sa[cur[s[x]]--] = x
#define pushL(x) sa[cur[s[x]]++] = x
#define induced_sort(v)                                                             \
    for (int i = 0; i < n; i++) sa[i] = -1;                                         \
    for (int i = 0; i < m; i++) cnt[i] = 0;                                         \
    for (int i = 0; i < n; i++) cnt[s[i]]++;                                        \
    for (int i = 1; i < m; i++) cnt[i] += cnt[i - 1];                               \
    for (int i = 0; i < m; i++) cur[i] = cnt[i] - 1;                                \
    for (int i = n1 - 1; ~i; i--) pushS(v[i]);                                      \
    for (int i = 1; i < m; i++) cur[i] = cnt[i - 1];                                \
    for (int i = 0; i < n; i++) if (sa[i] > 0 &&  t[sa[i] - 1]) pushL(sa[i] - 1);   \
    for (int i = 0; i < m; i++) cur[i] = cnt[i] - 1;                                \
    for (int i = n - 1; ~i; i--) if (sa[i] > 0 && !t[sa[i] - 1]) pushS(sa[i] - 1)
void sais(int n, int m, int * s, int * t, int * p) {
    int n1 = t[n - 1] = 0, ch = rk[0] = -1, * s1 = s + n;
    for (int i = n - 2; ~i; i--) t[i] = s[i] == s[i + 1] ? t[i + 1] : s[i] > s[i + 1];
    for (int i = 1; i < n; i++) rk[i] = t[i - 1] && !t[i] ? (p[n1] = i, n1++) : -1;
    induced_sort(p);
    for (int i = 0, x, y; i < n; i++) if (~(x = rk[sa[i]])) {
        if (ch < 1 || p[x + 1] - p[x] != p[y + 1] - p[y]) ch++;
        else for (int j = p[x], k = p[y]; j <= p[x + 1]; j++, k++)
            if ((s[j] << 1 | t[j]) != (s[k] << 1 | t[k])) { ch++; break; }
        s1[y = x] = ch;
    }
    if (ch + 1 < n1) sais(n1, ch + 1, s1, t + n, p + n1);
    else for (int i = 0; i < n1; i++) sa[s1[i]] = i;
    for (int i = 0; i < n1; i++) s1[i] = p[sa[i]];
    induced_sort(s1);
}
/**
 * SA-IS Algorithm (Suffix Array & LCP Array)
 * Complexity: O(n)
 */
template <typename T>
void suffix_array(T * str, int n) {
    int m = str[n] = str[0];
    for (int i = 1; i < n; i++) if (str[i] < str[n]) str[n] = str[i];
    str[n++]--;
    for (int i = 1; i < n; i++) if (str[i] > m) m = str[i];
    for (int i = 0; i <= m; i++) rk[i] = 0;
    for (int i = 0; i < n; i++) rk[str[i]] = 1;
    for (int i = 0; i < m; i++) rk[i + 1] += rk[i];
    for (int i = 0; i < n; i++) s[i] = rk[str[i]] - 1;
    sais(n, rk[m], s, t, p);
    for (int i = 0; i < n; i++) rk[sa[i]] = i;
    for (int i = 0, h = lcp[0] = 0; i < n - 1; i++) {
        int j = sa[rk[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
        if (lcp[rk[i]] = h) h--;
    }
}
} // SA_IS

int wa[N], wb[N], wu[N], wv[N], rk[N];
/**
 * Makes LCP Array for s based on the Suffix Array of s.
 * n = len(s)
 */
void calc_height(int s[], int sa[], int ht[], int n) {
    int i, j, k = 0;
    for (i = 0; i <= n; i++) rk[sa[i]] = i;
    for (i = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++);
}
inline bool cmp(int s[], int a, int b, int l)
{ return s[a] == s[b] && s[a + l] == s[b + l]; }
/**
 * Makes Suffix Array for s.
 * Complexity: O(n log(n))
 * s[len(s)] = 0 (empty string)
 * n = len(s) + 1
 * m > max(s[i])
 */
void da(int s[], int sa[], int n, int m) {
    int i, j, p, * x = wa, * y = wb;
    for (i = 0; i < m; i++) wu[i] = 0;
    for (i = 0; i < n; i++) wu[x[i] = s[i]]++;
    for (i = 1; i < m; i++) wu[i] += wu[i - 1];
    for (i = n - 1; i >= 0; i--) sa[--wu[x[i]]] = i;
    for (j = 1, p = 1; p < n; j <<= 1, m = p) {
        for (p = 0, i = n - j; i < n; i++) y[p++] = i;
        for (i = 0; i < n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
        for (i = 0; i < n; i++) wv[i] = x[y[i]];
        for (i = 0; i < m; i++) wu[i] = 0;
        for (i = 0; i < n; i++) wu[wv[i]]++;
        for (i = 1; i < m; i++) wu[i] += wu[i - 1];
        for (i = n - 1; i >= 0; i--) sa[--wu[wv[i]]] = y[i];
        for (swap(x, y), p = 1, x[sa[0]] = 0, i = 1; i < n; i++)
            x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
    }
}
void radix(int s[], int a[], int b[], int n, int m) {
    for (int i = 0; i < n; i++) wv[i] = s[a[i]];
    for (int i = 0; i < m; i++) wu[i] = 0;
    for (int i = 0; i < n; i++) wu[wv[i]]++;
    for (int i = 1; i < m; i++) wu[i] += wu[i - 1];
    for (int i = n - 1; i >= 0; i--) b[--wu[wv[i]]] = a[i];
}
inline int F(int x, int tb) { return x / 3 + (x % 3 == 1 ? 0 : tb); }
inline int G(int x, int tb) { return x < tb ? x * 3 + 1 : (x - tb) * 3 + 2; }
inline int c0(int s[], int a, int b) { return s[a] == s[b] && s[a + 1] == s[b + 1] && s[a + 2] == s[b + 2]; }
inline int c12(int k, int s[], int a, int b) {
    if (k == 2) return s[a] < s[b] || s[a] == s[b] && c12(1, s, a + 1, b + 1);
    else return s[a] < s[b] || s[a] == s[b] && wv[a + 1] < wv[b + 1]; }
/**
 * Makes Suffix Array for s.
 * Complexity: O(n)
 * ATTENTION: Additional space needed!
 * Let N be the maximum size of the given array s.
 * The size of any array should be N * 3.
 * s[len(s)] = 0 (empty string)
 * n = len(s) + 1
 * m > max(s[i])
 */
void dc3(int s[], int sa[], int n, int m) {
    int i, j, * sn = s + n, * san = sa + n, ta = 0, tb = (n + 1) / 3, tbc = 0, p;
    s[n] = s[n + 1] = 0;
    for (i = 0; i < n; i++) if (i % 3) wa[tbc++] = i;
    radix(s + 2, wa, wb, tbc, m);
    radix(s + 1, wb, wa, tbc, m);
    radix(s, wa, wb, tbc, m);
    for (p = 1, sn[F(wb[0], tb)] = 0, i = 1; i < tbc; i++)
        sn[F(wb[i], tb)] = c0(s, wb[i - 1], wb[i]) ? p - 1 : p++;
    if (p < tbc) dc3(sn, san, tbc, p);
    else for (i = 0; i < tbc; i++) san[sn[i]] = i;
    for (i = 0; i < tbc; i++) if (san[i] < tb) wb[ta++] = san[i] * 3;
    if (n % 3 == 1) wb[ta++] = n - 1;
    radix(s, wb, wa, ta, m);
    for (i = 0; i < tbc; i++) wv[wb[i] = G(san[i], tb)] = i;
    for (i = 0, j = 0, p = 0; i < ta && j < tbc; p++)
        sa[p] = c12(wb[j] % 3, s, wa[i], wb[j]) ? wa[i++] : wb[j++];
    for ( ; i < ta; p++) sa[p] = wa[i++];
    for ( ; j < tbc; p++) sa[p] = wb[j++];
}

const int M = (int)log2(N) + 5;
int st[N][M];
void init_st(int a[], int n) {
    for (int i = 0; i < n; i++)
        st[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 0; i + (1 << j) - 1 < n; i++)
            st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
}
int query_min(int l, int r) {
    int k = (int)log2(r - l + 1);
    return min(st[l][k], st[r - (1 << k) + 1][k]);
}

typedef unsigned long long ull;

ull BKDR_hash(const char * str) {
	// seed can be 31, 131, 1313, 13131, 131313, ...
	const ull seed = 131313;
	ull hash = 0;
	while (*str) hash = hash * seed + (*str++);
	return hash;
}
ull b[N], h[N];
void init_hash(const char * str) {
    const ull seed = 131313;
    b[0] = 1; h[0] = 0;
    for (int i = 1; *str; i++) {
        b[i] = b[i - 1] * seed;
        h[i] = h[i - 1] * seed + (*str++);
    }
}
ull BKDR_hash(int l, int r)
{ return h[r + 1] - h[l] * b[r - l + 1]; }

struct DLX {
    const int M;
    int n, m, sz, n_ans;
    vector<int> U, D, L, R,
        col, row, H, S, ans;
    DLX(int _n, int _m)
    : M(_n * _m + _m + 5), n(_n), m(_m),
    sz(m), n_ans(-1), U(M), D(M), L(M), R(M),
    col(M), row(M), H(n + 1, -1), S(m + 1), ans(n) {
        for (int i = 0; i <= m; i++) {
            U[i] = D[i] = i;
            L[i] = i - 1;
            R[i] = i + 1; }
        L[0] = m; R[m] = 0;
    }
    void add(int r, int c) {
        ++S[col[++sz] = c]; row[sz] = r;
        U[sz] = c;          D[sz] = D[c];
        U[D[c]] = sz;       D[c] = sz;
        if (~H[r]) {
            L[sz] = H[r];    R[sz] = R[H[r]];
            L[R[H[r]]] = sz; R[H[r]] = sz;
        } else H[r] = L[sz] = R[sz] = sz;
    }
    void remove(int c) {
        L[R[c]] = L[c]; R[L[c]] = R[c];
        for (int i = D[c]; i != c; i = D[i])
            for (int j = R[i]; j != i; j = R[j]) {
                U[D[j]] = U[j]; D[U[j]] = D[j];
                --S[col[j]]; }
    }
    void restore(int c) {
        for (int i = U[c]; i != c; i = U[i])
            for (int j = L[i]; j != i; j = L[j])
                ++S[col[U[D[j]] = D[U[j]] = j]];
        L[R[c]] = R[L[c]] = c;
    }
    bool dance(int d = 0) {
        if (R[0] == 0) { n_ans = d; return true; }
        int c = R[0];
        for (int i = R[0]; i; i = R[i])
            if (S[i] < S[c]) c = i;
        remove(c);
        for (int i = D[c]; i != c; i = D[i]) {
            ans[d] = row[i];
            for (int j = R[i]; j != i; j = R[j])
                remove(col[j]);
            if (dance(d + 1))
                return true;
            for (int j = L[i]; j != i; j = L[j])
                restore(col[j]);
        } restore(c);
        return false;
    }
};

#endif // _MISC_H
