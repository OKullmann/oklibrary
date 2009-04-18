// Oliver Kullmann, 18.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/NonBoolean.hpp
  \brief Plans for Maxima-components regarding generalised clause-sets


  \todo Notions for generalised literals
  <ul>
   <li> Compare "Better general naming conventions" in
   ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp. </li>
   <li> We have the "non-boolean" versions of literals, where literals are
   pairs [v,e], with e a value. </li>
   <li> And there are "power" versions, where then e is a set of values. </li>
   <li> The "monosigned literals" resp. "signed literals" are corresponding
   triples, with the third component in {-1,+1}. </li>
   <li> One could instead use terms "nb(v,e), nb(v,e,s), pl(v,e), pl(v,e,s)".
   </li>
   <li> A problem with non-boolean variables is, where to put the information
   about the domain of the variables:
    <ol>
     <li> See the discussions in
     ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp.
     </li>
     <li> Perhaps it's not part of clause-sets etc., but only part of
     "problems" given for example to SAT-solvers. </li>
     <li> So a signed clause-set etc. would always need to be accompanied
     by either a uniform domain, or by a domain function. </li>
     <li> But perhaps we should use pairs [FF, D], where D is a set,
     list or map. Perhaps the default for all types of clause-sets is
     a uniform domain. Actually, likely better to use triples [FF[1],FF[2],D].
     </li>
     <li> But since we can also use sensibly non-boolean clause-sets without
     the domain information (for example it is not needed in order to apply
     a partial assignment), we should use the (additional) suffix "ud" for
     uniform domain and "fd" for function domain. </li>
     <li> There is also the idea that a "domain association" is basically
     a partial assignment; actually it should be a "total partial assignment".
     </li>
     <li> One could allow then suffixes like "tpa_mp". </li>
    </ol>
   </li>
  </ul>

*/

