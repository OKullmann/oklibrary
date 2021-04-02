class EulerSquareGen {
  public static final int Nmin = 3, Nmax = 4;
  public final int N;

  // http://users.cecs.anu.edu.au/~bdm/data/latin.html
  // Main classes of Graeco-Latin squares
  // main_types[N][t][k][i][j]:
  static final int[][][][][] main_types = {
    {},
    {},
    {},
    {{{{0,2,1}, {2,1,0}, {1,0,2}},
      {{0,2,1}, {1,0,2}, {2,1,0}}}},
    {{{{0,3,1,2}, {2,1,3,0}, {3,0,2,1}, {1,2,0,3}},
      {{0,3,2,1}, {3,0,1,2}, {2,1,0,3}, {1,2,3,0}}}},
   };

  public EulerSquareGen(final int N_) {
    assert(Nmin <= N_ && N_ <= Nmax);
    N = N_;
  }

  public static boolean is_latin(final int[][] L, final int N) {
    if (L.length != N) return false;
    for (int i = 0; i < N; ++i)
      if (L[i].length != N) return false;
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        if (L[i][j] < 0 || L[i][j] >= N) return false;
    for (int i = 0; i < N; ++i) {
      final boolean[] found = new boolean[N];
      for (int j = 0; j < N; ++j)
        found[L[i][j]] = true;
      for (int j = 0; j < N; ++j)
        if (! found[j]) return false;
    }
    for (int j = 0; j < N; ++j) {
      final boolean[] found = new boolean[N];
      for (int i = 0; i < N; ++i)
        found[L[i][j]] = true;
      for (int i = 0; i < N; ++i)
        if (! found[i]) return false;
    }
    return true;
  }

  private static boolean all_true(final boolean[][] b) {
    for (int i = 0; i < b.length; ++i)
      for (int j = 0; j < b[i].length; ++j)
        if (! b[i][j]) return false;
    return true;
  }

  public static boolean is_euler(final int[][][] E, final int N) {
    if (E.length != 2) return false;
    if (! is_latin(E[0], N) || ! is_latin(E[1], N)) return false;
    final boolean[][] found = new boolean[N][N];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        found[E[0][i][j]][E[1][i][j]] = true;
    return all_true(found);
  }

  private static boolean all_different(final int[][] A, final int i, final int j,
    final int N) {
    final boolean[][] found = new boolean[N][N];
    for (int row = 0; row < A.length; ++row)
      found[A[row][i]][A[row][j]] = true;
    return all_true(found);
  }

  public static boolean is_orth_array(final int[][] A, final int N) {
    if (A.length != N*N) return false;
    for (int i = 0; i < A.length; ++i)
      if (A[i].length != 4) return false;
    for (int i = 0; i < A.length; ++i)
      for (int j = 0; j < A[i].length; ++j)
        if (A[i][j] < 0 || A[i][j] >= N) return false;
    for (int i = 0; i < 3; ++i)
      for (int j = i+1; j < 4; ++j)
        if (! all_different(A, i, j, N)) return false;
    return true;
  }

  public static int[][] to_orth_array(final int[][][] E, final int N) {
    assert(is_euler(E,N));
    final int[][] res = new int[N*N][4];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j) {
        final int row = i*N + j;
        res[row][0] = i; res[row][1] = j;
        res[row][2] = E[0][i][j]; res[row][3] = E[1][i][j];
    }
    return res;
  }

  public static int[][][] to_euler(final int[][] A, final int N) {
    assert(is_orth_array(A,N));
    final int[][][] res = new int[2][N][N];
    for (int row = 0; row < A.length; ++row) {
      final int i = A[row][0], j = A[row][1];
      res[0][i][j] = A[row][2]; res[1][i][j] = A[row][3];
    }
    return res;
  }

  private static void random_permutation(final int[] a, final int N) {
    final int[] val = new int[N];
    for (int i = 1; i < N; ++i) val[i] = i;
    for (int i = 0; i < N-1; ++i) {
      final int j = i + (int)(Math.random() * (N - i));
      final int t = val[i]; val[i] = val[j]; val[j] = t;
    }
    for (int i = 0; i < a.length; ++i)
      a[i] = val[a[i]];
  }
  private static void random_permutation(final int[][] a) {
    final int N = a.length;
    for (int i = 0; i < N-1; ++i) {
      final int j = i + (int)(Math.random() * (N - i));
      final int[] t = a[i]; a[i] = a[j]; a[j] = t;
    }
  }

  private static int[][] transpose(final int[][] A) {
    final int N = A.length;
    final int M = A[0].length;
    final int[][] res = new int[M][N];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < M; ++j)
        res[j][i] = A[i][j];
    return res;
  }

  public static int[][] random_permutation(final int[][] A, final int N) {
    assert(is_orth_array(A,N));
    final int[][] T = transpose(A);
    for (int i = 0; i < 4; ++i) random_permutation(T[i], N);
    random_permutation(T);
    return transpose(T);
  }

  public int[][][] generate() {
    final int tmax = main_types[N].length;
    final int t = (int) (Math.random() * tmax);
    final int[][][] E = main_types[N][t];
    assert(is_euler(E, N));
    return to_euler(random_permutation(to_orth_array(E,N), N), N);
  }

  // General helper-functions for establishing equality between arbitrary
  // int-arrays:
  public static boolean equals(final int[] R1, final int[] R2) {
    if (R1 == R2) return true;
    if (R1 == null || R2 == null) return false;
    if (R1.length != R2.length) return false;
    for (int i = 0; i < R1.length; ++i)
      if (R1[i] != R2[i]) return false;
    return true;
  }
  public static boolean equals(final int[][] L1, final int[][] L2) {
    if (L1 == L2) return true;
    if (L1 == null || L2 == null) return false;
    if (L1.length != L2.length) return false;
    for (int i = 0; i < L1.length; ++i)
      if (! equals(L1[i], L2[i])) return false;
    return true;
  }
  public static boolean equals(final int[][][] E1, final int[][][] E2) {
    if (E1 == E2) return true;
    if (E1 == null || E2 == null) return false;
    if (E1.length != E2.length) return false;
    for (int i = 0; i < E1.length; ++i)
      if (! equals(E1[i], E2[i])) return false;
    return true;
  }

  private static String toString(final int[][][] E, final int N) {
    String res = "";
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j)
        res += E[0][i][j] + " ";
      for (int j = 0; j < N; ++j)
        res += " " + E[1][i][j];
      res += "\n";
    }
    return res;
  }

  public static void main(String[] args) {
    for (int N = Nmin; N <= Nmax; ++N)
      for (int t = 0; t < main_types[N].length; ++t) {
        final int[][][] E = main_types[N][t];
        assert(is_euler(E, N));
        final int[][] A = to_orth_array(E, N);
        assert(is_orth_array(A, N));
        assert(equals(A, transpose(transpose(A))));
        final int[][][] E2 = to_euler(A,N);
        assert(equals(E, E2));
    }
    for (int N = Nmin; N <= Nmax; ++N) {
      final EulerSquareGen g = new EulerSquareGen(N);
      for (long i = 0; i < 100; ++i)
        assert is_euler(g.generate(), N);
    }
    if (args.length != 1) return;
    final int N = Integer.parseInt(args[0]);
    final EulerSquareGen g = new EulerSquareGen(N);
    System.out.println(toString(g.generate(), N));

  }
  
}
