// Matthew Gwynne, 15.9.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/plans/SubsumptionHypergraph.hpp
  \brief Plans for subsumption hypergraph generator
  
  
  \todo Write tests for the generator application

  
  \todo Test performance of Subsumption_Hypergraph generator with const fields
  <ul>
   <li> Currently fields (vertex_set and hyperedges) in the 
   Subsumption_hypergraph generator are public but const . </li>
   <li> To achieve this, C++ initialiser lists are used and functions
   generate and return the vertex_set and hyperedges and are used
   in the initialiser list. </li>
   <li> The problem lies in the fact that the C++ standard doesn't guarantee
   that such functions will be optimised away (Return Value Optimisation
   - RVO), only that such an optimisation is allowed. </li>
   <li> Due to this, it is possible that one creates these very large 
   structures (the hyperedges of some subsumption hypergraphs we consider are
   multiple gigabyte) twice, once in the function, and then again as
   the field object. </li>
   <li> We should experiment with gcc, at least, and ensure that the standard
   environment (with optimisations) performs RVO. </li>
  </ul>

  
  \todo Offering live subsumption hypergraph generation
  <ul>
   <li> The subsumption hypergraph for some inputs will be very large,
   however, given the set of subsuming sets (i.e. the vertices of
   the subsumption hypergraph), each hyperedge of the subsumption
   hypergraph can be computed separately from the others, and there is
   no need for data to be stored. </li>
   <li> Therefore, there is the possibility for both parallel and 
   output-flushing versions (i.e., writing partial results to disk as
   the algorithm computes each hyperedge). </li>
   <li> Such things should be very useful and fairly easy to implement. </li>
   <li> With regard to flushing output as it is computed, the easiest
   method seems to be simply offering an additional overloaded function
   which takes an output clause-set adaptor. This would then be wrapped in
   an adaptor so that the rest of the functionality works in the same way, 
   simply adding hyperedges to a hyperedge set, but the hyperedge set might 
   be this clause-set adaptor which then immediately flushes the hyperedges 
   and forgets it. </li>
   <li> With regard to parallel processing, it should be trivial to add this
   using boosts threading libraries, as it is a simply case of splitting
   the subsumed hyperedge set in 'n' parts, starting a separate thread for
   each and then waiting (joining) for the threads to finish, and
   appending their results. The number of threads to use should be added as a 
   default parameter to the subsumption hypergraph generator, as knowledge
   that the problem is splittable in this way is something that belongs
   to the algorithm, not the user. </li>
   <li> To combine these two techniques, there is the possibility of having
   an additional thread utilised by a class handling the hyperedge set, such
   that any call to the hyperedge set results in the hyperedge being added
   to a queue, which a separate thread then tries to write to disk as fast as 
   possible. Having the hyperedges added in parallel like this would not 
   guarantee output ordering of the hyperedges however, although they could be
   output to separate files. </li>
  </ul>

*/


