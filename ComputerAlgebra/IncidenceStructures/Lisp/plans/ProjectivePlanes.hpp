// Oliver Kullmann, 7.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/IncidenceStructures/Lisp/plans/ProjectivePlanes.hpp
  \brief Plans on projective incidence planes


  \todo Projective planes with polarity
  <ul>
   <li> First task is to sort out the cases of degeneration.
    <ol>
     <li> We have "empty projective incidence planes", "near-empty projective
     incidence planes", "pencils" and "near-pencils". </li>
     <li> Especially we need to sort out the cases of degeneration for
     the polar cases. </li>
     <li> Here the notions of "dominating vertices" are central. </li>
     <li> See the "friendship theorem" in
     CourseCombinatorics_LintWilson/Chapter21.hpp. </li>
    </ol>
   </li>
   <li> Then we should find all types of polar projective planes of
   order 2.
    <ol>
     <li> One type is already given by fano_gl. </li>
     <li> The question is whether this is everything? (That is, if we
     rearrange the rows and columns of fano_m to obtain a symmetric matrix,
     are all symmetric matrices obtained in this way, as square matrices,
     isomorphic to fano_m?) </li>
     <li> See "Duality and polarity" in
     ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac. </li>
    </ol>
   </li>
   <li> We need to find out what is known about projective planes with
   polarities.
    <ol>
     <li> Are the possible orders known? </li>
     <li> Instead of searching for arbitrary projective planes of a given
     order, searching for projective planes with polarity is easier, since
     we just have to search for design graphs with loops --- is this
     restriction interesting or "harmful" ? </li>
     <li> See "Exactly one common neighbour" in
     ComputerAlgebra/Graphs/Lisp/StrongRegularity/plans/general.hpp for the
     notion of a "(weak) design graph with loops". </li>
     <li> See the "friendship theorem" in
     CourseCombinatorics_LintWilson/Chapter21.hpp for considerations centred
     around the statement that polarities of non-degenerated projective
     incidence planes must have absolute points. </li>
    </ol>
   </li>
  </ul>

*/

