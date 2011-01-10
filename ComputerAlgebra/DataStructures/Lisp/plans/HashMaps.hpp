// Oliver Kullmann, 4.5.2008 (Guangzhou)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp
  \brief Plans for Maxima-components regarding maps


  \todo Introduce memoise function wrapper
  <ul>
   <li> Often we wish to call the same function many times in
   many different places with the same arguments, and the result
   takes a considerable amount of time to compute. </li>
   <li> A solution to this is to store the result in a hash table
   where the key is the arguments to the function. </li>
   <li> Such a solution can easily be written into a single function
   (say memoise_f) which given a function name, and arguments to that 
   function:
   <ul>
    <li> returns the result of that function from a global hash_table if the 
    hash key occurs. </li>
    <li> computes the result and then stores the value with the hash key
    for later re-use otherwise. </li>
   </ul>
   where the hash key is the list with the function name  and arguments.
   </li>
   <li> It seems better to take an empty hash table as an argument to 
   memoise_f rather than keeping a global hash_table, so that any other 
   function memoise_f has this hash_table properly scoped and can destroy
   it is no longer needed. </li>
   <li> See "Functions should not cache return values" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp .
   </li>
  </ul>


  \todo Using the Maxima "associative arrays"
  <ul>
   <li> One could try an alternative implementation of hash-maps via the
   associative arrays provided by Maxima.
   <li> Testing the speed:
   \verbatim
test1(n) := block([N : setn(n), P, MS, c0, h, t],
  P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 t : elapsed_run_time(),
 h : osm2hm(ll2osm(listify(MS),create_list(i,i,1,2^(2^n)))),
 print(elapsed_run_time()-t),
 map(lambda([S],ev_hm(h,c0(S))), listify(MS))
)$
test2(n) := block([N : setn(n), P, MS, c0, h, t],
  P : powerset(N),
 MS : powerset(P),
 c0 : lambda([S], setdifference(P,S)),
 t : elapsed_run_time(),
 block([i : 1],
   for S in MS do (h[S] : i, i : i+1)
 ),
 print(elapsed_run_time()-t),
 map(lambda([S],h[c0(S)]), listify(MS))
)$
   \endverbatim
   </li>
   <li> test2 is much faster, but h becomes a global variable. </li>
   <li> So one needs to create a new variable-name to be used here. </li>
  </ul>


  \todo New naming conventions
  <ul>
   <li> Rename old functions as indicated in
   ComputerAlgebra/DataStructures/Lisp/HashMaps.mac. </li>
   <li> The general abbreviations are:
    <ol>
     <li> "mp" for map (with one argument), with "lmp" for a "lambda map",
     that is, a map realised by a lambda-term, and with "lmpa" for a
     lambda map with build-in array </li>
     <li> "sm" for set-map </li>
     <li> "hm" for hash-map </li>
     <li> "ary" for okl-arrays </li>
     <li> "l" for list, with "il" for integer list, and "fl" for
     floating point list </li>
     <li> "s" for set </li>
    </ol>
   </li>
  </ul>


  \todo Concept of a set-theoretical map
  <ul>
   <li> All this should go to ComputerAlgebra/Sets. </li>
   <li> DONE (see ComputerAlgebra/DataStructures/Lisp/HashMaps.mac)
   We should institutionalise the concept of a "set-theoretic"
   map as a set of pairs. </li>
   <li> Operations:
    <ol>
     <li> DONE check for right-uniqueness </li>
     <li> extract domain </li>
     <li> extract range </li>
     <li> DONE compute value for a single argument </li>
     <li> compute value set for a set of arguments </li>
     <li> composition of partial and of total maps </li>
    </ol>
   </li>
   <li> We need the same functionality also for ordered set-maps. </li>
   <li> We also should have the concept of a (set-theoretical)
   relation as set of pairs!
    <ol>
     <li> Perhaps then we create a new sub-module "Relations.mac",
     which also contains the set-maps. </li>
    </ol>
   </li>
   <li> The alternative recursive implementation of allinj_sm
   \verbatim
allinj_sm(A,B) := if length(A) > length(B) then {} 
 elseif emptyp(A) then {{}}
 else
  block([a : first_element(A), RA],
   RA : disjoin(a,A),
   lappend(
    create_list(
      map(lambda([f],adjoin([a,b],f)), allinj_sm(RA,disjoin(b,B))),
      b, listify(B))))$
   \endverbatim
   should be mentioned in the documentation. </li>
  </ul>


  \todo Improving the hash-maps
  <ul>
   <li> Determining whether key x is contained in hash-map h:
    <ol>
     <li> Yet we can use ev_hm(h,x) if values are never "false". </li>
     <li> Otherwise one has to use a value y which can never occur,
     and one has to check via ev_hm_d(h,x,y). </li>
     <li> So well, should be doable, but it would be nice to have a direct
     way of querying whether x is already present. </li>
    </ol>
   </li>
   <li> Perhaps we provide specialised hash-maps for cases where we know
   a good hash-function for the key, and then we do not need to use the
   conversion to strings:
    <ol>
     <li> Especially for integer arguments this seems interesting. </li>
     <li> The underlying Lisp-functions seem to have already the ability
     to use different hash-functions; we need to understand this better. </li>
    </ol>
   </li>
  </ul>


  \todo Improving the okl-arrays
  <ul>
   <li> If "lambda" becomes abbreviated by "lm", why do we have the names
   "ary2lmp2", "lmpa2ary" and "lmpa2l" ? </li>
   <li> "p2" seems to be "part 2" (for the extraction). </li>
   <li> But also "pa2" seems to mean "part 2", so we should decide which
   form to choose. </li>
   <li> Compare "Conversions" in
   ComputerAlgebra/CombinatorialMatrices/Lisp/plans/Basics.hpp. </li>
  </ul>


  \bug DONE (removed the additional assumptions and tests)
  False assumption for testing hash-maps
  <ul>
   <li> The test-functions eq_ohmsm_p and eq_hmsm_p assume that two hash-maps,
   which are equal as set-maps, yield the same strings, but this is not true
   for ecl, where the strings are not, as with clisp, a translation of the
   set-maps, but just contain (apparently) a memory address. </li>
   <li> More precisely, with clisp the hash-maps as strings contains the
   list of assignments as well as the annotation-string (if present), while
   with ecl none of this information is contained, but a reference to the
   underlying object. </li>
   <li> DONE (we drop the tests --- they were created when we didn't fully
   understand the issues, and thus became overly cautious)
   The simplest solution is to abandon these extra tests.
   Or one could distinguish here between clisp and ecl. </li>
   <li> DONE (we don't make assumptions on the order of entries in a
   hash-map)
   Apparently one must treat the order of the pairs in the hash-map as
   implementation-defined (so that one shouldn't use it). </li>
  </ul>

*/
