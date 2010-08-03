// Matthew Gwynne, 29.7.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SubsumptionHypergraph.hpp
  \brief Module, which mainly provides the class template Subsumption_hypergraph. 

*/

#ifndef SUBSUMPTION_HYPERGRAPH_yhgBBv567
#define SUBSUMPTION_HYPERGRAPH_yhgBBv567

#include <algorithm>
#include <iterator>
#include <cassert>
#include <set>

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

    template <class ContainerSetsF,
	      class ContainerSetsG,
              class OutputContainerSets,
              class UniquenessTag = SubsumptionsTags::hyperedges_may_not_be_unique,
              class OrderTag = SubsumptionsTags::hyperedges_may_not_be_sorted_by_size,
              class SizeTag = typename boost::mpl::if_<
		boost::mpl::and_<
		  typename OKlib::traits::has_size_function<typename ContainerSetsF::value_type>::type,
		  typename OKlib::traits::has_size_function<typename ContainerSetsG::value_type>::type 
		  >, 
		SubsumptionsTags::use_size_of_hyperedges, 
		SubsumptionsTags::do_not_use_size_of_hyperedges>::type
    >
    struct Subsumption_hypergraph {

      typedef UniquenessTag uniqueness_tag;
      typedef OrderTag order_tag;
      typedef typename ContainerSetsF::iterator f_iterator;
      typedef typename ContainerSetsF::value_type::iterator c_iterator;
      typedef typename ContainerSetsG::iterator g_iterator;
      typedef typename OutputContainerSets::value_type InnerOutputContainerSets;


      InnerOutputContainerSets all_subsuming(const c_iterator c_begin, const c_iterator c_end, f_iterator f_begin, const f_iterator f_end) {
	InnerOutputContainerSets subsumes_set;
	for (; f_begin != f_end; ++f_begin) 
	  if (std::includes(c_begin, c_end, f_begin -> begin(), f_begin -> end()))
	    subsumes_set.insert(*f_begin);
	return(subsumes_set);
      }

      OutputContainerSets subsumption_hypergraph(const f_iterator f_begin, const f_iterator f_end, g_iterator g_begin, const g_iterator g_end) {
	OutputContainerSets subsumption_hyperedges;
	for (; g_begin != g_end; ++g_begin) 
	  subsumption_hyperedges.insert(all_subsuming(g_begin -> begin(), g_begin -> end(), f_begin, f_end));
	return(subsumption_hyperedges);
      }

      OutputContainerSets operator() (const f_iterator f_begin, const f_iterator f_end, g_iterator g_begin, const g_iterator g_end) {
	return subsumption_hypergraph(f_begin,f_end,g_begin,g_end);
      }

    };
      
  }

}


#endif
