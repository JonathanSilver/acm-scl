import java.util.*;
import java.math.*;
public class LargePrime {
	static final int S = 8;
	static final int INT_MAX = 2147483647;
	static final BigInteger ZERO = BigInteger.ZERO;
	static final BigInteger ONE = BigInteger.ONE;
	static final BigInteger TWO = BigInteger.valueOf(2L);
	static final BigInteger MAX = BigInteger.valueOf(2147483648L);
	static Random rd = new Random();
	static boolean check(BigInteger a, BigInteger n, BigInteger x, int t) {
		BigInteger r = a.modPow(x, n), last = r;
		for (int i = 1; i <= t; i++) {
			r = r.multiply(r).mod(n);
			if (r.compareTo(ONE) == 0 &&
                last.compareTo(ONE) != 0 &&
                last.compareTo(n.subtract(ONE)) != 0)
				return true;
			last = r;
		}
		if (r.compareTo(ONE) != 0)
			return true;
		return false;
	}
	static boolean miller_rabin(BigInteger n) {
		if (n.compareTo(TWO) < 0) return false;
		if (n.compareTo(TWO) == 0) return true;
		if (!n.testBit(0)) return false;
		BigInteger x = n.subtract(ONE);
		int t = 0;
		while (!x.testBit(0)) {
			x = x.shiftRight(1);
			t++;
		}
		for (int i = 0; i < S; i++) {
			BigInteger a;
			if (n.compareTo(MAX) > 0)
				a = new BigInteger(Integer.toString(rd.nextInt(INT_MAX) + 1));
			else {
				int tmp = n.subtract(ONE).intValue();
				a = new BigInteger(Integer.toString(rd.nextInt(INT_MAX) % tmp + 1));
			}
			if (check(a, n, x, t)) return false;
		}
		return true;
	}
	static BigInteger pollard_rho(BigInteger x, BigInteger c) {
		int i = 1, k = 2;
		BigInteger x0, y;
		if (x.compareTo(MAX) > 0)
			x0 = new BigInteger(Integer.toString(rd.nextInt(INT_MAX) + 1));
		else {
			int tmp = x.subtract(ONE).intValue();
			x0 = new BigInteger(Integer.toString(rd.nextInt(INT_MAX) % tmp + 1));
		}
		y = x0;
		while (true) {
			i++;
			x0 = x0.multiply(x0).add(c).mod(x);
			BigInteger d = y.subtract(x0).abs().gcd(x);
			if (d.compareTo(ONE) != 0 &&
                d.compareTo(x) != 0) return d;
			if (y.compareTo(x0) == 0) return x;
			if (i == k) {
				y = x0;
				k += k;
			}
		}
	}
	static void factor(BigInteger n, Map<BigInteger, Integer> m) {
		if (n.compareTo(ONE) == 0) return;
		if (miller_rabin(n)) {
			if (m.containsKey(n))
				m.put(n, m.get(n) + 1);
			else m.put(n, 1);
			return;
		}
		BigInteger c = BigInteger.valueOf(107L), p = n;
		while (p.compareTo(n) >= 0)
			p = pollard_rho(p, c = c.subtract(ONE));
		factor(p, m);
		factor(n.divide(p), m);
	}
	public static void main(String[] args) {
		Scanner s = new Scanner(System.in);
		while (s.hasNext()) {
			BigInteger n = s.nextBigInteger();
			Map<BigInteger, Integer> m = new TreeMap<>();
			factor(n, m);
			for (Map.Entry<BigInteger, Integer> e : m.entrySet())
				System.out.println(e.getKey() + ": " + e.getValue());
		}
	}
}