// Oliver Kullmann, 6.7.2018 (Swansea)

/*

For AmoAlo_board :

1. Add vectors for r,c,d,a-d ranks //done

2. Write  a helper function for getting a daig, anti-diag starting vertex and length of it. //done

3. Initialization with length of vectors, just do it. //done


  For functions :

  // done
    1. If a field is set placed then set all other fields(affected by amo) to forbidden.
    2. Update the rank counts (after setting a field).

  //after this write the propagation.
    1. Handle alo propagation.
       a. Add a stack. //done
       b. Update set function with stack addition and loop.   //done
       c. Add new fields into the stack in field_update function.  //done

  Add asserts everywhere.  //done
  Add const where appropriate. //done
  Have to work on making data private and creating public function to access them.

For GreedyAmo:
  Only heuristics is remaining.


*/

#include <stack>
#include <vector>
#include <limits>

#include <cassert>
#include <cmath>

#include "ChessBoard.hpp"

namespace NQueens {

  // The prototype:
  struct BasicACLS {

    const ChessBoard::coord_t N;

    explicit BasicACLS(const ChessBoard::coord_t N) : N(N) {}

    bool satisfied() const noexcept { return false; }
    bool falsified() const noexcept { return false; }

    // The total number of variables:
    ChessBoard::Var_uint n() const noexcept { return N*N; }
    // Number of variables set to true or false:
    ChessBoard::Var_uint nset() const noexcept { return 0; }

    // Occupy or forbid field v:
    void set(const ChessBoard::Var v, const bool val) {}

  };

  /*
    The decomposition of the NxN field into diagonals (fields with equal
     difference) and antidiagonals (fields with equal sum), where each
     such line is specified by a value of :
  */
  struct Diagonal {
    ChessBoard::Var s; // start field
    ChessBoard::Var_uint l; // length
    ChessBoard::Var_uint i;
    /* Field (variable) (x,y) has abstract diagonal-index x-y, which ranges
       from 1-N to N-1, and then we set i = (x-y) + (N-1) with
       0 <= i <= 2N-2.
    */
  };
  struct AntiDiagonal {
    ChessBoard::Var s; // start field
    ChessBoard::Var_uint l; // length
    ChessBoard::Var_uint i;
    /* (x,y) has abstract antidiagonal-index x+y, which ranges from 1+1 to
       N+N, and then we set i = (x+y) - 2.
    */
  };
  static_assert(std::is_pod<Diagonal>::value, "Diagonal is not POD.");
  static_assert(std::is_pod<AntiDiagonal>::value, "AntiDiagonal is not POD.");

  // The number of open, placed and forbidden fields for any line, that is, any
  // row, column, diagonal or antidiagonal:
  struct Rank {
    ChessBoard::Var_uint o;
    ChessBoard::Var_uint p;
    ChessBoard::Var_uint f;
  };
  static_assert(std::is_pod<Rank>::value, "Rank is not POD.");

  // The same numbers as with Rank, but now for the whole board:
  struct TotalRank {
    ChessBoard::Var_uint o;
    ChessBoard::Var_uint p;
    ChessBoard::Var_uint f;
  };
  static_assert(std::is_pod<TotalRank>::value, "TotalRank is not POD.");

  enum class State { open=0, placed, forbidden };

  // A concrete instance of BasicACLS:
  class AmoAlo_board {
    using coord_t = ChessBoard::coord_t;
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
    using Var_int = ChessBoard::Var_int;
  public :
    typedef std::vector<Rank> Ranks;
    typedef std::stack<Var> Stack;
    typedef std::vector<std::vector<State>> Board;
    const coord_t N;
  private :
    Board b;
    Ranks r_ranks;
    Ranks c_ranks;
    Ranks ad_ranks;
    Ranks d_ranks;
    TotalRank count;
    Stack stack;
    bool falsified_ = false;

    Board b_init() const {
      Board board;
      board.resize(N, std::vector<State>(N));
      return board;
    }
    Ranks r_init() const {
      Ranks r_ranks;
      for (Var_uint i = 1; i <= N ; ++i) r_ranks.push_back({N,0,0});
      return r_ranks;
    }
    Ranks c_init() const {
      Ranks c_ranks;
      for (Var_uint i = 1; i <= N ; ++i) c_ranks.push_back({N,0,0});
      return c_ranks;
    }
    Ranks ad_init() const {
      Ranks ad_ranks;
      for (Var_uint i = 1; i < N ; ++i) ad_ranks.push_back({i,0,0});
      for (Var_uint i = N; i > 0 ; --i) ad_ranks.push_back({i,0,0});
      return ad_ranks;
    }
    Ranks d_init() const {
      Ranks d_ranks;
      for (Var_uint i = 1; i < N ; ++i) d_ranks.push_back({i,0,0});
      for (Var_uint i = N; i > 0 ; --i) d_ranks.push_back({i,0,0});
      return d_ranks;
    }

  public :

    explicit AmoAlo_board(const coord_t N) :
      N(N), b(b_init()), r_ranks(r_init()), c_ranks(c_init()),
      ad_ranks(ad_init()), d_ranks(d_init()), count{N*N,0,0} {
        assert(N < std::numeric_limits<coord_t>::max() / 2);
    }

    // Returns anti_diagonal starting field, length and index:
    AntiDiagonal anti_diagonal(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const coord_t c_sum = v.first + v.second;
      if (c_sum < N) return {Var{0,c_sum}, c_sum+1, c_sum};
      else return {Var{c_sum-N+1,N-1}, 2*N-(c_sum+1), c_sum};
    }

    // Returns diagonal starting field, length and index:
    Diagonal diagonal(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const ChessBoard::scoord_t c_diff = v.first - v.second;
      if (c_diff > 0) {
	const coord_t cd = c_diff;
	return {Var{cd,0}, N - cd, (N-1)-cd};
      }
      else {
	const coord_t cd = -c_diff;
	return {Var{0,coord_t(cd)}, N-cd, (N-1)+cd};
      }
    }

    // Checks if the field v is open:
    bool v_open(const Var v) const noexcept {
      return (b[v.first][v.second] == State::open);
    }

  private:

    // Updates the placed rank:
    void placed_rank_update(const Var v) noexcept {
      const AntiDiagonal ad =   anti_diagonal(v);
      const Diagonal d = diagonal(v);
      ++r_ranks[v.first].p;
      ++c_ranks[v.second].p;
      ++ad_ranks[ad.i].p;
      ++d_ranks[d.i].p;
    }

    // Forbidden field ranks are updated only if no field is placed in the same r,c,d or ad
    // and falsified_ is updated if found:
    void forbidden_rank_update(const Var v) noexcept {
      const AntiDiagonal ad = anti_diagonal(v);
      const Diagonal d = diagonal(v);
      if (!r_ranks[v.first].p) {
        --r_ranks[v.first].o;
        --c_ranks[v.second].o;
        --ad_ranks[ad.i].o;
        --d_ranks[d.i].o;
        if (r_ranks[v.first].o == 0 or c_ranks[v.second].o == 0) falsified_ = true;
        else {
          if (r_ranks[v.first].o == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{v.first,i})) { stack.push(Var{v.first,i}); break; }
          if (c_ranks[v.second].o == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{i,v.second})) { stack.push(Var{i,v.second}); break; }
        }
      }
    }

    void count_update(const Var v) noexcept {
      --count.o;
      if (b[v.first][v.second] == State::placed) ++count.p;
      else ++count.f;
    }

    void r_update(const Var cur_v) noexcept {
      for (coord_t i = 0 ; i < N ; ++i) {
        const Var v = Var{cur_v.first,i};
        if (v_open(v)) {
          b[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void c_update(const Var cur_v) noexcept {
      for (coord_t i = 0 ; i < N ; ++i) {
        const Var v = Var{i,cur_v.second};
        if (v_open(v)) {
          b[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void ad_update(const Var cur_v) noexcept {
      const AntiDiagonal ad = anti_diagonal(cur_v);
      const Var ad_v = ad.s;
      assert(ad.l < N);
      for (coord_t i = 0 ; i < ad.l ; ++i) {
        const Var v = Var{ad_v.first + i,ad_v.second - i};
        if (v_open(v)) {
          b[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void d_update(const Var cur_v) noexcept {
      const Diagonal d = diagonal(cur_v);
      const Var d_v = d.s;
      assert(d.l < N);
      for (coord_t i = 0 ; i < d.l ; ++i) {
        const Var v = Var{d_v.first + i,d_v.second + i};
        if (v_open(v)) {
          b[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
        }
      }
    }

    public:

    Var_uint amo_count(const Var v) const noexcept {
      const AntiDiagonal ad = AmoAlo_board::anti_diagonal(v);
      const Diagonal d = AmoAlo_board::diagonal(v);
      return (r_ranks[v.first].o + c_ranks[v.second].o + ad_ranks[ad.i].o + d_ranks[d.i].o);
    }

    bool satisfied() const noexcept { return (count.p == N); }
    bool falsified() const noexcept { return falsified_; }
    Var_uint n() const noexcept { return N*N; }
    Var_uint nset() const noexcept { return count.p+count.f; }

    private :

    void set_true(const Var v) noexcept {
      assert(v.first < N);
      assert(v.second < N);
      b[v.first][v.second] = State::placed;
      count_update(v);
      placed_rank_update(v);
      r_update(v);
      c_update(v);
      ad_update(v);
      d_update(v);
    }
    void set_false(const Var v) noexcept {
      assert(v.first < N);
      assert(v.second < N);
      b[v.first][v.second] = State::forbidden;
      count_update(v);
      forbidden_rank_update(v);
    }

    public:
    // We only set a field if it is open:
    void set(const Var v,const bool val) noexcept {
      if (val) set_true(v);
      else set_false(v);
      while(!stack.empty() and !falsified()) {
        Var cur_v = stack.top();
        stack.pop();
        if (b[cur_v.first][cur_v.second] == State::forbidden) falsified_ = true;
        else if (v_open(cur_v)) set_true(cur_v);
        }
      }

      const Ranks& r_rank() const noexcept { return r_ranks; }
      const Ranks& c_rank() const noexcept { return c_ranks; }
      const Board& board() const noexcept { return b; }
  };


  // The prototype:
  class BasicBranching {
    using Var = ChessBoard::Var;
  public :
    const BasicACLS& F;

    BasicBranching(const BasicACLS& F) : F(F) {}

    Var operator()() const noexcept { return Var{0,0}; }

  };


  // A concrete instance of BasicBranching:
  class GreedyAmo {
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
  public :
    typedef double Weight_t;
    typedef std::vector<Weight_t> Weight_vector;
    Weight_vector weight_vector = {4.85,1,0.354,0.11,0.0694};
    const Var_uint vec_size = weight_vector.size();
    const AmoAlo_board& F;

    GreedyAmo(const AmoAlo_board& F) : F(F) {}

    inline Weight_t long_cw(const Var_uint long_c) const noexcept {
      return std::pow(1/1.46,long_c-vec_size)*weight_vector.back();
    }

    Weight_t weight(const Var v) const noexcept {
      const Weight_t amo_w = weight_vector[0]*F.amo_count(v);
      const Var_uint alo_r_cl = F.r_rank()[v.first].o; // alo_r_cl is at-least-one row clause length.
      const Var_uint alo_c_cl = F.c_rank()[v.second].o; // alo_c_cl is at-least-one column clause length.
      assert(alo_r_cl >= 2);
      assert(alo_c_cl >= 2);
      const Weight_t
        alo_r_w = (alo_r_cl > vec_size + 2) ? long_cw(alo_r_cl) : weight_vector[alo_r_cl-2],
        alo_c_w = (alo_c_cl > vec_size + 2) ? 1 + long_cw(alo_c_cl) : weight_vector[alo_c_cl-2];
      return ((1 + amo_w) * ( 1 + alo_r_w + alo_c_w));
    }

    Var operator()() const noexcept {
      Weight_t max = 0;
      Var bv{}; // this is NOT correct in general, since a valid value
      for (ChessBoard::coord_t i = 0; i < F.N ; ++i)
        for (ChessBoard::coord_t j = 0; j < F.N ; ++j)
          if (F.board()[i][j] == State::open) {
            const Weight_t w = weight(Var{i,j});
            if (w > max) {
              max = w;
              bv = Var{i,j};
            }
          }
      return bv;
    }

  };

}
