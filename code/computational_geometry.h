#ifndef _COMPUTATIONAL_GEOMETRY_H
#define _COMPUTATIONAL_GEOMETRY_H

#include <cmath>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

const double eps = 1e-8;
const double pi = acos(-1.0);

struct point
{
    double x, y;
    point(double _x = 0, double _y = 0) : x(_x), y(_y) { }
    bool operator==(const point & p) const
    { return fabs(p.x - x) < eps && fabs(p.y - y) < eps; }
    bool operator!=(const point & p) const
    { return !(*this == p); }
    point operator-(const point & p) const
    { return point(x - p.x, y - p.y); }
    point operator*(double d) const
    { return point(x * d, y * d); }
    point operator/(double d) const
    { return point(x / d, y / d); }
};

point operator*(double d, const point & p)
{ return p * d; }
inline double square(double d) { return d * d; }
double dist(const point & p1, const point & p2)
{ return sqrt(square(p1.x - p2.x) + square(p1.y - p2.y)); }
double dot(const point & p1, const point & p2)
{ return p1.x * p2.x + p1.y * p2.y; }
double cross(const point & p1, const point & p2)
{ return p1.x * p2.y - p2.x * p1.y; }

struct vec
{
    point from, to;
    vec(const point & p1, const point & p2) : from(p1), to(p2) { }
    vec(double x = 0, double y = 0) : from(0, 0), to(x, y) { }
    vec(const point & p) : from(0, 0), to(p) { }
};

double dot(const vec & v1, const vec & v2)
{ return dot(v1.to - v1.from, v2.to - v2. from); }
double cross(const vec & v1, const vec & v2)
{ return cross(v1.to - v1.from, v2.to - v2.from); }
double cross(const point & o, const point & p1, const point & p2)
{ return cross(vec(o, p1), vec(o, p2)); }

bool is_point_on_line(const vec & v, const point & p)
{ return fabs(cross(vec(v.from, p), v)) < eps; }
bool is_point_on_segment(const vec & v, const point & p)
{
    return is_point_on_line(v, p) &&
           p.x - min(v.from.x, v.to.x) > -eps &&
           p.x - max(v.from.x, v.to.x) <  eps &&
           p.y - min(v.from.y, v.to.y) > -eps &&
           p.y - max(v.from.y, v.to.y) <  eps;
}

bool are_lines_parallel(const vec & v1, const vec & v2)
{ return fabs(cross(v1, v2)) < eps; }
bool are_lines_equal(const vec & v1, const vec & v2)
{ return is_point_on_line(v1, v2.from) && is_point_on_line(v1, v2.to); }
bool are_line_segment_cross(const vec & line, const vec & seg)
{ return cross(line, vec(line.from, seg.from)) * cross(line, vec(line.from, seg.to)) < eps; }
bool are_segments_cross(const vec & v1, const vec & v2)
{
    return max(v1.from.x, v1.to.x) - min(v2.from.x, v2.to.x) > -eps &&
           max(v2.from.x, v2.to.x) - min(v1.from.x, v1.to.x) > -eps &&
           max(v1.from.y, v1.to.y) - min(v2.from.y, v2.to.y) > -eps &&
           max(v2.from.y, v2.to.y) - min(v1.from.y, v1.to.y) > -eps &&
           (cross(vec(v2.from, v1.from), v2) * cross(v2, vec(v2.from, v1.to)) >= eps &&
            cross(vec(v1.from, v2.from), v1) * cross(v1, vec(v1.from, v2.to)) >= eps ||
            is_point_on_segment(v1, v2.from) || is_point_on_segment(v1, v2.to) ||
            is_point_on_segment(v2, v1.from) || is_point_on_segment(v2, v1.to));
}

bool line_intersection(const vec & v1, const vec & v2, point & p)
{
    double D = cross(v1, v2),
           C1 = cross(vec(v1.from), v1),
           C2 = cross(vec(v2.from), v2);
    if (fabs(D) < eps) return false;
    p = (C2 * (v1.to - v1.from) - C1 * (v2.to - v2.from)) / D;
    return true;
}

double dist_between_point_and_seg(const point & p, const vec & seg)
{
    if (dot(seg, vec(seg.from, p)) > eps && dot(seg, vec(seg.to, p)) < -eps)
        return fabs(cross(seg, vec(seg.from, p)) / dist(seg.from, seg.to));
    else return min(dist(seg.from, p), dist(seg.to, p));
}

double area(point p[], int n)
{
    double r = 0.0;
    for (int i = 0; i < n; i++)
        r += cross(p[i], p[(i + 1) % n]);
    return fabs(r / 2.0);
}

point gravity(point p[], int n)
{
    point pt, s;
    double tp, area = 0, tpx = 0, tpy = 0;
    pt = p[0];
    for (int i = 1; i <= n; i++)
    {
        s = p[i == n ? 0 : i];
        tp = cross(pt, s);
        area += tp / 2.0;
        tpx += (pt.x + s.x) * tp;
        tpy += (pt.y + s.y) * tp;
        pt = s;
    }
    s.x = tpx / (6 * area);
    s.y = tpy / (6 * area);
    return s;
}

int dcmp(double x)
{
    if (x < -eps) return -1;
    else return x > eps;
}

int is_point_in_polygon(const point & pt, point p[], int n)
{
    int k, d1, d2, wn = 0;
    p[n] = p[0];
    for (int i = 0; i < n; i++)
    {
        if (is_point_on_segment(vec(p[i], p[i + 1]), pt)) return 2;
        k = dcmp(cross(p[i], p[i + 1], pt));
        d1 = dcmp(p[i + 0].y - pt.y);
        d2 = dcmp(p[i + 1].y - pt.y);
        if (k > 0 && d1 <= 0 && d2 > 0) wn++;
        if (k < 0 && d2 <= 0 && d1 > 0) wn--;
    }
    return wn != 0;
}

vector<point> convex_hull(vector<point> & p)
{
    sort(p.begin(), p.end(), [] (const point & p1, const point & p2) {
         if (p1.x != p2.x) return p1.x < p2.x;
         return p1.y < p2.y;
    });
    int n = p.size();
    int k = 0;
    vector<point> r(n << 1);
    for (int i = 0; i < n; i++)
    {
        while (k > 1 && cross(vec(r[k - 2], r[k - 1]), vec(r[k - 1], p[i])) < eps) k--;
        r[k++] = p[i];
    }
    for (int i = n - 2, t = k; i >= 0; i--)
    {
        while (k > t && cross(vec(r[k - 2], r[k - 1]), vec(r[k - 1], p[i])) < eps) k--;
        r[k++] = p[i];
    }
    r.resize(k - 1);
    return r;
}

vector<point> graham(vector<point> & p)
{
    vector<point> r;
    int k = 0;
    for (int i = 1; i < p.size(); i++)
        if (p[i].y - p[k].y < -eps || fabs(p[i].y - p[k].y) < eps && p[i].x - p[k].x < -eps)
            k = i;
    if (k != 0) swap(p[0], p[k]);
    sort(p.begin() + 1, p.end(),
         [&p] (const point & p1, const point & p2) -> bool
         {
             double d = cross(p[0], p1, p2);
             if (fabs(d) < eps) return dist(p[0], p1) - dist(p[0], p2) < -eps;
             else return d > eps;
         } );
    if (p.size() < 2) return r;
    r.push_back(p[0]);
    r.push_back(p[1]);
    for (int i = 2; i <= p.size(); i++)
    {
        while (r.size() > 1 && cross(r[r.size() - 2], r[r.size() - 1], p[i % p.size()]) < eps)
            r.pop_back();
        if (i != p.size())
            r.push_back(p[i]);
    }
    return r;
}

double rotating_calipers(point ch[], int n)
{
    int q = 1;
    double ans = 0;
    ch[n] = ch[0];
    for (int p = 0; p < n; p++)
    {
        while (cross(ch[p + 1], ch[q + 1], ch[p]) > cross(ch[p + 1], ch[q], ch[p]) + eps)
            q = (q + 1) % n;
        ans = max(ans, max(dist(ch[p], ch[q]), dist(ch[p + 1], ch[q + 1])));
    }
    return ans;
}

const double INF = 1e99;
const double delta = 0.98;
const double T = 100;
int dx[4] = {  0,  0, -1,  1 };
int dy[4] = { -1,  1,  0,  0 };
double get_dist_sum(point p, point pt[], int n)
{
    double ans = 0;
    while (n--)
        ans += dist(p, pt[n]);
    return ans;
}
double fermat_point(point p[], int n)
{
    point s = p[0];
    double t = T;
    double ans = INF;
    while (t > eps)
    {
        bool flag = true;
        while (flag)
        {
            flag = false;
            for (int i = 0; i < 4; i++)
            {
                point z(s.x + dx[i] * t, s.y + dy[i] * t);
                double tp = get_dist_sum(z, p, n);
                if (ans > tp)
                {
                    ans = tp;
                    s = z;
                    flag = true;
                }
            }
        }
        t *= delta;
    }
    return ans;
}

double closest_pair(vector<point>::iterator l, vector<point>::iterator r)
{
    if (r - l <= 1) return INF;
    int m = (r - l) >> 1;
    double x = (l + m)->x;
    double d = min(closest_pair(l, l + m), closest_pair(l + m, r));
    inplace_merge(l, l + m, r, [] (const point & p1, const point & p2) {
        return p1.y - p2.y < -eps;
    });
    vector<point> v;
    for (vector<point>::iterator i = l; i != r; i++)
    {
        if (fabs(i->x - x) - d > -eps) continue;
        for (vector<point>::reverse_iterator j = v.rbegin(); j != v.rend(); j++)
        {
            double dx = i->x - j->x;
            double dy = i->y - j->y;
            if (dy - d > -eps) break;
            d = min(d, sqrt(square(dx) + square(dy)));
        }
        v.push_back(*i);
    }
    return d;
}
double find_closest_pair(vector<point> & v)
{
    sort(v.begin(), v.end(), [] (const point & p1, const point & p2) {
         return p1.x - p2.x < -eps;
    });
    return closest_pair(v.begin(), v.end());
}

bool on_left(const vec & v, const point & p)
{ return cross(v, vec(v.from, p)) > eps; }
bool on_left(const vec & v1, const vec & v2)
{ return cross(v2, vec(v2.from, v1.to)) > eps; }
vector<point> half_plane_intersection(vector<vec> & v)
{
    sort(v.begin(), v.end(), [] (const vec & v1, const vec & v2) {
         point p1 = v1.to - v1.from, p2 = v2.to - v2.from;
         double d = atan2(p1.y, p1.x) - atan2(p2.y, p2.x);
         if (fabs(d) < eps) return on_left(v1, v2);
         else return d < -eps;
    });
    point p;
    deque<vec> q;
    deque<point> r;
    q.push_back(v[0]);
    for (int i = 1; i < v.size(); i++)
    {
        if (are_lines_parallel(q.back(), v[i])) continue;
        while (r.size() > 0 && !on_left(v[i], r.back()))
        {
            q.pop_back();
            r.pop_back();
        }
        while (r.size() > 0 && !on_left(v[i], r.front()))
        {
            q.pop_front();
            r.pop_front();
        }
        line_intersection(q.back(), v[i], p);
        q.push_back(v[i]);
        r.push_back(p);
    }
    while (r.size() > 0 && !on_left(q.front(), r.back()))
    {
        q.pop_back();
        r.pop_back();
    }
    line_intersection(q.front(), q.back(), p);
    r.push_back(p);
    return vector<point>(r.begin(), r.end());
}

point rotate(const point & p, double d)
{ return point(p.x * cos(d) - p.y * sin(d), p.x * sin(d) + p.y * cos(d)); }

/**
    vector<point> pt(4);
    pt[0] = point(-INF, -INF);
    pt[1] = point(INF, -INF);
    pt[2] = point(INF, INF);
    pt[3] = point(-INF, INF);
 */
bool outside(const vec & v, const point & p)
{ return cross(v, vec(v.from, p)) < -eps; }
void cut(vector<point> & pt, const vec & v)
{
    int n = pt.size();
    if (n == 0) return;
    pt.push_back(pt[0]);
    vector<point> tp;
    point p;
    for (int i = 0; i < n; i++)
    {
        if (!outside(v, pt[i])) tp.push_back(pt[i]);
        else
        {
            if (i == 0 && !outside(v, pt[n - 1]))
            {
                line_intersection(v, vec(pt[i], pt[n - 1]), p);
                tp.push_back(p);
            }
            if (i != 0 && !outside(v, pt[i - 1]))
            {
                line_intersection(v, vec(pt[i], pt[i - 1]), p);
                tp.push_back(p);
            }
            if (!outside(v, pt[i + 1]))
            {
                line_intersection(v, vec(pt[i], pt[i + 1]), p);
                tp.push_back(p);
            }
        }
    }
    pt.clear();
    for (int i = 0; i < tp.size(); i++)
        if (pt.size() == 0 || pt[pt.size() - 1] != tp[i])
            pt.push_back(tp[i]);
}

#endif // _COMPUTATIONAL_GEOMETRY_H
