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

For GreedyAmo:
  XXX


XXX work on state enum declaration and board declaration.

Question : How do we know that a solution is found other than placing N queens. If that is the only way, then there is no need for n(), nset() functions. All assignments are complete in this problem.

*/
#include "ChessBoard.hpp"
#include <vector>
#include <tuple>
#include <stdlib.h>
#include <stack>

#define not_valid 1000            //change this to appropriate value later.

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

  // A concrete instance of BasicACLS:
  class AmoAlo_board {

  public :
    typedef ChessBoard::coord_t coord_t;
    typedef ChessBoard::Var Var;
    typedef ChessBoard::Var_uint Var_uint;
    typedef std::vector<Var_uint> Ranks;
    typedef std::tuple<Var,Var_uint,Var_uint> Diagonal;
    typedef int diff_t;
    std::stack<Var> var_stack;
    enum state { unset = 0 , placed, forbidden };
  //private :
    std::vector<std::vector<state>> board;
    Ranks r_rank;
    Ranks c_rank;
    Ranks ad_rank;
    Ranks d_rank;
    Var_uint placed_count = 0;
    bool unsatisfiable = false;

  //public :
    const coord_t N;
    explicit AmoAlo_board(const coord_t N) : N(N) {
      board.resize(N, std::vector<state>(N));
      for (Var_uint i = 0; i < N ; ++i) r_rank.push_back(N);
      for (Var_uint i = 0; i < N ; ++i) c_rank.push_back(N);
      for (Var_uint i = 1; i < N ; ++i) {
        ad_rank.push_back(i);
        d_rank.push_back(i);
        }
      for (Var_uint i = N; i > 0 ; --i) {
        ad_rank.push_back(i);
        d_rank.push_back(i);
        }
    }

    // Returns anti_diagonal starting feild, length and index:
    Diagonal anti_diagonal(Var v) const noexcept {
      coord_t c_sum = v.first + v.second;
      if (c_sum < N) return std::make_tuple(Var{0,c_sum},c_sum+1,c_sum);
      else return std::make_tuple(Var{c_sum-N+1,N-1},2*N - (c_sum+1),c_sum);
      }

    // Returns diagonal starting feild, length and index:
    Diagonal diagonal(Var v) const noexcept {
      diff_t c_diff = v.first - v.second;
      if (c_diff > 0) return std::make_tuple(Var{c_diff,0},N - c_diff,(N-1) - c_diff);
      else return std::make_tuple(Var{0,-c_diff},N + c_diff,(N-1) - c_diff);
      }

    // Checks if the field v is unset:
    bool v_unset(Var v) { return (board[v.first][v.second] == unset); }

    // Updates the field to forbidden:
    void field_update(Var v) {
        board[v.first][v.second] = forbidden;
        rank_update(v,false);
     }

    // When a field is forbidden the ranks are updated and unsatisfiable is updated if found:
    void rank_update(Var v, const bool val) {
      Diagonal ad =   anti_diagonal(v);
      Diagonal d = diagonal(v);
      if (val) {
        r_rank[v.first] = not_valid;
        c_rank[v.second] = not_valid;
        ad_rank[std::get<2>(ad)] = not_valid;
        d_rank[std::get<2>(d)] = not_valid;
        }
      else if (r_rank[v.first] != not_valid) {
        --r_rank[v.first];
        --c_rank[v.second];
        --ad_rank[std::get<2>(ad)];
        --d_rank[std::get<2>(d)];
        if (r_rank[v.first] == 0 or c_rank[v.second] == 0) unsatisfiable = true;
        if (r_rank[v.first] == 1)
          for (coord_t i = 0; i < N ; ++i)
            if (v_unset(Var{v.first,i})) var_stack.push(Var{v.first,i});

        if (c_rank[v.second] == 1)
          for (coord_t i = 0; i < N ; ++i)
            if (v_unset(Var{i,v.second})) var_stack.push(Var{i,v.second});
        }
      }

    void r_update(const Var cur_v) {
      for (coord_t i=0 ; i < N ; ++i) {
        Var v = Var{cur_v.first,i};
        if (v_unset(v)) field_update(v);
        }
      }
    void c_update(const Var cur_v) {
      for (coord_t i=0 ; i < N ; ++i) {
        Var v = Var{i,cur_v.second};
        if (v_unset(v)) field_update(v);
        }
      }
    void ad_update(const Var cur_v) {
      Diagonal ad = anti_diagonal(cur_v);
      Var ad_v = std::get<0>(ad);
      for (coord_t i=0 ; i < std::get<1>(ad) ; ++i) {
        Var v = Var{ad_v.first + i,ad_v.second - i};
        if (v_unset(v)) field_update(v);
        }
      }
    void d_update(const Var cur_v) {
      Diagonal d = diagonal(cur_v);
      Var d_v = std::get<0>(d);
      for (coord_t i=0 ; i < std::get<1>(d) ; ++i) {
        Var v = Var{d_v.first + i,d_v.second + i};
        if (v_unset(v)) field_update(v);
        }
     }

    bool satisfied() const noexcept { return (placed_count == N); }
    bool falsified() const noexcept { return unsatisfiable; }
    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return N; }

    // We only set a field if it is unset:
    void set(const Var v, bool val) {
      var_stack.push(v);
      while(!var_stack.empty()) {
        Var cur_v = var_stack.top();
        var_stack.pop();
        if (val == true ) {
          if (board[cur_v.first][cur_v.second] == forbidden) unsatisfiable = true;
          else if (v_unset(cur_v)) {
            board[cur_v.first][cur_v.second] = placed;
            rank_update(cur_v,val);
            ++placed_count;
            r_update(cur_v);
            c_update(cur_v);
            ad_update(cur_v);
            d_update(cur_v);
            }
          }
        else {
          field_update(cur_v);
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
          if (F.board[i][j] == F.unset) return Var{i,j};
      }

    // XXX have to add heuristics

  };

}
