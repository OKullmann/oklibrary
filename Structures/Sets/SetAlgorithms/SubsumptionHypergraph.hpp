// Matthew Gwynne, 29.7.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SubsumptionHypergraph.hpp
  \brief Module, which mainly provides the class template Subsumption_hypergraph.

  The Maxima-specification is subsumption_ghg(F,G) (see
  ComputerAlgebra/Hypergraphs/Lisp/Basics.mac).


  \todo Improve code quality
  <ul>
   <li> "Brief" should speak in intuitive words about the functionality
   provided. </li>
   <li> Then the paragraph must specify it more precisely, *in words*. </li>
   <li> So the reference to the Maxima-level is misplaced there. </li>
   <li> And it needs more precision. </li>
   <li> Specification of Subsumption_hypergraph needs more explanations, and
   it needs the concept! </li>
   <li> All member functions need a specification. </li>
   <li> Everything not used by the user must be private. </li>
   <li> Every class and function needs documentation! </li>
   <li> Adopt the new for-loop-style. </li>
   <li> In cases of larger arguments the argument-types of functions should be
   const-references. </li>
   <li> Proper constructor: The two data members should be const, and so their
   construction must take place in the initialiser list. </li>
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

      Standardised form (using standardise_ohg of
      subsumption_ohg in ComputerAlgebra/Hypergraphs/Lisp/Basics.mac).
    */

    template <class RangeF,
	      class RangeG,
              typename Int = typename boost::range_difference<RangeF>::type>
    class Subsumption_hypergraph {

    public :

      typedef typename std::list<Int> hyperedge_type;
      typedef Int vertex_type;
      typedef std::list<hyperedge_type> set_system_type;
      
      hyperedge_type vertex_set;
      set_system_type hyperedges;

      Subsumption_hypergraph(const RangeF f_range, const RangeG g_range) {
        const boost::counting_iterator<Int> v_begin(0);
        const boost::counting_iterator<Int> v_end(boost::size(f_range));
        std::copy(v_begin, v_end, boost::begin(vertex_set));
        subsumption_hypergraph(f_range, g_range);
      }

    private:

      typedef typename boost::range_const_iterator<RangeF>::type f_iterator_type;
      typedef typename boost::range_const_iterator<RangeG>::type g_iterator_type;
      typedef typename boost::range_value<RangeF>::type f_value_type;
      typedef std::map<f_value_type, Int> hyperedge_map_type;
      typedef std::list<f_value_type> hyperedge_nonstd_type;

      static hyperedge_map_type fill_hyperedge_map(const RangeF f_range) {
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
        const hyperedge_nonstd_const_iterator_type end = boost::end(edge);
        for(hyperedge_nonstd_const_iterator_type iter = boost::begin(edge);
            iter != end; ++iter)
          new_edge.push_back(hmap.find(*iter)->second);
        return(new_edge);
      }

      template <class RangeC>
      static
      hyperedge_nonstd_type all_subsuming(const RangeC c_range, 
                                          const RangeF f_range) {
        hyperedge_nonstd_type subsumes_set;
        for (f_iterator_type f_begin = boost::begin(f_range); 
             f_begin != boost::end(f_range); ++f_begin) 
          if (std::includes(boost::begin(c_range), boost::end(c_range), 
                            boost::begin(*f_begin),boost::end(*f_begin)))
            subsumes_set.push_back(*f_begin);
        return(subsumes_set);
      }

      void subsumption_hypergraph(const RangeF f_range, 
                                  const RangeG g_range) {
        hyperedge_map_type hmap = fill_hyperedge_map(f_range);
        const g_iterator_type g_end = boost::end(g_range);
        for (g_iterator_type g_begin = boost::begin(g_range); 
             g_begin != g_end; ++g_begin)
          hyperedges.push_back(standardise_hyperedge(all_subsuming(*g_begin, f_range), hmap));
      }
    };


    template<class RangeF, class RangeG>
    typename std::list<std::list<typename boost::range_difference<RangeF>::type> >  
    subsumption_hypergraph(const RangeF f_range, const RangeG g_range) {
      Subsumption_hypergraph<RangeF, RangeG> sub_hyp(f_range,g_range);
      return sub_hyp.hyperedges;
    }

  }

}


#endif
