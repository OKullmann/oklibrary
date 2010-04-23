// Matthew Gwynne 20.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/LogicalDataAnalysis/plans/SnowCrest2000Analysis.hpp
  \brief General plans regarding investigations on the logical analysis of data given in [The Outcomes of Homeless Mobilization; Cress, Snow, The American Journal of Sociology, 2000]
  

  \todo Create milestones.


  \todo Links
  <ul>
   <li> See "Matrices" in
   ComputerAlgebra/LogicalDataAnalysis/Lisp/plans/general.hpp for
   the data. </li>
  </ul>


  \todo List statistics and types of analysis needed.
  <ul>
   <li> Initially we have a truth table specification of a
   partial boolean function, and we wish to understand the
   assumptions (in terms of unspecified total assignments)
   which one would need to make to derive different 
   representations. </li>
   <li> We usually consider size here, looking for shortest
   DNF or CNF representations, but we might also look for 
   hitting clause-set representations, or representations 
   as DFAs etc. </li>
   <li> When considering shortest DNF and CNF representations,
   at a basic level we need the following statistics:
    <ul>
     <li> For each minimum-sized clause-set: 
      <ul>
       <li> the size of the clause-set. </li>
       <li> the prime implicates covered by this clause-set. </li>
      </ul>
      and for each of these, summary statistics such as:
      <ul>
       <li> the number of literal occurrences in the clause-set. </li>
       <li> the number of clauses of each size in the clause-set. </li>
      </ul>
     </li>
     <li> A list of 
      <ul>
       <li> which total assignments are assumed satisfying (resp falsfying)
       by *all* minimum-sized clause-sets. </li>
       <li> which total assignments are assumed satisfying (resp falsifying)
       by *none* of the minimum-sized clause-sets. </li>
       <li> which total assignments are assumed satisfying (resp falsifying)
       by *some but not all* of the minimum-size clause-sets, with 
       additionally information on which minimum-sized clause-sets need which.
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Representations for data analysis
  <ul>
   <li> Given the types of statistics and analysis listed in
   "List statistics and types of analysis needed", how can
   we represent these statistics in a compact and yet informative
   way? </li>
   <li> As an example, truth tables such as those given in the Snow/Crest
   paper include a great deal of redundancy (for us), as they list 
   cases as individual entries in the truth table, as they consider
   each case individually. </li>
   <li> We on the other hand consider each configuration (the values in
   the truth table), and so some cases collapse as they have the same
   configuration. </li>   
   <li> Assignments assumptions
    <ul>
     <li> When considering the assumptions made for each minimum-sized 
     clause-set (see " List statistics and types of analysis needed."),
     we have the question of how to represent this data in a succinct
     and informative way. </li>
     <li> Simply listing the partitioning as 3 sets or lists does not
     seem a particularly good representation, as at a glance the reader
     learns very little and must delve immediately into the details
     and do a lot of additional computation to learn anything. </li>
     <li> A Venn diagram or Karnaugh map using 3 different colours would 
     illustrate this partitioning in a succinct and reasonable way, and
     would also illustrate the connections and relations between individual
     total assignments (hamming distances etc). </li>  
    </ul>
   </li>
   <li> Are illustrations such as conflict graphs etc useful here? </li>
   <li> How do representations such as DFAs, neural networks etc
   relate to our CNF/DNF representations? </li>
  </ul>


  \todo Terminology 
  <ul>
   <li> We need a comprehensive list of the terminology to be used
   here along with basic definitions, so that we can be consistent
   and ensure that as much as possible, any terminology that does not
   come from boolean algebra or satisfiability, or that refers specifically
   to concepts in QCA, should be called by a reasonable name or it's
   original term in the literature. </li>
  </ul>

*/

