# ACM - SCL

*This is the Standard Code Library (SCL) for Competitive Programming.*

Many commonly seen/used algorithms and data structures in competitive programming have been implemented. You can find the implementations in `code`.

A tool used to compare the output files has also been implemented, the source code of which can be found in `_cmp`.

Word/PDF documents containing well-organized source code and some useful theorems and formulas can be found in `_SCL`. Most of the theorems and formulas are from *Wikipedia* and *Concrete Mathematics*.

Some notes can be found in `notes`. They are `Markdown` files with `LaTeX` formulas. I use `VSCode` + `Markdown All in One` + `Markdown Preview Enhanced` to read and edit these notes.

### Notes:
- My C++ compiler is [MinGW-W64](https://sourceforge.net/projects/mingw-w64/) 8.1.0. Compile the code with `c++0y` (a.k.a. C++ 14) compiling option.
- My Java compiler's version is 11.0.1.
- Surely, I am **NOT** the inventor of these amazing algorithms and data structures! I learned them (or the code) from the Internet or some textbooks!

### Tips:
- The code works. I have tested it in several contests. All the algorithms and data structures implemented here have been thoroughly checked. You are sure to get `AC` if they are correctly used.
- Many `vector`s can be replaced with arrays! So as to many other C++ STL classes in the code. If you are getting `TLE` or `MLE`, replace them with arrays or other self-implemented data structures. The SCL should be clear and easy to understand so I used C++ STL classes.
- There are few comments in the code. It would be appreciated if you add any! ^_^

## Table of Contents
- Data Structures [`code/`](/code)
    - Hash Table [`hash_table.h`](/code/hash_table.h)
        - Basic Hash Table [`dictionary`](/code/hash_table.h)
        - Skip List [`skip_list`](/code/hash_table.h)
    - Binary Search Tree (BST)
        - Basic BST `search_tree.h:search_tree`
        - AVL `search_tree.h:avl_tree` `tree.h:avl_tree`
        - Red-Black Tree `search_tree.h:rb_tree`
        - Treap `search_tree.h:treap` `tree.h:treap` `tree.h:TREAP::treap`
        - Splay `search_tree.h:splay_tree`
        - Modified Splay (Range Query) `tree.h:splay_tree`
    - Heap `heap.h`
        - Binary Heap `binary_heap`
        - Leftist Heap `leftist_heap`
        - Pairing Heap `pairing_heap`
    - Binary Indexed Tree `tree.h:binary_indexed_tree` `tree.h:binary_indexed_tree_2`
    - Segment Tree `tree.h`
        - Basic Segment Tree `segment_tree` `segment_tree_2`
        - Persistent Segment Tree `persistent_segment_tree` `persistent_segment_tree_2`
    - Automaton `tree.h`
        - Trie & Aho-Corasick Algorithm `trie` `trie_2` `trie_01`
        - Palindrome Automaton `palindrome_automaton`
        - Suffix Automaton `suffix_automaton`
        - Extended Suffix Automaton `ext_suffix_automaton::suffix_automaton` (`ext_suffix_automaton::merge` *is used to merge nodes in segment tree*)
    - Disjoint Set `misc.h:disj_sets`
    - Sparse Table `misc.h:init_st` `&` `misc.h:query_min`
    - Techniques in Tree `tree.h`
        - Heavy-Light Decomposition `heavy_light_decomposition`
        - Centroid Decomposition `centroid_decomposition_of_tree`
        - DSU `dsu_on_tree`
        - Auxiliary Tree `auxiliary_tree`
    - Dancing Links `misc.h:DLX`
- Sorting `code/sort.h`
    - Quick Sort `quick_sort(vector<T> &)`
    - Quick Select `quick_select(vector<T> &, int k)`
    - Merge Sort `merge_sort` `merge_sort_2(vector<T> &)`
    - Introspective Sort `introspective_sort`
    - Radix Sort `radix_sort_1` `radix_sort_2` `counting_radix_sort`
- String `code/misc.h`
    - Hash `BKDR_hash(const char *)` `init_hash(const char *)` `&` `BKDR_hash(int, int)`
    - KMP `KMP_match`
    - Extended KMP `ext_KMP`
    - Manacher `manacher`
    - Suffix Array & LCP Array
        - $O(n\log^2 n)$ Algorithm `construct_sa` `&` `construct_lcp`
        - $O(n\log n)$ Algorithm `construct_sa_lcp` `da` `&` `calc_height`
        - $O(n)$ Algorithm `create_suffix_array` `SA_IS::suffix_array` `dc3` `&` `calc_height`
- Computational Geometry (2D) `code/computational_geometry.h`
    - Points `point`
    - Vectors `vec`
    - Dot, Cross Product
    - Line, Segment Intersection
    - Distances
    - Areas `area`
    - Center of Mass `gravity`
    - Convex Hulls
        - Graham Scan `graham`
        - Andrew `convex_hull`
    - Rotating Calipers `rotating_calipers`
    - Fermat Point (Simulated Annealing) `fermat_point`
    - Closest Pair of Points `find_closest_pair`
    - Half Plane Intersection `half_plane_intersection` `cut`
- Graph Theory `code/graph.h`
    - Shortest Path
        - Bellman-Ford `bellman_ford`
        - Dijsktra `dijkstra`
        - Floyd Warshall `floyd_warshall`
    - Spanning Tree
        - Prim `prim`
        - Kruskal `kruskal`
    - Max Flow
        - Edmonds-Karp `edmonds_karp`
        - Dinic `dinic`
        - ISAP `isap`
    - Min Cost Max Flow
        - Stardard SPFA Version `min_cost_max_flow`
        - Dijsktra Optimized `graph_ext::min_cost_max_flow`
    - Bipartite Matching
        - Hungarian `hungary`
        - KM (BFS, DFS) `KM_BFS::km` `KM_DFS::km`
    - Strongly Connected Components
        - Tarjan `tarjan(int)`
        - Kosaraju `scc`
    - Cut Vertices & Bridges
        - Tarjan `tarjan(int, int)`
    - Lowest Common Ancestor
        - Binary Search `lca_dfs` `&` `lca`
        - RMQ `graph_ext::init_rmq_lca` `&` `graph_ext::rmq_lca`
        - Tarjan `lca_tarjan`
    - Topological Sort `topological_sort`
- Math `code/`
    - Number Theory `number_theory.h`
        - Greatest Common Divisor (GCD) `gcd`
        - Extended GCD `ext_gcd`
        - Multiplicative Inverse `inv`
        - Chinese Remainder Theorem (CRT) `CRT`
        - Modular Linear Equations `modular_linear_equations`
        - Prime `is_prime` `prime_factors`
        - Euler's Totient Function `euler`
        - Sieve
            - Sieve of Eratosthenes `sieve_of_eratosthenes` 
            - Euler's Sieve `eulers_sieve`
        - Quick Power `mod_pow`
        - Lucas `lucas`
        - Extended Lucas `ext_lucas`
        - Baby Step Giant Step (BSGS) `BSGS`
        - Extended BSGS `ext_BSGS`
        - Sqrt `mod_sqr`
        - Primitive Root `primitive_root`
        - N-th Root `nth_root`
        - Miller-Rabin `miller_rabin`
        - Pollard-Rho `find_prime_factors`
    - Fast Fourier Transform (FFT)
        - FFT `misc.h:FFT`
        - Number-Theoretic Transform (NTT) `number_theory.h:NTT`
        - Fast Walsh-Hadamard Transform (FWT) `number_theory.h:FWT`
    - Matrix `matrix.h`
        - Matrix `matrix`
        - Gaussian Elimination `gaussian_elimination`
- Misc `code/`
    - Longest Increasing Sequence (LIS) `extra.h:LIS::LIS`
    - Counting DP `extra.h:solve`
    - Sprague-Grundy Function `misc.h:get_SG`
    - Mo's Algorithm `extra.h`
        - 2D `MO::MO_2`
        - 3D `MO::MO_3`
        - On Tree (2D) `MO_on_tree::MO_2_on_tree`
    - CDQ `extra.h:CDQ::cdq`
    - Divide & Conquer with FFT `extra.h:divide_and_conquer_fft::solve`
    - Fast Input `extra.h:fast_input`
