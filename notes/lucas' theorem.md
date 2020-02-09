# Notes for Lucas' Theorem

*View this note with `VSCode` + `Markdown All in One` or any other ways you like.*

## Lucas' Theorem
*Lucas' theorem* states that for non-negative integers $m$ and $n$, and a prime $p$,
$$\begin{pmatrix}m\\n\end{pmatrix}\equiv\prod_{i=0}^{k}{\begin{pmatrix}m_i\\n_i\end{pmatrix}} \pmod p$$
where
$$m=m_kp_k+m_{k-1}p^{k-1}+\cdots+m_1p+m_0$$
and
$$n=n_kp^k+n_{k-1}p^{k-1}+\cdots+n_1p+n_0$$
are the base $p$ expansions of $m$ and $n$, respectively.  
This uses the convention that
$$\begin{pmatrix}m\\n\end{pmatrix}=0,if\;m<n$$
> **Proof.** The proof proceeds by induction. The idea is to prove:
>> Let $s,t\in\mathbb{N}$ and let $p$ be a prime. With $0 \le q,r < p$,
>> $$\begin{pmatrix}sp+q\\tp+r\end{pmatrix}\equiv\begin{pmatrix}s\\t\end{pmatrix}\begin{pmatrix}q\\r\end{pmatrix} \pmod p$$
> Since
> $$\begin{aligned}
(1+x)^p&=\sum_{i=0}^{p}{\begin{pmatrix}p\\i\end{pmatrix}x^i}\\
&=\sum_{i=0}^{p}{\frac{p!}{i!(p-i)!}x^i}\\
&=1+\sum_{i=1}^{p-1}{\frac{p!}{i!(p-i)!}x^i}+x^p
\end{aligned}$$ 
> Therefore
> $$(1+x)^p \equiv 1+x^p \pmod p$$
> This is because for $i=1, 2, \cdots p-1$, the multiplicative inverses $\pmod p$ exist.  
> Using the above equation, we have
> $$\begin{aligned}
(1+x)^{sp+q}&\equiv(1+x)^{sp}\cdot(1+x)^q&\pmod p\\
&\equiv(1+x^p)^s\cdot(1+x)^q&\pmod p\\
&\equiv\sum_{i=0}^{s}{\begin{pmatrix}s\\i\end{pmatrix}x^{ip}}\cdot\sum_{j=0}^{q}{\begin{pmatrix}q\\j\end{pmatrix}x^j}&\pmod p\\
&\equiv\sum_{i=0}^{s}{\sum_{j=0}^{q}{\begin{pmatrix}s\\i\end{pmatrix}\begin{pmatrix}q\\j\end{pmatrix}x^{ip+j}}}&\pmod p
\end{aligned}$$
> On the left side, we have
> $$(1+x)^{sp+q}\equiv\sum_{k=0}^{sp+q}{\begin{pmatrix}sp+q\\k\end{pmatrix}x^k}\pmod p$$
> We know the coefficient of $x^{tp+r}$ is $\begin{pmatrix}sp+q\\tp+r\end{pmatrix}$.  
> On the right side, every power of $x$ appears only once.  
> Since the coefficients of $x^{tp+r}$ should be the same on both sides, we conclude
> $$\begin{pmatrix}sp+q\\tp+r\end{pmatrix}\equiv\begin{pmatrix}s\\t\end{pmatrix}\begin{pmatrix}q\\r\end{pmatrix}\pmod p$$
> This completes the proof.

Below is the C++ implementation used to compute $\begin{pmatrix}n\\m\end{pmatrix}\bmod p$:
```C++
template <typename T>
T lucas(T n, T m, T p) {
    if (m > n) return 0;
    T r = 1;
    for ( ; m; n /= p, m /= p)
        r = r * C(n % p, m % p, p) % p;
    return r;
}
```
If we can calculate $\begin{pmatrix}n\\m\end{pmatrix}\;\left(n,m<p\right)$ in $O(1)$, the running time of the above code is $O(\log p)$. This can be done if $p$ is fixed, and not too large (about $10^7$), since we can pre-compute $n!$ and $(n!)^{-1}\pmod p$ to obtain $\begin{pmatrix}n\\m\end{pmatrix}$ by the definition $\begin{pmatrix}n\\m\end{pmatrix}=\frac{n!}{m!(n-m)!}$. Pre-computing $n!$ is straight forward, because we can use the recurrence:
$$\left\{\begin{array}{llll}
0!&=&1&\\
n!&=&n(n-1)!&n=1,2,\cdots,N
\end{array}\right.$$
In order to pre-compute $(n!)^{-1}\pmod p$, we can use this trick:
$$\left\{\begin{array}{lllll}
(N!)^{-1}&\equiv&(N!)^{p-2}&\pmod p&\\
(n!)^{-1}&\equiv&((n+1)!)^{-1}(n+1)&\pmod p&n=N-1,N-2,\cdots,0
\end{array}\right.$$
The pre-computational cost is $O(N+\log p+N)$
> **Hint.** There is another way to pre-compute $(n!)^{-1}\pmod p$.  
> We are going to use a recurrence similar to the straight forward way:
> $$\left\{\begin{array}{lllll}
(0!)^{-1}&\equiv&1&\pmod p&\\
(n!)^{-1}&\equiv&((n-1)!)^{-1} \cdot n^{-1}&\pmod p&n=1,2,\cdots,N
\end{array}\right.$$
> The only problem would be how to calculate $n^{-1}\pmod p$ efficiently. The below recurrence help us solve the problem:
> $$\left\{\begin{array}{lllll}
1^{-1}&\equiv&1&\pmod p&\\
n^{-1}&\equiv&-\lfloor\frac{p}{n}\rfloor\cdot(p\bmod n)^{-1}&\pmod p&n=2,3,\cdots,N
\end{array}\right.$$
>> **Proof.** It is obvious that $1^{-1}\equiv 1\pmod p$. Now, let $q=\lfloor\frac{p}{n}\rfloor$, $r=p\bmod n$, $p=q\cdot n+r$, where $0<r<n<p$.  
>> Or,
>> $$q\cdot n+r\equiv 0\pmod p$$
>> Multiply both sides by $n^{-1}\cdot r^{-1}$,
>> $$\begin{aligned}
q\cdot r^{-1}+n^{-1}&\equiv 0 &\pmod p\\
\Longrightarrow n^{-1}&\equiv-q\cdot r^{-1}&\pmod p\\
\Longrightarrow n^{-1}&\equiv-\lfloor\frac{p}{n}\rfloor\cdot (p\bmod n)^{-1}&\pmod p
\end{aligned}$$

However, if $p$ is not fixed, we have to compute $\begin{pmatrix}n\\m\end{pmatrix}$ in $O(p)$ time:
```C++
template <typename T>
T C(T n, T m, T p) {
    if (m > n) return 0;
    T a = 1, b = 1;
    for (T i = n - m + 1; i <= n; ++i) a = a * i % p;
    for (T i = 1; i <= m; ++i) b = b * i % p;
    return a * mod_pow(b, p - 2, p) % p;
}
```
Since $p$ is a prime, we can use the *Fermat's little theorem* to obtain the multiplicative inverse. In this case, `lucas` runs in $O(p\log p)$ time. Depending on the number of times you need to call `lucas`, $p$ is usually limited up to about $10^5$.

References:
- [*Lucas' theorem*](https://brilliant.org/wiki/lucas-theorem/)
- [Proof for *Lucas' theorem*](https://www.cut-the-knot.org/arithmetic/algebra/LucasTheorem.shtml)
- [Additional reference for proof of *Lucas' theorem*](https://baike.baidu.com/item/lucas/4326261?fr=aladdin)
- [Trick to pre-compute the multiplicative inverses of factorials](https://blog.csdn.net/github_39363510/article/details/77413740)
- [Another way to pre-compute the multiplicative inverses of factorials](https://www.cnblogs.com/solvit/p/9820607.html)
- [Proof for another way to pre-compute the multiplicative inverses of factorials](https://blog.csdn.net/weixin_44178736/article/details/104217449)


## Extended Form of the Lucas' Theorem
Given nonnegative integers $n$ and $m$,  **arbitrary** positive integer $p$, compute
$$\begin{pmatrix}n\\m\end{pmatrix}\bmod p$$
The is the generalized version of the problem which can be nicely solved by the *Lucas' theorem*. However, this time, we have to use a less elegant approach.
> **Observation 1.** We can decompose $p$ into the powers of its prime factors:
> $$p=\prod_{i}{p_i^{k_i}}$$
> Since for any $i$ and $j$ ($i\ne j$), $\gcd(p_i^{k_i}, p_j^{k_j})=1$. If we are capable of finding all the $\begin{pmatrix}n\\m\end{pmatrix}\bmod p_i^{k_i}$, we can use the *CRT* to 'restore' the answer to $\begin{pmatrix}n\\m\end{pmatrix}\bmod p$.

Now, our problem becomes:
> Given nonnegative integers $n$ and $m$, a prime $p$, compute
> $$\begin{pmatrix}n\\m\end{pmatrix}\bmod p^k$$
> where $k>0$.

Our starting point would be using the definition:
$$\begin{pmatrix}n\\m\end{pmatrix}=\frac{n!}{m!(n-m)!}$$
> **Observation 2.** What has stopped us from using the multiplicative inverses is that $m!$ or $(n-m)!$ may contain prime factor $p$. The remedy is to exclude the multiples of $p$ from all the factorials, i.e.,
> $$\begin{aligned}
\begin{pmatrix}n\\m\end{pmatrix}\bmod p^k&=\frac{n!}{m!(n-m)!}\bmod p^k\\
&=\frac{\frac{n!}{p^{k_1}}}{\frac{m!}{p^{k_2}}\cdot\frac{(n-m)!}{p^{k_3}}}\cdot\frac{p^{k_1}}{p^{k_2}\cdot p^{k_3}}\bmod p^k\\
&=\left(\frac{\frac{n!}{p^{k_1}}}{\frac{m!}{p^{k_2}}\cdot\frac{(n-m)!}{p^{k_3}}}\bmod p^k\right)\cdot\left(p^{k_1-k_2-k_3}\bmod p^k\right)\bmod p^k
\end{aligned}$$
> where $p^{k_1}\mid n!$, $p^{k_1+1}\nmid n!$, $p^{k_2}\mid m!$, $p^{k_2+1}\nmid m!$, $p^{k_3}\mid (n-m)!$, $p^{k_3+1}\nmid (n-m)!$.  
> In this way, the multiplicative inverses work!

We only need to focus on $\frac{n!}{p^{k_1}}\bmod p^k$, since the other terms can be solved similarly. But, first, we need to compute $k_1$. Since $k_1$ is the number of $p$'s in $n!$, which can be computed by
$$k_1=\lfloor\frac{n}{p}\rfloor+\lfloor\frac{n}{p^2}\rfloor+\lfloor\frac{n}{p^3}\rfloor+\cdots$$
Next, we need to handle $\frac{n!}{p^{k_1}}\bmod p^k$:
> **Observation 3.** Let's review $n!$:
> $$n!=1\times 2\times\cdots\times n$$
> We try to keep consecutive $p^k$ elements in a group:
> $$\begin{array}{lll}
n!&=&(1\times 2\times\cdots\times p^k)\times\\
&&\left((p^k+1)\times(p^k+2)\times\cdots\times 2p^k\right)\times\cdots\times\\
&&\left(\left(\lfloor\frac{n}{p^k}\rfloor p^k+1\right)\times\left(\lfloor\frac{n}{p^k}\rfloor p^k+2\right)\times\cdots\times n\right)
\end{array}$$
> Since
> $$qp^k+r\equiv r\pmod{p^k}$$
> where $1\le q\le\lfloor\frac{n}{p^k}\rfloor$, $0\le r<p^k$, $qp^k+r\le n$.  
> Therefore, for any $1\le q<\lfloor\frac{n}{p^k}\rfloor$,
> $$\prod_{r=1}^{p^k}{r}\equiv\prod_{r=1}^{p^k}{qp^k+r}\pmod{p^k}$$
> So,
> $$\begin{aligned}
n!\equiv&\left(1\times 2\times\cdots\times p^k\right)^{\lfloor\frac{n}{p^k}\rfloor}\\
&\cdot\left(\left(\lfloor\frac{n}{p^k}\rfloor p^k+1\right)\times\left(\lfloor\frac{n}{p^k}\rfloor p^k+2\right)\times\cdots\times n\right)\pmod{p^k}
\end{aligned}$$
> The idea applies to computing $n!\bmod p^k$ when $n$ is very large and $p^k$ is relatively small. However, as is mentioned earlier, we should exclude the multiples of $p$ from $n!$.

To apply the idea in **observation 3**, we make some small changes.
> **Observation 4.** Let's rewrite $n!$ in the following way:
> $$\begin{array}{lll}
n!&=&1\times 2\times\cdots\times p\times\cdots\times 2p\times\cdots\times\lfloor\frac{n}{p}\rfloor p\times\cdots\times n\\
&=&p^{\lfloor\frac{n}{p}\rfloor}\cdot\lfloor\frac{n}{p}\rfloor!\cdot\left(1\times 2\times\cdots\times(p-1)\times(p+1)\times\right.\\
&&\cdots\times(2p-1)\times(2p+1)\times\\
&&\cdots\times\left(\lfloor\frac{n}{p}\rfloor p-1\right)\times\left(\lfloor\frac{n}{p}\rfloor p+1\right)\times\\
&&\left.\cdots\times n\right)\\
&=&p^{\lfloor\frac{n}{p}\rfloor}\cdot\lfloor\frac{n}{p}\rfloor!\cdot(\textit{the rest terms})
\end{array}$$
> We exclude $p^{\lfloor\frac{n}{p}\rfloor}$, recursively solve for $\lfloor\frac{n}{p}\rfloor!$, and apply the idea in **observation 3** to $\textit{the rest terms}$.  
> Originally, the $r$-th element in the $q$-th group is $qp^k+r$, where $0\le q<\lfloor\frac{n}{p^k}\rfloor$ and $1\le r\le p^k$. If it is a multiple of $p$, then $p\mid\left(qp^k+r\right)\Longrightarrow p\mid r\Longrightarrow r=p,2p,...,p^{k-1}\cdot p$, which implies that the positions of the excluded elements in every group are exactly the same.  
> So, all we need to do, is to group consecutive $p^k-p^{k-1}$ elements together (excluding the mutiples of $p$) in $\textit{the rest terms}$. Of course, the last group doesn't necessarily have $p^k-p^{k-1}$ elements. Now, we calculate the product of the first group, raise it to the power of the number of groups, multiply the power by the product of the last group, and we will have the result of $\textit{the rest terms}$.

We proceed with an illustration.
> **Illustration** We want to compute
> $$\frac{22!}{3^{k_1}}\bmod 3^2$$
> First, we group consecutive $3^2$ elements together:
> $$\begin{array}{lrcccl}
22!=&(1\times 2\times \mathbf{3}&\times&4\times 5\times \mathbf{6}&\times&7\times 8\times \mathbf{9})\times\\
&(10\times 11\times \mathbf{12}&\times&13\times 14\times \mathbf{15}&\times&16\times 17\times \mathbf{18})\times\\
&(19\times 20\times \mathbf{21}&\times&22)
\end{array}$$
> Second, we exclude the multiple of $3$:
> $$\begin{array}{l}
22!=3^7\times7!\times\left(\right.&(1\times 2\times 4\times 5\times 7\times 8)&\times\\
&(10\times 11\times 13\times 14\times 16\times 17)&\times\\
&(19\times 20\times 22)&\left.\right)
\end{array}$$
> The last group has less than $6$ elements. Now, we calculate the product of the first group:
> $$1\times 2\times 4\times 5\times 7\times 8\equiv 8\pmod{3^2}$$
> Raise it to the power of the number of groups (excluding the last):
> $$(1\times 2\times 4\times 5\times 7\times 8)^2\equiv 1\pmod{3^2}$$
> The product of the last group:
> $$19\times 20\times 22\equiv 8\pmod{3^2}$$
> Multiply the power by the product of the last group:
> $$(1\times 2\times 4\times 5\times 7\times 8)^2\cdot(19\times 20\times 22)\equiv 8\pmod{3^2}$$
> We are leaving the $3^7$ 'untouched', and compute $7!$ recursively. Then, we can have the result.

You can read the C++ code for further understanding:
```C++
/* compute n!/p^x mod p^k */
/* remember that we need to exclude the multiple of p */
template <typename T>
T fac(T n, T p, T pk) {
    if (n <= 1) return 1; // termination for recursion
    T r = 1;
    if (n >= pk) {
        // we have at least one 'complete' group
        // calculate the product of the first group
        for (T i = 2; i < pk; i++)
            if (i % p) (r *= i) %= pk;
        // raise it to the power of the number of groups
        r = mod_pow(r, n / pk, pk);
    }
    // multiply the remaining elements
    for (T i = 2; i <= n % pk; i++)
        if (i % p) (r *= i) %= pk;
    // recursively solve for the result
    return r * fac(n / p, p, pk) % pk;
}
/* compute C(n, m) mod p^k */
template <typename T>
T ext_C(T n, T m, T p, T pk) {
    T a = fac(n, p, pk); // n!/p^k1 mod p^k
    T b = inv(fac(m, p, pk), pk); // (m!/p^k2)^(-1) mod p^k
    T c = inv(fac(n - m, p, pk), pk); // ((n-m)!/p^k3)^(-1) mod p^k
    T k = 0; // the left 'untouched' p^(k1-k2-k3)
    for (T i = n; i; i /= p) k += i / p;
    for (T i = m; i; i /= p) k -= i / p;
    for (T i = n - m; i; i /= p) k -= i / p;
    return a * b % pk * c % pk * mod_pow(p, k, pk) % pk;
}
/* compute C(n, m) mod p*/
template <typename T>
T ext_lucas(T n, T m, T p) {
    T r = 0, tmp = p;
    for (T i = 2; i * i <= tmp; i++) { // prime factor factorization
        if (tmp % i == 0) {
            T pk = 1; // p^k
            while (tmp % i == 0) {
                pk *= i;
                tmp /= i;
            }
            // combine the results C(n, m) mod p^k by CRT
            (r += ext_C(n, m, i, pk) * (p / pk) % p * inv(p / pk, pk) % p) %= p;
        }
    }
    if (tmp > 1) (r += ext_C(n, m, tmp, tmp) * (p / tmp) % p * inv(p / tmp, tmp) % p) %= p;
    return r;
}
```
Analysis of Complexity:
- `fac`: The recursion depth is $O(\log n)$. During the recursion, we need to iterate from $1$ to $p^k$ which costs $O\left(p^k\right)$, and compute the power which costs $O\left(\log\frac{n}{p^k}\right)=O(\log n-k\log p)=O(\log n)$. The total running time is $O\left(\left(p^k+\log n\right)\log n\right)$.
- `ext_C`: To compute `a`, `b` and `c`, we need $O\left(\left(p^k+\log n\right)\log n\right)$ time. To compute the multiplicative inverses $\pmod{p^k}$ by *Euclid's extended algorithm*, we need $O(\log p^k)$ time. To compute `k`, we need $O(\log n)$ time. To raise the power, we also need $O(\log\frac{n}{p})=O(\log n-\log p)=O(\log n)$ time, since $k=\lfloor\frac{n}{p}\rfloor+\lfloor\frac{n}{p^2}\rfloor+\cdots\le\frac{n}{p}+\frac{n}{p^2}+\cdots<\frac{n}{p-1}$. The total running time is $O\left(\left(p^k+\log n\right)\log n+\log p^k\right)$.
- `ext_lucas`: The naive approach to factorize prime factors costs $O\left(\sqrt{p}\right)$ time in the worst case. During the factorization, we need to call `ext_C` and compute the multiplicative inverse $\pmod{p^k}$. Since the complexity of `ext_C` depends on the prime power `pk` which is at most $O(p)$, the running time of the `for` loop is $O\left(\sqrt{p}\left(\left(p+\log n\right)\log n+\log p\right)\right)$. As for the last step, the running time is $O\left(\left(p+\log n\right)\log n+\log p\right)$. So, the total running time is $O\left(\sqrt{p}\left(\left(p+\log n\right)\log n+\log p\right)\right)$.

References:
- [Link 1](https://blog.csdn.net/hqddm1253679098/article/details/82897638)
- [Link 2](https://blog.csdn.net/qq_42899221/article/details/82194529)
