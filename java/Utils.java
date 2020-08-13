import java.util.*;

/**
 * Stores a pair of ints.
 * Pair can be used as keys in HashMaps
 * (which is because hashCode() and equals() are implemented)
 * or entries in PriorityQueues
 * (since Pair implements Comparable).
 * Note: hashCode() can be implemented using better hash functions.
 */
class Pair<T1 extends Comparable<T1>, T2 extends Comparable<T2>>
        implements Comparable<Pair<T1, T2>> {

    T1 x;
    T2 y;

    Pair(T1 x, T2 y) {
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Pair<?, ?>)
            return x.equals(((Pair<?, ?>)obj).x) && y.equals(((Pair<?, ?>)obj).y);
        else return super.equals(obj);
    }

    @Override
    public int compareTo(Pair<T1, T2> o) {
        int r = x.compareTo(o.x);
        if (r == 0) return y.compareTo(o.y);
        else return r;
    }

    @Override
    public int hashCode() {
        return (x.hashCode() << 16) | (y.hashCode() & ((1 << 16) - 1));
    }

    @Override
    public String toString() {
        return "(" + x + ", " + y + ")";
    }
}

public class Utils {

    /** swaps nums[i] and nums[j]. */
    public static void swap(int[] nums, int i, int j) {
        int temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }

    public static <T> void swap(T[] nums, int i, int j) {
        T temp = nums[i];
        nums[i] = nums[j];
        nums[j] = temp;
    }

    public static <T> void swap(List<T> nums, int i, int j) {
        T temp = nums.get(i);
        nums.set(i, nums.get(j));
        nums.set(j, temp);
    }

    /** reverses nums[l, r), r is excluded. */
    public static void reverse(int[] nums, int l, int r) {
        int len = r - l;
        for (int i = l; i < l + len / 2; i++)
            swap(nums, i, r - i + l - 1);
    }

    public static <T> void reverse(List<T> nums, int l, int r) {
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

    public static <T extends Comparable<T>> int lower(List<T> nums, T x, int l, int r) {
        while (l < r) {
            int mid = (l + r) / 2;
            if (nums.get(mid).compareTo(x) >= 0) r = mid;
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

    public static <T extends Comparable<T>> int upper(List<T> nums, T x, int l, int r) {
        while (l < r) {
            int mid = (l + r) / 2;
            if (nums.get(mid).compareTo(x) > 0) r = mid;
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

    /** sorts the list in ascending order,
     *  and rearranges the list with equal elements removed.
     *  the operation is done in place.
     *  returns the length of the list.
     *  (there might be extra elements at the end of the list,
     *   so the returned new length should be used instead of list.size()) */
    public static <T extends Comparable<T>> int sortUnique(List<T> list) {
        list.sort(T::compareTo);
        int i = 0, j = 1;
        while (j < list.size()) {
            if (!list.get(i).equals(list.get(j)))
                swap(list, ++i, j);
            j++;
        }
        return ++i;
    }

    /** gets the next permutation of nums.
     *  the operation is done in place.
     *  the ascending order is considered the first permutation,
     *  and the descending order is considered the last.
     *  the next permutation of the last permutation is the first permutation.
     *  returns false if nums is already the last permutation, otherwise true.
     *  
     *  common usage:
     *  
     *  do {
     *      // do something with nums...
     *  } while (nextPermutation(nums)); */
    public static boolean nextPermutation(int[] nums) {
        for (int i = nums.length - 1; i > 0; i--) {
            if (nums[i] > nums[i - 1]) {
                for (int j = nums.length - 1; j >= i; j--) {
                    if (nums[j] > nums[i - 1]) {
                        swap(nums, j, i - 1);
                        break;
                    }
                }
                reverse(nums, i, nums.length);
                return true;
            }
        }
        reverse(nums, 0, nums.length);
        return false;
    }
}