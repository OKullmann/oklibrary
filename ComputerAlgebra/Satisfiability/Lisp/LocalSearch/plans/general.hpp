// Oliver Kullmann, 31.8.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp
  \brief Plans for local search SAT algorithms at the Maxima/Lisp level in general


  \todo Relations to other modules
  <ul>
   <li> See Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> See ExperimentSystem/ControllingLocalSearch/plans/general.hpp. </li>
  </ul>


  \todo A general framework
  <ul>
   <li> See [Stochastic Local Search: Foundations and Applications; Hoos,
   Stuetzle, 2005]. </li>
   <li> We should be able to establish a generic framework. </li>
   <li> This generic framework should allow us to use specialised algorithm
   for hypergraph colouring (see "Faster local search" in
   Experimentation/Investigations/plans/VanderWaerdenProblems.hpp). </li>
  </ul>


  \todo Examing search landscapes
  <ul>
   <li> See "Framework for the analysis of search space structures" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> A good overview is given by chapter 5 ("Search space structure
   and SLS performance") in [Hoos, Stuetzle; Stochastic Local Search];
   we should strive to implement all methods there at least in their
   exact versions.
    <ol>
     <li> Perhaps we should first consider ordinary satisfiability (boolean,
     and in clausal form), while generalisations come later. </li>
     <li> Generalisations which apply to arbitrary optimisation problems
     perhaps should not be considered in this module, but for example
     in a supermodule ComputerAlgebra/LocalSearch. </li>
     <li> For signed clause-sets we consider the hypercube of total
     assignments (edges between total assignments which differ in exactly
     one position), where the vertices are labelled with the number of
     falsified clauses. </li>
     <li> One could then consider the directed graph with edges only to
     positions where the score is improved. </li>
    </ol>
   </li>
   <li> Additionally we should look into the literature on random
   problems and statistical physics (see also the work of Elitza
   Maneva et al). </li>
  </ul>


  \todo Theoretical random walk algorithms
  <ul>
   <li> The base cases are Papadimitriou's and Schoening's algorithms. </li>
   <li> They should be easy instances of the generic framework. </li>
   <li> What to do with these implementations:
    <ol>
     <li> They serve educational purposes. </li>
     <li> Regarding their real efficiency, since they only use features which
     are analysable (to a certain extend), they should be inferior to
     "practical" schemes. </li>
     <li> Perhaps one could also integrate their analysis into our system.
     </li>
     <li> And worse-case instances are of interest. </li>
     <li> As well as general evaluations of their runtime behaviour (which
     should be simpler, since "purer", than for practical schemes). </li>
    </ol>
   </li>
  </ul>

*/

