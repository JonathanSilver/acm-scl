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

## References
- [*Lucas' theorem*](https://brilliant.org/wiki/lucas-theorem/)
- [Proof for *Lucas' theorem*](https://www.cut-the-knot.org/arithmetic/algebra/LucasTheorem.shtml)
- [Additional reference for proof of *Lucas' theorem*](https://baike.baidu.com/item/lucas/4326261?fr=aladdin)
