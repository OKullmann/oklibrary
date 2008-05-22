// Oliver Kullmann, 26.4.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/Matrices.hpp
  \brief Simple class template for square matrices, as a wrapper around
  arrays.

  Given a matrix M, via M(i,j) we obtain the element at position (i,j),
  while by M[i] we obtain a pointer to row i.

  \deprecated Old code
*/

#ifndef MATRICESWAECHTER_pokGr45E
#define MATRICESWAECHTER_pokGr45E

#include <algorithm>

namespace Matrices {

  enum storage_class { mutable_elements, non_mutable_elements };

  /*!
    \class QuadMatrix
    \brief Simple square-matrices class, wrapping arrays

    Providing
    <ul>
     <li> dimension </li>
     <li> equality test </li>
     <li> type-safe assignment </li>
     <li> element-access via operator () </li>
     <li> row-access via operator [] </li>
     <li> two version for non-mutable (default) and mutable elements </li>
    </ul>
  */

  template <typename i_type,
            typename v_type,
            i_type dimension,
            storage_class st = non_mutable_elements>
  class QuadMatrix {
  public :
    typedef const v_type array_type[dimension][dimension];
    typedef  v_type array_type_mutable[dimension][dimension];
    typedef i_type index_type;
    typedef v_type value_type;
    
    static const index_type dim = dimension;

    QuadMatrix(const array_type& a_) : a(a_) {}
    template <storage_class stc>
    QuadMatrix(const QuadMatrix<i_type, v_type, dimension, stc>& M) : a(M.a) {}
    
    value_type operator() (index_type i, index_type j) const { return a[i][j]; }
    const v_type* operator[] (index_type i) const { return a[i]; }
    
  private :
    const array_type& a;
    QuadMatrix& operator =(const QuadMatrix&);
  };

  template <typename i_type, typename v_type, i_type dimension>
  class QuadMatrix<i_type, v_type, dimension, mutable_elements> {
  public :
    typedef v_type array_type[dimension][dimension];
    typedef i_type index_type;
    typedef v_type value_type;
    
    static const index_type dim = dimension;

    QuadMatrix(array_type& a_) : a(a_) {}
    
    value_type&  operator() (index_type i, index_type j) { return a[i][j]; }
    value_type operator() (index_type i, index_type j) const { return a[i][j]; }
    v_type* operator[] (index_type i) { return a[i]; }
    const v_type* operator[] (index_type i) const { return a[i]; }
    
  private :
    array_type& a;
    QuadMatrix& operator =(const QuadMatrix&);
    friend class QuadMatrix<i_type, v_type, dimension, non_mutable_elements>;
  };

  template <typename i_type, typename v_type, i_type dimension, storage_class st>
  const typename QuadMatrix<i_type, v_type, dimension, st>::index_type QuadMatrix<i_type, v_type, dimension, st>::dim;

  template <typename i_type_1, typename v_type_1, i_type_1 dimension_1, storage_class st1, typename i_type_2, typename v_type_2, i_type_2 dimension_2, storage_class st2>
  inline bool operator ==(const QuadMatrix<i_type_1, v_type_1, dimension_1, st1>& A1, const QuadMatrix<i_type_2, v_type_2, dimension_2, st2>& A2) {
    return (dimension_1 == dimension_2) and std::equal(A1[0], A1[dimension_1], A2[0]);
  }

  template <typename i_type_1, typename v_type_1, i_type_1 dimension_1, storage_class st1, typename i_type_2, typename v_type_2, i_type_2 dimension_2, storage_class st2>
  inline bool operator !=(const QuadMatrix<i_type_1, v_type_1, dimension_1, st1>& A1, const QuadMatrix<i_type_2, v_type_2, dimension_2, st2>& A2) {
    return not (A1 == A2);
  }

}

#endif
