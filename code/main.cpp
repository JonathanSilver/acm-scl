#include <bits/stdc++.h>
#include <bits/extc++.h>
//#include <iostream>
//#include <cstring>
//#include <cstdio>
//#include <cmath>
//#include <cstdlib>
//#include <cctype>
//#include <bitset>
//#include <complex>
//#include <functional>
//#include <algorithm>
//#include <string>
//#include <vector>
//#include <queue>
//#include <stack>
//#include <list>
//#include <map>
//#include <set>
//#include <unordered_map>
//#include <unordered_set>
//#include <tuple>
//#include <ratio>
//#include <chrono>
//#include <random>
//#include <ext/rope>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace chrono;
using namespace __gnu_cxx;
using namespace __gnu_pbds;
const int N = 1000005;
const int INF = 0x3f3f3f3f;
const double PI = acos(-1);
const double eps = 1e-8;
#define ms(x, y) memset((x), (y), sizeof(x))
#define mc(x, y) memcpy((x), (y), sizeof(y))
typedef long long ll;
typedef unsigned long long ull;
#define fi first
#define se second
#define mp make_pair
typedef pair<int, int> pii;
typedef pair<ll, int> pli;
#define bg begin
#define ed end
#define pb push_back
#define al(x) (x).bg(), (x).ed()
#define st(x) sort(al(x))
#define un(x) (x).erase(unique(al(x)), (x).ed())
#define fd(x, y) (lower_bound(al(x), (y)) - (x).bg() + 1)
#define ls(x) ((x) << 1)
#define rs(x) (ls(x) | 1)
/// int order_of_key(T);
/// iterator find_by_order(int);
template <typename T>
using rbtree = tree<T, null_type, less<T>, rb_tree_tag,
                    tree_order_statistics_node_update>;
/// point_iterator push(T);
/// void modify(point_iterator, T);
template <typename T>
using pheap = __gnu_pbds::priority_queue<T, greater<T>, pairing_heap_tag>;
template <class T>
bool read_int(T & x) {
    char c;
    while (!isdigit(c = getchar()) && c != '-' && c != EOF);
    if (c == EOF) return false;
    T flag = 1;
    if (c == '-') { flag = -1; x = 0; } else x = c - '0';
    while (isdigit(c = getchar())) x = x * 10 + c - '0';
    x *= flag;
    return true;
}
template <class T, class ...R>
bool read_int(T & a, R & ...b) {
    if (!read_int(a)) return false;
    return read_int(b...);
}
mt19937 gen(steady_clock::now().time_since_epoch().count());
int main()
{
    time_point<steady_clock> start = steady_clock::now();

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cerr.tie(nullptr);

//    int size = 256 << 20; // 256 M
//    char * p = (char *)malloc(size) + size;
//    #if (defined _WIN64) or (defined __unix)
//        __asm__("movq %0, %%rsp\n" :: "r"(p));
//    #else
//        __asm__("movl %0, %%esp\n" :: "r"(p));
//    #endif

    int n;
    while (~scanf("%d", &n)) {
    }

//    int T, n;
//    scanf("%d", &T);
//    while (T--) {
//        scanf("%d", &n);
//    }

    cerr << endl << "------------------------------" << endl << "Time: "
         << duration<double, milli>(steady_clock::now() - start).count()
         << " ms." << endl;

//    exit(0);

    return 0;
}
