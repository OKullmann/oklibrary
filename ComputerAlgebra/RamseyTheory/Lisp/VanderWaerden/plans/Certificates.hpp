// Oliver Kullmann, 12.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/Certificates.hpp
  \brief Plans regarding van der Waerden certificates


  \todo The notion of a "certificate"
  <ul>
   <li> A "certificate" for a pair [L,n], where L is a parameter tuple of
   length m and n is a natural number, corresponds to a solution of
   vanderwaerden_nbfclud(L,n), that is, certifies that vanderwaerden(L) > n.
   </li>
   <li> We need a good mathematical presentation of such certificates. </li>
   <li> Reasonably seems to use a list Z of length n with numbers 1, ..., m,
   denoting the map {1,...,n} -> {1,...,m}. </li>
   <li> "Z" looks like a reasonable letter here: capital since it's a list, and
   "z" for "Zertifikat" ("C" can not be used). </li>
   <li> More precisely these are "vdW-certificates". </li>
   <li> certificate_vdW_p(Z,L,n) checks whether Z is indeed a certificate.
   </li>
  </ul>


  \todo Implementation check_certificate_vdw(L,n,P)
  <ul>
   <li> See
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/Hypergraphs.mac. </li>
   <li> This function uses an alternative representation of a certificate for
   [L,n] as a list P of the same size as L, with elements subsets of
   {1,...,n} representing a partitioning. </li>
   <li> DONE
   Perhaps we should have a dedicated file "Certificates.mac". </li>
   <li> The current check is rather slow:
   check_certificate_vdw([3,24],578,P) in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/24.hpp
   needed 82 seconds on csltok (the new laptop, not slow). </li>
   <li> An alternative algorithm is to create all possible arithmetic
   progressions of the given lengths and to check whether each of them is
   not contained in the corresponding blocks of the partition.
    <ol>
     <li> This should be faster if the block contains many elements. </li>
     <li> However otherwise it should be slower. </li>
    </ol>
   </li>
   <li> Or the current algorithm could use an array, which yields for each
   possible vertex its block (colour); this depends on having a linear order
   and "all vertices", but for that situation it is faster. </li>
   <li> At the Maxima-level we consider only the fundamental algorithmic
   possibilities; this issue seems also very much a data-structure issue,
   and this can be handled properly only at C++ level. See "Checking
   certificates" in Applications/RamseyTheory/plans/VanderWaerden.hpp. </li>
   <li> We should also use "every_s" (once it can handle multiple lists).
   </li>
  </ul>


  \todo Conversions
  <ul>
   <li> We need translations from satisfying assignments of
   vanderwaerden_nbfclud(L,n) to certificates and back.
    <ol>
     <li> If a vertex i in {1,...,n} does not get a value, we can denote this
     by value 0 in the certificate. </li>
     <li> Or we use the maxima-value "ind". </li>
    </ol>
   </li>
   <li> It would also be useful to translate certificates to partitions and
   back. This can be done in general (every map induces a canonical partition
   on its domain). </li>
  </ul>


  \todo Analysing certificates
  <ul>
   <li> We need a function to compute the Hamming-distance between two
   certificates. </li>
   <li> Given one certificate, we need a function to find all other
   certificates which have at most a specified distance to the given one. </li>
   <li> At least with vdw_2(3,k) it seems common that in the neighbourhoods of
   certificates you find other certificates; see "Analysing certificates" in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/general.hpp
   </li>
  </ul>

*/
