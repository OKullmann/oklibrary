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

  public static boolean is_euler(final int[][][] E, final int N) {
    if (E.length != 2) return false;
    if (! is_latin(E[0], N) || ! is_latin(E[1], N)) return false;
    final boolean[][] found = new boolean[N][N];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        found[E[0][i][j]][E[1][i][j]] = true;
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        if (! found[i][j]) return false;
    return true;
  }

  // the two ls's are res[0][i][j] and res[1][i][j]:
  int[][][] generate() {
    int[][][] res = new int[N][N][2];
    // chosen randomly from fixed "prototypes":
    final int tmax = main_types[N].length;
    final int t = (int) (Math.random() * tmax);
    final int[][][] E = main_types[N][t];
    assert(is_euler(E, N));
    // create a random permutation of E in the four possible "dimensions" of
    // permuations XXX
    // needs to create a random permutation

    res = E;
    return res;
  }

  public static void main(String[] args) {
    assert(main_types.length == 5);
    assert(main_types[0].length == 0);
    assert(main_types[1].length == 0);
    assert(main_types[2].length == 0);
    assert(main_types[3].length == 1);
    assert(main_types[3][0].length == 2);
    assert(main_types[3][0][0].length == 3);
    assert(main_types[3][0][0][0].length == 3);
    assert(main_types[3][0][1].length == 3);
    assert(main_types[3][0][1][0].length == 3);

    assert(is_euler(main_types[3][0], 3));
    assert(is_euler(main_types[4][0], 4));

    for (int N = Nmin; N <= Nmax; ++N) {
      final EulerSquareGen gen = new EulerSquareGen(N);
      for (long i = 0; i < 100; ++i)
        assert is_euler(gen.generate(), N);
    }
  }
  
}
