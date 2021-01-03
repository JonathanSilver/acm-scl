import java.util.*;

class TreeNode {
    int val;
    TreeNode left;
    TreeNode right;
    TreeNode() {}
    TreeNode(int val) { this.val = val; }
    TreeNode(int val, TreeNode left, TreeNode right) {
        this.val = val;
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "<Node: " + this.val + ">";
    }
}

class Node {
    public int val;
    public List<Node> neighbors;

    public Node() {
        val = 0;
        neighbors = new ArrayList<>();
    }

    public Node(int _val) {
        val = _val;
        neighbors = new ArrayList<>();
    }

    public Node(int _val, ArrayList<Node> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
}

class ListNode {
    int val;
    ListNode next;
    ListNode() {}
    ListNode(int val) { this.val = val; }
    ListNode(int val, ListNode next) { this.val = val; this.next = next; }
}

public class LeetCodeIO {

    public static TreeNode inputTree(Scanner sc) {
        int n = sc.nextInt();
        TreeNode root = new TreeNode();
        root.val = sc.nextInt();
        n--;
        Queue<TreeNode> q = new LinkedList<>();
        q.offer(root);
        while (n > 0) {
            TreeNode p = q.poll();
            assert p != null;
            String s = sc.next();
            try {
                int x = Integer.parseInt(s);
                p.left = new TreeNode();
                p.left.val = x;
                q.offer(p.left);
            } catch (NumberFormatException ignored) {
            }
            n--;
            if (n > 0) {
                s = sc.next();
                try {
                    int x = Integer.parseInt(s);
                    p.right = new TreeNode();
                    p.right.val = x;
                    q.offer(p.right);
                } catch (NumberFormatException ignored) {
                }
                n--;
            }
        }
        return root;
    }

    public static List<Integer> walkTree(TreeNode root) {
        Deque<Integer> list = new LinkedList<>();
        Queue<TreeNode> q = new LinkedList<>();
        q.offer(root);
        while (!q.isEmpty()) {
            TreeNode node = q.poll();
            if (node == null) {
                list.add(null);
            } else {
                list.add(node.val);
                q.offer(node.left);
                q.offer(node.right);
            }
        }
        while (list.peekLast() == null) list.pollLast();
        return new ArrayList<>(list);
    }

    public static Node inputGraph(Scanner sc) {
        int n = sc.nextInt(); // total number of nodes
        // nodes are labeled from 1..n
        Map<Integer, Node> nodes = new HashMap<>();
        for (int i = 1; i <= n; i++)
            nodes.put(i, new Node(i));
        for (int i = 1; i <= n; i++) {
            int k = sc.nextInt();
            for (int j = 0; j < k; j++) {
                int x = sc.nextInt();
                nodes.get(i).neighbors.add(nodes.get(x));
            }
        }
        return nodes.get(1);
    }

    public static List<List<Integer>> outputGraph(Node node) {
        Map<Integer, List<Integer>> map = new TreeMap<>();
        Queue<Node> q = new LinkedList<>();
        q.offer(node);
        while (!q.isEmpty()) {
            node = q.poll();
            map.put(node.val, new ArrayList<>());
            for (var x : node.neighbors) {
                if (!map.containsKey(x.val)) q.offer(x);
                map.get(node.val).add(x.val);
            }
        }
        List<List<Integer>> res = new ArrayList<>();
        for (var e : map.entrySet()) {
            res.add(e.getValue());
        }
        return res;
    }

    public static int[] inputArray(Scanner sc) {
        int n = sc.nextInt();
        int[] nums = new int[n];
        for (int i = 0; i < n; i++) nums[i] = sc.nextInt();
        return nums;
    }

    public static long[] inputLongArray(Scanner sc) {
        int n = sc.nextInt();
        long[] nums = new long[n];
        for (int i = 0; i < n; i++) nums[i] = sc.nextLong();
        return nums;
    }

    public static int[][] inputTuple(Scanner sc, int k) {
        int n = sc.nextInt();
        int[][] nums = new int[n][k];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                nums[i][j] = sc.nextInt();
            }
        }
        return nums;
    }

    public static int[][] inputGrid(Scanner sc) {
        int n = sc.nextInt(), m = sc.nextInt();
        int[][] nums = new int[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                nums[i][j] = sc.nextInt();
            }
        }
        return nums;
    }

    public static String[] inputWords(Scanner sc) {
        int n = sc.nextInt(); sc.nextLine();
        String[] words = new String[n];
        for (int i = 0; i < n; i++) words[i] = sc.nextLine();
        return words;
    }

    public static char[][] inputMap(Scanner sc) {
        int n = sc.nextInt();
        char[][] map = new char[n][];
        for (int i = 0; i < n; i++)
            map[i] = sc.next().toCharArray();
        return map;
    }

    public static ListNode inputList(Scanner sc) {
        int n = sc.nextInt();
        ListNode head = null;
        ListNode tail = null;
        for (int i = 0; i < n; i++) {
            if (tail == null) {
                head = tail = new ListNode(sc.nextInt());
            } else {
                tail.next = new ListNode(sc.nextInt());
                tail = tail.next;
            }
        }
        return head;
    }
}
