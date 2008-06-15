// Oliver Kullmann, 15.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Lisp/Reductions/plans/DP-Reductions.hpp
  \brief Plans on special cases of DP-resolution which lead to "reductions"


  \todo Singular DP-reduction
  <ul>
   <li> Transfer SingDPuncontracted, ISingDPuncontracted from
   Mupad/Orthogonal.mup. </li>
   <li> This should be just the randomised version of singular_dp_reduction
   (easily achieved by randomly choosing a singular variable). </li>
   <li> Perhaps the input should better be a formal clause-set, and the
   reduction happens in-place.
    <ol>
     <li> Perhaps then we better create a new-function. </li>
     <li> Perhaps we have a general naming scheme for distinguishing
     between in-place operations and operations which leave their arguments
     intact? </li>
    </ol>
   </li>
   <li> The current implementation of redsingdpp could be used for
   testing purposes, and otherwise replaced by the obvious check for
   the existence of a singular variable. </li>
  </ul>

*/

