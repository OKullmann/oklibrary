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

  Add asserts everywhere.
  Add const where appropriate.

For GreedyAmo:
  XXX


XXX work on State enum declaration and board declaration.

Question : How do we know that a solution is found other than placing N queens. If that is the only way, then there is no need for n(), nset() functions. All assignments are complete in this problem.

*/

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

  // A concrete instance of BasicACLS:
  class AmoAlo_board {

  public :
    typedef ChessBoard::coord_t coord_t;
    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;
    typedef std::vector<Rank> Ranks;
    typedef std::stack<Var> Stack;
    typedef int diff_t;
    enum class State { open, placed, forbidden };
    typedef std::vector<std::vector<State>> Board;
  //private :
    const coord_t N;
    Board board;
    Ranks r_rank;
    Ranks c_rank;
    Ranks ad_rank;
    Ranks d_rank;
    Stack stack;
    Var_uint placed_count = 0;
    bool Falsified = false;

    Board b_init(Board board) {
      board.resize(N, std::vector<State>(N));
      return board;
      }
    Ranks r_init (Ranks r_rank) {
      for (Var_uint i = 0; i < N ; ++i) r_rank.push_back(Rank{N,0,0});
      return r_rank;
      }
    Ranks c_init (Ranks c_rank) {
      for (Var_uint i = 0; i < N ; ++i) c_rank.push_back(Rank{N,0,0});
      return c_rank;
      }
    Ranks ad_init (Ranks ad_rank) {
      for (Var_uint i = 1; i < N ; ++i) ad_rank.push_back(Rank{i,0,0});
      for (Var_uint i = N; i > 0 ; --i) ad_rank.push_back(Rank{i,0,0});
      return ad_rank;
      }
    Ranks d_init (Ranks d_rank) {
      for (Var_uint i = 1; i < N ; ++i) d_rank.push_back(Rank{i,0,0});
      for (Var_uint i = N; i > 0 ; --i) d_rank.push_back(Rank{i,0,0});
      return d_rank;
      }
  //public :
    explicit AmoAlo_board(const coord_t N, Board board, Ranks r_rank,
    Ranks c_rank, Ranks ad_rank, Ranks d_rank) :
    N(N),board(b_init(board)),r_rank(r_init(r_rank)),c_rank(c_init(c_rank)),
    ad_rank(ad_init(ad_rank)),d_rank(d_init(d_rank)) {}

    // Returns anti_diagonal starting feild, length and index:
    Diagonal anti_diagonal(Var v) const noexcept {
      coord_t c_sum = v.first + v.second;
      if (c_sum < N) return Diagonal{Var{0,c_sum},c_sum+1,c_sum};
      else return Diagonal{Var{c_sum-N+1,N-1},2*N - (c_sum+1),c_sum};
      }

    // Returns diagonal starting feild, length and index:
    Diagonal diagonal(Var v) const noexcept {
      diff_t c_diff = v.first - v.second;
      if (c_diff > 0) return Diagonal{Var{c_diff,0},N - c_diff,(N-1) - c_diff};
      else return Diagonal{Var{0,-c_diff},N + c_diff,(N-1) - c_diff};
      }

    // Checks if the field v is open:
    bool v_open(Var v) { return (board[v.first][v.second] == State::open); }

    void placed_rank_update(const Var v) {
      Diagonal ad =   anti_diagonal(v);
      Diagonal d = diagonal(v);
      ++r_rank[v.first].p_r;
      ++c_rank[v.second].p_r;
      ++ad_rank[ad.i].p_r;
      ++d_rank[d.i].p_r;
      }
    // Forbidden field ranks are updated only if no field is placed in the same r,c,d or ad
    // and Falsified is updated if found:
    void forbidden_rank_update(const Var v) {
      Diagonal ad =   anti_diagonal(v);
      Diagonal d = diagonal(v);
      if (!r_rank[v.first].p_r) {
        --r_rank[v.first].o_r;
        --c_rank[v.second].o_r;
        --ad_rank[ad.i].o_r;
        --d_rank[d.i].o_r;
        if (r_rank[v.first].o_r == 0 or c_rank[v.second].o_r == 0) Falsified = true;
        else {
          if (r_rank[v.first].o_r == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{v.first,i})) { stack.push(Var{v.first,i}); break; }
          if (c_rank[v.second].o_r == 1)
            for (coord_t i = 0; i < N ; ++i)
              if (v_open(Var{i,v.second})) { stack.push(Var{i,v.second}); break; }
          }
        }
      }

    void r_update(const Var cur_v) {
      for (coord_t i=0 ; i < N ; ++i) {
        Var v = Var{cur_v.first,i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          forbidden_rank_update(v);
          }
        }
      }
    void c_update(const Var cur_v) {
      for (coord_t i=0 ; i < N ; ++i) {
        Var v = Var{i,cur_v.second};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          forbidden_rank_update(v);
          }
        }
      }
    void ad_update(const Var cur_v) {
      Diagonal ad = anti_diagonal(cur_v);
      Var ad_v = ad.s;
      for (coord_t i=0 ; i < ad.l ; ++i) {
        Var v = Var{ad_v.first + i,ad_v.second - i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          forbidden_rank_update(v);
          }
        }
      }
    void d_update(const Var cur_v) {
      Diagonal d = diagonal(cur_v);
      Var d_v = d.s;
      for (coord_t i=0 ; i < d.l ; ++i) {
        Var v = Var{d_v.first + i,d_v.second + i};
        if (v_open(v)) {
          board[v.first][v.second] = State::forbidden;
          forbidden_rank_update(v);
          }
        }
     }

    bool satisfied() const noexcept { return (placed_count == N); }
    bool falsified() const noexcept { return Falsified; }
    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return N; }

    // We only set a field if it is open:
    void set(const Var v, bool val) {
      stack.push(v);
      while(!stack.empty() and !falsified()) {
        Var cur_v = stack.top();
        stack.pop();
        if (val == true ) {
          if (board[cur_v.first][cur_v.second] == State::forbidden) Falsified = true;
          else if (v_open(cur_v)) {
            board[cur_v.first][cur_v.second] = State::placed;
            placed_rank_update(cur_v);
            ++placed_count;
            r_update(cur_v);
            c_update(cur_v);
            ad_update(cur_v);
            d_update(cur_v);
            }
          }
        else {
          board[v.first][v.second] = State::forbidden;
          forbidden_rank_update(v);
          val = true;
          }
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
