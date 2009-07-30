// Oliver Kullmann, 12.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/plans/HindmanProblems.hpp
  \brief Plans for Maxima-generators related to Hindman problems (and generalisations)


  \todo Improve generator names
  <ul>
   <li> The current names are rather unsystematic. </li>
   <li> Also the notion of "degeneration" needs to be replaced by "injective"
   (appropriately). </li>
   <li> hindman2_nbfcsud(r,n) is for hindmani_r(2).
    <ol>
     <li> This function should be called hindmani_a1k2_nbfcsud(r,n). </li>
    </ol>
   </li>
   <li> hindman2d_nbfcsud(r,n) is for hindman_r(2).
    <ol>
     <li> This function should be called hindman_a1k2_nbfcsud(r,n). </li>
    </ol>
   </li>
   <li> Shouldn't we have these functions just for general a? Then called
   hindman_k2_nbfcsud(a,r,n) and hindmani_k2_nbfcsud(a,r,n). </li>
   <li> And likely we should just have hindman_nbfcsud(a,r,k,n) and
   hindmani_nbfcsud(a,r,k,n). </li>
   <li> How to call the non-diagonal versions?
    <ol>
     <li> Perhaps just using the suffix "nd". </li>
     <li> Perhaps after the (possible) "i". </li>
    </ol>
   </li>
   <li> hindman2gen_fcs(a,n) is for hindmani_2^a(2).
    <ol>
     <li> This function should be called hindmani_r2k2_fcs(a,n). </li>
     <li> And also hindman_r2k2_fcs(a,n) is needed. </li>
    </ol>
   </li>
   <li> There is also the non-diagonal versions, using a parameter tuple
   [k1,k2].
    <ol>
     <li> Again, just using suffix "nd". </li>
     <li> So the functions should be called hindmannd_r2_fcs(a,k1,k2,n) and
     hindmanind_r2_fcs(a,k1,k2,n). </li>
    </ol>
   </li>
  </ul>


  \todo The notion of a "Hindman parameter tuple"
  <ul>
   <li> As we have it currently for van-der-Waerden numbers (see
   vanderwaerden_p in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/Numbers.mac), we need the
   notion of a "Hindman parameter tuple". </li>
   <li> Actually, we should centralise all such notions; likely in module
   ComputerAlgebra/RamseyTheory. </li>
   <li> A "Hindman parameter tuple" should be a list [a,L], where a is a
   natural number >= 1, and L is a list of natural numbers >= 1, sorted in
   ascending order; but see below. </li>
   <li> The parameter a (changing the starting number of the list of
   vertices) could, of course, be used also for vdW-problems etc.
    <ol>
     <li> Do we need a general scheme? </li>
     <li> For the Hindman-problems perhaps the special motivation to consider
     this additional parameter was to get a handle on the otherwise too big
     numbers? </li>
     <li> So here we have special (historical) reasons to include parameter a.
     </li>
     <li> But for a general systematic exploration of the field of Ramsey
     theory likely this variation should also be applied to vdW-problems etc.
     </li>
     <li> Perhaps then those should be called tuples should be called
     "extended". And then we should also have Hindman parameter tuples
     L, and extended Hindman parameter tuples [a,L]. </li>
    </ol>
   </li>
  </ul>


  \todo Write tests

*/

