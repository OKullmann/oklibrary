// Oliver Kullmann, 15.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/Trees/plans/Generators.hpp
  \brief Plans for the generation of various forms of trees


  \todo Evaluating the Maxima function random_tree
  <ul>
   <li> The following function
   \verbatim
test_generator(f,h) := do enter_new_occurrence(h,f())$
   \endverbatim
   is called with a generator f() for some object type and with
   a hash-map h, passed by reference, i.e., quoted. </li>
   <li> Initialisation:
   \verbatim
oklib_load_all();
set_random_state(make_random_state(0));
h_rt5 : sm2hm({});
f_rt5 : lambda([],mg2g(random_tree(5))[2]);
test_generator(f_rt5,h_rt5);
   \endverbatim
   </li>
   <li> This computation can be interrupted, h evaluated by
   \verbatim
freq_rt5 : map(second,get_distribution(h_rt5));
N_rt5 : apply("+",freq_rt5);
min_rt5 : lmin(freq_rt5);
max_rt5 : lmax(freq_rt5);
q_rt5 : float(max_rt5 / min_rt5);
   \endverbatim
   and then the computation continued (by "test_generator(f_rt5,h_rt5);").
   </li>
   <li> We obtain the data
    <ol>
     <li> N_rt5 = 7014267 </li>
     <li> q_rt5 = 24.65672898770423 </li>
    </ol>
   </li>
   <li> Considering the stars, we get
   \verbatim
create_list(ev_hm(h_rt5,star_g(5,i)), i,1,5);
[292774,72603,73530,73473,72716]
   \endverbatim
   </li>
   <li> For n=4:
   \verbatim
set_random_state(make_random_state(0));
h_rt4 : sm2hm({});
f_rt4 : lambda([],mg2g(random_tree(4))[2]);
test_generator(f_rt4,h_rt4);
dist_rt4 : get_distribution(h_rt4);
freq_rt4 : map(second,dist_rt4);
N_rt4 : apply("+",freq_rt4);
   \endverbatim
   </li>
   <li> We have four classes of trees (n=4; each with same likelihood):
    <ol>
     <li> C1 : The 6 trees (all pathgraphs)
     \verbatim
{{1,4},{2,3},{3,4}}
{{1,4},{2,3},{2,4}}
{{1,3},{2,4},{3,4}}
{{1,3},{2,3},{2,4}}
{{1,2},{2,4},{3,4}}
{{1,2},{2,3},{3,4}}
     \endverbatim
     have probability p. They all have vertex 1 as endpoint. </li>
     <li> C2 : The 3 trees (all stars)
     \verbatim
{{1,4},{2,4},{3,4}}
{{1,3},{2,3},{3,4}}
{{1,2},{2,3},{2,4}}
     \endverbatim
     have probability 2p (they don't have vertex 1 as centre). </li>
     <li> C3 : The 6 trees (all pathgraphs)
     \verbatim
{{1,3},{1,4},{2,4}}
{{1,3},{1,4},{2,3}}
{{1,2},{1,4},{3,4}}
{{1,2},{1,4},{2,3}}
{{1,2},{1,3},{3,4}}
{{1,2},{1,3},{2,4}}
     \endverbatim
     have probability 3p (they don't have vertex 1 as endpoint). </li>
     <li> C4 : The tree (a star)
     \verbatim
{{1,2},{1,3},{1,4}}
     \endverbatim
     has probability 6p (it has vertex 1 as centre). </li>
    </ol>
   </li>
   <li> The explanation is that "random_tree" implements a simple random
   process, which is captured by "randomtree_pr1". </li>
   <li> There are 4 isomorphism types of rooted trees with root 1 for n=4:
    <ol>
     <li> T1 = {{1,2},{2,3},{3,4}} (C1) </li>
     <li> T2 = {{1,2},{2,3},{2,4}} (C2) </li>
     <li> T3 = {{1,2},{1,3},{2,4}} (C3) </li>
     <li> T4 = {{1,2},{1,3},{1,4}} (C4). </li>
    </ol>
   </li>
   <li> We have the following probabilities for these four trees
   (showing the probabilities for the right new vertex and the right tree
   vertex):
    <ol>
     <li> T1 : (1/3*1) * (1/2*1/2) * (1*1/3) = 1/36 </li>
     <li> T2 : (1/3*1) * (1*1/2) * (1*1/2) = 2/36 </li>
     <li> T3 : (1/3*1) * (1/2*1/2) * (1*1/3) + (1/3*1) * (1*1/2) * (1*1/3) = 
     1/36 + 2/26 = 3/36 </li>
     <li> T4 : (1*1) * (1*1/2) * (1*1/3) = 6/36 </li>
    </ol>
   </li>
   <li> It remains to analyse randomtree_pr1:
    <ol>
     <li> Given a concrete tree, what's its probability? </li>
     <li> Regarding isomorphisms of graphs: Do isomorphic trees have the
     same probability? No (see above). </li>
     <li> Regarding isomorphisms of rooted trees (root always vertex 1):
     Do isomorphic rooted trees have the same probability? </li>
     <li> If this is true, given an isomorphism class, what's its
     probability? For n=4 we have C1 -> 6/36, C2 -> 6/36, C3 -> 18/36, 
     C4 -> 6/36. </li>
    </ol>
   </li>
  </ul>

*/

