// Oliver Kullmann, 9.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Quantification/plans/general.hpp
  \brief Module for quantified boolean formulas and generalisations


  \todo Create milestones.


  \todo Main challenge are the concepts.
  <ul>
   <li> Arbitrary "constraint-problems" can be quantified. </li>
   <li> "Quantification" is a structuring-process on the variables. </li>
   <li> "Models" for a true formula are now maps which realise the existential
   variables, while "counter-examples" for a false formula realise the
   universal variables. </li>
   <li> For such realisations different specifications can be used to yield a
   compression. </li>
   <li> The validation of "models" and "counter-examples" becomes more
   important (and complex) --- the field includes for example the proof
   systems of propositional logic (only universal variables, and validity is
   to be shown, or only existential variables, and falsity is to be shown)!
   </li>
   <li> A question is, whether also other quantifier (besides "for all" and
   "exists") should be considered here, or whether they %mean really a
   different thing (to be considered in another module)? </li>
   <li> Another link is to first-order logic (with equality), considering a
   fixed structure {0,1} with the operations "and, or, not" --- this is a
   special case of checking whether for a given structure a first-order formula
   holds ("expression complexity"). One could consider the whole problem of
   "quantified generalised constraint-problems" as the problem of deciding
   whether in some fixed structure (with equality, functions and relations)
   a first-order sentence holds (however this view is too much fixed on
   a specific representation of a problem!). </li>
   <li> First explorations in the ComputerAlgebra-part; see
   ComputerAlgebra/Satisfiability/Lisp/Quantification/plans/general.hpp. </li>
  </ul>


  \todo What are the input formats?
  <ul>
   <li> http://www.qbflib.org/qdimacs.html </li>
   <li> See SAT evaluation. </li>
   <li> This should become a part of Satisfiability/Interfaces. </li>
  </ul>


  \todo Applications
  <ul>
   <li> See sub-module Games/plans/Chess.hpp. </li>
   <li> See "Positional games" in
   ComputerAlgebra/Hypergraphs/Lisp/plans/Colouring.hpp. </li>
  </ul>


  \todo Basic autarkies
  <ul>
   <li> Since QBF is a harder problem than SAT, here it might be worth to
   eliminate all basic autarkies (i.e., just crossing out all universal
   variables), i.e., to compute the lean kernel of the matrix (ignoring all
   universal variables). </li>
   <li> Of course, more heuristical schemes or polynomial-time algorithms for
   autarky search can also be used. </li>
   <li> See Autarkies/Search/plans/general.hpp. </li>
  </ul>


  \todo Bounded maximal deficiency
  <ul>
   <li> Implement the algorithm for QCNF with deficiency at most one
   (after reduction to the lean kernel) as developed by Kleine Buening
   and Zhao. </li>
   <li> Perhaps a submodule is appropriate. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::Quantification
  \brief Components for quantified generalised SAT problems

  Alias "Qua".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Quantification {
    }
    namespace Qua = Quantification;
  }
}

