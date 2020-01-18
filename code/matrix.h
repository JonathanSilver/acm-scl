#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include <vector>
#include <initializer_list>

#include "number_theory.h"

const int MOD = 1000000007;

using namespace std;

template <class T>
class matrix
{
private:
    int m;
    int n;
    vector<vector<T> > a;
public:
    static matrix<T> identity_matrix(int n)
    {
        matrix<T> m(n, n);
        for (int i = 0; i < n; i++)
            m(i, i) = 1;
        return m;
    }
    matrix(int _m = 0, int _n = 0)
    : m(_m), n(_n), a(m, vector<T>(n)) { }
    matrix(const initializer_list<initializer_list<T> > & v)
    : m(v.size()), n(v.begin()->size()), a(m, vector<T>(n))
    {
        int x = 0;
        for (auto i = v.begin(); i != v.end(); i++, x++)
        {
            int y = 0;
            for (auto j = i->begin(); j != i->end(); j++, y++)
                a[x][y] = *j;
        }
    }
    int get_m() const { return m; }
    int get_n() const { return n; }
    T & operator()(int x, int y) { return a[x][y]; }
    const T & operator()(int x, int y) const { return a[x][y]; }
    matrix<T> operator-() const
    {
        matrix<T> t(*this);
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            t.a[i][j] = -t.a[i][j];
        return t;
    }
    matrix<T> operator+(const matrix<T> & b) const
    {
        if (m == b.m && n == b.n)
        {
            matrix<T> t(*this);
            for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                t.a[i][j] += b.a[i][j];
            return t;
        } else return matrix<T>(0, 0);
    }
    matrix<T> operator-(const matrix<T> & b) const
    {
        if (m == b.m && n == b.n)
        {
            matrix<T> t(*this);
            for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                t.a[i][j] -= b.a[i][j];
            return t;
        } else return matrix<T>(0, 0);
    }
    template <class U>
    matrix<T> operator*(const U & t) const
    {
        matrix<T> temp(*this);
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            temp.a[i][j] *= t;
        return temp;
    }
    template <class U>
    matrix<T> operator/(const U & t) const
    {
        matrix<T> temp(*this);
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            temp.a[i][j] /= t;
        return temp;
    }
    template <class U>
    matrix<T> operator%(const U & t) const
    {
        matrix<T> temp(*this);
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            temp.a[i][j] %= t;
        return temp;
    }
    matrix<T> operator*(const matrix<T> & b) const
    {
        if (n == b.m)
        {
            matrix<T> t(m, b.n);
            for (int i = 0; i < m; i++)
            for (int j = 0; j < b.n; j++)
            for (int k = 0; k < n; k++)
                t.a[i][j] = ((t.a[i][j] + a[i][k] * b.a[k][j] % MOD) % MOD + MOD) % MOD;
                // (t.a[i][j] += (a[i][k] * b.a[k][j]) % MOD) %= MOD;
            return t;
        } else return matrix<T>(0, 0);
    }
    template <class U>
    matrix<T> & operator*=(const U & t)
    {
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i][j] *= t;
        return *this;
    }
    template <class U>
    matrix<T> & operator/=(const U & t)
    {
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i][j] /= t;
        return *this;
    }
    template <class U>
    matrix<T> & operator%=(const U & t)
    {
        for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = (a[i][j] % t + t) % t;
        return *this;
    }
    matrix<T> & operator+=(const matrix<T> & b)
    {
        if (m == b.m && n == b.n)
        {
            for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                a[i][j] += b.a[i][j];
        }
        return *this;
    }
    matrix<T> & operator-=(const matrix<T> & b)
    {
        if (m == b.m && n == b.n)
        {
            for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                a[i][j] -= b.a[i][j];
        }
        return *this;
    }
    template <class U>
    matrix<T> pow(U k) const
    {
        if (m == n)
        {
            matrix<T> t = identity_matrix(n);
            matrix<T> b(*this);
            while (k)
            {
                if (k & 1) t = t * b;
                b = b * b;
                k >>= 1;
            }
            return t;
        } else return matrix<T>(0, 0);
    }
    template <class U>
    friend matrix<T> operator*(const U & t, const matrix<T> & b) { return b * t; }
    friend ostream & operator<<(ostream & os, const matrix<T> & b)
    {
        for (int i = 0; i < b.m; i++)
        {
            for (int j = 0; j < b.n; j++)
                os << b.a[i][j] << ' ';
            os << endl;
        }
        return os;
    }
};

const double eps = 1e-8;

pair<int, vector<pair<bool, double> > > gaussian_elimination(matrix<double> & mat)
{
    int m = mat.get_m(), n = mat.get_n() - 1, r = 0;
    vector<pair<bool, double> > v(n, make_pair(true, 0));
    for (int i = 0; i < n && r < m; i++)
    {
        if (fabs(mat(r, i)) < eps)
            for (int j = r + 1; j < m; j++)
                if (fabs(mat(j, i)) > eps)
                {
                    for (int k = i; k <= n; k++)
                        swap(mat(j, k), mat(r, k));
                    break;
                }
        if (fabs(mat(r, i)) > eps)
        {
            v[i].first = false;
            for (int k = n; k >= i; k--)
                mat(r, k) /= mat(r, i);
            for (int j = 0; j < m; j++)
                if (j != r && fabs(mat(j, i)) > eps)
                    for (int k = n; k >= i; k--)
                        mat(j, k) -= mat(j, i) * mat(r, k);
            r++;
        }
    }
    for (int i = r; i < m; i++)
        if (fabs(mat(i, n)) > eps)
            return make_pair(-1, vector<pair<bool, double> >());
    for (int i = 0, j = 0; i < n; i++)
        if (!v[i].first)
        {
            v[i].second = mat(j, n);
            j++;
        }
    return make_pair(r == n, v);
}

typedef long long ll;

pair<int, vector<pair<bool, ll> > > gaussian_elimination(matrix<ll> & mat, ll p)
{
    mat %= p;
    int m = mat.get_m(), n = mat.get_n() - 1, r = 0;
    vector<pair<bool, ll> > v(n, make_pair(true, 0));
    for (int i = 0; i < n && r < m; i++)
    {
        if (!mat(r, i))
            for (int j = r + 1; j < m; j++)
                if (mat(j, i))
                {
                    for (int k = i; k <= n; k++)
                        swap(mat(j, k), mat(r, k));
                    break;
                }
        if (mat(r, i))
        {
            v[i].first = false;
            for (int j = 0; j < m; j++)
                if (j != r && mat(j, i))
                {
                    ll t = mat(j, i) * inv(mat(r, i), p) % p;
                    for (int k = n; k >= i; k--)
                        mat(j, k) = ((mat(j, k) - mat(r, k) * t % p) % p + p) % p;
                }
            r++;
        }
    }
    for (int i = r; i < m; i++)
        if (mat(i, n))
            return make_pair(-1, vector<pair<bool, ll> >());
    for (int i = 0, j = 0; i < n; i++)
        if (!v[i].first)
        {
            v[i].second = (mat(j, n) * inv(mat(j, i), p) % p + p) % p;
            j++;
        }
    return make_pair(r == n, v);
}

#endif // _MATRIX_H
