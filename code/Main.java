import java.io.*;
import java.util.*;
import java.math.*;

class FastInput {

    private BufferedReader reader;
    private StringTokenizer tokenizer;

    /**
     * init FastInput
     * @param sz buffer size
     */
    private void init(int sz) {
        reader = new BufferedReader(new InputStreamReader(System.in), sz);
        tokenizer = null;
    }

    /**
     * init FastInput with default buffer size
     */
    public FastInput() { init(32768); }

    /**
     * init FastInput with customized buffer size
     * @param sz buffer size
     */
    public FastInput(int sz) { init(sz); }

    /**
     * get next string (strings without separators such as spaces or tabs)
     * @return null if EOF or next string
     */
    public String next() {
        while (tokenizer == null || !tokenizer.hasMoreTokens()) {
            try {
                tokenizer = new StringTokenizer(reader.readLine());
            } catch (Exception e) {
                return null;
            }
        }
        return tokenizer.nextToken();
    }

    /**
     * next line (strings with spaces or tabs but no new line separators)
     * @return null if EOF or next line
     */
    public String nextLine() {
        try {
            return reader.readLine();
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * next integer
     * @return null if EOF or next integer
     */
    public Integer nextInt() {
        try {
            return Integer.valueOf(next());
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * next long
     * @return null if EOF or next long
     */
    public Long nextLong() {
        try {
            return Long.valueOf(next());
        } catch (Exception e) {
            return null;
        }
    }
}

/**
 * Stores a pair of ints.
 * Pair can be used as keys in HashMaps
 * (which is because hashCode() and equals() are implemented)
 * or entries in PriorityQueues
 * (since Pair implements Comparable).
 * Note: hashCode() can be implemented using better hash functions.
 */
class Pair implements Comparable<Pair> {

    int x;
    int y;

    Pair(int x, int y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Pair)
            return x == ((Pair) obj).x && y == ((Pair) obj).y;
        return super.equals(obj);
    }

    @Override
    public int compareTo(Pair o) {
        return x == o.x ? y - o.y : x - o.x;
    }

    @Override
    public int hashCode() {
        return x << 16 | (y & ((1 << 16) - 1));
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}

public class Main {

    /** swaps nums[i] and nums[j]. */
    public static void swap(int[] nums, int i, int j) {
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }

    /** reverses nums[l, r), r is excluded. */
    public static void reverse(int[] nums, int l, int r) {
        int len = r - l;
        for (int i = l; i < l + len / 2; i++)
            swap(nums, i, r - i + l - 1);
    }

    /** finds the lower bound for x in nums[l..r),
     *  i.e., finds the smallest i (l <= i < r) such that nums[i] >= x.
     *  if there is no such i, then r is returned. */
    public static int lower(int[] nums, int x, int l, int r) {
        while (l < r) {
            int mid = (l + r) / 2;
            if (nums[mid] >= x) r = mid;
            else l = mid + 1;
        }
        return l;
    }

    /** finds the upper bound for x in nums[l..r),
     *  i.e., finds the smallest i (l <= i < r) such that nums[i] > x.
     *  if there is no such i, then r is returned. */
    public static int upper(int[] nums, int x, int l, int r) {
        while (l < r) {
            int mid = (l + r) / 2;
            if (nums[mid] > x) r = mid;
            else l = mid + 1;
        }
        return l;
    }

    /** finds the Greatest Common Divisor (GCD) of a and b.
     *  a and b should be greater than 0. */
    public static long gcd(long a, long b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    /** calculates a ^ n % p in O(log(n)).
     *  n should be non-negative. */
    public static long qpow(long a, long n, long p) {
        long r = 1;
        a %= p;
        while (n > 0) {
            if ((n & 1) != 0) r = r * a % p;
            a = a * a % p;
            n >>= 1;
        }
        return r;
    }

    /** returns the first element of an ORDERED set (e.g. TreeSet),
     *  or null if the given set is an empty set. */
    public static <T> T first(Set<T> set) {
        for (var x : set) return x;
        return null;
    }

    /** returns the first element of an ORDERED map (e.g. TreeMap),
     *  or null if the given map is an empty map. */
    public static <K, V> Map.Entry<K, V> first(Map<K, V> map) {
        return first(map.entrySet());
    }

    static FastInput in = new FastInput();
    static PrintWriter out = new PrintWriter(new OutputStreamWriter(System.out));

    static final int INF = Integer.MAX_VALUE;
    static final int M = 1000000007;
//    static final int M = 998244353;
    static final int N = 100005;

    public static void main(String[] args) {
        // NOTE: DEFAULT JAVA STACK SIZE IS UNCERTAIN.
        //       in my computer, max depth for long (int) is 10000 (the same as C++).
        //       however, there is no guarantee that the server has the same configurations.
        //       if you did use DFS and the returned verdict is Runtime-Error (RE),
        //       try to convert DFS into something else.

        // TODO: WRITE YOUR SOLUTION HERE...



        // ATTENTION: DO NOT DELETE THIS LINE!
        out.flush(); // this is very very important!
    }
}