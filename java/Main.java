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

public class Main {

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