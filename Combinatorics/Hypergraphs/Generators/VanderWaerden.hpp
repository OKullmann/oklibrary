// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp
  \brief %Tools for generating van der Waerden hypergraphs
*/

#ifndef VANDERWAERDENHYPERGRAPH_77bQMQM_5993KKl
#define VANDERWAERDENHYPERGRAPH_77bQMQM_5993KKl

#include <iterator>
#include <cassert>
#include <cstdlib>
#include <vector>

#include <OKlib/Concepts/Iterators.hpp>

namespace OKlib {
 namespace Combinatorics {
  namespace Hypergraphs {
   namespace Generators {
  
     /*!
       \class Iterator_arithmetic_progression
       \brief Iterator to iterate through the elements of an arithmetical progression.
       \todo Make a const random iterator out of it.
     */
  
     template <typename Integer>
     class Iterator_arithmetic_progression : public std::iterator<std::input_iterator_tag, Integer> {
       Integer current_value;
      Integer slope;
    public :
      typedef ::OKlib::Concepts::MultiPassInputIterator_tag concept_tag;
      Iterator_arithmetic_progression(const Integer start_value, const Integer slope) : current_value(start_value), slope(slope) {}
      Integer operator* () const { return current_value; }
      Iterator_arithmetic_progression& operator ++() {
        current_value += slope;
        return *this;
      }
      Iterator_arithmetic_progression operator ++(int) {
        Iterator_arithmetic_progression i(*this);
        operator ++();
        return i;
      }
    };
    
    template <typename Integer>
    inline bool operator ==(const Iterator_arithmetic_progression<Integer>& lhs, const Iterator_arithmetic_progression<Integer>& rhs) {
      return *lhs == *rhs;
    }
    template <typename Integer>
    inline bool operator !=(const Iterator_arithmetic_progression<Integer>& lhs, const Iterator_arithmetic_progression<Integer>& rhs) {
      return not (lhs == rhs);
    }
    
    // -----------------------------------------------------------------------------------------------------------------------------
    
    /*!
      \class Arithmetic_progression
      \brief An arithmetic progression as a container
  
      \todo Add the concept tag (a special case of a "const collection with begin, end and size; not default-constructible, but copyable, assignable, equality comparable").
  
      \todo See class IteratorHandling::Arithmetical_progression in
     General/IteratorHandling.hpp
  
    */
    
    template <typename Integer>
    class Arithmetic_progression {
      Integer start_, length, slope_;
      Integer finish;
    public :
      Arithmetic_progression(const Integer start, const Integer length, const Integer slope) : start_(start), length(length), slope_(slope), finish(start_ + length * slope_) {
        assert(length >= 0);
      }
      typedef Integer value_type;
      typedef Integer size_type;
      size_type size() const { return length; }
      typedef Iterator_arithmetic_progression<Integer> iterator;
      typedef iterator const_iterator;
      iterator begin() const { return iterator(start_, slope_); }
      iterator end() const { return iterator(finish, 0); }
        friend bool operator ==(const Arithmetic_progression& lhs, const Arithmetic_progression& rhs) {
        return lhs.start == rhs.start and lhs.length == rhs.length and lhs.slope == rhs.slope;
      }
      value_type start() const { return start_; }
      value_type slope() const { return slope_; }
    };
    
    // -----------------------------------------------------------------------------------------------------------------------------
  
    /*!
      \class Arithprog_finish
      \brief Functor which produces the list of arithmetic progressions of
      given length k in {1, ..., n} finishing in n.
  
      Specified by function arithprog_finish(k,n) in
      ComputerAlgebra/Hypergraphs/Lisp/Generators/VanderWaerden.mac.
    */
  
    template <class Hyperedges, class SetSystem, typename Int = unsigned int>
    struct Arithprog_finish {
  
      typedef Int int_type;
      typedef Arithmetic_progression<int_type> progression_type;
      typedef Hyperedges hyperedge_type;
      typedef SetSystem set_system_type;
      typedef typename hyperedge_type::value_type vertex_type;
  
      int_type k; // length of arithmetic progression
  
      Arithprog_finish() : k(0) {}
      Arithprog_finish(const int_type k) : k(k) {
        assert(k >= 1);
      }
      void set(const int_type k_new) {
        assert(k_new >= 1);
        k = k_new;
      }
  
      set_system_type operator()(const int_type n) const {
        assert(k >= 1);
        assert(n >= 1);
        typedef std::vector<vertex_type> vector_t;
        vector_t H;
        H.reserve(k);
        typedef std::vector<hyperedge_type> vector2_t;
        vector2_t S;
        if (k == 1) {
          H.push_back(n);
          S.push_back(hyperedge_type(H.begin(), H.end()));
          return set_system_type(S.begin(),S.end());
        }
        typedef long int lib_int_type;
        const int_type q = std::ldiv(lib_int_type(n-1), lib_int_type(k-1)).quot;
        S.reserve(q);
        for (int_type d = 1; d <= q; ++d) {
          const progression_type P(n - (k-1) * d, k, d);
          H.assign(P.begin(), P.end());
          assert(P.size() == k);
          S.push_back(hyperedge_type(H.begin(), H.end()));
        }
        assert(S.size() == q);
        return set_system_type(S.begin(),S.end());
      }
    };
  
    // -----------------------------------------------------------------------------------------------------------------------------

   /*!
     \brief Computing the number of arithmetic progressions of length k in
     {1,...,n}

     The Maxima-specification is nhyp_arithprog_ohg(k,n) in
     ComputerAlgebra/Hypergraphs/Lisp/Generators/VanderWaerden.mac.
   */
   template <typename UInt>
   inline UInt nhyp_arithprog_hg(const UInt k, const UInt n) {
     if (k == 0) return 1;
     if (k == 1) return n;
     if (n < k) return 0;
     const UInt q = (n-1) / (k-1);
     return q*n - (q*(k - 1)*(q + 1)) / 2;
   }

    /*!
      \class Arithmetical_progressions
      \brief All arithmetical progressions in {1,...,n}, in lexicographical order (in iterator-fashion, without storage)
      \deprecated Old, "quick and dirty" implementation, to be improved (via using a standard output interface).

      \detail

      Usage:
      <ul>
       <li> Construct an object
       <code>Arithmetical_progressions ap(k,n)</code>, where k is the length
       of the arithmetic progressions, to be considered in {1,...,n}. </li>
       <li> In ap.count the total number of ap's is to be found. </li>
       <li> Via calling ap.next() one then obtains the ap's, as vectors, in
       lexicographical order. </li>
       <li> It is an error if ap.next() is called more than ap.count many
       times. </li>
      </ul>

      \todo Integration
      <ul>
       <li> See IteratorHandling::Arithmetical_progression in
       General/IteratorHandling.hpp. </li>
      </ul>
    */

    template <typename Int>
    class Arithmetical_progressions {
    public :
      typedef Int Index;
      const Index n;
      //! size of arithmetic progression in {1, ..., n}
      const Index k;
      //! number of arithmetic progressions of length k in {1, ..., n}
      const Index count;

    private :
      //! first element in current arithmetic progression
      Index current_element;
      //! slope of current arithmetic progression
      Index current_distance;

    public :

      Arithmetical_progressions(const Index k, const Index n) :
          n(n), k(k),
          count(nhyp_arithprog_hg(k,n)),
          current_element(1),
          current_distance(1) {
        assert(k >= 1);
        assert(n >= 2);
        assert(n >= k);
      }
      std::string static message() {
        return "Iterating through the arithmetic progressions in lexicographical order.";
      }

      typedef std::vector<Index> Arithmetical_progression;

      Arithmetical_progression next() {
        Arithmetical_progression ap;
        ap.reserve(k);
        for (Index i = 0; i < k; ++i)
	  ap.push_back(current_element + i * current_distance);
        if (current_element + (k-1) * (current_distance + 1) <= n and k >= 2)
	  ++current_distance;
        else {
	  ++current_element; current_distance = 1;
        }
        return ap;
      }
    };


    /*!
      \class Arithmetical_progressions_colex
      \brief All arithmetical progressions in {1,...,n}, in colexicographical order (in iterator-fashion, without storage)
      \deprecated To be improved (via using a standard output interface).

      \detail

      Usage:
      <ul>
       <li> Construct an object
       <code>Arithmetical_progressions_colex ap(k,n)</code>, where k is the
       length of the arithmetic progressions, to be considered in {1,...,n}.
       </li>
       <li> In ap.count the total number of ap's is to be found. </li>
       <li> Via calling ap.next() one then obtains the ap's, as vectors, in
       colexicographical order. </li>
       <li> ap.next() can be called arbitrarily often, and runs through all
       arithmetic progressions of length k in {1,...}. </li>
      </ul>

      \todo Integration
      <ul>
       <li> See classes in
       Combinatorics/Hypergraphs/Generators/plans/VanderWaerden.hpp and
       General/IteratorHandling.hpp. </li>
      </ul>
    */

    template <typename Int>
    class Arithmetical_progressions_colex {
    public :
      typedef Int Index;
      const Index n;
      //! size of arithmetic progression in {1, ..., n}
      const Index k;
      //! number of arithmetic progressions of length k in {1, ..., n}
      const Index count;

    private :
      //! last element in current arithmetic progression
      Index current_element;
      //! slope of current arithmetic progression
      Index current_distance;

    public :

      Arithmetical_progressions_colex(const Index k, const Index n) :
          n(n), k(k),
          count(nhyp_arithprog_hg(k,n)),
          current_element(k),
          current_distance(1) {
        assert(k >= 2);
        assert(n >= 2);
        assert(n >= k);
      }
      std::string static message() {
        return "Iterating through the arithmetic progressions in colexicographical order.";
      }

      typedef std::vector<Index> Arithmetical_progression;

      Arithmetical_progression next() {
        Arithmetical_progression ap;
        ap.reserve(k);
        const Index first_element = current_element - (k-1) * current_distance;
        for (Index i = 0; i < k; ++i)
	  ap.push_back(first_element + i * current_distance);
        if (current_distance > 1) --current_distance;
        else
          current_distance = current_element++ / (k-1);
        return ap;
      }
    };

   }
  }
 }
}

#endif
