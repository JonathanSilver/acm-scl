#ifndef _EXTRA_H
#define _EXTRA_H

#include <iostream>
#include <cstring>
#include <climits>
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 10005;
typedef long long ll;

const int M = 15;
const int STATE0 = 0;
int a[N];
ll dp[N][M];
ll dfs(int pos, int state, bool lead, bool limit) {
	if (pos == -1) return 1;
	if (!limit && !lead && dp[pos][state] != -1) return dp[pos][state];
	int up = limit ? a[pos] : 9; ll ans = 0;
	for (int i = 0; i <= up; i++)
		ans += dfs(pos - 1, state,
			       lead && i == 0, limit && i == a[pos]);
	if (!limit && !lead) dp[pos][state] = ans;
	return ans;
}
ll solve(ll x) {
	int pos = 0;
	while (x) { a[pos++] = x % 10; x /= 10; }
	return dfs(pos - 1, STATE0, true, true);
}

namespace LIS {

const int INF = 0x3f3f3f3f;

int dp[N], pos[N], a[N], n;
int LIS() {
    int len = 1;
    dp[1] = a[1];
    pos[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (a[i] >= dp[len]) { // without "=" if strictly increasing
            dp[++len] = a[i];
            pos[i] = len;
        } else {
            // lower_bound if strictly increasing
            int idx = upper_bound(dp + 1, dp + len + 1, a[i]) - dp;
            dp[idx] = a[i];
            pos[i] = idx;
        }
    }
    int mx = INF, tmp = len;
    for (int i = n; tmp && i >= 1; i--) {
        if (pos[i] == tmp && mx >= a[i]) { // without "=" if strictly increasing
            mx = a[i];
            tmp--;
        }
    }
    return len;
}

}

namespace CDQ {

void update(int i, int x) { }
int query(int i) { return 0; }

struct data {
    int a, b, c, w, f;
    bool operator==(const data & d) const {
        return a == d.a && b == d.b && c == d.c;
    }
    bool operator<(const data & d) const {
        if (a != d.a) return a < d.a;
        if (b != d.b) return b < d.b;
        return c < d.c;
    }
} d[N], t[N];
// no duplicated values
// sort by a first
// cdq(0, cnt - 1);
void cdq(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    cdq(l, mid); cdq(mid + 1, r);
    int p = l, q = mid + 1, cnt = l;
    while (p <= mid && q <= r) {
        if (d[p].b <= d[q].b) {
            update(d[p].c, d[p].w);
            t[cnt++] = d[p++];
        } else {
            d[q].f += query(d[q].c);
            t[cnt++] = d[q++];
        }
    }
    while (p <= mid) {
        update(d[p].c, d[p].w);
        t[cnt++] = d[p++];
    }
    while (q <= r) {
        d[q].f += query(d[q].c);
        t[cnt++] = d[q++];
    }
    for (int i = l; i <= mid; i++) update(d[i].c, -d[i].w);
    for (int i = l; i <= r; i++) d[i] = t[i];
}

} // CDQ

typedef long long ll;

namespace MO {

struct query { int l, r, t, id; } q[N];
void add(int i) { }
void sub(int i) { }
void upd(int i, int t) { }

ll res[N], ans;
/**
 * Mo's Algorithm
 * Complexity: O(n^(3/2))
 */
void MO_2(int n, int m) {
    const int block = sqrt(n);
    sort(q, q + m, [&] (const query & a, const query & b) {
        return a.l / block == b.l / block ? a.r < b.r : a.l < b.l;
    });
    for (int i = 0, l = 1, r = 0; i < m; i++) {
        while (l < q[i].l) sub(l++);
        while (l > q[i].l) add(--l);
        while (r < q[i].r) add(++r);
        while (r > q[i].r) sub(r--);
        res[q[i].id] = ans;
    }
}
/**
 * Mo's Algorithm
 * Complexity: O(n^(5/3))
 */
void MO_3(int n, int m) {
    const int block = pow(n, 2.0 / 3.0);
    sort(q, q + m, [&] (const query & a, const query & b) {
        if (a.l / block != b.l / block) return a.l < b.l;
        if (a.r / block != b.r / block) return a.r < b.r;
        return a.t < b.t;
    });
    for (int l = 0, r = -1, t = 0, i = 0; i < m; i++) {
        while (l < q[i].l) sub(l++);
        while (l > q[i].l) add(--l);
        while (r > q[i].r) sub(r--);
        while (r < q[i].r) add(++r);
        while (t < q[i].t) upd(i, t++);
        while (t > q[i].t) upd(i, --t);
        res[q[i].id] = ans;
    }
}

} // MO

namespace MO_on_tree {

struct edge {int to, next;} e[N * 2];
int head[N], cnt, col[N], num[N];
void add_edge(int u, int v) {
    e[cnt] = {v, head[u]};
    head[u] = cnt++;
}

int lca(int u, int v) { return 0; }

struct query {
    int u, v, id, lca, l, r;
} q[N];
int s[N], t[N], vs[N * 2], vis[N], res[N], ans;
void _add(int u) {
    num[col[u]]++;
    if (num[col[u]] == 1) ans++;
}
void _sub(int u) {
    num[col[u]]--;
    if (num[col[u]] == 0) ans--;
}
void add(int u) {
    if (vis[u]) _sub(u);
    else _add(u);
    vis[u] ^= 1;
}
void dfs(int u, int p, int & k) {
    s[u] = ++k; vs[k] = u;
    for (int i = head[u]; ~i; i = e[i].next) {
        int v = e[i].to;
        if (v == p) continue;
        dfs(v, u, k);
    }
    t[u] = ++k; vs[k] = u;
}
void MO_2_on_tree(int m) {
    int k = 0;
    dfs(1, -1, k);
    for (int i = 0; i < m; i++) {
        if (s[q[i].u] > s[q[i].v]) swap(q[i].u, q[i].v);
        q[i].id = i;
        q[i].lca = lca(q[i].u, q[i].v);
        if (q[i].lca == q[i].u) {
            q[i].l = s[q[i].u];
            q[i].r = s[q[i].v];
            q[i].lca = -1;
        } else {
            q[i].l = t[q[i].u];
            q[i].r = s[q[i].v];
        }
    }
    const int block = sqrt(k);
    sort(q, q + m, [&] (const query & a, const query & b) {
        return a.l / block == b.l / block ? a.r < b.r : a.l < b.l;
    });
    for (int i = 0, l = 1, r = 0; i < m; i++) {
        while (l < q[i].l) add(vs[l++]);
        while (l > q[i].l) add(vs[--l]);
        while (r < q[i].r) add(vs[++r]);
        while (r > q[i].r) add(vs[r--]);
        if (~q[i].lca) add(q[i].lca);
        res[q[i].id] = ans;
        if (~q[i].lca) add(q[i].lca);
    }
}

} // MO_on_tree

namespace divide_and_conquer_fft {

const int g = 3;
const int p = 998244353;
void NTT(ll a[], int n, int op) { }

ll a[N], b[N], c[N], f[N];
/**
 * Given g[1], ..., g[n - 1],
 * find f[0], ..., f[n - 1],
 * where f[i] = sum(f[i - j] * g[j], j = 1..i),
 * and f[0] = 1.
 * f[0] = 1; solve(0, n - 1);
 * Complexity: O(n log^2 n)
 */
void solve(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    solve(l, mid);
    int sz = 1;
    while (sz <= (mid + r - 2 * l - 1)) sz <<= 1;
    for (int i = l; i <= mid; i++) a[i - l] = f[i];
    for (int i = mid - l + 1; i < sz; i++) a[i] = 0;
    for (int i = 1; i <= r - l; i++) b[i - 1] = c[i];
    for (int i = r - l; i < sz; i++) b[i] = 0;
    NTT(a, sz, 1); NTT(b, sz, 1);
    for (int i = 0; i < sz; i++) a[i] = a[i] * b[i] % p;
    NTT(a, sz, -1);
    for (int i = mid + 1; i <= r; i++) f[i] = (f[i] + a[i - l - 1]) % p;
    solve(mid + 1, r);
}

} // divide_and_conquer_fft

#endif // _EXTRA_H
