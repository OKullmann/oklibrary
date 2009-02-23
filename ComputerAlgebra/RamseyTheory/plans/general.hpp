// Oliver Kullmann, 20.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/plans/general.hpp
  \brief General plans regarding %Ramsey theory (in computer algebra)


  \todo Systematic notations for the numbers in %Ramsey theory
  <ul>
   <li> Currently we have %Ramsey, van-der-Waerden and Green-Tao
   numbers. </li>
   <li> Those numbers appear in plans, (soon) as functions (computing
   them), and in Latex-documents. </li>
   <li> For the plans-file (and other ascii-code) I propose ramsey_k^r(...),
   vanderwaerden_k(...) and greentao_k(...), following the usage of the
   latter two functions as it is already established:
    <ol>
     <li> k is the number of "colours" (or parts) </li>
     <li> "..." is either one number in case of diagonal problems, or a
     list of k numbers. </li>
     <li> r is the hyperedge-length. </li>
    </ol>
   </li>
   <li> For the Maxima function I propose ramsey(k,r,[...]),
   vanderwaerden(k,[...]) and greentao(k,[...]), where in case of diagonal
   problems "[...]" actually is a single number. </li>
   <li> The various generators for hypergraphs and clause-sets should be
   adapted to these new notions:
    <ol>
     <li> The order of parameters should be as above. </li>
     <li> Keeping, as we have it now, a "2" in the name for the
     case of k=2 (where then this parameter is no longer used). </li>
     <li> n is always appended to the end of the parameter list. </li>
    </ol>
   </li>
   <li> For Latex we introduce macros \ramz[3], \waez[2] and \gtz[2], where
   for the output "ram_k^r", "vdw_k" and "grt_k" is used. </li>
   <li> See the plans on the machinery for providing Ramsey-numbers in
   ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/Numbers.hpp. </li>
  </ul>

*/
