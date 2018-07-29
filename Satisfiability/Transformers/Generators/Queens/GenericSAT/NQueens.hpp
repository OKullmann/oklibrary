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

#include <cassert>
#include <stack>
#include <vector>
#include "ChessBoard.hpp"

namespace NQueens {

  // The prototype:
  struct BasicACLS {

    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;

    const ChessBoard::coord_t N;

    explicit BasicACLS(const ChessBoard::coord_t N) : N(N) {}

    bool satisfied() const noexcept { return false; }
    bool falsified() const noexcept { return false; }

    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return 0; }

    void set(const Var v, const bool val) {}

  };

  class Diagonal {
    public :
      ChessBoard::Var s;
      ChessBoard::Var_uint l;
      ChessBoard::Var_uint i;
      Diagonal(ChessBoard::Var s,ChessBoard::Var_uint l, ChessBoard::Var_uint i) : s(s),l(l),i(i) {}
  };

  class Rank {
    public :
      ChessBoard::Var_uint o_r;
      ChessBoard::Var_uint p_r;
      ChessBoard::Var_uint f_r;
      Rank(ChessBoard::Var_uint o_r,ChessBoard::Var_uint p_r, ChessBoard::Var_uint f_r) : o_r(o_r),p_r(p_r),f_r(f_r) {}
  };

  class Count {
    public :
      ChessBoard::Var_uint open;
      ChessBoard::Var_uint placed;
      ChessBoard::Var_uint forbidden;
      Count(ChessBoard::Var_uint open,ChessBoard::Var_uint placed, ChessBoard::Var_uint forbidden) : open(open),placed(placed),forbidden(forbidden) {}
    };
  // A concrete instance of BasicACLS:
  class AmoAlo_board {

  public :
    typedef ChessBoard::coord_t coord_t;
    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;
    typedef std::vector<Rank> Ranks;
    typedef std::stack<Var> Stack;
    typedef std::int32_t diff_t;
    enum class State { open, placed, forbidden };
    typedef std::vector<std::vector<State>> Board;
  //private :
    const coord_t N;
    Board board;
    Ranks r_ranks;
    Ranks c_ranks;
    Ranks ad_ranks;
    Ranks d_ranks;
    Count count;
    Stack stack;
    bool Falsified = false;

    Board b_init(Board board) const noexcept {
      board.resize(N, std::vector<State>(N));
      return board;
      }
    Ranks r_init (Ranks r_ranks) const noexcept {
      for (Var_uint i = 0; i < N ; ++i) r_ranks.push_back(Rank{N,0,0});
      return r_ranks;
      }
    Ranks c_init (Ranks c_ranks) const noexcept {
      for (Var_uint i = 0; i < N ; ++i) c_ranks.push_back(Rank{N,0,0});
      return c_ranks;
      }
    Ranks ad_init (Ranks ad_ranks) const noexcept {
      for (Var_uint i = 1; i < N ; ++i) ad_ranks.push_back(Rank{i,0,0});
      for (Var_uint i = N; i > 0 ; --i) ad_ranks.push_back(Rank{i,0,0});
      return ad_ranks;
      }
    Ranks d_init (Ranks d_ranks) const noexcept {
      for (Var_uint i = 1; i < N ; ++i) d_ranks.push_back(Rank{i,0,0});
      for (Var_uint i = N; i > 0 ; --i) d_ranks.push_back(Rank{i,0,0});
      return d_ranks;
      }

  //public :
    explicit AmoAlo_board(const coord_t N, Board board, Ranks r_ranks,
    Ranks c_ranks, Ranks ad_ranks, Ranks d_ranks) :
    N(N),board(b_init(board)),r_ranks(r_init(r_ranks)),c_ranks(c_init(c_ranks)),
    ad_ranks(ad_init(ad_ranks)),d_ranks(d_init(d_ranks)),count(Count{N*N,0,0}) {}

    // Returns anti_diagonal starting feild, length and index:
    Diagonal anti_diagonal(const Var v) const noexcept {
      coord_t c_sum = v.first + v.second;
      if (c_sum < N) return Diagonal{Var{0,c_sum},c_sum+1,c_sum};
      else return Diagonal{Var{c_sum-N+1,N-1},2*N-(c_sum+1),c_sum};
      }

    // Returns diagonal starting feild, length and index:
    Diagonal diagonal(const Var v) const noexcept {
      diff_t c_diff = v.first - v.second;
      if (c_diff > 0) return Diagonal{Var{c_diff,0},N - c_diff,(N-1)-c_diff};
      else return Diagonal{Var{0,-c_diff},N+c_diff,(N-1)-c_diff};
      }

    // Checks if the field v is open:
    bool v_open(const Var v) const noexcept { return (board[v.first][v.second] == State::open); }

    // Updates the placed rank:
    void placed_rank_update(const Var v) noexcept {
      Diagonal ad =   anti_diagonal(v);
      Diagonal d = diagonal(v);
      ++r_ranks[v.first].p_r;
      ++c_ranks[v.second].p_r;
      ++ad_ranks[ad.i].p_r;
      ++d_ranks[d.i].p_r;
      }

    // Forbidden field ranks are updated only if no field is placed in the same r,c,d or ad
    // and Falsified is updated if found:
    void forbidden_rank_update(const Var v) noexcept {
      Diagonal ad =   anti_diagonal(v);
      Diagonal d = diagonal(v);
      if (!r_ranks[v.first].p_r) {
        --r_ranks[v.first].o_r;
        --c_ranks[v.second].o_r;
        --ad_ranks[ad.i].o_r;
        --d_ranks[d.i].o_r;
        if (r_ranks[v.first].o_r == 0 or c_ranks[v.second].o_r == 0) Falsified = true;
        else {
          if (r_ranks[v.first].o_r == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{v.first,i})) { stack.push(Var{v.first,i}); break; }
          if (c_ranks[v.second].o_r == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{i,v.second})) { stack.push(Var{i,v.second}); break; }
          }
        }
      }

    void count_update(const Var v) noexcept {
      --count.open;
      if (board[v.first][v.second] == State::placed) ++count.placed;
      else ++count.forbidden;
      }

    void r_update(const Var cur_v) noexcept {
      for (coord_t i = 0 ; i < N ; ++i) {
        Var v = Var{cur_v.first,i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
          }
        }
      }
    void c_update(const Var cur_v) noexcept {
      for (coord_t i = 0 ; i < N ; ++i) {
        Var v = Var{i,cur_v.second};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
          }
        }
      }
    void ad_update(const Var cur_v) noexcept {
      Diagonal ad = anti_diagonal(cur_v);
      Var ad_v = ad.s;
      assert(ad.l < N);
      for (coord_t i = 0 ; i < ad.l ; ++i) {
        Var v = Var{ad_v.first + i,ad_v.second - i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
          }
        }
      }
    void d_update(const Var cur_v) noexcept {
      Diagonal d = diagonal(cur_v);
      Var d_v = d.s;
      assert(d.l < N);
      for (coord_t i = 0 ; i < d.l ; ++i) {
        Var v = Var{d_v.first + i,d_v.second + i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          count_update(v);
          forbidden_rank_update(v);
          }
        }
      }

    bool satisfied() const noexcept { return (count.placed == N); }
    bool falsified() const noexcept { return Falsified; }
    Var_uint n() const noexcept { return N*N; }
    Var_uint nset() const noexcept { return count.placed+count.forbidden; }

    void set_true(const Var v) noexcept {
      assert(v.first < N);
      assert(v.second < N);
      board[v.first][v.second] = State::placed;
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
      board[v.first][v.second] = State::forbidden;
      count_update(v);
      forbidden_rank_update(v);
      }
    // We only set a field if it is open:
    void set(const Var v,const bool val) noexcept {
      if (val) set_true(v);
      else set_false(v);
      while(!stack.empty() and !falsified()) {
        Var cur_v = stack.top();
        stack.pop();
        if (board[cur_v.first][cur_v.second] == State::forbidden) Falsified = true;
        else if (v_open(cur_v)) set_true(cur_v);
        }
      }
  };


  // The prototype:
  struct BasicBranching {

    typedef ChessBoard::Var Var;

    const BasicACLS& F;

    BasicBranching(const BasicACLS& F) : F(F) {}

    Var operator()() const noexcept { return Var{0,0}; }

  };

  // A concrete instance of BasicBranching:
  struct GreedyAmo {

    typedef ChessBoard::Var Var;
    typedef ChessBoard::coord_t coord_t;

    const AmoAlo_board& F;

    GreedyAmo(const AmoAlo_board& F) : F(F) {}

    Var operator()() const noexcept {
      for (coord_t i = 0; i < F.N ; ++i)
        for (coord_t j = 0; j < F.N ; ++j)
          if (F.board[i][j] == F.State::open) return Var{i,j};
      }

    // XXX have to add heuristics

  };

}
