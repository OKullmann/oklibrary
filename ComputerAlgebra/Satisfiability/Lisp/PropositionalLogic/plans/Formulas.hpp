// Oliver Kullmann, 13.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/Formulas.hpp
  \brief Plans on representations of propositional logic


  \todo Basic concepts
  <ul>
   <li> We need to reflect and document the basic concepts about propositional
   formulas. </li>
   <li> Current names for operations: "and,not,xor,equiv,impl,not". </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BasicFunctions.hpp
   for the list of the 16 binary functions. </li>
   <li> See "Input and output of propositional formulas" in
   Interfaces/InputOutput/plans/general.hpp for input/output of propositional
   formulas in various formats. </li>
  </ul>


  \todo Using Maxima expressions
  <ul>
   <li> There is a Maxima package on the topic (not in the current
   distribution). </li>
   <li> As a second method for handling propositional logic, define
   appropriate new functions with appropriate properties, so that
   we can express propositional logic via Maxima terms. </li>
   <li> Then exploit the built-in simplification methods. </li>
   <li> It would be a "user-interface". </li>
   <li> Simplifications would really be only the "straightforward" ones. </li>
   <li> We cannot use the build-in "and, or, not", since they do not form
   expressions (and likely we would mess up the whole system). </li>
   <li> And then perhaps we should also change their names in our tree-
   representation? </li>
   <li> Proposed translations (trailing "o" for "operation"):
    <ol>
     <li> "not" becomes "neo" </li>
     <li> "and" becomes "ano" </li>
     <li> "or" becomes "oro" </li>
     <li> "impl" becomes "imo" </li>
     <li> "equiv" becomes "eqo" </li>
     <li> "xor" becomes "xoo" </li>
     <li> "if-then-else" becomes "ifo" </li>
     <li> "nand" becomes "nao" </li>
     <li> "nor" becomes "noo" </li>
     <li> The Sheffer-op becomes "sho" </li>
    </ol>
   </li>
   <li> Or perhaps we should use longer (easier readable) names for our
   tree-representation? But then the translation becomes more tedious. </li>
   <li> Attempting more readable operation-names (though less systematical):
    <ol>
     <li> "not" becomes "nego" </li>
     <li> "and" becomes "ando" </li>
     <li> "or" becomes "oro" </li>
     <li> "impl" becomes "impo" </li>
     <li> "equiv" becomes "eqo" </li>
     <li> "xor" becomes "xoro" </li>
     <li> "if-then-else" becomes "ifo" </li>
     <li> "nand" becomes "nano" </li>
     <li> "nor" becomes "noro" </li>
     <li> The Sheffer-op becomes "sheo" </li>
    </ol>
    I (OK) like these more.
   </li>
   <li> Perhaps we should have all 16 binary operations.
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BasicFunctions.hpp
     for the list. </li>
     <li> Likely we should very similar names? </li>
    </ol>
   </li>
   <li> In principle the following should do the job for the boolean basis:
   \verbatim
matchdeclare(logexpr,true);
matchdeclare(logexpr2,true);
kill(ando,oro,nego);
declare(ando, commutative);
declare(ando, nary);
tellsimp(ando(true,logexpr),logexpr);
tellsimp(ando(false,logexpr),false);
tellsimp(ando(),true);
tellsimp(ando(logexpr),logexpr);
tellsimp(ando(logexpr,logexpr),logexpr);
declare(oro, commutative);
declare(oro, nary);
matchdeclare(logexpr,true);
tellsimp(oro(true,logexpr),true);
tellsimp(oro(false,logexpr),logexpr);
tellsimp(oro(),false);
tellsimp(oro(logexpr),logexpr);
tellsimp(oro(logexpr,logexpr),logexpr);
tellsimp(ando(logexpr,oro(logexpr,logexpr2)),logexpr);
tellsimp(oro(logexpr,ando(logexpr,logexpr2)),logexpr);
tellsimp(oro(ando(logexpr,logexpr2),ando(logexpr,nego(logexpr2))),logexpr);
tellsimp(ando(oro(logexpr,logexpr2),oro(logexpr,nego(logexpr2))),logexpr);
tellsimp(nego(false), true);
tellsimp(nego(true), false);
tellsimp(nego(nego(logexpr)),logexpr);
tellsimp(ando(logexpr,nego(logexpr)),false);
tellsimp(oro(logexpr,nego(logexpr)),true);
tellsimp(nego(ando(logexpr,logexpr2)),oro(nego(logexpr),nego(logexpr2)));
tellsimp(nego(oro(logexpr,logexpr2)),ando(nego(logexpr),nego(logexpr2)));
   \endverbatim
   The last two rules could be separated (they establish NNF).
   </li>
   <li> Are these rules together confluent? (Should be; perhaps we should
   install Maude, as a strong system for handling rewrite-rules.) </li>
   <li> The constant-simplification apparently needs to be strengthened, so
   that constants in arbitrary nary-expressions (at arbitrary positions)
   are recognised. </li>
   <li> The commutative-decl has no effect? ando(a,b) should be recognised
   as equal to ando(b,a) ?? It works alone, but not together with nary.
   So perhaps we have a bug here. Using "lassociative" instead of "nary"
   seems to render the whole thing useless?? </li>
   <li> See the neg-declarations in
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/Formulas.mac. </li>
   <li> The Maxima-documentation on these issues seems completely obscure.
   Ask at the mailing-list. </li>
  </ul>

*/

