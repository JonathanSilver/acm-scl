# Notes for FFT

*View this note with `VSCode` + `Markdown All in One` or any other ways you like.*

*This is an quite interesting topic. I'm not familiar with the real-world application of FFT, so what I have here is simply the mathematical idea behind it.*

## Fast Fourier Transform (FFT)

> **Problem Statement.** Given
> $$A(x)=\sum_{i=0}^{n-1}{a_ix^i}$$
> $$B(x)=\sum_{i=0}^{n-1}{b_ix^i}$$
> Calculate
> $$C(x)=A(x)B(x)=\sum_{i=0}^{n-1}{c_ix^i}$$
> where
> $$c_i=\sum_{k=0}^{i}{a_kb_{i-k}}$$

> **Hint.** If you find the above equations impossible, note that I never said $a_{n-1}\ne 0$ or $b_{n-1}\ne 0$, which suggests that you can always *pad* enough zeros or use a larger $n$ so that the above equations are satisfied.
> 
> Here is an example:
> $$A(x)=2x^2$$
> $$B(x)=1+x$$
> In this case, let $n=4$ and rewrite
> $$A(x)=0+0x^1+2x^2+0x^3$$
> $$B(x)=1+1x^1+0x^2+0x^3$$
> See?
> $$C(x)=0+0x^1+2x^2+2x^3$$

Now, you should be wondering what makes this problem difficult. It must take you no time to figure out the $O(n^2)$ solution. But, can you speed it up?

We must admit, in the above representation of polynomials, calculating the products is quite inefficient.

Wait a minute. What is the representation of polynomials? We call $A(x)=\sum_{i=0}^{n-1}{a_ix^i}$ the *coefficient representation* of a polynomial, since a vector of coefficients $a=(a_0,a_1,\cdots,a_{n-1})$ is enough to determine the polynomial.

Now, consider a different representation of polynomials. For a set of $n$ point-value pairs $\{(x_0,y_0),(x_1,y_1),\cdots,(x_{n-1},y_{n-1})\}$, ($x_i\ne x_j$, $i\ne j$), if $y_k=A(x_k)$ ($k=0,1,\cdots n-1$), then it is the *point-value representation* of $A(x)$.

This raises a new question. Can a $n$ point-value pairs uniquely determine the polynomial?

Definitely yes. Check out the matrix equation below.

$$\begin{pmatrix}1&x_0&x_0^2&\cdots&x_0^{n-1}\\1&x_1&x_1^2&\cdots&x_1^{n-1}\\ \vdots&\vdots&\vdots&\ddots&\vdots\\1&x_{n-1}&x_{n-1}^2&\cdots&x_{n-1}^{n-1}\end{pmatrix}\begin{pmatrix}a_0\\a_1\\ \vdots\\a_{n-1}\end{pmatrix}=\begin{pmatrix}y_0\\y_1\\ \vdots\\y_{n-1}\end{pmatrix}$$

Does the leftmost matrix look familiar? I will remind you of the *Vander-monde* matrix from linear algebra, which has determinant

$$\prod_{0\le i<j\le n-1}{(x_j-x_i)}$$

Since $x_0$, $x_1$, ..., $x_{n-1}$ are pairwise distinct, $a_0$, $a_1$, ..., $a_{n-1}$ can be uniquely determined.

Now, I believe you have known what makes this representation better. If $\{(x_0,y_0),(x_1,y_1),\cdots,(x_{n-1},y_{n-1})\}$ is the *point-value representation* for $A(x)$, and $\{(x_0,y_0'),(x_1,y_1'),\cdots,(x_{n-1},y_{n-1}')\}$ is the *point-value representation* for $B(x)$, then $\{(x_0,y_0y_0'),(x_1,y_1y_1'),\cdots,(x_{n-1},y_{n-1}y_{n-1}')\}$ is the *point-value representation* for $C(x)=A(x)B(x)$. This can be done in $O(n)$.

Here comes the final problem. How do we quickly convert *coefficient representation* to *point-value representation* and vice versa?

We introduce the *complex $n$-th root of unity*, which is a complex number $\omega_n^k$ such that

$$\left(\omega_n^k\right)^n=1$$

There are exactly $n$ complex $n$-th roots of unity

$$\omega_n^k=e^{2k\pi i/n}$$

where $k=0,1,\cdots,n-1$ and $i=\sqrt{-1}$.

> **Hint.** Since,
> $$e^{ix}=\cos{x}+i\sin{x}$$
> Thus,
> $$\left(\omega_n^k\right)^n=e^{2k\pi i}=\cos{2k\pi}+i\sin{2k\pi}=1$$

$\omega_n^k$ has some interesting properties:

1. $$\omega_{dn}^{dk}=e^{\frac{2dk\pi i}{dn}}=e^{\frac{2k\pi i}{n}}=\omega_n^k$$
2. If $n$ is even,
   - $$\omega_n^{n/2}=e^{2\frac{n}{2}\pi i/n}=e^{\pi i}=\cos{\pi}+i\sin{\pi}=-1$$
   - $$\omega_n^{k+n/2}=\omega_n^k\omega_n^{n/2}=-\omega_n^k$$

I can tell you what we are doing next. Let $x_k=\omega_n^k$. We are going to calculate

$$y_k=A(\omega_n^k)=\sum_{i=0}^{n-1}{a_i\omega_n^{ki}}$$

In order to quickly calculate all the $y_k$. We are going to apply the divide and conquer technique. Now, assume $n=2^m$ ($m>0$).

Let

$$A^{[0]}(x)=a_0+a_2x+a_4x^2+\cdots+a_{n-2}x^{n/2-1}$$
$$A^{[1]}(x)=a_1+a_3x+a_5x^2+\cdots+a_{n-1}x^{n/2-1}$$

> **Hint.** $A^{[0]}$ contains all the even-indexed coefficients of $A$ (the binary representation of the index ends in $0$) and $A^{[1]}$ contains all the odd-indexed coefficients (the binary representation of the index ends in $1$).

Then,

$$A(x)=A^{[0]}(x^2)+xA^{[1]}(x^2)$$

Or,

$$\begin{aligned}A(\omega_n^k)&=A^{[0]}\left(\omega_n^{2k}\right)+\omega_n^kA^{[1]}\left(\omega_n^{2k}\right)\\
&=A^{[0]}\left(\omega_{n/2}^k\right)+\omega_n^kA^{[1]}\left(\omega_{n/2}^k\right)\end{aligned}$$

> **Hint.** This uses the first property of $\omega_n^k$.

And,

$$\begin{aligned}A(\omega_n^{k+n/2})&=A^{[0]}\left(\left(\omega_n^{k+n/2}\right)^2\right)+\omega_n^{k+n/2}A^{[1]}\left(\left(\omega_n^{k+n/2}\right)^2\right)\\
&=A^{[0]}\left(\left(-\omega_n^k\right)^2\right)-\omega_n^kA^{[1]}\left(\left(-\omega_n^k\right)^2\right)\\
&=A^{[0]}\left(\omega_n^{2k}\right)-\omega_n^kA^{[1]}\left(\omega_n^{2k}\right)\\
&=A^{[0]}\left(\omega_{n/2}^k\right)-\omega_n^kA^{[1]}\left(\omega_{n/2}^k\right)\end{aligned}$$

> **Hint.** This uses the second property.

Note that $k=0,1,\cdots,n/2-1$.

So, we have the following recursive algorithm, which accepts $a=(a_0,a_1,\cdots,a_{n-1})$ and length $n$ as the input.

RECURSIVE-FFT($a$, $n$):

1. **if** $n$ == 1:
   1. **return** $a$
2. $\omega_n=e^{2\pi i/n}$
3. $\omega=1$
4. $a^{[0]}=(a_0,a_2,\cdots,a_{n-2})$
5. $a^{[1]}=(a_1,a_3,\cdots,a_{n-1})$
6. $y^{[0]}=$ RECURSIVE-FFT($a^{[0]}$, $n/2$)
7. $y^{[1]}=$ RECURSIVE-FFT($a^{[1]}$, $n/2$)
8. **for** $k=0..n/2-1$:
   1. $y_k=y_k^{[0]}+\omega y_k^{[1]}$
   2. $y_{k+n/2}=y_k^{[0]}-\omega y_k^{[1]}$
   3. $\omega=\omega\omega_n$
9. **return** $y$

> **Hint.** If $n=1$, this would be the case when we are handling $A(x)=a_0$. No matter what $x$ is, $y=a_0$. We just return $a$.

The time complexity is obviously $O(n\log n)$. But, it is NOT a good idea to allocate memory at such a frequent basis.

Can you know the positions of all the elements at the beginning and calculate the values in a bottom-up manner?

You should pay attention to the binary representation of the even/odd indices.

> **Example.** If $n=16$, $a=(a_{0000},a_{0001},\cdots,a_{1111})$. Indices are in the binary representation. There are $4$ bits. Let's denote them as bit $0$, bit $1$, bit $2$, and bit $3$, with the least significant bit denoted by bit $0$, the most significant bit denoted by bit $3$.
> 
> Depth $0$ ($len=16$):
> $$(a_{0000},a_{0010},a_{0100},a_{0110},a_{1000},a_{1010},a_{1100},a_{1110})$$
> $$(a_{0001},a_{0011},a_{0101},a_{0111},a_{1001},a_{1011},a_{1101},a_{1111})$$
> *Look at bit $0$.*
> 
> Depth $1$ ($len=8$):
> $$A:(a_{0000},a_{0100},a_{1000},a_{1100}),(a_{0010},a_{0110},a_{1010},a_{1110})$$
> $$B:(a_{0001},a_{0101},a_{1001},a_{1101}),(a_{0011},a_{0111},a_{1011},a_{1111})$$
> *Look at bit $1$. Note that bit $0$ of all the elements is the same, due to the work at depth $0$. ($A$ and $B$ are two separate function calls).*
> 
> Depth $2$ ($len=4$):
> $$A:(a_{0000},a_{1000}),(a_{0100},a_{1100});B:(a_{0010},a_{1010}),(a_{0110},a_{1110})$$
> $$C:(a_{0001},a_{1001}),(a_{0101},a_{1101});D:(a_{0011},a_{1011}),(a_{0111},a_{1111})$$
> *Look at bit $2$. Note that bit $0$ and bit $1$ of all the elements are the same.*
> 
> Depth $3$ ($len=2$):
> $$A:a_{0000},a_{1000};B:a_{0100},a_{1100};C:a_{0010},a_{1010};D:a_{0110},a_{1110}$$
> $$E:a_{0001},a_{1001};F:a_{0101},a_{1101};G:a_{0011},a_{1011};H:a_{0111},a_{1111}$$
> *Look at bit $3$, though it doesn't matter... Note that bit $0$, bit $1$ and bit $2$ of all the elements are the same.*
> 
> So, at depth $d$, due to prior work, bit $0$, bit $1$, ..., bit $d-1$ of all the elements are the same. You only need to look at bit $d$ to determine where to place the element. When you iterate through all the bits, you can uniquely determine the position.
> 
> You may draw a binary tree, with edges connecting parent and left child labeled $0$ and edges connecting parent and right child labeled $1$.
> 
> For example, we want to know where to place the element whose index is $0101$. There are $16$ elements in total. Since bit $0$ is $1$, we know there will be at least $8$ elements placed before it. Bit $1$ is $0$, which means nothing happens. (If bit $1$ is $1$, there will be at least $4$ elements placed before it.) Bit $2$ is $1$, so there will be $2$ more elements placed before it. Finally, bit $3$ is $0$. There are $8+2=10$ elements placed before it, which means $0101$'s new index is $10$.

I think the example is straight forward. You just reverse all the bits!

Before I show you the C++ code, we also need to find a way to convert *point-value representation* back to *coefficient representation*.

Let's review the matrix equation with proper value filled.

$$\begin{pmatrix}1&1&1&\cdots&1\\1&\omega_n&\omega_n^2&\cdots&\omega_n^{n-1}\\ \vdots&\vdots&\vdots&\ddots&\vdots\\1&\omega_n^{n-1}&\omega_n^{2(n-1)}&\cdots&\omega_n^{(n-1)(n-1)}\end{pmatrix}\begin{pmatrix}a_0\\a_1\\ \vdots\\a_{n-1}\end{pmatrix}=\begin{pmatrix}y_0\\y_1\\ \vdots\\y_{n-1}\end{pmatrix}$$

Is there a way to rewrite the equation like this?

$$\begin{pmatrix}a_0\\a_1\\ \vdots\\a_{n-1}\end{pmatrix}=\begin{pmatrix}1&1&1&\cdots&1\\1&\omega_n&\omega_n^2&\cdots&\omega_n^{n-1}\\ \vdots&\vdots&\vdots&\ddots&\vdots\\1&\omega_n^{n-1}&\omega_n^{2(n-1)}&\cdots&\omega_n^{(n-1)(n-1)}\end{pmatrix}^{-1}\begin{pmatrix}y_0\\y_1\\ \vdots\\y_{n-1}\end{pmatrix}$$

Let

$$V=\begin{pmatrix}1&1&1&\cdots&1\\1&\omega_n&\omega_n^2&\cdots&\omega_n^{n-1}\\ \vdots&\vdots&\vdots&\ddots&\vdots\\1&\omega_n^{n-1}&\omega_n^{2(n-1)}&\cdots&\omega_n^{(n-1)(n-1)}\end{pmatrix}$$

And

$$V_{i,j}=\omega_n^{ij}, (0\le i,j\le n-1)$$

In fact,

$$V_{i,j}^{-1}=\omega_n^{-ij}/n$$

such that 

$$V^{-1}V=I$$

where $I$ is the $n\times n$ identity matrix.

This is because

$$\begin{aligned}[V^{-1}V]_{i,j}&=\frac{1}{n}\sum_{k=0}^{n-1}{\omega_n^{-ik}\omega_n^{kj}}\\&=\frac{1}{n}\sum_{k=0}^{n-1}{\omega_n^{k(j-i)}}\\&=\left\{\begin{matrix}1,&i=j\\0,&i\ne j\end{matrix}\right.\end{aligned}$$

If $i\ne j$,

$$\begin{aligned}\sum_{k=0}^{n-1}{\left(\omega_n^{j-i}\right)^k}&=\frac{1-\left(\omega_n^{j-i}\right)^n}{1-\omega_n^{j-i}}\\&=\frac{1-1}{1-\omega_n^{j-i}}\\&=0\end{aligned}$$

Let's go back to the matrix equation.

$$\begin{pmatrix}a_0\\a_1\\ \vdots\\a_{n-1}\end{pmatrix}=\frac{1}{n}\begin{pmatrix}1&1&1&\cdots&1\\1&\omega_n^{-1}&\omega_n^{-2}&\cdots&\omega_n^{-(n-1)}\\ \vdots&\vdots&\vdots&\ddots&\vdots\\1&\omega_n^{-(n-1)}&\omega_n^{-2(n-1)}&\cdots&\omega_n^{-(n-1)(n-1)}\end{pmatrix}\begin{pmatrix}y_0\\y_1\\ \vdots\\y_{n-1}\end{pmatrix}$$

If FFT is capable of determining $y$ from $a$, then it can also determine $na$ from $y$, if only $\omega_n^{-k}$ satisfies certain conditions. I will not elaborate since $\omega_n^{-k}$ shares similar properties with $\omega_n^k$.

Here is the C++ implementation of FFT.

```C++
// reverses bits.
int rev(int idx, int n) {
    int r = 0;
    for (int i = 0; (1 << i) < n; i++) {
        r <<= 1;
        if (idx & (1 << i)) r |= 1;
    }
    return r;
}
/**
 * FFT - Fast Fourier Transform
 * Complexity: O(n log(n))
 *
 * Parameters:
 * If op ==  1, convert coefficient representation to point-value representation,
 * or op == -1, convert point-value representation back to coefficient representation.
 */
vector<complex<double> > FFT(const vector<complex<double> > & a, int op) {
    int n = a.size(); // the length must be a power of 2.
    vector<complex<double> > v(n);
    for (int i = 0; i < n; i++) v[rev(i, n)] = a[i];
    for (int s = 1; (1 << s) <= n; s++) {
        int m = (1 << s);
        complex<double> wm = complex<double>(cos(op * 2 * acos(-1) / m), sin(op * 2 * acos(-1) / m));
        for (int k = 0; k < n; k += m) {
            complex<double> w = complex<double>(1, 0);
            for (int j = 0; j < (m >> 1); j++) {
                complex<double> t = w * v[k + j + (m >> 1)];
                complex<double> u = v[k + j];
                v[k + j] = u + t;
                v[k + j + (m >> 1)] = u - t;
                w = w * wm;
            }
        }
    }
    if (op == -1)
        for (int i = 0; i < n; i++)
            v[i] = complex<double>(v[i].real() / n, v[i].imag() / n);
    return v;
}
```

The above code takes advantage of the C++ STL. However, `complex<double>` is slow. You can implement you own `complex` as follows.

```C++
struct complex {
    double r, i;
    complex(double a = 0, double b = 0) : r(a), i(b) { }
    double real() const { return r; }
    double imag() const { return i; }
    complex operator*(const complex & b) const {
        return complex(r * b.r - i * b.i, r * b.i + i * b.r);
    }
    complex operator+(const complex & b) const {
        return complex(r + b.r, i + b.i);
    }
    complex operator-(const complex & b) const {
        return complex(r - b.r, i - b.i);
    }
};
```

References:
- Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, et al. *Introduction to Algorithms*, 3rd ed. pp 898-925.


## Number-Theoretic Transform (NTT)

As we can see, FFT suffers from losing precision. Some clever people have extended FFT to better suit integer type.

Here is a summary of the conditions required to perform FFT-like divide and conquer algorithm.

For an even positive integer $n$ and for any $k=0,1,\cdots,n-1$:

1. $\omega_n^{2k}=\omega_{n/2}^k$.
2. $\omega_n^{n/2}=-1$.
3. $\omega_n^{-k}$ shares similar properties with $\omega_n^k$.

Let

$$\omega_n=g^{\frac{p-1}{n}}$$

where $g$ is the *primitive root* $\pmod p$ and $p$ is a prime with the form $a2^b+1$ ($2^b\ge n$).

> **Hint.** I suggest you learn about [*primitive root*](primitive%20roots.md) before you go on.

Let's look at the conditions above one by one:

1. $$\begin{aligned}\omega_n^{2k}&=\left(g^{\frac{p-1}{n}}\right)^{2k}\\&=\left(g^{\frac{p-1}{n/2}}\right)^k\\&=\omega_{n/2}^k\end{aligned}$$
2. $$\begin{aligned}\omega_n^{n/2}&\equiv\left(g^{\frac{p-1}{n}}\right)^{\frac{n}{2}}\\&\equiv g^{\frac{p-1}{2}}\pmod p\end{aligned}$$
   Let
   $$x\equiv g^{\frac{p-1}{2}}\pmod p$$
   Since,
   $$g^{p-1}\equiv 1\pmod p$$
   We have
   $$\begin{array}{rrlc}&x^2&\equiv 1&\pmod p\\ \Longrightarrow&(x-1)(x+1)&\equiv 0&\pmod p\\ \Longrightarrow&p\mid(x-1)&or&p\mid(x+1)\\ \Longrightarrow&x&\equiv\pm 1&\pmod p\end{array}$$
   But, if $x\equiv 1\pmod p$, then
   $$g^{\frac{p-1}{2}}\equiv 1\pmod p$$
   which contradicts the definition of *primitive root*.

   Thus,
   $$\omega_n^{n/2}\equiv g^{\frac{p-1}{2}}\equiv -1\pmod p$$
3. Since $p$ is an odd prime,
   $$\omega_n^{-k}\equiv\left(\omega_n^k\right)^{p-2}$$
   And $p-2$ is also odd.

   As for the second property,
   $$\omega_n^{-n/2}\equiv(-1)^{p-2}\equiv -1\pmod p$$

This completes the proofs. So we can happily extend FFT to NTT.

```C++
// T is a proper integer type like int64.
// mod_pow(a, n, p): a^n % p.
// p is a prime and
// p = r * 2^k + 1
//
// examples:
// 23068673 = 11 * 2^21 + 1
// 104857601 = 25 * 2^22 + 1
// 167772161 = 5 * 2^25 + 1
// 469762049 = 7 * 2^26 + 1
// 998244353 = 119 * 2^23 + 1
// 1004535809 = 479 * 2^21 + 1
template <typename T>
vector<T> NTT(const vector<T> & a, T p, int op) {
    int n = a.size(); // the length must be a power of 2.
    T g = primitive_root(p);
    vector<T> v(n);
    for (int i = 0; i < n; i++) v[rev(i, n)] = a[i];
    for (int s = 1; (1 << s) <= n; s++) {
        int m = (1 << s);
        T wm = mod_pow(g, (p - 1) / m, p);
        if (op == -1) wm = mod_pow(wm, p - 2, p);
        for (int k = 0; k < n; k += m) {
            T w = 1;
            for (int j = 0; j < (m >> 1); j++) {
                T t = w * v[k + j + (m >> 1)] % p;
                T u = v[k + j] % p;
                v[k + j] = (u + t) % p;
                v[k + j + (m >> 1)] = ((u - t) % p + p) % p;
                w = w * wm % p;
            }
        }
    }
    if (op == -1) {
        T inv = mod_pow(n, p - 2, p);
        for (int i = 0; i < n; i++)
            v[i] = v[i] * inv % p;
    }
    return v;
}
```