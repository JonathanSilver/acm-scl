#include <bits/stdc++.h>
using namespace std;
const int N = 1000005;
template <class T>
double romberg(const T & f, double a, double b, double eps = 1e-8) {
    vector<double> t;
    double h = b - a, last, cur;
    int k = 1, i = 1;
    t.push_back(h * (f(a) + f(b)) / 2);
    do {
        last = t.back();
        cur = 0;
        double x = a + h / 2;
        for (int j = 0; j < k; j++) {
            cur += f(x);
            x += h;
        }
        cur = (t[0] + h * cur) / 2;
        double k1 = 4.0 / 3.0, k2 = 1.0 / 3.0;
        for (int j = 0; j < i; j++) {
            double tmp = k1 * cur - k2 * t[j];
            t[j] = cur;
            cur = tmp;
            k2 /= 4 * k1 - k2;
            k1 = k2 + 1;
        }
        t.push_back(cur);
        k *= 2;
        h /= 2;
        i++;
    } while (fabs(last - cur) > eps);
    return t.back();
}
int main()
{
    cout << romberg([] (double x) { return x * x; }, 0, 2) << endl;
    return 0;
}
