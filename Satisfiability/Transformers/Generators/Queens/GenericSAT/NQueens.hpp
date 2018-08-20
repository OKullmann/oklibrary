// Oliver Kullmann, 6.7.2018 (Swansea)

/* TODOS

1. Good statistics on the inference process

2. Simple heuristics

   (a) First open field (this should still have a better q(N) than
       Somers algorithm).
   (b) Random choice (from all the remaining open fields).
   (c) Maximum o-degree.

3. Optimising heuristics

   - A heuristics generalising TawHeuristics, with free weights to be
     optimised.
   - That is, generalising TawHeuristics::heuristics, while keeping operator().
   - Some simple algorithms for optimising the weights need to be written:
     - Using given ranges for the weights, sweep through them in some order,
       and optimise each single weight, by running through the range with
       some given number of steps.
     - Starting this process at random points (within the ranges), and repeat
       often.
   - These optimisations need to be performed for different N.

4. Strengthened inference

   (a) Using that a solution has an even number of white fields.
   (b) Using that a solution occupies exactly N diagonals and exactly N
       antidiagonals.

*/

#include <stack>
#include <vector>
#include <utility>
#include <array>
#include <type_traits>
#include <algorithm>
#include <random>

#include <cassert>
#include <cmath>
#include <cstdint>

#include "ChessBoard.hpp"

namespace NQueens {

  // The prototype:
  struct BasicACLS {

    const ChessBoard::coord_t N;

    explicit BasicACLS(const ChessBoard::coord_t N) : N(N) {}

    // We declare the problem to be satisfied in order to run it without backtracking:
    bool satisfied() const noexcept { return true; }
    bool falsified() const noexcept { return false; }

    // The total number of variables:
    ChessBoard::Var_uint n() const noexcept { return N*N; }
    // Number of variables set to true or false:
    ChessBoard::Var_uint nset() const noexcept { return 0; }

    // Occupy or forbid field v:
    void set(const ChessBoard::Var, const bool) noexcept {}

  };

  // A concrete instance of BasicACLS:
  class AmoAlo_board {
    using coord_t = ChessBoard::coord_t;
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
    using Var_int = ChessBoard::Var_int;
    using Diagonal = ChessBoard::Diagonal;
    using AntiDiagonal = ChessBoard::AntiDiagonal;
    using Rank = ChessBoard::Rank;
    using TotalRank = ChessBoard::TotalRank;
    using State = ChessBoard::State;
  public :
    const coord_t N;

    explicit AmoAlo_board(const coord_t N) :
      N(N), b({N+1, std::vector<State>(N+1)}),
      r_ranks({N+1, {N,0,0}}), c_ranks(r_ranks),
      d_ranks(dad_init()), ad_ranks(d_ranks), trank{N*N,0,0} {
        assert(N <= ChessBoard::max_coord);
        assert(b.size() == N+1);
        assert(r_ranks.size() == N+1);
        assert(c_ranks.size() == N+1);
        assert(d_ranks.size() == 2*N-1);
        assert(ad_ranks.size() == 2*N-1);
        r_ranks[0].o = 0; c_ranks[0].o = 0;
        if (N == 1) set_true({1,1});
    }

    bool satisfied() const noexcept { return trank.p == N; }
    bool falsified() const noexcept { return falsified_; }
    Var_uint n() const noexcept { return N*N; }
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
      if (c_diff >= 0) {
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

    Var_uint odegree(const Var v) const noexcept {
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

    Ranks dad_init() const {
      Ranks ranks(2*N-1);
      Var_uint i = 0;
      for (Var_uint r = 1; r < N ; ++r) ranks[i++].o = r;
      for (Var_uint r = N; r > 0 ; --r) ranks[i++].o = r;
      return ranks;
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

    explicit BasicBranching(const BasicACLS& F) noexcept : F(F) {}

    Var operator()() const noexcept { return {0,0}; }

  };


  /* Exactly the heuristics from tawSolver
     https://github.com/OKullmann/oklibrary/commits/master/Satisfiability/Solvers/TawSolver
     ID a227f64a6c66a817e4b53fa4c1a1244d530a25c5
  */

  class TawHeuristics {
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
    using State = ChessBoard::State;
  public :
    typedef double Weight_t;
    typedef std::pair<Weight_t, Weight_t> Bp;

    const AmoAlo_board& F;

    explicit TawHeuristics(const AmoAlo_board& F) noexcept : F(F) {}

    constexpr static Weight_t weight(const Var_uint cl) noexcept {
      return (cl < size) ? weights[cl] :
        std::pow(basis, -ChessBoard::Var_int((cl-size)+1)) * weights.back();
    }

    Bp heuristics(const Var v) const noexcept {
      return {F.odegree(v) * weight(2),
              weight(F.r_rank(v.first).o) + weight(F.c_rank(v.second).o)};
    }

    Var operator()() const noexcept {
      Weight_t max1 = 0, max2 = 0;
      Var bv{0,0};
      for (ChessBoard::coord_t i = 1; i <= F.N; ++i) {
        if (F.r_rank(i).p != 0) continue;
        const auto& R = F.board()[i];
        Var v; v.first = i;
        for (ChessBoard::coord_t j = 1; j <= F.N ; ++j) {
          if (R[j] != State::open) continue;
          v.second = j;
          const Bp h = heuristics(v);
          const Weight_t prod = h.first * h.second;
          if (prod < max1) continue;
          const Weight_t sum = h.first + h.second;
          if (prod > max1) max1 = prod;
          else if (sum <= max2) continue;
          max2 = sum;
          bv = v;
        }
      }
      return bv;
    }

    private :

    constexpr static Var_uint size{7};
    typedef std::array<Weight_t,size> Weights;
    constexpr static Weights weights{{0, 0, 4.85, 1, 0.354, 0.11, 0.0694}};
    constexpr static Weight_t basis = 1.46;

  };
  constexpr TawHeuristics::Weights TawHeuristics::weights;
  static_assert(TawHeuristics::weight(0) == 0, "TawHeuristics: weight(0)");
  static_assert(TawHeuristics::weight(1) == 0, "TawHeuristics: weight(1)");
  static_assert(TawHeuristics::weight(2) == 4.85, "TawHeuristics: weight(2)");
  static_assert(TawHeuristics::weight(3) == 1, "TawHeuristics: weight(3)");
  static_assert(TawHeuristics::weight(4) == 0.354, "TawHeuristics: weight(4)");
  static_assert(TawHeuristics::weight(5) == 0.11, "TawHeuristics: weight(5)");
  static_assert(TawHeuristics::weight(6) == 0.0694, "TawHeuristics: weight(6)");
  static_assert(TawHeuristics::weight(7) == 0.0694 * std::pow(1.46,-1), "TawHeuristics: weight(7)");
  static_assert(TawHeuristics::weight(8) == 0.0694 * std::pow(1.46,-2), "TawHeuristics: weight(7)");

  // Choosing the first open variable:
    class FirstOpen {
    using Var = ChessBoard::Var;
    using State = ChessBoard::State;
    public :
    const AmoAlo_board& F;
    explicit FirstOpen(const AmoAlo_board& F) noexcept : F(F) {}
    Var operator()() const noexcept {
      for (ChessBoard::coord_t i = 1; i <= F.N; ++i) {
        if (F.r_rank(i).p != 0) continue;
        const auto& R = F.board()[i];
        for (ChessBoard::coord_t j = 1; j <= F.N ; ++j) {
          if (R[j] != State::open) continue;
          return {i,j};
        }
      }
      assert(false);
      return {};
    }
  };

  // Fixing a random order of fields, and choosing the first open variable:
    class FirstOpenRandom {
      using Var = ChessBoard::Var;
      using coord_t = ChessBoard::coord_t;
      using Var_uint = ChessBoard::Var_uint;
      typedef std::vector<Var> varvec_t;
    public :
      const AmoAlo_board& F;
      explicit FirstOpenRandom(const AmoAlo_board& F) noexcept : F(F) {}
      Var operator()() const noexcept {
        assert(Var_uint(F.N) * Var_uint(F.N) == random_permutation.size());
        for (const Var v : random_permutation) if (F.open(v)) return v;
        assert(false);
        return {};
      }
      typedef std::uint32_t seed_t;
      typedef std::vector<seed_t> vec_seed_t;
      static void init(const coord_t N, const vec_seed_t s = vec_seed_t{}) {
        random_permutation.resize(Var_uint(N) * Var_uint(N));
        {varvec_t::size_type index = 0;
         for (coord_t i = 1; i <= N; ++i)
           for (coord_t j = 1; j <= N; ++j)
             random_permutation[index++] = {i,j};
        }
        std::seed_seq seq(s.begin(), s.end());
        std::shuffle(random_permutation.begin(), random_permutation.end(), std::mt19937_64(seq));
      }
    private :
      static varvec_t random_permutation;
    };
    FirstOpenRandom::varvec_t FirstOpenRandom::random_permutation;

}
