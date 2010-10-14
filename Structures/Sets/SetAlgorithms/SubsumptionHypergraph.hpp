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
#include <iterator>
#include <cassert>
#include <map>
#include <list>

#include <boost/range.hpp>
#include <boost/utility.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>

#include <OKlib/traits/TypeTraitsContainer.hpp>
#include <OKlib/Programming/MetaProgramming/TaggingPolymorphism.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/Subsumption.hpp>


namespace OKlib {
  namespace SetAlgorithms {


    // ############################################

    /*!
      \class Subsumption_hypergraph
      \brief Generating the subsumption hypergraph

      Standardised form (using standardise_ohg of
      subsumption_ohg in ComputerAlgebra/Hypergraphs/Lisp/Basics.mac).
    */

    template <class RangeF,
	      class RangeG,
              typename Int = typename boost::range_difference<RangeF>::type>
    struct Subsumption_hypergraph {

      typedef typename boost::range_iterator<RangeF>::type f_iterator_type;
      typedef typename boost::range_iterator<RangeG>::type g_iterator_type;
      typedef typename boost::range_value<RangeF>::type f_value_type;
      typedef typename std::list<Int> hyperedge_type;
      typedef Int vertex_type;
      typedef std::list<hyperedge_type> set_system_type;
      
      RangeF vertex_set;
      set_system_type hyperedges;
      std::map<f_value_type,Int> hyperedge_map;

      template <class range_c>
      hyperedge_type all_subsuming(const range_c c_range, RangeF f_range) {
        hyperedge_type subsumes_set;
        f_iterator_type f_begin = boost::begin(f_range);
        for (; f_begin != boost::end(f_range); ++f_begin) 
          if (std::includes(boost::begin(c_range), boost::end(c_range), boost::begin(*f_begin),boost::end(*f_begin)))
            subsumes_set.push_back(hyperedge_map[*f_begin]);
        return(subsumes_set);
      }

      void fill_hyperedge_map(RangeF f_range) {
        f_iterator_type f_begin = boost::begin(f_range);
        for(Int count = 0; f_begin != boost::end(f_range); ++f_begin)
          hyperedge_map[*f_begin] = ++count;
      }

      void subsumption_hypergraph(const RangeF f_range, RangeG g_range) {
        hyperedges.clear(); // We might be dealing with a different set system.
        hyperedge_map.clear();
        fill_hyperedge_map(f_range);

        g_iterator_type g_begin = boost::begin(g_range);
        for (; g_begin != boost::end(g_range); ++g_begin)
          hyperedges.push_back(all_subsuming(*g_begin, f_range));
      }

      set_system_type operator() (const RangeF f_range, const RangeG g_range) {
        vertex_set = f_range;
        subsumption_hypergraph(f_range, g_range);
        return hyperedges;
      }

    };


    template<class RangeF, class RangeG>
    typename std::list<std::list<typename boost::range_difference<RangeF>::type> >  
    subsumption_hypergraph(const RangeF f_range, const RangeG g_range) {
      Subsumption_hypergraph<RangeF, RangeG> sub_hyp;
      sub_hyp(f_range,g_range);
      return sub_hyp.hyperedges;
    }

  }

}


#endif
