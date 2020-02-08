# Notes for Number Theory

*View this note with `VSCode` + `Markdown All In One` or any other ways you like.*


## Euclid's Algorithm
For any nonnegative integer $a$ and any positive integer $b$,
$$\gcd(a, b)=\gcd(b, a \bmod b)$$
In addition, $\gcd(a, 0)=a$.
> **Proof.** We are going to prove that $\gcd(a, b)$ and $\gcd(b, a \bmod b)$ divide each other, so they must be equal (since they are both nonnegative). It is done in the following two steps:  
> - First, we are going prove that $\gcd(a, b)\mid\gcd(b, a \bmod b)$. If we let $d=\gcd(a, b)$, then $d \mid a$ and $d \mid b$. Since $a \bmod b=a- \lfloor a/b \rfloor \cdot b$, we have that $d\mid(a \bmod b)$. Therefore, $\gcd(a, b)\mid\gcd(b, a \bmod b)$.
> - Second, we are going to prove that $\gcd(b, a \bmod b)\mid\gcd(a, b)$, which is quite similar. If we let $d=\gcd(b, a \bmod b)$, then $d \mid b$ and $d\mid(a \bmod b)$. Since $a=(a \bmod b)+\lfloor a/b \rfloor \cdot b$, we have that $d \mid a$. Therefore, $\gcd(b, a \bmod b)\mid\gcd(a, b)$.

Reference:
- *Introduction to Algorithms*. p934.


## Extended Euclid's Algorithm
Compute the integer coefficients $x$ and $y$ such that
$$d=\gcd(a, b)=ax+by$$
Note that $x$ and $y$ may be zero or negative.
> **Observation.** Write down the recursion:
> $$\begin{aligned}
d=\gcd(a, b)&=ax+by\\
d=\gcd(b, a \bmod b)&=bx'+(a \bmod b)y'\\
&=bx'+(a-\lfloor a/b \rfloor \cdot b)y'\\
&=ay'+b(x'-\lfloor a/b \rfloor \cdot y')
\end{aligned}$$
> During the recursion, we first calculate $x'$ and $y'$. Then, we can have $x$ and $y$:
> $$\begin{aligned}
x &\leftarrow y' \\
y &\leftarrow x'-\lfloor a/b \rfloor \cdot y'
\end{aligned}$$

```C++
// read the C++ code for clearer understanding:
// solves: ax + by = gcd(a, b)
int ext_gcd(int a, int b, int & x, int & y) {
    if (b == 0) { // ax = a
        x = 1;
        y = 0;
        return a;
    }
    // y := x'
    // x := y'
    int d = ext_gcd(b, a % b, y, x);
    y -= a / b * x; // y := x' - a / b * y'
    return d;
}
```


## Bezout's Identity
If $a$ and $b$ are any integers, not both zero, then $\gcd(a, b)$ is the smallest positive element of the set $\left\{ax+by:x,y\in\mathbb{Z}\right\}$ of linear combinations of $a$ and $b$.
> **Proof.** Let $s$ be the smallest positive such linear combination of $a$ and $b$, i.e., let $s=ax+by$ for some $x,y\in\mathbb{Z}$. and let $q=\lfloor a/s \rfloor$.
> $$\begin{aligned}
r=a \bmod s&=a-qs\\
&=a-q(ax+by)\\
&=a(1-qx)+b(-qy)
\end{aligned}$$
> So $r$ is a linear combination of $a$ and $b$. However, since $0 \le r < s$ and $s$ is the smallest positive such linear combination, we can know that $r=0$. Therefore, $s \mid a$ and, similarly, $s \mid b$. Thus, $s\mid\gcd(a, b)$. But, since $\gcd(a, b) \mid a$ and $\gcd(a, b) \mid b$, and $s=ax+by$, which implies that $\gcd(a, b) \mid s$, we conclude that $s=\gcd(a, b)$.
  
Reference:
- *Introduction to Algorithms*. p930.


## Solution for Linear Equation
Given any integers $a$ and $b$, not both of them are zero, positive integer $c$, find all the integer solutions to the equation
$$ax+by=c$$

> **Observation 1.** Let $d=\gcd(a, b)$. By the *Bezout's identity*, $d$ is the smallest positive integer of a linear combination of $a$ and $b$ that can represent. So, if $d \nmid c$, then there is no solution.
  
Now, we only need to consider the case when $d \mid c$. Let $k=\frac{c}{d}$, or $c=kd$.

> **Observation 2.** We use the *Euclid's extended algorithm* to find one solution $x'$ and $y'$ to the equation $ax+by=d$, i.e., $ax'+by'=d$, or $a \cdot kx'+b \cdot ky'=kd=c$, which implies that $kx'$ and $ky'$ are one solution to the equation $ax+by=c$.  
  
Since we are looking for all the integer solutions.

> **Solution 1.** We introduce the integer parameters $t$, $\Delta_1$ and $\Delta_2$, and solve $\Delta_1$ and $\Delta_2$:
> $$\begin{matrix}
a(kx'+\Delta_1 \cdot t)+b(ky'-\Delta_2 \cdot t)=c \\
\Longrightarrow a\Delta_1=b\Delta_2 \\
\Longrightarrow\frac{\Delta_1}{\Delta_2}=\frac{b}{a}=\frac{b/d}{a/d}
\end{matrix}$$
> The last step follows because we want to minimize the integer 'intervals' ($\Delta_1$ and $\Delta_2$) between the solutions.  
> Thus, we have the solution:
> $$\left\{\begin{matrix}
x=kx'+\frac{b}{d} \cdot t \\
y=ky'-\frac{a}{d} \cdot t
\end{matrix}\right.$$

However, **solution 1** is too intuitive. Here is another solution:

> **Solution 2.** Let $x_1$ and $y_1$, $x_2$ and $y_2$ be two different solutions to the equation $ax+by=c$, i.e.,
> $$\begin{matrix}
\left\{\begin{matrix}
ax_1+by_1=c \\
ax_2+by_2=c
\end{matrix}\right. \\
\Longrightarrow a(x_1-x_2)=b(y_2-y_1) \\
\Longrightarrow \frac{a}{d}(x_1-x_2)=\frac{b}{d}(y_2-y_1)
\end{matrix}$$
> Since $\gcd(\frac{a}{d}, \frac{b}{d})=1$, we have that $\frac{b}{d}\mid(x_1-x_2)$ and $\frac{a}{d}\mid(y_2-y_1)$, i.e.,
> $$\left\{\begin{matrix}
x_1-x_2=\frac{b}{d} \cdot t_1 \\
y_2-y_1=\frac{a}{d} \cdot t_2
\end{matrix}\right.$$
> This suggests that if we have one solution $x_0$ and $y_0$ to the equation $ax+by=c$, i.e., $ax_0+by_0=c$, we can have all the integer solutions:
> $$\left\{\begin{matrix}
x=x_0+\frac{b}{d} \cdot t_1 \\
y=y_0-\frac{a}{d} \cdot t_2
\end{matrix}\right.$$
> Since
> $$\begin{matrix}
a(x_0+\frac{b}{d} \cdot t_1)+b(y_0-\frac{a}{d} \cdot t_2)=c \\
\Longrightarrow ax_0+\frac{ab}{d} \cdot t_1 + by_0-\frac{ab}{d} \cdot t_2 = c \\
\Longrightarrow \frac{ab}{d} \cdot t_1 = \frac{ab}{d} \cdot t_2
\end{matrix}$$
> Therefore
> $$t_1=t_2$$
> Let
> $$t=t_1=t_2$$
> Lastly, according to **observation 2**, $kx'$ and $ky'$ are one solution to the equation $ax+by=c$.  
> We conclude that
> $$\left\{\begin{matrix}
x=kx'+\frac{b}{d} \cdot t \\
y=ky'-\frac{a}{d} \cdot t
\end{matrix}\right.$$


## Chinese Remainder Theorem (CRT)
Given pairwise coprime positive integers $n_1$, $n_2$, ..., $n_k$ and arbitrary integers $a_1$, $a_2$, ..., $a_k$, the system of simultaneous congruences
$$\left\{
\begin{matrix}
x \equiv a_1 \pmod {n_1} \\
x \equiv a_2 \pmod {n_2} \\
\vdots \\
x \equiv a_k \pmod {n_k}
\end{matrix}
\right.$$
has a solution, and the solution is unique modulo $N=n_1n_2 \cdots n_k$.
> **Solution.** The following is a general construction to find a solution to a system of congruences using CRT:
> 1. Compute $N=n_1n_2 \cdots n_k$.
> 2. Let $y_i=\frac{N}{n_i}$, for $i=1, 2, \cdots, k$.
> 3. Compute $z_i \equiv y_{i}^{-1} \pmod {n_i}$, for $i=1, 2, \cdots, k$, using *Euclid's extended algorithm* ($z_i$ exists since $n_1$, $n_2$, ..., $n_k$ are pairwise coprime).
> 4. The integer $x=\sum_{i=1}^{k}{a_iy_iz_i}$ is a solution to the system of congruences, and $x \bmod N$ is the unique solution modulo $N$.
  
> **Proof.** To see why $x$ is a solution, for $i=1, 2, \cdots, k$, we have
> $$\begin{aligned}
x&\equiv\sum_{j=1}^{k}{a_jy_jz_j}&\pmod{n_i}\\
&\equiv a_iy_iz_i&\pmod{n_i}\\
&\equiv a_i&\pmod{n_i}
\end{aligned}$$
> where the second line follows since $y_j \equiv 0 \pmod{n_i}$ for each $j \ne i$, and the third line follows since $y_iz_i \equiv 1 \pmod{n_i}$.  
> Now, suppose there are two different solutions $u$ and $v$ to the system of congruences ($u \ne v$). Then $n_1\mid(u-v), n_2\mid(u-v), \cdots, n_k\mid(u-v)$, and since $n_1$, $n_2$, ..., $n_k$ are relatively prime, we have that $n_1n_2 \cdots n_k$ divides $u-v$, or
> $$u \equiv v \pmod{n_1n_2 \cdots n_k}$$
> Thus, the solution is unique modulo $n_1n_2 \cdots n_k$.
  
> **Hint.** In order to compute $x \equiv a^{-1} \pmod n$, we can construct the equation:
> $$ax+ny=1$$
> Equivalently,
> $$ax \equiv 1 \pmod n$$
> The equation can be solved by *Euclid's extended algorithm*. The solution $x$ exists if and only if $\gcd(a, n)=1$.

Reference:
- [Theorem, Solution & Proof](https://brilliant.org/wiki/chinese-remainder-theorem/)


## Euler's Totient Function
The *totient function* $\varphi(n)$, also called *Euler's totient function*, is defined as the number of positive integers no more than $n$ that are relatively prime to (i.e., do not contain any factor in common with) $n$, where $1$ is counted as being relatively prime to all numbers.
$$\begin{aligned}
\varphi(n)&=n\prod_{p|n}{\left(1-\frac{1}{p}\right)}\\
&=n\left(1-\frac{1}{p_1}\right)\left(1-\frac{1}{p_2}\right)\cdots\left(1-\frac{1}{p_k}\right)
\end{aligned}$$
where the product runs over all primes $p$ dividing $n$.
> **Proof.** First, we need to prove that $\varphi(n)$ is *multiplicative*, i.e.,
> $$\left\{\begin{matrix}
\varphi(1)=1 & \\
\varphi(n_1n_2)=\varphi(n_1)\varphi(n_2) & \gcd(n_1, n_2)=1
\end{matrix}\right.$$
> If $n=1$, according to the definition, it is obvious that $\varphi(1)=1$.  
> If $n>1$ is not a prime power, we can write $n=n_1n_2$ where $\gcd(n_1, n_2)=1$. Then the numbers $0 \le x < n$ can be represented in a residue number as
> $$\left\{\begin{matrix}
    x \equiv r_1 \pmod{n_1} & 0 \le r_1 < n_1 \\
    x \equiv r_2 \pmod{n_2} & 0 \le r_2 < n_2
\end{matrix}\right.$$
> Now, we want to know the number of $x$'s such that $\gcd(x, n)=1$.  
> Since
> $$\begin{matrix}
\left\{\begin{matrix}
\gcd(x, n)=\gcd(x, n_1n_2)=1 \\
\gcd(n_1, n_2)=1
\end{matrix}\right. \\
\iff
\left\{\begin{matrix}
\gcd(x, n_1)=1 \\
\gcd(x, n_2)=1
\end{matrix}\right. \\
\iff
\left\{\begin{matrix}
\gcd(x \bmod n_1, n_1)=1 \\
\gcd(x \bmod n_2, n_2)=1
\end{matrix}\right. \\
\iff
\left\{\begin{matrix}
\gcd(r_1, n_1)=1 \\
\gcd(r_2, n_2)=1
\end{matrix}\right.
\end{matrix}$$
> and we have $\varphi(n_1)$ choices of $r_1$'s and $\varphi(n_2)$ choices of $r_2$'s such that
> $$\left\{\begin{matrix}
\gcd(r_1, n_1)=1 \\
\gcd(r_2, n_2)=1
\end{matrix}\right.$$
> which makes a total of $\varphi(n_1)\varphi(n_2)$ **distinct** choices of $x$'s (by *CRT*) such that $\gcd(x, n)=1$.  
> Therefore, $\varphi(n)=\varphi(n_1)\varphi(n_2), \; \gcd(n_1, n_2)=1$.  
> Now, we try to prove the original formula.  
> For a prime $p$,
> $$\varphi(p)=p-1$$
> since all numbers less than $p$ are relatively prime to $p$.  
> If $n=p^e$ is a power of a prime, then the numbers that have a common factor with $n$ are the multiples of $p$: $p$, $2p$, ..., $\left(p^{e-1}\right)p$. There are $p^{e-1}$ of these multiples, so the number of factors relatively prime to $p^e$ is
> $$\begin{aligned}
\varphi\left(p^e\right)&=p^e-p^{e-1}\\
&=p^e\left(1-\frac{1}{p}\right)
\end{aligned}$$
> Since we have proved that $\varphi(n)$ is multiplicative. A multiplicative function is defined completely by its values at prime powers, because we can decompose any positive integer $n$ into its prime-power factors, which are relatively prime to each other.  
> Therefore, if
> $$n=\prod_{p}{p^{e_p}}$$
> then
> $$\begin{aligned}
\varphi(n)&=\prod_{p}{\varphi(p^{e_p})}\\
&=\prod_{p}{p^{e_p}\left(1-\frac{1}{p}\right)}\\
&=n\prod_{p}{\left(1-\frac{1}{p}\right)}
\end{aligned}$$

References:
- [Totient Function & Proof](http://mathworld.wolfram.com/TotientFunction.html)
- *Concrete Mathematics*. p133-134.


## Euler's Theorem
In number theory, *Euler's theorem* (a.k.a. the *Fermat-Euler theorem* or *Euler's totient theorem*) states that if $n$ and $a$ are coprime positive integers, then
$$a^{\varphi(n)} \equiv 1 \pmod n$$
where $\varphi(n)$ is *Euler's totient function*.
> **Proof.** Let $r_1$, $r_2$, ..., $r_{\varphi(n)}$ be all the distinct positive integers which are no more than $n$ and are relatively prime to $n$.  
> We multiply them by $a$, and we have $ar_1$, $ar_2$, ..., $ar_{\varphi(n)}$.  
> First, we try to prove that $ar_i  \bmod n \ne ar_j \bmod n$, for any $i$ and $j$ ($i \ne j$).  
>> **Proof.** (By Contradiction.) Assume for some $i$ and $j$ ($i \ne j$), $ar_i  \bmod n = ar_j \bmod n$, i.e., $ar_i \equiv ar_j \pmod n$. Since $n$ and $a$ are coprime to each other, we have $r_i \equiv r_j \pmod n$. Equivalently, $r_i-r_j=qn$. Thus, $r_i=r_j$, because both $r_i$ and $r_j$ are positive integers no more than $n$, which makes $q=0$. This contradicts the condition that $r_i \ne r_j$ for any $i \ne j$.
> 
> Second, we try to prove that $\gcd(ar_i \bmod n, n)=1$, for any $i$.  
>> **Proof.** We know that $\gcd(ar_i, n)=1$ because $\gcd(a, n)=1$ and $\gcd(r_i, n)=1$. By the *Euclid's algorithm*, $\gcd(ar_i, n)=\gcd(n, ar_i \bmod n)=1$.
> 
> With the above proofs, we conclude that $\left\{ar_1 \bmod n, ar_2 \bmod n, \cdots, ar_{\varphi(n)} \bmod n \right\}=\left\{r_1, r_2, \cdots, r_{\varphi(n)}\right\}$.  
> Therefore,
> $$\begin{aligned}
ar_1 ar_2 \cdots ar_{\varphi(n)} & \equiv r_1 r_2 \cdots r_{\varphi(n)} & \pmod n \\
a^{\varphi(n)}r_1 r_2 \cdots r_{\varphi(n)} & \equiv r_1 r_2 \cdots r_{\varphi(n)} & \pmod n \\
a^{\varphi(n)} & \equiv 1 & \pmod n
\end{aligned}$$
> where the cancellation of $r_i$ is allowed because they all have multiplicative inverses $\pmod n$.

References:
- [Proof for the *Euler's Theorem*](https://brilliant.org/wiki/eulers-theorem/)
- [Additional reference for the proof of the *Euler's Theorem*](https://www.cnblogs.com/wangxiaodai/p/9758242.html)


## Generalized Form of the Euler's Theorem
$$\begin{array}{l}
a^n \bmod m \\
=\left\{\begin{matrix}
a^{n\bmod\varphi(m)} \bmod m & \gcd(a, m)=1 \\
a^n \bmod m & \gcd(a, m) \ne 1, n<\varphi(m) \\
a^{\left(n\bmod\varphi(m)\right)+\varphi(m)} \bmod m & \gcd(a, m) \ne 1, n\ge\varphi(m)
\end{matrix}\right.
\end{array}$$
> **Proof.** The first equation directly derives from the *Euler's theorem*:
> $$\begin{aligned}
a^n \bmod m&=a^{q\varphi(m)+r} & \bmod m \\
&=(a^q)^{\varphi(m)} \cdot a^r & \bmod m \\
&=a^r & \bmod m
\end{aligned}$$
> where $q=\lfloor\frac{n}{\varphi(m)}\rfloor$, $r=n-q\varphi(m)=n \bmod \varphi(m)$.  
> The last line follows because $(a^q)^{\varphi(m)} \equiv 1 \pmod m$ by the *Euler's theorem*. $\gcd(a^q, m)=1$ since $\gcd(a, m)=1$.  
> We don't need to prove the second equation. Now, we come to the third equation.  
> First, we decompose $m$ such that $m=s \times p^r$, $\gcd(s, p)=1$, where $p$ is a prime, $r>0$, and $s \ge 1$.  
> By the *Euler's theorem*, we have $p^{\varphi(s)} \equiv 1 \pmod s$. Since $\gcd(s, p)=1\Longrightarrow\gcd(s, p^r)=1$, by the definition of the *Euler's totient function*, $\varphi(m)=\varphi(s)\cdot\varphi(p^r)$.  
> Then,
> $$\begin{aligned}
p^{\varphi(m)} & \equiv p^{\varphi(s)\cdot\varphi(p^r)} & \pmod s \\
& \equiv \left(p^{\varphi(p^r)}\right)^{\varphi(s)} & \pmod s \\
& \equiv 1 & \pmod s
\end{aligned}$$
> Or,
> $$p^{\varphi(m)}-1=q \cdot s$$
> Multiply both sides by $p^r$,
> $$p^{\varphi(m)+r}-p^r=q \cdot s \times p^r$$
> Equivalently,
> $$p^{\varphi(m)+r} \equiv p^r \pmod m$$
> We can use the above equation as long as $r>0$. In other words, we need to **keep the exponent from zero**.  
> Second, we try to generalize the above equation a little bit so that the base can be prime powers:
> $$\begin{aligned}
\left(p^e\right)^r & \equiv p^{er} & \pmod m \\
& \equiv p^{er+\varphi(m)} & \pmod m \\
& \equiv p^{er+2\varphi(m)} & \pmod m \\
& \cdots \\
& \equiv p^{er+e\varphi(m)} & \pmod m \\
& \equiv \left(p^e\right)^{\varphi(m)+r} & \pmod m
\end{aligned}$$
> where $e>0$ and $r>0$.  
> Now, for $a=\prod_{p}{p^{e_p}}$, $r>0$,
> $$\begin{aligned}
a^r & \equiv \left(\prod_{p}{p^{e_p}}\right)^r & \pmod m \\
& \equiv \prod_{p}{\left(p^{e_p}\right)^r} & \pmod m \\
& \equiv \prod_{p}{\left(p^{e_p}\right)^{\varphi(m)+r}} & \pmod m \\
& \equiv \left(\prod_{p}{p^{e_p}}\right)^{\varphi(m)+r} & \pmod m \\
& \equiv a^{\varphi(m)+r} & \pmod m
\end{aligned}$$
> Thus, if $\varphi(m) \nmid n$, we can have
> $$a^n \equiv a^{n\bmod\varphi(m)} \pmod m$$
> We can generalize the equation in the following way so that the constraint is no longer needed:
> $$a^n \equiv a^{\left(n\bmod\varphi(m)\right)+\varphi(m)} \pmod m$$
> Adding an extra $\varphi(m)$ in the exponent perfectly solves the problem.

Reference:
- [Generalized Form of the *Euler's Theorem*](https://blog.csdn.net/hzj1054689699/article/details/80693756)


## Fermat's Little Theorem
*Fermat's little theorem* states that if $p$ is a prime number, then for any integer $a$,
$$a^p \equiv a \pmod p.$$
If $a$ is not divisible by $p$,
$$a^{p-1} \equiv 1 \pmod p.$$





## References
- Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein. *Introduction to Algorithms*, 3rd ed.
- Ronald L. Graham, Donald E. Knuth, Oren Patashnik. *Concrete Mathematics*, 2nd ed.
