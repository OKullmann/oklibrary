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
    typedef std::vector<Var_uint> rank_t;
		typedef std::tuple<Var,Var_uint,Var_uint> diagonal_t;
    typedef int diff_t;
    std::stack<Var> var_stack;
    enum state { unset = 0 , placed, forbidden };
  //private :
    std::vector<std::vector<state>> board;
    rank_t r_rank;
    rank_t c_rank;
		rank_t d_rank;
		rank_t ad_rank;
    Var_uint placed_count = 0;
    bool unsatisfiable = false;

  //public :
    const coord_t N;
    explicit AmoAlo_board(const coord_t N) : N(N){
      board.resize(N, std::vector<state>(N));
      for (Var_uint i = 0; i < N ; ++i) r_rank.push_back(N);
      for (Var_uint i = 0; i < N ; ++i) c_rank.push_back(N);
      for (Var_uint i = 1; i < N ; ++i) {
        d_rank.push_back(i);
        ad_rank.push_back(i); }
      for (Var_uint i = N; i > 0 ; --i) {
        d_rank.push_back(i);
        ad_rank.push_back(i); }
    }

    //returns diagonal starting feild, len and index :
    diagonal_t diagonal(Var v) const noexcept {
      coord_t c_sum = v.first + v.second;
      Var new_v;
			Var_uint len;
      Var_uint d_index = c_sum;
      if (c_sum < N) {
        new_v.first = 0;
        new_v.second = c_sum;
        len = c_sum + 1; }
      else {
        new_v.first = c_sum-N+1;
        new_v.second = N-1;
        len = 2*N - (c_sum + 1); }
      return std::make_tuple(new_v,len,d_index); }

    //returns anti_diagonal starting feild, len and index :
    diagonal_t anti_diagonal(Var v) const noexcept {
      diff_t c_diff = v.first - v.second;
      Var new_v;
			Var_uint len;
      Var_uint d_index;
      if (c_diff > 0) {
        new_v.first = c_diff;
        new_v.second = 0;
        len = N - c_diff; }
      else {
        new_v.first = 0;
        new_v.second = -1*c_diff;
        len = N + c_diff; }
        d_index = (N-1) - c_diff;
      return std::make_tuple(new_v,len,d_index); }

    //checks if the field v is unset :
    bool v_unset(Var v) {
      if (board[v.first][v.second] == unset) return 1;
      else return 0; }

    //updates the field to forbidden :
    void field_update(Var v) {
      if (v_unset(v)) {
        board[v.first][v.second] = forbidden;
        rank_update(v,false); }
     }

    //when a field is forbidden the ranks are updated and unsatisfiable is updated if found :
    void rank_update(Var v, const bool val) {
      diagonal_t d = diagonal(v);
      diagonal_t ad = anti_diagonal(v);
      if (val) {
        r_rank[v.first] = not_valid;
        c_rank[v.second] = not_valid;
        d_rank[std::get<2>(d)] = not_valid;
        ad_rank[std::get<2>(ad)] = not_valid;
        }
      else if (r_rank[v.first] != not_valid) {
        --r_rank[v.first];
        --c_rank[v.second];
        --d_rank[std::get<2>(d)];
        --ad_rank[std::get<2>(ad)];
        if (r_rank[v.first] == 0 or c_rank[v.second] == 0) unsatisfiable = true;
        if (r_rank[v.first] == 1)
          for (coord_t i = 0; i < N ; ++i)
            if (v_unset(Var{v.first,i})) var_stack.push(Var{v.first,i});

        if (c_rank[v.second] == 1)
          for (coord_t i = 0; i < N ; ++i)
            if (v_unset(Var{i,v.second})) var_stack.push(Var{i,v.second});
        }
      }

    bool satisfied() const noexcept {
      if (placed_count == N) return true;
      else return false; }
    bool falsified() const noexcept {
      return unsatisfiable; }
    Var_uint n() const noexcept { return N; }
    Var_uint nset() const noexcept { return N; }

    //we only set a field if it is unset
    void set(const Var v, bool val) {
    var_stack.push(v);
    while(!var_stack.empty()) {
      Var cur_v = var_stack.top();
      var_stack.pop();
      if (val == true ) {
        if (board[cur_v.first][cur_v.second] == forbidden) unsatisfiable = true;
        else if (v_unset(cur_v)){
          board[cur_v.first][cur_v.second] = placed;
          rank_update(cur_v,val);
          ++placed_count;
          for (coord_t i=0 ; i < N ; ++i) {
            Var n_v = Var{cur_v.first,i};
            field_update(n_v); }

          for (coord_t i=0 ; i < N ; ++i) {
            Var n_v = Var{i,cur_v.second};
            field_update(n_v); }

          diagonal_t d = diagonal(cur_v);
          Var d_v = std::get<0>(d);
          for (coord_t i=0 ; i < std::get<1>(d) ; ++i) {
            Var n_v = Var{d_v.first + i,d_v.second - i};
            field_update(n_v); }

          diagonal_t ad = anti_diagonal(cur_v);
          Var ad_v = std::get<0>(ad);
          for (coord_t i=0 ; i < std::get<1>(ad) ; ++i) {
            Var n_v = Var{ad_v.first + i,ad_v.second + i};
            field_update(n_v); }
          }
        }
      else {
          board[cur_v.first][cur_v.second] = forbidden;
          rank_update(cur_v,val);
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
          if (F.board[i][j] == F.unset) return Var{i,j}; }

    // XXX have to add heuristics

  };

}
