#ifndef _GRAPH_H
#define _GRAPH_H

#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

const int N = 1000;
const int INF = 0x3f3f3f3f;

template <class T, class W>
struct edge
{
    T from;
    T to;
    W weight;

    bool operator<(const edge & e) const { return weight < e.weight; }
    bool operator>(const edge & e) const { return weight > e.weight; }
};

template <class T, class W>
struct vertex
{
    T id;
    W data;
    bool flag;
    vector<edge<T, W> > edges;

    bool operator<(const vertex<T, W> & v) const { return data < v.data; }
    bool operator>(const vertex<T, W> & v) const { return data > v.data; }
};

template <class T>
struct ptr_cmp
{
    bool operator()(const T * a, const T * b) const { return *a > *b; }
};

template <class T, class W>
struct graph : public map<T, vertex<T, W> > { };

/**
 * The Bellman-Ford algorithm - Single Source Shortest Path
 * Complexity: O(VE)
 */
template <typename T, typename W>
bool bellman_ford(graph<T, W> & g, vector<edge<T, W> > & all_edges, T source)
{
    for (typename graph<T, W>::iterator i = g.begin(); i != g.end(); i++)
        i->second.data = INF;
    g[source].data = 0;
    for (int j = 0; j < g.size() - 1; j++)
        for (typename vector<edge<T, W> >::iterator i = all_edges.begin(); i != all_edges.end(); i++)
            if (g[i->to].data > g[i->from].data + i->weight)
                g[i->to].data = g[i->from].data + i->weight;
    for (typename vector<edge<T, W> >::iterator i = all_edges.begin(); i != all_edges.end(); i++)
        if (g[i->to].data > g[i->from].data + i->weight)
            return false;
    return true;
}

/**
 * Dijkstra's algorithm - Single Source Shortest Path
 * Complexity: O(E log(V))
 */
template <typename T, typename W>
void dijkstra(graph<T, W> & g, T source)
{
    for (typename graph<T, W>::iterator i = g.begin(); i != g.end(); i++)
        i->second.data = INF;
    g[source].data = 0;
    priority_queue<vertex<T, W> *, vector<vertex<T, W> *>, ptr_cmp<vertex<T, W> > > q;
    q.push(&g[source]);
    while (!q.empty())
    {
        vertex<T, W> * p = q.top();
        q.pop();
        for (typename vector<edge<T, W> >::iterator i = p->edges.begin(); i != p->edges.end(); i++)
        {
            if (g[i->to].data > p->data + i->weight)
            {
                g[i->to].data = p->data + i->weight;
                q.push(&g[i->to]);
            }
        }
    }
}

/**
 * Prim's algorithm - Minimum Spanning Tree
 * Complexity: O(E log(V))
 */
template <typename T, typename W>
W prim(graph<T, W> & g)
{
    map<T, bool> visited;
    for (typename graph<T, W>::iterator i = g.begin(); i != g.end(); i++)
    {
        i->second.data = INF;
        i->second.flag = false;
        visited[i->first] = false;
    }
    W w = 0;
    g.begin()->second.data = 0;
    vector<vertex<T, W> *> v;
    v.push_back(&g.begin()->second);
    g.begin()->second.flag = true;
    auto cmp = [] (const vertex<T, W> * a, const vertex<T, W> * b) -> bool { return *a > *b; };
    while (!v.empty())
    {
        pop_heap(v.begin(), v.end(), cmp);
        vertex<T, W> * p = v.back();
        v.pop_back();
        w += p->data;
        p->flag = false;
        visited[p->id] = true;
        for (typename vector<edge<T, W> >::iterator i = p->edges.begin(); i != p->edges.end(); i++)
        {
            if (!visited[i->to] && i->weight < g[i->to].data)
            {
                g[i->to].data = i->weight;
                if (g[i->to].flag)
                    make_heap(v.begin(), v.end(), cmp);
                else
                {
                    v.push_back(&g[i->to]);
                    push_heap(v.begin(), v.end(), cmp);
                    g[i->to].flag = true;
                }
            }
        }
    }
    return w;
}

template <class T>
T union_find(map<T, T> & m, T x)
{ return m[x] == x ? x : m[x] = union_find(m, m[x]); }

template <class T>
bool union_join(map<T, T> & m, T x, T y)
{
    T a = union_find(m, x), b = union_find(m, y);
    if (a == b) return false;
    m[a] = b;
    return true;
}

/**
 * Kruskal's algorithm - Minimum Spanning Tree
 * Complexity: O(E log(V))
 */
template <typename T, typename W>
void kruskal(vector<T> & vertex_ids, vector<edge<T, W> > & all_edges, vector<edge<T, W> > & mst)
{
    mst.clear();
    map<T, T> m;
    for (typename vector<T>::iterator i = vertex_ids.begin(); i != vertex_ids.end(); i++)
        m[*i] = *i;
    priority_queue<edge<T, W> *, vector<edge<T, W> *>, ptr_cmp<edge<T, W> > > q;
    for (typename vector<edge<T, W> >::iterator i = all_edges.begin(); i != all_edges.end(); i++)
        q.push(&*i);
    while (!q.empty())
    {
        edge<T, W> * p = q.top();
        q.pop();
        if (union_join(m, p->from, p->to))
            mst.push_back(*p);
    }
}

int edges[N][N], pre[N];

/**
 * Edmonds-Karp algorithm - Maximum Flow
 * Complexity: O(V E^2)
 */
int edmonds_karp(int s, int t, int n)
{
    int max_flow = 0;
    while (true)
    {
        memset(pre, -1, sizeof(pre));
        pre[s] = s;
        queue<int> q;
        q.push(s);
        bool flag = false;
        while (!q.empty())
        {
            int p = q.front();
            q.pop();
            if (p == t)
            {
                flag = true;
                int d = INF;
                for (int i = t; i != s; i = pre[i])
                    d = min(d, edges[pre[i]][i]);
                for (int i = t; i != s; i = pre[i])
                {
                    edges[pre[i]][i] -= d;
                    edges[i][pre[i]] += d;
                }
                max_flow += d;
                break;
            }
            for (int i = 0; i < n; i++)
            {
                if (edges[p][i] > 0 && pre[i] == -1)
                {
                    pre[i] = p;
                    q.push(i);
                }
            }
        }
        if (!flag) break;
    }
    return max_flow;
}

struct edge_link
{
    int to, cap, cost, next;
    bool used;
} e[N << 4];
/**
 * head must be preset to -1.
 * cnt must be preset to 0.
 */
int depth[N], head[N], cnt = 0;
void add(int u, int v, int cap = 0, int cost = 0)
{
    e[cnt] = (edge_link){v, cap, cost, head[u], false};
    head[u] = cnt++;
}

bool visited[N];

bool dinic_bfs(int s, int t)
{
    queue<int> q;
    memset(depth, 0, sizeof(depth));
    q.push(s);
    depth[s] = 1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; ~i; i = e[i].next)
        {
            if (!depth[e[i].to] && e[i].cap)
            {
                depth[e[i].to] = depth[u] + 1;
                q.push(e[i].to);
            }
        }
    }
    return depth[t];
}
int dinic_dfs(int u, int t, int c)
{
    int tmp = c;
    visited[u] = true;
    if (u == t) return c;
    for (int i = head[u]; tmp && ~i; i = e[i].next)
    {
        if (!visited[e[i].to] && depth[u] + 1 == depth[e[i].to] && e[i].cap)
        {
            int d = dinic_dfs(e[i].to, t, min(tmp, e[i].cap));
            e[i].cap -= d;
            e[i ^ 1].cap += d;
            tmp -= d;
        }
    }
    return c - tmp;
}
/**
 * Dinic's algorithm - Maximum Flow
 * Complexity: O(V^2 E)
 */
int dinic(int s, int t)
{
    int max_flow = 0, tmp;
    while (dinic_bfs(s, t))
    {
        memset(visited, 0, sizeof(visited));
        while (tmp = dinic_dfs(s, t, INF))
        {
            max_flow += tmp;
            memset(visited, 0, sizeof(visited));
        }
    }
    return max_flow;
}

int pre_v[N], pre_e[N], dist[N];
int min_cost_max_flow(int s, int t)
{
    int min_cost = 0;
    while (true)
    {
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > q;
        memset(dist, INF, sizeof(dist));
        dist[s] = 0;
        q.push(make_pair(0, s));
        while (!q.empty())
        {
            pair<int, int> p = q.top(); q.pop();
            int u = p.second;
            if (dist[u] < p.first) continue;
            for (int i = head[u]; ~i; i = e[i].next)
            {
                if (e[i].cap && dist[e[i].to] > dist[u] + e[i].cost)
                {
                    dist[e[i].to] = dist[u] + e[i].cost;
                    pre_v[e[i].to] = u;
                    pre_e[e[i].to] = i;
                    q.push(make_pair(dist[e[i].to], e[i].to));
                }
            }
        }
        if (dist[t] == INF)
            break;
        int delta = INF;
        for (int u = t; u != s; u = pre_v[u])
            delta = min(delta, e[pre_e[u]].cap);
        for (int u = t; u != s; u = pre_v[u])
        {
            e[pre_e[u]].cap -= delta;
            e[pre_e[u] ^ 1].cap += delta;
        }
        min_cost += dist[t] * delta;
    }
    return min_cost;
}

int group[N];

bool isap_bfs(int s, int t)
{
    memset(depth, -1, sizeof(depth));
    memset(group, 0, sizeof(group));
    group[depth[t] = 0]++;
    queue<int> q;
    q.push(t);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; ~i; i = e[i].next)
        {
            int v = e[i].to;
            if (~depth[v]) continue;
            group[depth[v] = depth[u] + 1]++;
            q.push(v);
        }
    }
    return ~depth[s];
}
int isap_dfs(int u, int t, int c)
{
    if (u == t) return c;
    int tmp = c;
    for (int i = head[u]; tmp && ~i; i = e[i].next)
    {
        int v = e[i].to;
        if (depth[v] + 1 == depth[u] && e[i].cap)
        {
            int d = isap_dfs(v, t, min(tmp, e[i].cap));
            e[i].cap -= d;
            e[i ^ 1].cap += d;
            tmp -= d;
        }
    }
    if (tmp)
    {
        if (!--group[depth[u]])
            depth[t] = -1;
        group[++depth[u]]++;
    }
    return c - tmp;
}
/**
 * ISAP (Improved Shortest Augmenting Path) - Maximum Flow
 * Complexity: O(V^2 E)
 */
int isap(int s, int t)
{
    if (!isap_bfs(s, t)) return 0;
    int max_flow = 0;
    while (~depth[t])
        max_flow += isap_dfs(s, t, INF);
    return max_flow;
}

int match[N];

/**
 * The Hungarian method
 * A combinatorial optimization algorithm that solves
 * the assignment problem in polynomial time.
 * Complexity: O(n^3)
 */
bool hungary_find(int x, int n2)
{
    for (int i = 0; i < n2; i++)
    {
        if (edges[x][i] && !visited[i])
        {
            visited[i] = true;
            if (match[i] == -1 || hungary_find(match[i], n2))
            {
                match[i] = x;
                return true;
            }
        }
    }
    return false;
}
int hungary(int n1, int n2)
{
    memset(match, -1, sizeof(match));
    int r = 0;
    for (int i = 0; i < n1; i++)
    {
        memset(visited, 0, sizeof(visited));
        if (hungary_find(i, n2))
            r++;
    }
    return r;
}

/**
 * The Floyd-Warshall algorithm - All Pairs Shortest Path
 * Complexity: O(n^3)
 */
void floyd_warshall(int n)
{
    for (int k = 0; k < n; k++)
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        if (edges[i][k] + edges[k][j] < edges[i][j])
            edges[i][j] = edges[i][k] + edges[k][j];
}

int dfs_stack[N], top = 0;
bool in_stack[N];
int dfn[N], low[N];
int component_number = 0, idx = 0;
int component_idx[N];
vector<int> adj[N];
vector<int> component[N];

/**
 * Tarjan's algorithm - Strongly Connected Components
 * In the mathematical theory of Directed Graphs, a graph
 * is said to be Strongly Connected or Diconnected
 * if every vertex is reachable from every other vertex.
 * The Strongly Connected Components or Diconnected Components
 * of an arbitrary directed graph form a partition into subgraphs
 * that are themselves strongly connected.
 * Complexity: O(V + E)
 */
void tarjan(int i)
{
    int j;
    dfn[i] = low[i] = idx++;
    in_stack[i] = true;
    dfs_stack[++top] = i;
    for (vector<int>::iterator e = adj[i].begin(); e != adj[i].end(); e++)
    {
        j = *e;
        if (dfn[j] == -1)
        {
            tarjan(j);
            low[i] = min(low[i], low[j]);
        }
        else if (in_stack[j])
            low[i] = min(low[i], dfn[j]);
    }
    if (dfn[i] == low[i])
    {
        component_number++;
        do {
            j = dfs_stack[top--];
            in_stack[j] = false;
            component[component_number].push_back(j);
            component_idx[j] = component_number;
        } while (j != i);
    }
}

set<int> cut_vertex;
vector<pair<int, int> > bridge;

/**
 * Tarjan's algorithm - Biconnected Component
 * In graph theory, a Biconnected Component is
 * a maximal biconnected subgraph.
 * Any connected graph decomposes into a tree of biconnected
 * components called the Block-Cut Tree of the graph.
 * The blocks are attached to each other at shared vertices
 * called Cut Vertices or Articulation Points. Specifically,
 * a Cut Vertex is any vertex whose removal increases the
 * number of connected components.
 * Complexity: O(V + E)
 */
void tarjan(int u, int pre)
{
    int children = 0;
    dfn[u] = low[u] = idx++;
    for (int i = head[u]; ~i; i = e[i].next)
    {
        if ((i ^ 1) == pre) continue;
        int v = e[i].to;
        if (dfn[v] == -1)
        {
            children++;
            tarjan(v, i);
            low[u] = min(low[u], low[v]);
            if ((pre == -1 && children > 1) || (pre != -1 && low[v] >= dfn[u]))
                cut_vertex.insert(u);
            if (low[v] > dfn[u])
                bridge.push_back(make_pair(u, v));
        }
        else if (dfn[v] < dfn[u])
            low[u] = min(low[u], dfn[v]);
    }
}

const int M = 1000; // The number of queries.
// lca_ans should be preset -1.
int father[N], lca_root[N], lca_ans[M];
struct lca_query
{ int id, x; };
vector<lca_query> lca_queries[N];
int find(int x)
{
    if (father[x] != x)
        father[x] = find(father[x]);
    return father[x];
}
/**
 * Lowest Common Ancestor (LCA)
 */
void lca_tarjan(int x, int r)
{
    lca_root[x] = r;
    for (vector<int>::iterator i = adj[x].begin(); i != adj[x].end(); i++)
    {
        if (lca_root[*i] == -1)
        {
            lca_tarjan(*i, r);
            father[*i] = x;
        }
    }
    for (vector<lca_query>::iterator i = lca_queries[x].begin(); i != lca_queries[x].end(); i++)
        if (lca_root[i->x] == r)
            lca_ans[i->id] = find(i->x);
}

const int C = (int)log2(N) + 5;
int anc[N][C + 1];
// Set pre[init] = init;
// Set depth[init] = 0;
// Before calling lca_dfs(init);
void lca_dfs(int x)
{
    anc[x][0] = pre[x];
    for (int i = 1; i <= C; i++)
        anc[x][i] = anc[anc[x][i - 1]][i - 1];
    for (vector<int>::iterator i = adj[x].begin(); i != adj[x].end(); i++)
        if (*i != pre[x])
        {
            pre[*i] = x;
            depth[*i] = depth[x] + 1;
            lca_dfs(*i);
        }
}
int lca(int x, int y)
{
    if (depth[x] < depth[y]) swap(x, y);
    for (int i = C; i >= 0; i--)
        if (depth[y] <= depth[anc[x][i]])
            x = anc[x][i];
    if (x == y) return x;
    for (int i = C; i >= 0; i--)
        if (anc[x][i] != anc[y][i])
        {
            x = anc[x][i];
            y = anc[y][i];
        }
    return anc[x][0] == anc[y][0] ? anc[x][0] : -1;
}

/**
 * A Topological Sort or Topological Ordering of a directed graph
 * is a linear ordering of its vertices such that for every
 * directed edge (u, v) from vertex u to vertex v, u comes before
 * v in the ordering.
 * A topological ordering is possible if and only if the graph
 * has no directed cycles, that is a Directed Acyclic Graph (DAG).
 */
bool topological_sort(int n, vector<int> & v)
{
    vector<int> degree(n);
    for (int i = 0; i < n; i++)
        for (vector<int>::iterator j = adj[i].begin(); j != adj[i].end(); j++)
            degree[*j]++;
    priority_queue<int, vector<int>, greater<int> > q;
    for (int i = 0; i < n; i++)
        if (!degree[i])
            q.push(i);
    v.clear();
    while (!q.empty())
    {
        int i = q.top();
        q.pop();
        v.push_back(i);
        for (vector<int>::iterator j = adj[i].begin(); j != adj[i].end(); j++)
        {
            degree[*j]--;
            if (!degree[*j])
                q.push(*j);
        }
    }
    return v.size() == n;
}

vector<int> vs;
void bcc_dfs_1(int u)
{
    visited[u] = true;
    vs.push_back(u);
    for (int i = head[u]; ~i; i = e[i].next)
        if (!visited[e[i].to])
        {
            e[i].used = true;
            bcc_dfs_1(e[i].to);
        }
}
void bcc_dfs_2(int u, int k)
{
    visited[u] = true;
    component_idx[u] = k;
    for (int i = head[u]; ~i; i = e[i].next)
        if (!visited[e[i].to] && !e[i].used)
            bcc_dfs_2(e[i].to, k);
}
/**
 * Kosaraju's algorithm - Biconnected Components
 * Biconnected Component: There are no Bridges in the graph.
 */
void bcc(int n)
{
    memset(visited, 0, sizeof(visited));
    vs.clear();
    for (int i = 0; i < n; i++)
        if (!visited[i]) bcc_dfs_1(i);
    memset(visited, 0, sizeof(visited));
    int k = 0;
    for (int i = 0; i < vs.size(); i++)
        if (!visited[vs[i]]) bcc_dfs_2(vs[i], ++k);
}

vector<int> G[N];
vector<int> rG[N];
void scc_dfs(int u)
{
    visited[u] = true;
    for (int i = 0; i < G[u].size(); i++)
        if (!visited[G[u][i]]) scc_dfs(G[u][i]);
    vs.push_back(u);
}
void scc_rdfs(int u, int k)
{
    visited[u] = true;
    component_idx[u] = k;
    for (int i = 0; i < rG[u].size(); i++)
        if (!visited[rG[u][i]]) scc_rdfs(rG[u][i], k);
}
/**
 * Kosaraju's algorithm - Strongly Connected Components
 */
int scc(int n)
{
    memset(visited, 0, sizeof(visited));
    vs.clear();
    for (int i = 0; i < n; i++)
        if (!visited[i]) scc_dfs(i);
    memset(visited, 0, sizeof(visited));
    int k = 0;
    for (int i = vs.size() - 1; i >= 0; i--)
        if (!visited[vs[i]]) scc_rdfs(vs[i], k++);
    return k;
}

namespace graph_ext {

const int M = log2(N) + 5;
vector<int> G[N];
int id[N], vs[N * 2 - 1], dep[N * 2 - 1], st[N * 2 - 1][M];
void rmq_lca_dfs(int d, int u, int p, int & k) {
    id[u] = k; vs[k] = u; dep[k++] = d;
    for (int i = 0; i < G[u].size(); i++) {
        if (G[u][i] != p) {
            rmq_lca_dfs(d + 1, G[u][i], u, k);
            vs[k] = u; dep[k++] = d;
        }
    }
}
void init_rmq_lca(int root) {
    int k = 0; rmq_lca_dfs(0, root, -1, k);
    for (int i = 0; i < k; i++) st[i][0] = i;
    for (int j = 1; (1 << j) <= k; j++)
        for (int i = 0; i + (1 << j) - 1 < k; i++)
            if (dep[st[i][j - 1]] <= dep[st[i + (1 << (j - 1))][j - 1]])
                st[i][j] = st[i][j - 1];
            else st[i][j] = st[i + (1 << (j - 1))][j - 1];
}
int rmq_lca_query(int l, int r) {
    int k = log2(r - l + 1);
    if (dep[st[l][k]] <= dep[st[r - (1 << k) + 1][k]])
        return st[l][k];
    else return st[r - (1 << k) + 1][k];
}
int rmq_lca(int u, int v) {
    return vs[rmq_lca_query(min(id[u], id[v]), max(id[u], id[v]))];
}

const int INF = 0x3f3f3f3f;
typedef pair<int, int> pii;
struct edge { int to, cap, cost, rev; };
vector<edge> g[N];
int dist[N], h[N], prev_v[N], prev_e[N];
void add_edge(int u, int v, int cap, int cost) {
    g[u].push_back((edge){v, cap, cost, g[v].size()});
    g[v].push_back((edge){u, 0, -cost, g[u].size() - 1});
}
int min_cost_max_flow(int s, int t, int f, int n) {
    fill(h, h + n, 0);
    int res = 0;
    while (f) {
        fill(dist, dist + n, INF);
        priority_queue<pii, vector<pii>, greater<pii> > q;
        dist[s] = 0;
        q.push(make_pair(0, s));
        while (!q.empty()) {
            pii p = q.top();
            q.pop();
            int u = p.second;
            if (dist[u] < p.first) continue;
            for (int i = 0; i < g[u].size(); i++) {
                edge & e = g[u][i];
                if (e.cap && dist[e.to] > dist[u] + e.cost + h[u] - h[e.to]) {
                    dist[e.to] = dist[u] + e.cost + h[u] - h[e.to];
                    prev_v[e.to] = u;
                    prev_e[e.to] = i;
                    q.push(make_pair(dist[e.to], e.to));
                }
            }
        }
        if (dist[t] == INF) break;
        for (int u = 0; u < n; u++) h[u] += dist[u];
        int d = f;
        for (int u = t; u != s; u = prev_v[u])
            d = min(d, g[prev_v[u]][prev_e[u]].cap);
        f -= d;
        res += d * h[t];
        for (int u = t; u != s; u = prev_v[u]) {
            edge & e = g[prev_v[u]][prev_e[u]];
            e.cap -= d;
            g[u][e.rev].cap += d;
        }
    }
    return res;
}

} // graph_ext

namespace KM_BFS {
const int N = 305;
const int INF = 0x3f3f3f3f;
/**
 * KM Algorithm (Maximum weighted bipartite matching)
 * Complexity: O(n^3)
 */
int w[N][N]; // INPUT: weights for all edges
int linker[N]; // OUTPUT: matches
int pre[N];
int lx[N], ly[N], slack[N];
bool vis_y[N];
void km_bfs(int u, int n) {
    for (int i = 0; i <= n; i++) {
        pre[i] = 0;
        slack[i] = INF;
        vis_y[i] = false;
    }
    int y = 0, yy = 0;
    linker[y] = u;
    while (true) {
        int x = linker[y];
        int d = INF;
        vis_y[y] = true;
        for (int i = 1; i <= n; i++) {
            if (!vis_y[i]) {
                if (slack[i] > lx[x] + ly[i] - w[x][i]) {
                    slack[i] = lx[x] + ly[i] - w[x][i];
                    pre[i] = y;
                }
                if (slack[i] < d) {
                    d = slack[i];
                    yy = i;
                }
            }
        }
        for (int i = 0; i <= n; i++) {
            if (vis_y[i]) {
                lx[linker[i]] -= d;
                ly[i] += d;
            } else slack[i] -= d;
        }
        y = yy;
        if (!linker[y]) break;
    }
    while (y) {
        linker[y] = linker[pre[y]];
        y = pre[y];
    }
}
int km(int n) {
    for (int i = 0; i <= n; i++)
        linker[i] = lx[i] = ly[i] = 0;
    for (int i = 1; i <= n; i++) km_bfs(i, n);
    int r = 0;
    for (int i = 1; i <= n; i++) r += w[linker[i]][i];
    return r;
}
}

namespace KM_DFS {

typedef long long ll;

const ll INF = 0x3f3f3f3f3f3f3f3f;
ll w[N][N], x[N], y[N], slack[N];
int prev_x[N], prev_y[N], son_y[N], par[N];
void adjust(int v) {
    son_y[v] = prev_y[v];
    if (prev_x[son_y[v]] != -2)
        adjust(prev_x[son_y[v]]);
}
bool find(int v, int n) {
    for (int i = 0; i < n; i++) {
        if (prev_y[i] == -1) {
            if (slack[i] > x[v] + y[i] - w[v][i]) {
                slack[i] = x[v] + y[i] - w[v][i];
                par[i] = v;
            }
            if (x[v] + y[i] == w[v][i]) {
                prev_y[i] = v;
                if (son_y[i] == -1) {
                    adjust(i);
                    return true;
                }
                if (prev_x[son_y[i]] != -1) continue;
                prev_x[son_y[i]] = i;
                if (find(son_y[i], n)) return true;
            }
        }
    }
    return false;
}
ll km(int n) {
    for (int i = 0; i < n; i++) {
        son_y[i] = -1;
        y[i] = 0;
        x[i] = w[i][0];
        for (int j = 1; j < n; j++)
            x[i] = max(x[i], w[i][j]);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            prev_x[j] = prev_y[j] = -1;
            slack[j] = INF;
        }
        prev_x[i] = -2;
        if (find(i, n)) continue;
        bool flag = false;
        while (!flag) {
            ll m = INF;
            for (int j = 0; j < n; j++)
                if (prev_y[j] == -1) m = min(m, slack[j]);
            for (int j = 0; j < n; j++) {
                if (prev_x[j] != -1) x[j] -= m;
                if (prev_y[j] != -1) y[j] += m;
                else slack[j] -= m;
            }
            for (int j = 0; j < n; j++) {
                if (prev_y[j] == -1 && !slack[j]) {
                    prev_y[j] = par[j];
                    if (son_y[j] == -1) {
                        adjust(j);
                        flag = true;
                        break;
                    }
                    prev_x[son_y[j]] = j;
                    if (find(son_y[j], n)) {
                        flag = true;
                        break;
                    }
                }
            }
        }
    }
    ll res = 0;
    for (int i = 0; i < n; i++) res += w[son_y[i]][i];
    return res;
}
}

#endif // _GRAPH_H
