// Matthew Gwynne, 29.7.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SubsumptionHypergraph.hpp
  \brief Module, which implements a subsumption hypergraph generator.

  Given two set systems F and G, the subsumption hypergraph of F with
  respect to G is the hypergraph with F as the vertex set and a hyperedge H
  for every set S in G, where H is the largest subset of F where every T in H 
  subsumes (i.e. is a subset of) S.

  In reality, one typically doesn't need the original sets from G and simply
  wants a standardised version, and also wants an ordered version rather than
  using sets.

  Therefore assuming an order on F and G, one considers the standardised 
  ordered subsumption hypergraph of F and G with vertex list [1,..,|F|] and
  hyperedge list [H_1,...,H_|G|] where for all 1 <= j <= |G|, we have H[j]
  is the sublist of [1,...,|F|] and i is in H[j] if F[i] subsumes
  G[j].

  The class implemented here implements the generation of such an ordered
  subsumption hypergraph given (ordered) set systems F and G.
  

  \todo Improve code quality
  <ul>
   <li> Specification of Subsumption_hypergraph needs more explanations, and
   it needs the concept! </li>
   <li> All member functions need a specification. </li>
   <li> Every class and function needs documentation! </li>
   <li> DONE In cases of larger arguments the argument-types of functions 
   should be const-references. </li>
   <li> DONE Everything not used by the user must be private. </li>
   <li> DONE Adopt the new for-loop-style. </li>
   <li> DONE Proper constructor: The two data members should be const, and so 
   their construction must take place in the initialiser list. </li>
   <li> DONE "Brief" should speak in intuitive words about the functionality
   provided. </li>
   <li> DONE Then the paragraph must specify it more precisely, *in words*. </li>
   <li> DONE So the reference to the Maxima-level is misplaced there. </li>
   <li> DONE And it needs more precision. </li>
  </ul>

  \todo Organisation
  <ul>
   <li> This submodule must go to Combinatorics/Hypergraphs, since it does not
   provide general set-operations. </li>
  </ul>


  \bug DONE Test failure
  <ul>
   <li> Bug was introduced by OK during cleanup of code, as 
   "count++" -> "++count" was not equivalent in the place it
   was changed. </li>
   <li> MG's use of "count++" *was* intentional but misguided. </li>
  </ul>

*/

#ifndef SUBSUMPTION_HYPERGRAPH_yhgBBv567
#define SUBSUMPTION_HYPERGRAPH_yhgBBv567

#include <algorithm>
#include <map>
#include <list>

#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>


namespace OKlib {
  namespace SetAlgorithms {

    // ############################################

    /*!
      \class Subsumption_hypergraph
      \brief Constructing the subsumption hypergraph

      Given two set systems F and G, as ranges of ranges (of type RangeF
      and RangeG respectively) of type Int, constructs the 
      standardised ordered subsumption hypergraph.

      The subsumption hypergraph is available as the object constructed
      by this class, with the vertex set (vertex_set) as the list 
      [1,...,length(F)] and the hyperedge set as the list [H_1,...,H_|G|] 
      where for all 1 <= j <= |G|, we have H[j] is the sublist of [1,...,|F|],
      i is in H[j] if F[i] subsumes G[j] and H[j] is a list of Int.

      The Maxima-specification is subsumption_std_ohg(F,G) (see
      ComputerAlgebra/Hypergraphs/Lisp/Basics.mac).
    */

    template <class RangeF,
	      class RangeG,
              typename Int = typename boost::range_difference<RangeF>::type>
    class Subsumption_hypergraph {

    public :

      typedef typename std::list<Int> hyperedge_type;
      typedef Int vertex_type;
      typedef std::list<hyperedge_type> set_system_type;
      
      const hyperedge_type vertex_set;
      const set_system_type hyperedges;

      Subsumption_hypergraph(const RangeF& f_range, 
                             const RangeG& g_range):
        vertex_set(fill_vertex_set(boost::size(f_range))), 
        hyperedges(subsumption_hypergraph(f_range, g_range)) {}

    private:

      typedef typename boost::range_const_iterator<RangeF>::type f_iterator_type;
      typedef typename boost::range_const_iterator<RangeG>::type g_iterator_type;
      typedef typename boost::range_value<RangeF>::type f_value_type;
      typedef std::map<f_value_type, Int> hyperedge_map_type;
      typedef std::list<f_value_type> hyperedge_nonstd_type;
      typedef typename boost::range_size<RangeF>::type f_size_type;

      static const hyperedge_type fill_vertex_set(const f_size_type size_f) {
        hyperedge_type vertex_set;
        const boost::counting_iterator<Int> v_begin(0);
        const boost::counting_iterator<Int> v_end(size_f);
        std::copy(v_begin, v_end, boost::begin(vertex_set));
        return(vertex_set);
      }

      static hyperedge_map_type fill_hyperedge_map(const RangeF& f_range) {
        hyperedge_map_type hyperedge_map;
        for(
            struct {
              f_iterator_type it;
              Int count;
              const f_iterator_type f_end;
            } l = {boost::begin(f_range), 0, boost::end(f_range)};
            l.it != l.f_end;
            ++l.it)
          hyperedge_map[*l.it] = ++l.count;
        return(hyperedge_map);
      }

      static
      hyperedge_type standardise_hyperedge(const hyperedge_nonstd_type& edge, 
                                           const hyperedge_map_type& hmap) {
        typedef typename boost::range_const_iterator<hyperedge_nonstd_type>::type
          hyperedge_nonstd_const_iterator_type;

        hyperedge_type new_edge;
        for(
            struct {
              hyperedge_nonstd_const_iterator_type it;
              const hyperedge_nonstd_const_iterator_type end;
            } l = {boost::begin(edge),boost::end(edge)};
            l.it != l.end; 
            ++l.it)
          new_edge.push_back(hmap.find(*l.it)->second);
        return(new_edge);
      }

      template <class RangeC>
      static
      hyperedge_nonstd_type all_subsuming(const RangeC c_range, 
                                          const RangeF& f_range) {
        hyperedge_nonstd_type subsumes_set;
        for (
             struct {
               f_iterator_type it;
               const f_iterator_type end;
             } f = { boost::begin(f_range), boost::end(f_range) };
             f.it != f.end; ++f.it) 
          if (std::includes(boost::begin(c_range), boost::end(c_range), 
                            boost::begin(*f.it),boost::end(*f.it)))
            subsumes_set.push_back(*f.it);
        return(subsumes_set);
      }

      static const set_system_type subsumption_hypergraph(const RangeF& f_range,
                                                          const RangeG& g_range) {
        set_system_type hyperedges;
        for (
             struct {
               g_iterator_type it;
               const g_iterator_type end;
               const hyperedge_map_type map;
             } g = {boost::begin(g_range), boost::end(g_range), 
                    fill_hyperedge_map(f_range)};
             g.it != g.end; ++g.it)
          hyperedges.push_back(standardise_hyperedge(all_subsuming(*g.it, f_range), g.map));
        return hyperedges;
      }
    };


    template<class RangeF, class RangeG>
    typename std::list<std::list<typename boost::range_difference<RangeF>::type> >  
    subsumption_hypergraph(const RangeF& f_range, const RangeG& g_range) {
      Subsumption_hypergraph<RangeF, RangeG> sub_hyp(f_range,g_range);
      return sub_hyp.hyperedges;
    }

  }

}


#endif
