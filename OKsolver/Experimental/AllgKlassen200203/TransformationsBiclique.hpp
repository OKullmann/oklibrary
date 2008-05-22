// Oliver Kullmann, 12.4.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TransformationsBiclique.hpp
  \brief  Components for transforming the problem of finding a biclique partitioning
  of size at most max_bc for a multigraph given by its adjacency matrix.

  \deprecated Old code

*/

#ifndef TRANSFORMATIONSWAECHTER_845rFew2
#define TRANSFORMATIONSWAECHTER_845rFew2

#include <ostream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <list>
#include <map>
#include <string>

#include <Transitional/General/ErrorHandling.hpp>
#include <Transitional/General/StringHandling.hpp>
#include <Transitional/General/TimeHandling.hpp>
#include <Transitional/General/StreamHandling.hpp>

namespace TransformationsBiclique {

  /*!
    \enum error_codes_precondition
    \brief Error codes for checking the inputs (the adjacency matrices and
    the upper bound on the number of bicliques)
  */
  enum error_codes_precondition { correct = 0, non_positive_dimension = 1, negative_number_bicliques = 2, non_zero_diagonal = 3, negative_entry = 4, non_symmetric = 5 };

  /*!
    \brief Free-standing function for checking validity of the input
  */
  template <class Matrix>
  error_codes_precondition preconditions(const Matrix& M, const typename Matrix::index_type dim, const unsigned int max_bc) {
    if (dim <= 0) return non_positive_dimension;
    if (max_bc < 0) return negative_number_bicliques;
    typedef typename Matrix::index_type index_type;
    typedef typename Matrix::value_type value_type;
    for (index_type i = 0; i < dim; ++i)
      if (M(i,i) != 0) return non_zero_diagonal;
    for (index_type i = 0; i < dim; ++i)
      for (index_type j = i+1; i < dim; ++i) {
	const value_type v = M(i,j);
	if (v < 0) return negative_entry;
	if (M(i,j) != M(j,i)) return non_symmetric;
      }
    return correct;
  }

  /*!
    \brief Function for computing the number of variables used by the translation

    n = 2 * max_bc * (sum of entries of M).
  */
  template <class Matrix, class Formula>
  typename Formula::size_type number_variables_of_transformation(const Matrix& M, const typename Matrix::index_type dim, const unsigned int max_bc, Formula& F) {
    assert(preconditions(M, dim, max_bc) == correct);
    typename Formula::size_type sum = 0;
    for (typename Matrix::index_type i = 0; i < dim; ++i)
      sum = std::accumulate(M[i] + i + 1, M[i] + dim, sum);
    sum *= max_bc * 2;
    return sum;
  }
  /*!
    \brief Function for computing the number of clauses created by the translation
  */
  template <class Matrix, class Formula>
  typename Formula::size_type number_clauses_of_transformation(const Matrix& M, const typename Matrix::index_type dim, const unsigned int max_bc, Formula& F) {
    assert(preconditions(M, dim, max_bc) == correct);
    typedef typename Matrix::index_type index_type;
    typedef typename Matrix::value_type value_type;
    typedef typename Formula::size_type size_type;

    size_type sum_I = 0;
    for (index_type i = 0; i < dim; ++i)
      sum_I = std::accumulate(M[i] + i + 1, M[i] + dim, sum_I);

    size_type sum_II = sum_I * ( ((2 * max_bc) * (2 * max_bc - 1)) / (1 * 2) );

    size_type sum_III = 0;
    for (index_type i = 0; i < dim; ++i)
      for (index_type j = i+1; j < dim; ++j) {
	const value_type v = M(i,j);
	sum_III += v * (v-1);
      }
    sum_III *= 2 * max_bc;

    class SizeIntersect {
      typedef index_type ivec[2];
      ivec a, b, c;
    public :
      index_type operator() (index_type i1, index_type j1, index_type i2, index_type j2) {
	assert(i1 < j1 and i2 < j2);
	a[0] = i1; a[1] = j1;
	b[0] = i2; b[1] = j2;
	return std::set_intersection(a, a+2, b, b+2, c) - c;
      }
    };
    SizeIntersect SI;

    size_type sum_IV = 0;
    for (index_type i = 0; i < dim; ++i)
      for (index_type j = i+1; j < dim; ++j) {
	size_type sum = 0;
	for (index_type i2 = 0; i2 < dim; ++i2)
	  for (index_type j2 = i2+1; j2 < dim; ++j2)
	    if (SI(i,j,i2,j2) == 1)
	      sum += M(i2,j2);
	sum_IV += M(i,j) * sum;
      }
    sum_IV *= (2 * max_bc) / 2;

    size_type sum_V = 0;
    for (index_type i = 0; i < dim; ++i)
      for (index_type j = i+1; j < dim; ++j) {
	size_type sum = 0;
	for (index_type i2 = 0; i2 < dim; ++i2)
	  for (index_type j2 = i2+1; j2 < dim; ++j2)
	    if (SI(i,j,i2,j2) == 0)
	      sum += M(i2,j2);
	sum_V += M(i,j) * sum;
      }
    sum_V *= (4 * 2 * max_bc) / 2;

    return sum_I + sum_II + sum_III + sum_IV + sum_V;
  }
  
  /*!
    \brief Translating an adjacency matrix M and a desired max biclique-number
    max_bc into the (boolean CNF) SAT problem F.

    Given the matrix M of dimension dim, and two nodes (i.e., indices)
    i and j, we have M(i,j) conflicts between i and j. Using indices
    0 <= v < M(i,j), for each such conflict we have 2 * max_bc variables,
    such that exactly one of them must become true, where the first chunk of
    max_bc variables asserts that this conflict is in the corresponding biclique
    with orientation from "left to right", while the second chunk of max_bc
    variables refers to the opposite orientation.

  */
  template <class Matrix, class Formula>
  void symmetric_conflict_number_to_SAT(const Matrix& M, const typename Matrix::index_type dim, const unsigned int max_bc, Formula& F) {

    assert(preconditions(M, dim, max_bc) == correct);

    typedef typename Matrix::index_type index_type;
    typedef typename Matrix::value_type value_type;

    const unsigned int max_bc_sides = 2 * max_bc;

    /*!
      \class Var
      \brief Nested class for the creation of variable identifiers.

      The positive literal RrowCcolumnNvBk asserts, that the conflict
      with index v between node row and node column is in the biclique with
      index k % max_bc, where node row is in the "left side" if k / max_bc = 0,
      and is in the "right side" if k / max_bc = 1.
    */
    class Var {
      const Matrix& M;
      const typename Matrix::index_type dim;
      const unsigned int max_bc_sides;
    public :
      Var (const Matrix& M, typename Matrix::index_type dim, unsigned int max_bc_sides) : M(M), dim(dim), max_bc_sides(max_bc_sides) {}

      std::string operator() (index_type row, index_type column, value_type v, unsigned int k) {
	assert(0 <= row and row < column and column < dim);
	assert(0 <= v and v < M(row, column));
	assert(0 <= k and k < max_bc_sides);
	using StringHandling::toString;
	return std::string("R" + toString(row) + "C" + toString(column) + "N" + toString(v) + "B" + toString(k));
      }
    };
    Var V(M, dim, max_bc_sides);

    /*!
      \class Rev
      \brief Simple helper class to compute for a given biclique index k
      the index for the biclique with "sides reversed".
    */
    class Rev {
      const unsigned int max_bc;
      const unsigned int max_bc_sides;
    public :
      Rev(unsigned int max_bc, unsigned int max_bc_sides) :max_bc(max_bc), max_bc_sides(max_bc_sides) {}
      unsigned int operator() (unsigned int k) {
	assert(0 <= k and k < max_bc_sides);
	return (k < max_bc) ? k + max_bc : k - max_bc;
      }
    };
    Rev R(max_bc, max_bc_sides);

    F.set_number_variables(number_variables_of_transformation(M, dim, max_bc, F));
    F.set_number_clauses(number_clauses_of_transformation(M, dim, max_bc, F));
    F.begin_construction();

    F.comment_line("Transformations::symmetric_conflict_number_to_SAT");
    F.comment_line(TimeHandling::currentDateTime("%A, %B %d, %H:%M, %Y"));
    F.comment_line("Dimension = " + StringHandling::toString(dim) +", Maximal number of bicliques = " + StringHandling::toString(max_bc));
    F.comment_line("Matrix:");
    for (index_type i = 0; i < dim; ++i) {
      std::ostringstream s;
      std::copy(M[i], M[i] + dim, StreamHandling::output_iterator<value_type, 3>(s, " "));
      F.comment_line(s.str());
    }

    using Values::pos; using Values::neg;

    F.explanation("Each conflict belongs to exactly one biclique.");
    F.explanation("Parallel conflicts belong to different bicliques.");
    for (index_type i = 0; i < dim; ++i)
      for (index_type j = i+1; j < dim; ++j) {
	const value_type value = M(i,j);
	// Part I
	// every conflict is element of at least one biclique
	for (value_type v = 0; v < value; ++v) {
	    F.new_clause();
	    for (unsigned int k = 0; k < max_bc_sides; ++k)
	      F.add(V(i,j,v,k), pos);
	  }
	// Part II
	// every conflict is element of at most one biclique
	for (value_type v = 0; v < value; ++v) {
	  for (unsigned int k1 = 0; k1 < max_bc_sides; ++k1)
	    for (unsigned int k2 = k1+1; k2 < max_bc_sides; ++k2) {
	      F.new_clause();
	      F.add(V(i,j,v,k1), neg).add(V(i,j,v,k2), neg);
	    }
	}
	// Part III
	// parallel edges are in different bicliques
	for (value_type v1 = 0; v1 < value; ++v1)
	  for (value_type v2 = v1+1; v2 < value; ++v2)
	    for (unsigned int k = 0; k < max_bc; ++k) {
	      F.new_clause();
	      F.add(V(i,j,v1,k), neg).add(V(i,j,v2,k), neg);
	      F.new_clause();
	      F.add(V(i,j,v1,R(k)), neg).add(V(i,j,v2,k), neg);
	      F.new_clause();
	      F.add(V(i,j,v1,k), neg).add(V(i,j,v2,R(k)), neg);
	      F.new_clause();
	      F.add(V(i,j,v1,R(k)), neg).add(V(i,j,v2,R(k)), neg);
	    }
      }

    // Part IV
    F.explanation("The biclique assignments are consistent.");
    for (index_type i1 = 0; i1 < dim; ++i1)
      for (index_type j1 = i1+1; j1 < dim; ++j1) {
	const value_type value1 = M(i1,j1);
	for (value_type v1 = 0; v1 < value1; ++v1) {
	  // conflicts with the same begin
	  for (index_type j2 = j1+1; j2 < dim; ++j2) {
	    const value_type value2 = M(i1,j2);
	    for (value_type v2 = 0; v2 < value2; ++v2)
	      for (unsigned int k = 0; k < max_bc_sides; ++k) {
		F.new_clause();
		F.add(V(i1,j1,v1,k), neg).add(V(i1,j2,v2,R(k)), neg);
	      }
	  }
	  // conflicts with the same end
	  for (index_type i2 = 0; i2 < i1; ++i2) {
	    const value_type value2 = M(i2,j1);
	    for (value_type v2 = 0; v2 < value2; ++v2)
	      for (unsigned int k = 0; k < max_bc_sides; ++k) {
		F.new_clause();
		F.add(V(i1,j1,v1,k), neg).add(V(i2,j1,v2,R(k)), neg);
	      }
	  }
	  // "succeeding" conflicts
	  {
	    for (index_type j2 = j1+1; j2 < dim; ++j2) {
	      const value_type value2 = M(j1,j2);
	      for (value_type v2 = 0; v2 < value2; ++v2)
		for (unsigned int k = 0; k < max_bc_sides; ++k) {
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(j1,j2,v2,k), neg);
		}
	    }
	  }
	}
      }

    // Part V
    F.explanation("All bicliques are complete.");
    // If conflicts (a,b) and (c,d) are non-incident and in the same
    // biclique, then also conflicts (a,c) and (b,d) are in this
    // biclique.
    for (index_type i1 = 0; i1 < dim; ++i1)
      for (index_type j1 = i1+1; j1 < dim; ++j1) {
	const value_type value1 = M(i1,j1);
	for (value_type v1 = 0; v1 < value1; ++v1) {
	  for (index_type i2 = i1+1; i2 < j1; ++i2) {
	    for (index_type j2 = i2+1; j2 < j1; ++j2) {
	      const value_type value2 = M(i2,j2);
	      for (value_type v2 = 0; v2 < value2; ++v2)
		for (unsigned int k = 0; k < max_bc_sides; ++k) {
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(i1,j2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,j2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(i2,j1);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i2,j1,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(i1,i2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,i2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(j2,j1);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(j2,j1,v3,R(k)), pos);
		  }
		}
	    }
	    for (index_type j2 = j1+1; j2 < dim; ++j2) {
	      const value_type value2 = M(i2,j2);
	      for (value_type v2 = 0; v2 < value2; ++v2)
		for (unsigned int k = 0; k < max_bc_sides; ++k) {
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(i1,j2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,j2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(i2,j1);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i2,j1,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(i1,i2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,i2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(j1,j2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(j1,j2,v3,R(k)), pos);
		  }
		}
	    }
	  }
	  for (index_type i2 = j1+1; i2 < dim; ++i2) {
	    for (index_type j2 = i2+1; j2 < dim; ++j2) {
	      const value_type value2 = M(i2,j2);
	      for (value_type v2 = 0; v2 < value2; ++v2)
		for (unsigned int k = 0; k < max_bc_sides; ++k) {
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(i1,j2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,j2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,k), neg);
		  {
		    const value_type value3 = M(j1,i2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(j1,i2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(i1,i2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(i1,i2,v3,k), pos);
		  }
		  F.new_clause();
		  F.add(V(i1,j1,v1,k), neg).add(V(i2,j2,v2,R(k)), neg);
		  {
		    const value_type value3 = M(j1,j2);
		    for (unsigned int v3 = 0; v3 < value3; ++v3)
		      F.add(V(j1,j2,v3,R(k)), pos);
		  }
		}
	    }
	  }
	}
      }
    F.finish();
    assert(F.get_number_clauses() == F.clause_number());
  }
}

namespace TransformationsBiclique {

  template <class Matrix, class Formula>
  class Symmetric_conflict_number_to_SAT {
    const Matrix& M;
    const unsigned int max_bc;
    Formula& F;
  public :
    typedef Matrix Mat;
    typedef Formula Form;

    typedef unsigned int Biclique_index;
    // values in [0, max_bc)

    enum Side { left, right };
    struct Biclique_side {
      Biclique_index i;
      Side s;
      Biclique_side(Biclique_index i, Side s) : i(i), s(s) {}
    };

    typedef typename Mat::index_type Node_index;
    typedef typename Mat::value_type Parallel_edges_index;

    struct Conflict {
      Node_index i, j;
      // Invariante : i < j
      Parallel_edges_index k;
      Conflict(Node_index i, Node_index j, Parallel_edges_index k) : i(i), j(j), k(k) {}
      friend inline bool operator <(const Conflict a, const Conflict b) {
	if (a.i < b.i) return true;
	else if (b.i < a.i) return false;
	else if (a.j < b.j) return true;
	else if (b.j < a.j) return false;
	else return a.k < b.k;
      }
    };

    std::string var(const Conflict& c, const Biclique_side b) const {
      // Error in g++, version 3.2.1: "Conflict c" not possible !
      return "R" + StringHandling::toString(c.i) + "C" + StringHandling::toString(c.j) + "N" + StringHandling::toString(c.k) + "B" + StringHandling::toString(b.i) + "S" + (StringHandling::toString(b.i) == left) ? "l" : "r";
      // R : row; C : column; N : number of conflict (with respect to parallel conflicts); B : biclique; S : side.
    }

    typedef typename Formula::size_type Conflict_index;
    const Conflict_index number_of_conflicts;

    Symmetric_conflict_number_to_SAT(const Matrix& M, const Biclique_index max_bc, Formula& F) : M(M), max_bc(max_bc), F(F), number_of_conflicts(set_number_of_conflicts()), full_list(0), number_of_variables(set_number_of_variables()) {
      assert(preconditions(M, Mat::dim, max_bc) == correct);
    }

  private :

    Symmetric_conflict_number_to_SAT& operator =(const Symmetric_conflict_number_to_SAT&);
    Symmetric_conflict_number_to_SAT(const Symmetric_conflict_number_to_SAT&);

    Conflict_index set_number_of_conflicts() {
      Conflict_index sum = 0;
      for (Node_index i = 0; i < Mat::dim; ++i)
	sum = std::accumulate(M[i] + i + 1, M[i] + Mat::dim, sum);
      return sum;
    }

  private :

    typedef std::list<Biclique_side> List_biclique_sides;
    typedef std::map<Conflict, List_biclique_sides*> Map_conflicts_to_biclique_sides;
    // If biclique bc is in the list assigned to conflict c, then possibly
    // conflict c belongs to biclique bc.i, where node s.i is on side bc.s.
    List_biclique_sides* full_list;
    Map_conflicts_to_biclique_sides possible_bicliques;

  public :
    ~Symmetric_conflict_number_to_SAT() {
      for (typename Map_conflicts_to_biclique_sides::iterator i = possible_bicliques.begin(); i != possible_bicliques.end(); ++i)
	if (i -> second != full_list)
	  delete i -> second;
      delete full_list;
    }

    const Conflict_index number_of_variables;

  private :
    Conflict_index set_number_of_variables() {
      // Assigns possible_bicliques as side effect.
      Conflict_index n = 0;
      Biclique_index current_bc = 0;
      for (Node_index i = 0; i < Mat::dim; ++i) {
	for (Node_index j = i+1; j < Mat::dim; ++j) {
	  const Parallel_edges_index k_max = M(i,j);
	  for (Parallel_edges_index k = 0; k < k_max; ++k) {
	    const Conflict c(i, j, k);
	    if (current_bc < max_bc) {
	       List_biclique_sides* current_list = new List_biclique_sides;
	       assert(current_list != 0);
	       for (Biclique_index bc = 0; bc < current_bc - k; ++bc) {
		  current_list -> push_back(Biclique_side(bc, left));
		  current_list -> push_back(Biclique_side(bc, right));
	       }
	       n += 2 * (current_bc - k);
	       current_list -> push_back(Biclique_side(current_bc, left)); ++n;
	       possible_bicliques[c] = current_list;
	       assert(current_list -> size() == 2 * (current_bc - k) + 1);
	       ++current_bc;
	    }
	    else { // no biclique can be excluded
	      if (not full_list) {
		full_list = new List_biclique_sides;
		assert(full_list != 0);
		for (Biclique_index bc = 0; bc < max_bc; ++bc) {
		  full_list -> push_back(Biclique_side(bc, left));
		  full_list -> push_back(Biclique_side(bc, right));
		}
		assert(full_list -> size() == 2 * max_bc);
	      }
	      possible_bicliques[c] = full_list;
	      assert(possible_bicliques.find(c) != possible_bicliques.end() and possible_bicliques.find(c) -> second == full_list);
	      n += 2 * max_bc;
	    }
	  }
	}
      }
      return n;
    }

  };
}


#endif
