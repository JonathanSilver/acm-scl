# Reaching Out with *Primitive Roots*

## Basics of the *Primitive Roots*

There are many definitions for *primitive roots*. We start with this one.

> **Definition 1.**
>> Define
>> $$\mathbb{Z}_n^*=\left\{a\in\mathbb{N}:1\le a<n,\gcd(a,n)=1\right\}$$
>> So $\mathbb{Z}_n^*$ has $\varphi(n)$ elements, where $\varphi$ is *Euler's totient function*.
> 
> A *primitive root* $\pmod n$ is an element $g\in\mathbb{Z}_n^*$ whose powers generate all of $\mathbb{Z}_n^*$, i.e., for every element $b\in\mathbb{Z}_n^*$ there exists some integer $x$ such that $g^x\equiv b\pmod n$.

We take $n=5$ as an example. For $\mathbb{Z}_5^*=\left\{1,2,3,4\right\}$, $2$ is a *primitive root* $\pmod 5$ because

$x$|$2^x$|$2^x\bmod 5$
:-:|:-:|:-:
0|1|1
1|2|2
2|4|4
3|8|3
4|16|1

For every integer $b\in\mathbb{Z}_5^*$, there is a power of 2 that is congruent.

As another example, we take $n=5$, again, but $4$ is not a *primitive root* $\pmod 5$ because

$x$|$4^x$|$4^x\bmod 5$
:-:|:-:|:-:
0|1|1
1|4|4
2|16|1
3|64|4

We don't need to examine further since the *Euler's theorem* tells us that

$$4^x\equiv 4^{x\bmod(5-1)}\pmod 5$$

So, if you have checked that for $x=0,1,2,3$, $4^x\bmod 5$ is not capable of generating every $b\in\mathbb{Z}_5^*$, you can conclude immediately that $4$ is not a *primitive root* $\pmod 5$.

Thanks to the above observation, we now have another definition for *primitive roots*.

> **Definition 2.** Let the smallest **positive** integer $x$ such that $a^x\equiv 1\pmod n$ be the *order* of $a$ $\mod n$. Then $a$ is a *primitive root* $\pmod n$ if and only if the *order* of $a$ $\pmod n$ is $\varphi(n)$.

According to **definition 2**, for $n=5$, $2$ is a *primitive root* $\pmod 5$, while $4$ is not, since the *order* of $2$ $\pmod 5$ is $4=\varphi(5)$, but the *order* of $4$ is $2$.

> **Hint.** You can consider the *order* of $a$ $\pmod n$ the **minimum** *period* of the sequence $\left\{a^i\bmod n\right\}_{i=0}^{\infty}$. This sequence starts with $a^0\bmod n=1$ if $n>1$. Though the *Euler's theorem* guarantees that the the sequence has a *period* of $\varphi(n)$, this may **not** be the **minimum** one.

The above observation also gives us a relation between the *order* and the *Euler's totient function*.

> **Lemma 1.** For each element $a\in\mathbb{Z}_n^*$, the *order* of $a$ $\pmod n$ divides $\varphi(n)$.
>> **Proof.** (By Contradiction.) Let $s$ be the *order* of $a$ $\pmod n$. We assume that $s\nmid\varphi(n)$. Let $\varphi(n)=qs+r$, where $q=\lfloor\frac{\varphi(n)}{s}\rfloor$, $r=\varphi(n)\bmod s$ and $0<r<s$. Since $s$ is the *order* of $a$ $\pmod n$, we have $a^s\equiv 1\pmod n$. By the *Euler's theorem*, we have $a^{\varphi(n)}\equiv a^{qs+r}\equiv a^r\equiv 1\pmod n$, implying that $r$ is the order of $a$ $\pmod n$ because we know that $0<r<s$, which contradicts our assumption. Therefore, we conclude that $s\mid\varphi(n)$.

Now, we can even generalize **lemma 1**.

> **Corollary 1.** For each element $a\in\mathbb{Z}_n^*$, the *order* of $a$ $\pmod n$ divides the nonnegative integer $x$ such that $a^x\equiv 1\pmod n$.

The proof of **corollary 1** is similar to that of **lemma 1**.

When it comes to *order*, we must also mention **lemma 2**.

> **Lemma 2.** If $t$ is the *order* of $a$ $\pmod n$, then $t$ is the *order* of $a^k$ $\pmod n$ if and only if $\gcd(k,t)=1$.
>> **Proof.**  
>> First, suppose $\gcd(k,t)=1$. Let $s$ be the *order* of $a^k$ $\pmod n$. Since $\left(a^k\right)^t\equiv\left(a^t\right)^k\equiv 1\pmod n$, we have that $s\mid t$, by **corollary 1**. Similarly, since $\left(a^k\right)^s\equiv a^{ks}\equiv 1\pmod n$, we have that $t\mid ks$, by **corollary 1**. Because $\gcd(k,t)=1$, we know that $t\mid s$. So, we can conclude that $s=t$.  
>> Second, suppose $t$ is the *order* of $a^k$ $\pmod n$. We **assume** (proof by contradiction) that $\gcd(k,t)=d>1$. Let $k=du$, $t=dv$, where $\gcd(u,v)=1$. Since $\left(a^k\right)^v\equiv a^{duv}\equiv\left(a^t\right)^u\equiv 1\pmod n$, we have $t\mid v$, by **corollary 1**. However, we know that $t=dv$ and $d>1$, which makes $t\mid v$ impossible. So, we conclude that $\gcd(k,t)=1$.

With **lemma 2**, you may probably be wondering the number of *primitive roots* $\pmod n$ if there is any.

> **Theorem 1.** If there are *primitive roots* $\pmod n$, then there are exactly $\varphi(\varphi(n))$ of them.
>> **Proof.** Let $a$ be a *primitive root* $\pmod n$. By **definition 2**, the *order* of $a$ $\pmod n$ is $\varphi(n)$. Now, we rewrite $\mathbb{Z}_n^*=\left\{a^i\bmod n:0\le i<\varphi(n)\right\}$. By **lemma 2**, we have $a^k\bmod n$, $\gcd(k,\varphi(n))=1$, total of $\varphi(\varphi(n))$ **distinct** elements, whose *orders* $\pmod n$ are all $\varphi(n)$. Meanwhile, also by **lemma 2**, if $\gcd(k,\varphi(n))\ne 1$, $a^k\bmod n$ will not share the *order* of $\varphi(n)$. This completes the proof.

How do we know whether *primitive roots* exist or not?

> **Theorem 2.** There are primitive roots $\pmod n$ if and only if $n=1$, $2$, $4$, $p^k$, or $2p^k$, where $p$ is an odd prime and $k\ge 1$.

## An Algorithm to Find *Primitive Roots*

Given positive integer $n$, we try to find one *primitive root* $\pmod n$.

> **Observation.** By **definition 2**, $a$ is a *primitive root* $\pmod n$ if the smallest positive integer $x$ such that $a^x\equiv 1\pmod n$ is $\varphi(n)$. If $a$ is a *primitive root* $\pmod n$, then there should not exist a proper divisor $d$ of $\varphi(n)$, $1\le d<\varphi(n)$ and $d\mid\varphi(n)$, such that $a^d\equiv 1\pmod n$, by **lemma 1**.
>> **Hint.** **Lemma 1** tells us that all the *orders* $\pmod n$ divides $\varphi(n)$, so we don't need to check integers which cannot divide $\varphi(n)$.

Checking all the divisors is also time consuming. In fact, we can even do it faster.

> **Solution.** First, we prime factorize $\varphi(n)=\prod_{i}{p_i^{e_i}}$. Second, we only check the divisors with the form $\frac{\varphi(n)}{p_i}$ for any $i$. This is because if there exists such $d$ and $j$ that $d\mid\frac{\varphi(n)}{p_j}$ and $a^d\equiv 1\pmod n$, let $dk=\frac{\varphi(n)}{p_j}$ and we will have $a^{\frac{\varphi(n)}{p_j}}\equiv a^{dk}\equiv\left(a^d\right)^k\equiv 1\pmod n$.

Below is the C++ implementation of the above algorithm:

```C++
template <typename T>
bool g_test(const vector<T> & v, T g, T p) {
    for (int i = 0; i < v.size(); i++)
        if (mod_pow(g, (p - 1) / v[i], p) == 1)
            return false;
    return true;
}
/**
 * find the smallest primitive root of p,
 * where p is a prime.
 */
template <typename T>
T primitive_root(T p) {
    T t = p - 1, g = 1; vector<T> v;
    for (T i = 2; i * i <= t; i++)  // prime factorization
        if (t % i == 0) {
            v.push_back(i);
            while (t % i == 0) t /= i;
        }
    if (t != 1) v.push_back(t);
    while (true) { // check by brute force
        if (g_test(v, g, p)) return g;
        g++;
    }
}
```

The running time of the above algorithm is $O(\sqrt{p}+g\log^2 p)$, assuming that $p-1$ has $O(\log p)$ prime factors. Usually, $g$ is not very large. So, the above algorithm is sufficient.

## Discrete Logarithm
> Given three non-negative integers $a$, $b$ and $p$, and $a$ is relatively prime to $p$, i.e., $\gcd(a,p)=1$, find the smallest non-negative integer $x$ such that
> $$a^x\equiv b\pmod p$$

> **Hint.** If $a$ is a *primitive root* $\pmod p$, then we know, for any $b\in\mathbb{Z}_p^*$, the solution of $x$ always exists.

If we want to find the solution, we can use the *sqrt-decomposition* technique. We are going to enumerate all the possible solutions, in a smart way. Let $t=\lceil\sqrt{p}\rceil$. Then, rewrite $x=it-j$, where $1\le i\le t$ and $0\le j\le t$. Our equation becomes
$$\begin{array}{lrcll}
&a^{it-j}&\equiv&b&\pmod p\\
\Longrightarrow&a^{it}\cdot a^{-j}&\equiv&b&\pmod p\\
\Longrightarrow&a^{it}&\equiv&ba^j&\pmod p
\end{array}$$

> **Hint.** Since we require that $\gcd(a,p)=1$, $a$ has multiplicative inverse $\pmod p$, which is why the second line follows.

We first enumerate $j=0,1,\cdots,t$, record each value of $ba^j\bmod p$. Second, we enumerate $i=1,2,\cdots,t$, find the largest $j$ such that $a^it\equiv ba^j\pmod p$. The smallest $i$ and the largest $j$ minimize the solution $x$. We can use a binary search tree or a hash table to do that. This algorithm is known as the *Baby-Step Giant-Step (BSGS) algorithm*.

Below is the C++ implementation of *BSGS*.

```C++
/**
 * find the smallest non-negative
 * integer x such that
 * a^x=b (mod p)
 * where a>0, b>0, p>0
 * AND (a,p)=1.
 */
template <typename T>
T BSGS(T a, T b, T p) {
    T t = ceil(sqrt(p)); map<T, T> m;
    // m[x] stores the largest j such that x=b*a^j mod p
    for (T j = 0, x = b % p; j <= t; j++, x = x * a % p) m[x] = j;
    // enumerate x=a^(it) mod p
    for (T i = 1, y = mod_pow(a, t, p), x = y; i <= t; i++, x = x * y % p)
        if (m.find(x) != m.end()) return i * t - m[x];
    return -1;
}
```

The running time of the above algorithm is $O\left(\sqrt{p}\log\sqrt{p}\right)$. If we use a hash table (`unordered_map` in C++), then we speed it up to $O\left(\sqrt{p}+\log\sqrt{p}\right)$.

Now, it is time to extend the above algorithm.

> Given three non-negative integers $a$, $b$ and $p$, find the smallest non-negative integer $x$ such that
> $$a^x\equiv b\pmod p$$

Note that we do not require $\gcd(a,p)=1$. So, in the above algorithm, $a$ does not necessarily have multiplicative inverse $\pmod p$. However, we can get around the problem in the following way.

We are doing some pre-processing work first.

Let's rewrite the equation:
$$a\cdot a^{x-1}+p\cdot y=b$$

This implies that if $d=\gcd(a,p)\nmid b$, then we have no solution for $a^{x-1}$ and $y$. Otherwise, we divide both sides by $d$:
$$\frac{a}{d}\cdot a^{x-1}+\frac{p}{d}\cdot y=\frac{b}{d}$$
Or,
$$\frac{a}{d}\cdot a^{x-1}\equiv\frac{b}{d}\pmod{\frac{p}{d}}$$
Since $\gcd(\frac{a}{d},\frac{p}{d})=1$, $\frac{a}{d}$ has multiplicative inverse $\pmod{\frac{p}{d}}$. So, we have

$$a^{x-1}\equiv\frac{b}{d}\left(\frac{a}{d}\right)^{-1}\pmod{\frac{p}{d}}$$

We again check whether $\gcd(a,\frac{p}{d})=1$ is satisfied. If not, we repeat the above pre-processing work. If it is, then the original *BSGS* can handle it.

Below is the C++ implementation of the extended *BSGS*.

```C++
/**
 * find the smallest non-negative
 * integer x such that
 * a^x=b (mod p)
 * where a>0, b>0, p>0.
 */
template <typename T>
T ext_BSGS(T a, T b, T p) {
    T n = 0, t = 1, d;
    while ((d = gcd(a, p)) != 1) {
        if (b % d) return -1;
        b /= d; p /= d; n++;
        t = t * (a / d) % p;
        if (b % p == t) return n;
    }
    T r = BSGS(a, b * inv(t, p), p);
    return r == -1 ? -1 : r + n;
}
```

The running time of the above code is $O(\log^2p+\sqrt{p})$, assuming that $p$ has $O(\log p)$ prime factors.

> **Hint.** There is a minor, or rather, major problem to figure out before we move on. Haven't you found this line confusing?
> ```
> t = t * (a / d) % p;
> ```
> Does the above code make any sense? We can view `t` as the coefficient of $a^{x-n}$. Originally, we solve for $x-n$ such that
> $$\frac{a}{d_1}\frac{a}{d_2}\cdots\frac{a}{d_n}\cdot a^{x-n}\equiv\frac{b}{d_1d_2\cdots d_n}\pmod{\frac{p}{d_1d_2\cdots d_n}}$$
> But in the code above, we are solving a different equation since the coefficient of $a^{x-n}$ is, in fact,
> $$\left(\cdots\left(\left(\frac{a}{d_1}\bmod\frac{p}{d_1}\right)\frac{a}{d_2}\bmod\frac{p}{d_1d_2}\right)\cdots\right)\frac{a}{d_n}\bmod\frac{p}{d_1d_2\cdots d_n}$$
> So, does this difference really matter? We can simplify the problem. If $ab\bmod\frac{p}{d_1d_2}=\left(a\bmod\frac{p}{d_1}\right)b\bmod\frac{p}{d_1d_2}$, then by induction, the above difference will not make any actual difference. Let $a=q_1\frac{p}{d_1}+q_2\frac{p}{d_1d_2}+r$, where $q_1=\lfloor\frac{a}{p/d_1}\rfloor$, $q_2=\lfloor\frac{p\bmod(a/d_1)}{p/(d_1d_2)}\rfloor$ and $r=a\bmod\left(\frac{p}{d_1d_2}\right)$. Then,
> $$ab\bmod\frac{p}{d_1d_2}=\left(a\bmod\frac{p}{d_1d_2}\right)b\bmod\frac{p}{d_1d_2}=rb\bmod\frac{p}{d_1d_2}$$
> and
> $$\begin{aligned}
\left(a\bmod\frac{p}{d_1}\right)b\bmod\frac{p}{d_1d_2}&=\left(q_2\frac{p}{d_1d_2}+r\right)b\bmod\frac{p}{d_1d_2}\\
&=\left(\left(q_2\frac{p}{d_1d_2}+r\right)\bmod\frac{p}{d_1d_2}\right)b\bmod\frac{p}{d_1d_2}\\
&=rb\bmod\frac{p}{d_1d_2}
\end{aligned}$$
> So,
> $$ab\bmod\frac{p}{d_1d_2}=\left(a\bmod\frac{p}{d_1}\right)b\bmod\frac{p}{d_1d_2}$$
> This explains why the above code is indeed correct.




## References
- [*Primitive roots* definitions, **Theorem 2**](https://brilliant.org/wiki/primitive-roots/)
- [*Primitive roots* additional reference](https://www.cnblogs.com/cytus/p/9296661.html)
- [**Lemma 1**, **Lemma 2**](http://mathonline.wikidot.com/primitive-roots)
- [**Theorem 1**, **Theorem 2**](http://mathworld.wolfram.com/PrimitiveRoot.html)
- [An algorithm to find *primitive roots*](https://cp-algorithms.com/algebra/primitive-root.html)
- [Discrete logarithm](http://mathworld.wolfram.com/DiscreteLogarithm.html)
- [Baby-Step Giant-Step](http://blog.miskcoo.com/2015/05/discrete-logarithm-problem)