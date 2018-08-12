// Oliver Kullmann, 6.7.2018 (Swansea)

#include <stack>
#include <vector>
#include <utility>

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
    const coord_t N;

    explicit AmoAlo_board(const coord_t N) :
      N(N), b(b_init()), r_ranks(r_init()), c_ranks(c_init()),
      d_ranks(d_init()), ad_ranks(ad_init()), trank{N*N,0,0} {
        assert(N <= ChessBoard::max_coord);
        assert(b.size() == N+1);
        assert(r_ranks.size() == N+1);
        assert(c_ranks.size() == N+1);
        assert(d_ranks.size() == 2*N-1);
        assert(ad_ranks.size() == 2*N-1);
        if (N == 1) set_true({1,1});
    }

    bool satisfied() const noexcept { return trank.p == N; }
    bool falsified() const noexcept { return falsified_; }
    // The total number of variables:
    Var_uint n() const noexcept { return N*N; }
    // Number of variables set to true or false:
    Var_uint nset() const noexcept { return trank.p+trank.f; }

    void set(const Var v, const bool val) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      assert(r_ranks[v.first].o >= 2);
      assert(c_ranks[v.second].o >= 2);
      if (val) set_true(v); else set_false(v);
      while(not stack.empty() and not falsified()) {
        const Var cur_v = stack.top(); stack.pop();
        if (b[cur_v.first][cur_v.second] == State::forbidden)
          falsified_ = true;
        else if (open(cur_v)) set_true(cur_v);
      }
    }

    typedef std::vector<Rank> Ranks;
    typedef std::vector<std::vector<State>> Board;
    const Ranks& r_rank() const noexcept { return r_ranks; }
    const Rank& r_rank(const coord_t i) const noexcept { return r_ranks[i]; }
    const Ranks& c_rank() const noexcept { return c_ranks; }
    const Rank& c_rank(const coord_t j) const noexcept { return c_ranks[j]; }
    const Ranks& d_rank() const noexcept { return d_ranks; }
    const Rank& d_rank(const coord_t i) const noexcept { return d_ranks[i]; }
    const Ranks& ad_rank() const noexcept { return ad_ranks; }
    const Rank& ad_rank(const coord_t i) const noexcept { return ad_ranks[i]; }
    const TotalRank& t_rank() const noexcept { return trank; }
    const Board& board() const noexcept { return b; }
    State board(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
    bool open(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return (board(v) == State::open);
    }


    // Returns diagonal starting field, length and index:
    Diagonal diagonal(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const ChessBoard::scoord_t c_diff = v.first - v.second;
      if (c_diff > 0) {
        const coord_t cd = c_diff; return {{cd+1,1}, N-cd, N-cd-1};
      }
      else {
        const coord_t cd = -c_diff; return {{1,cd+1}, N-cd, N+cd-1};
      }
    }
    // Returns anti_diagonal starting field, length and index:
    AntiDiagonal anti_diagonal(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const coord_t c_sum = v.first + v.second;
      if (c_sum <= N) return {{1,c_sum-1}, c_sum-1, c_sum-2};
      else return {{c_sum-N,N}, 2*N-(c_sum-1), c_sum-2};
    }

    Var_uint amo_count(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      const Diagonal d = diagonal(v);
      const AntiDiagonal ad = anti_diagonal(v);
      assert(d.i < d_ranks.size());
      assert(ad.i < ad_ranks.size());
      return r_ranks[v.first].o + c_ranks[v.second].o + d_ranks[d.i].o + ad_ranks[ad.i].o - 4;
    }

  private:

    // Updates the corresponding r,c,d and ad ranks of placed field:
    void placed_rank_update(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::placed);
      ++r_ranks[v.first].p;
      ++c_ranks[v.second].p;
      ++d_ranks[diagonal(v).i].p;
      ++ad_ranks[anti_diagonal(v).i].p;
    }

    // Forbidden field ranks of r,c,d or ad are updated individually
    // only if no field is placed in them and falsified_ is updated if found:
    void forbidden_rank_update(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::forbidden);
      if (r_ranks[v.first].p != 1) {
        --r_ranks[v.first].o;
        ++r_ranks[v.first].f;
        if (r_ranks[v.first].o == 0)
          falsified_ = true;
        if (r_ranks[v.first].o == 1)
          for (coord_t i = 1; i <= N ; ++i)
            if (open({v.first,i})) {stack.push({v.first,i}); break;}
      }
      if (c_ranks[v.second].p != 1) {
        --c_ranks[v.second].o;
        ++c_ranks[v.second].f;
        if (c_ranks[v.second].o == 0)
          falsified_ = true;
        if (c_ranks[v.second].o == 1)
          for (coord_t i = 1; i <= N ; ++i)
            if (open({i,v.second})) {stack.push({i,v.second}); break;}
      }
      const Diagonal d = diagonal(v);
      assert(d.i < d_ranks.size());
      if (d_ranks[d.i].p != 1) {
        --d_ranks[d.i].o;
        ++d_ranks[d.i].f;
      }
      const AntiDiagonal ad = anti_diagonal(v);
      assert(ad.i < ad_ranks.size());
      if (ad_ranks[ad.i].p != 1) {
        --ad_ranks[ad.i].o;
        ++ad_ranks[ad.i].f;
      }
    }

    // Called if v is set to placed or forbidden:
    void trank_update(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) != State::open);
      assert(trank.o+trank.p+trank.f == n());
      --trank.o;
      if (board(v) == State::placed) ++trank.p;
      else ++trank.f;
    }

    void r_update(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      for (coord_t i = 1 ; i <= N ; ++i) {
        const Var v = {cur_v.first,i};
        if (open(v)) {
          board(v) = State::forbidden;
          trank_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void c_update(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      for (coord_t i = 1 ; i <= N ; ++i) {
        const Var v = {i,cur_v.second};
        if (open(v)) {
          board(v) = State::forbidden;
          trank_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void d_update(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      const Diagonal d = diagonal(cur_v);
      const Var d_v = d.s;
      assert(d.l <= N);
      for (coord_t i = 0 ; i < d.l ; ++i) {
        const Var v = {d_v.first + i,d_v.second + i};
        if (open(v)) {
          board(v) = State::forbidden;
          trank_update(v);
          forbidden_rank_update(v);
        }
      }
    }
    void ad_update(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      const AntiDiagonal ad = anti_diagonal(cur_v);
      const Var ad_v = ad.s;
      assert(ad.l <= N);
      for (coord_t i = 0 ; i < ad.l ; ++i) {
        const Var v = {ad_v.first + i,ad_v.second - i};
        if (open(v)) {
          board(v) = State::forbidden;
          trank_update(v);
          forbidden_rank_update(v);
        }
      }
    }

    void set_true(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      board(v) = State::placed;
      trank_update(v);
      placed_rank_update(v);
      r_update(v);
      c_update(v);
      d_update(v);
      ad_update(v);
    }
    void set_false(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      board(v) = State::forbidden;
      trank_update(v);
      forbidden_rank_update(v);
    }

    Board b;
    Ranks r_ranks;
    Ranks c_ranks;
    Ranks d_ranks;
    Ranks ad_ranks;
    TotalRank trank;
    typedef std::stack<Var> Stack;
    Stack stack;
    bool falsified_ = false;

    Board b_init() const {
      Board board;
      board.resize(N+1, std::vector<State>(N+1));
      return board;
    }
    Ranks r_init() const {
      Ranks r_ranks; r_ranks.reserve(N+1);
      r_ranks.push_back({});
      for (Var_uint i = 1; i <= N ; ++i) r_ranks.push_back({N,0,0});
      return r_ranks;
    }
    Ranks c_init() const {
      Ranks c_ranks; c_ranks.reserve(N+1);
      c_ranks.push_back({});
      for (Var_uint i = 1; i <= N ; ++i) c_ranks.push_back({N,0,0});
      return c_ranks;
    }
    Ranks d_init() const {
      Ranks d_ranks; d_ranks.reserve(2*N-1);
      for (Var_uint i = 1; i < N ; ++i) d_ranks.push_back({i,0,0});
      for (Var_uint i = N; i > 0 ; --i) d_ranks.push_back({i,0,0});
      return d_ranks;
    }
    Ranks ad_init() const {
      Ranks ad_ranks; ad_ranks.reserve(2*N-1);
      for (Var_uint i = 1; i < N ; ++i) ad_ranks.push_back({i,0,0});
      for (Var_uint i = N; i > 0 ; --i) ad_ranks.push_back({i,0,0});
      return ad_ranks;
    }

    State& board(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
  };


  // The prototype:
  class BasicBranching {
    using Var = ChessBoard::Var;
  public :
    const BasicACLS& F;

    BasicBranching(const BasicACLS& F) : F(F) {}

    Var operator()() const noexcept { return {0,0}; }

  };


  // A concrete instance of BasicBranching with TawSolver heuristics:
  class TawBranching {
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
  public :
    typedef double Weight_t;
    typedef std::vector<Weight_t> Weights;
    /* Using weights from TawSolver
       https://github.com/OKullmann/oklibrary/commits/master/Satisfiability/Solvers/TawSolver
       ID a227f64a6c66a817e4b53fa4c1a1244d530a25c5
    */
    const Weights weights = {0, 0, 4.85, 1, 0.354, 0.11, 0.0694};
    const Var_uint size = weights.size();
    typedef std::pair<Weight_t, Weight_t> Bp;

    const AmoAlo_board& F;

    TawBranching(const AmoAlo_board& F) : F(F) {}

    Weight_t weight(const Var_uint cl) const noexcept {
      if (cl < size) return weights[cl];
      else return std::pow(1.46, -ChessBoard::Var_int((cl - size) + 1)) * weights.back();
    }

    Bp heuristics(const Var v) const noexcept {
      const Weight_t amo_w = F.amo_count(v) * weight(2);
      const Var_uint alo_r_cl = F.r_rank()[v.first].o;
      const Var_uint alo_c_cl = F.c_rank()[v.second].o;
      assert(alo_r_cl >= 2);
      assert(alo_c_cl >= 2);
      const Weight_t
        alo_r_w = weight(alo_r_cl),
        alo_c_w = weight(alo_c_cl);
      return Bp{amo_w, alo_r_w + alo_c_w};
    }

    Var operator()() const noexcept {
      Weight_t max1 = 0, max2 = 0;
      Var bv{};
      for (ChessBoard::coord_t i = 1; i <= F.N ; ++i) {
        if (F.r_rank(i).p != 0) continue;
        for (ChessBoard::coord_t j = 1; j <= F.N ; ++j)
          if (F.board({i,j}) == State::open) {
            const Bp h = heuristics({i,j});
            const Weight_t prod = h.first * h.second;
            if (prod < max1) continue;
            const Weight_t sum = h.first + h.second;
            if (prod > max1) max1 = prod;
            else if (sum <= max2) continue;
            max2 = sum;
            bv = {i,j};
          }
      }
      return bv;
    }

  };

}
