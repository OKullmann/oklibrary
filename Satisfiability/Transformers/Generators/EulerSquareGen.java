class EulerSquareGen {
  public final int Nmin = 3, Nmax = 7;
  public final int N;

  // main_types[N][t][k][i][j]:
  static final int[][][][][] main_types = {
    {},
    {},
    {},
    {{{{0,2,1}, {2,1,0}, {1,0,2}},  {{0,2,1}, {1,0,2}, {2,1,0}}}}
  };

  public EulerSquareGen(final int N_) {
    assert(Nmin <= N_ && N_ <= Nmax);
    N = N_;
  }

  // the two ls's are res[0][i][j] and res[1][i][j]:
  int[][][] generate() {
    final int[][][] res = new int[N][N][2];
    // chosen randomly from fixed "prototypes"
    
    return res;
  }

  public static void main(String[] args) {
    assert(main_types.length == 4);
    assert(main_types[0].length == 0);
    assert(main_types[1].length == 0);
    assert(main_types[2].length == 0);
    assert(main_types[3].length == 1);
    assert(main_types[3][0].length == 2);
    assert(main_types[3][0][0].length == 3);
    assert(main_types[3][0][0][0].length == 3);
    assert(main_types[3][0][1].length == 3);
    assert(main_types[3][0][1][0].length == 3);
  }
  
}
