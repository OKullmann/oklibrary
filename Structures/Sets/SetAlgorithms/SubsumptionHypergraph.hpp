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

*/

#ifndef SUBSUMPTION_HYPERGRAPH_yhgBBv567
#define SUBSUMPTION_HYPERGRAPH_yhgBBv567

#include <algorithm>
#include <iterator>
#include <cassert>
#include <set>
#include <list>

#include<boost/range.hpp>
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
      \brief Functor: Generates the subsumption hypergraph of set system F w.r.t. G.
    */

    template <class RangeF,
	      class RangeG,
              class OutputContainerSets = std::list<std::list<typename boost::range_iterator<RangeF>::type::value_type > > >
    struct Subsumption_hypergraph {

      typedef typename boost::range_iterator<RangeF>::type f_iterator;
      typedef typename boost::range_iterator<RangeG>::type g_iterator;
      typedef typename OutputContainerSets::value_type InnerOutputContainerSets;


      template <class range_c>
      InnerOutputContainerSets all_subsuming(const range_c c_range, RangeF f_range) {
        InnerOutputContainerSets subsumes_set;
        f_iterator f_begin = boost::begin(f_range);
        for (; f_begin != boost::end(f_range); ++f_begin) 
          if (std::includes(boost::begin(c_range), boost::end(c_range), boost::begin(*f_begin),boost::end(*f_begin)))
            subsumes_set.push_back(*f_begin);
        return(subsumes_set);
      }

      OutputContainerSets subsumption_hypergraph(const RangeF f_range, const RangeG g_range) {
        OutputContainerSets subsumption_hyperedges;
        g_iterator g_begin = boost::begin(g_range);
        for (; g_begin != boost::end(g_range); ++g_begin) {
          subsumption_hyperedges.push_back(all_subsuming(*g_begin, f_range));
        }
        return(subsumption_hyperedges);
      }

      OutputContainerSets operator() (const RangeF f_range, const RangeG g_range) {
        return subsumption_hypergraph(f_range,g_range);
      }

    };

    template<class RangeF, class RangeG>
    typename std::list<std::list<typename boost::range_iterator<RangeF>::type::value_type> > 
    subsumption_hypergraph(const RangeF f_range, const RangeG g_range) {
      Subsumption_hypergraph<RangeF, RangeG> sub_hyp;
      return sub_hyp(f_range,g_range);
    }
    
      
  }

}


#endif
