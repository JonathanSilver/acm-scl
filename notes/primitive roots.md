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


## References
- [*Primitive roots* definitions, **Theorem 2**](https://brilliant.org/wiki/primitive-roots/)
- [*Primitive roots* additional reference](https://www.cnblogs.com/cytus/p/9296661.html)
- [**Lemma 1**, **Lemma 2**](http://mathonline.wikidot.com/primitive-roots)
- [**Theorem 1**, **Theorem 2**](http://mathworld.wolfram.com/PrimitiveRoot.html)
- [An algorithm to find *primitive roots*](https://cp-algorithms.com/algebra/primitive-root.html)