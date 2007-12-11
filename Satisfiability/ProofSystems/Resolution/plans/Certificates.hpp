// Oliver Kullmann, 22.5.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProofSystems/Resolution/plans/Certificates.hpp
  \brief Plans for resolution certificates (for unsatisfiability)


  \todo Verification of r-resolution refutations:
  <ul>
   <li> Given some r: CLS -> CLS such that for F' >= F and bot in r(F)
        we also have bot in r(F'), the basic calculus for refuting
        some F allows the following two steps, starting with empty F':
        <ol>
         <li> Add some C from F to F'. </li>
         <li> Add some C_phi to F' if bot in r(phi * F') (where F'
              is the old value). </li>
         <li> Add a resolvent of two clauses from F' to F'. </li>
        </ol>
   </li>
   <li> Especially for the r_k we need a file-format to store such refutations.
        And of course we need experimentations, how good we can create such
        refutations, and which size reductions can be achieved.
   </li>
   <li> A verifier is needed for such files. Here one should additionally
        seek to provide verified verifiers (collaboration with Markus
        Roggenbach and/or Ulrich Berger?) </li>
  </ul>


  \todo Stronger proof systems:
  <ul>
   <li> At least a file format and a verifier for extended resolution,
        and for the combination with the above r-resolution. </li>
   <li> What about Frege-Systems, extended Frege-Systems? (They should go
        to a different module; see ProofSystems/plans/general.hpp, topic
        "Stronger propositional proof systems than resolution".) </li>
   <li> What about proof systems for parameterised UNSAT formulas
        (or for tautologies), that is for UNSAT generators?
        Of course, also SAT generators are relevant.
        Here one could go up to ZFC. (Again, this should go to a different
        module.) </li>
  </ul>

*/

