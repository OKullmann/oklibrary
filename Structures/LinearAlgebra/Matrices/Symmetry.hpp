// Oliver Kullmann, 17.6.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Matrices/Symmetry.hpp
  \brief Testing matrices for (hidden) symmetry
*/

#ifndef SYMMETRY_jJHhYT56Tv
#define SYMMETRY_jJHhYT56Tv

#include <functional>

#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <OKlib/Combinatorics/traits/range_type.hpp>


namespace OKlib {
  namespace Matrices {

     /*!
      \class Is_equal
      \brief Functor-predicate for checking whether two matrices in the uBLAS-sense are equal
      \todo TEST IT.
      \todo Write a concept.
      \todo Enquiry to the boost mailing list, why the matrix concept has no == and =!.
      (Emphasising that a generic library should exploit its generality, while apparently
      uBLAS has only numerical computations in mind, ignoring the use of matrices in
      combinatorics (where the entries are (potentially big) integers, and algebra
      (where for example matrices over finite fields are considered).
      It seems that there is no easy way for the user to extend the library so that == and !=
      are available.
     */

    template <class Matrix>
    struct Is_equal : std::binary_function<const Matrix&, const Matrix&, bool> {
      bool operator()(const Matrix& lhs, const Matrix& rhs) const {
        typedef typename Matrix::size_type size_type;
        const size_type& dim(m.size1());
        if (dim != m.size2()) return false;
        for (size_type i = 0; i < dim; ++i)
          for (size_type j = 0; j < dim; ++j)
            if (lhs(i, j) != rhs(i, j)) return false;
        return true;
      }
    };

    template <class Matrix>
    inline bool is_equal(const Matrix& lhs, const Matrix& rhs) {
      return Is_equal<Matrix>()(lhs, rhs);
    }

   /*!
      \class Is_symmetric
      \brief Functor-predicate for checking whether a matrix in the uBLAS-sense is symmetric
      \todo TEST IT.
      \todo Write a concept.
      \todo A nice timing test would be to see whether the simple implementation
      return ::OKlib::Matrix::is_equal(m, trans(p));
      would be just by a factor of 2 slower.
      \todo Enquiry to the boost e-mail list why is_symmetric is not provided (see above).
    */

    template <class Matrix>
    struct Is_symmetric : std::unary_function<const Matrix&, bool> {
      bool operator()(const Matrix& m) const {
        typedef typename Matrix::size_type size_type;
        const size_type& dim(m.size1());
        if (dim != m.size2()) return false;
        for (size_type i = 0; i < dim; ++i)
          for (size_type j = i+1; j < dim; ++j)
            if (m(i, j) != m(j, i)) return false;
        return true;
      }
    };

    template <class Matrix>
    inline bool is_symmetric(const Matrix& m) {
      return Is_symmetric<Matrix>()(m);
    }

    /*!
      \class Is_self_dual
      \brief Functor-predicate for checking whether a matrix in the uBLAS-sense is self_dual (is isomorphic to its transposition)

      Reference implementation, following just the definition.
      \todo TEST IT.
      \todo Write a concept.
      \todo Here returning the isomorphism found would be very useful. Perhaps this
      could be done by providing an overload of operator() with three arguments,
      where the last two arguments are references for the permutations; before doing
      so a concept of "permutations" is required.
      Likely a policy is best here, which says what to do with the results, and which also allows to extract further
      information.
      \todo It would be useful to have "matrix expressions" (in uBLAS terminology ?),
      so that exactly the same as below would be done, but for suitable permutations pr and pc
      we would just write the test as
      ::OKlib::Matrices::is_equal(permutation(m, pr, pc), trans(m))
      (as said, this should have the same complexity as the direct approach below).
      \todo Over time a great variety of algorithms and implementations for this basic
      problem should be developed. It arises the general problem, how to name all these
      different methods for doing the same thing? It would be good to develop a general
      scheme.
      \todo If the permutation generator delivers Gray-codes, then an improved implementation should be able to
      exploit this.
      \todo Enquiry to the boost e-mail list, whether somehow uBLAS can handle
      row and column permutations?
    */

    template <class Matrix, class PermutationsGenerator>
    // Matrix models uBLAS matrices, PermutationsGenerator is a model of concept PermutationsGenerator
    // as defined in XXX (see Combinatorics/plans/Permutations.hpp)
    struct Is_self_dual : std::unary_function<const Matrix&, bool> {
      bool operator()(const Matrix& m) const {
        typedef typename Matrix::size_type size_type;
        const size_type& dim(m.size1());
        if (dim != m.size2()) return false;
        if (dim <= 1) return true;
        typedef typename ::OKlib::Combinatorics::traits::range_type<PermutationsGenerator>::type permutations_range;
        typedef typename boost::range_const_iterator<permutations_range>::type iterator;
        typedef typename boost::value_type<permutations_range>::type permutations;
        const permutations_range row_permutations(PermutationsGenerator(dim));
        const permutations_range column_permutations(PermutationsGenerator(dim));
        for (iterator p_r(boost::begin(row_permutations)); p_r != boost::end(row_permutations); ++p_r) {
          const permutations& pr(*p_r);
          for (iterator p_c(boost::begin(column_permutations)); p_c != boost::end(column_permutations); ++p_c) {
            const permutations& pc(*p_c);
            for (size_type i = 0; i < dim; ++i)
              for (size_type j = 0; j < dim; ++j)
                if m(pr(i), pc(j)) != m(j, i)
                  goto ColumnLoop;
            return true;
          }
        ColumnLoop : ;
        }
      }
    };

    template <class Matrix>
    inline bool is_self_dual(const Matrix& m) {
      return Is_self_dual<Matrix>()(m);
    }

    /*!
      \class Is_self_polar
      \brief Functor-predicate for checking whether a matrix in the uBLAS-sense is self_polar (is isomorphic to a symmetric matrix)

      Reference implementation, following just the definition.
      \todo TEST IT.
      \todo Write a concept.
      \todo Here returning the isomorphism found would be very useful. See discussion above for Is_self_dual.
      \todo Having a matrix expression for permuting row and column indices simultaneously
      would be useful; see the discussion above for Is_self_dual.
    */

    template <class Matrix, class PermutationsGenerator>
    // Matrix models uBLAS matrices, PermutationsGenerator is a model of concept PermutationsGenerator
    // as defined in XXX (see Combinatorics/plans/Permutations.hpp)
    struct Is_self_polar : std::unary_function<const Matrix&, bool> {
      bool operator()(const Matrix& m) const {
        typedef typename Matrix::size_type size_type;
        const size_type& dim(m.size1());
        if (dim != m.size2()) return false;
        if (dim <= 1) return true;
        typedef typename ::OKlib::Combinatorics::traits::range_type<PermutationsGenerator>::type permutations_range;
        typedef typename boost::range_const_iterator<permutations_range>::type iterator;
        typedef typename boost::value_type<permutations_range>::type permutations;
        const permutations_range row_permutations(PermutationsGenerator(dim));
        for (iterator p_r(boost::begin(row_permutations)); p_r != boost::end(row_permutations); ++p_r) {
          const permutations& pr(*p_r);
            for (size_type i = 0; i < dim; ++i)
              for (size_type j = i+1; j < dim; ++j)
                if m(pr(i), j) != m(pr(j), i)
                  goto ColumnLoop;
            return true;
          }
        ColumnLoop : ;
        }
      }
    };

    template <class Matrix>
    inline bool is_self_polar(const Matrix& m) {
      return Is_self_polar<Matrix>()(m);
    }


  }

}

#ifndef
