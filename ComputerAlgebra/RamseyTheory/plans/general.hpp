// Oliver Kullmann, 20.2.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/plans/general.hpp
  \brief General plans regarding %Ramsey theory (in computer algebra)


  \todo Systematic notations for the numbers in %Ramsey theory : DONE
  <ul>
   <li> DONE
   Currently we have %Ramsey, van-der-Waerden and Green-Tao
   numbers. </li>
   <li> DONE
   Those numbers appear in plans, (soon) as functions (computing
   them), and in Latex-documents. </li>
   <li> DONE (we follow these conventions)
   For the plans-file (and other ascii-code) I propose ramsey_s^r(...),
   vanderwaerden_m(...) and greentao_m(...), following the usage of the
   latter two functions as it is already established:
    <ol>
     <li> s resp. m is the number of "colours" (or parts) </li>
     <li> "..." is either one number in case of diagonal problems, or a
     list of s resp. m numbers. </li>
     <li> r is the hyperedge-length. </li>
    </ol>
   </li>
   <li> DONE
    Maxima functions:
    <ol>
     <li> DONE
     For the Maxima function I propose ramsey([r,[...]]),
     vanderwaerden([...]) and greentao([...]), while in case of diagonal
     problems we use vanderwaerdend(m,k) resp. greentaod(m,k) resp.
     ramseyd(s,r,k) is used. </li>
     <li> DONE (no redundancy)
     Shall we keep the s resp. m? It is redundant except of the diagonal
     cases. </li>
     <li> DONE (no, but special functions for the diagonal cases)
     We could always write out the diagonal cases? Perhaps this is
     best. </li>
     <li> DONE
     If only bounds are known, then the return value is a pair consisting
     of lower and upper bound. </li>
    </ol>
   </li>
   <li> DONE
   The various generators for hypergraphs and clause-sets should be
   adapted to these new notions:
    <ol>
     <li> The order of parameters should be as above. </li>
     <li> Keeping, as we have it now, a "2" in the name for the
     case of k=2 (where then this parameter is no longer used). </li>
     <li> n is always appended to the end of the parameter list. </li>
    </ol>
   </li>
   <li> DONE
   For Latex we introduce macros \ramz{s}{r}{...}, \waez{m}{...} and
   \gtz{m}{...}, where for the output "ram_k^r", "vdw_k" and "grt_k" is used.
   </li>
   <li> See the plans on the machinery for providing Ramsey-numbers in
   ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/Numbers.hpp. </li>
  </ul>

*/
