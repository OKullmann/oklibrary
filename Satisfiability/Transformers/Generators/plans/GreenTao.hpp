// Oliver Kullmann, 11.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/GreenTao.hpp
  \brief Plans for generators for Green-Tao problems


  \todo Connections and scope
  <ul>
   <li> Generators for VanderWaerden, Ramsey-, GreenTao etc.-problems
   should be located in this module (as satisfiability problems). </li>
   <li> The specifications are to be provided in module
   ComputerAlgebra/Satisfiability/Lisp/Generators; see
   ComputerAlgebra/Satisfiability/Lisp/Generators/plans/VanderWaerdenProblems.hpp.
   </li>
   <li> While hypergraph generators should be located in module
   Combinatorics/Hypergraphs/Generators; see
   Combinatorics/Hypergraphs/Generators/plans/general.hpp. </li>
   <li> In module Applications/RamseyTheory one finds applications for
   investigating the associated numbers, hypergraphs and SAT problems. </li>
  </ul>


  \todo The basic generators
  <ul>
   <li> The basic generators are greentao2_fcs, greentao2nd_fcs,
   greentao_nbfcsud and greentaod_nbfcsud in
   ComputerAlgebra/Satisfiability/Lisp/Generators/VanderWaerdenProblems.mac.
   </li>
   <li> The symmetry-breaking variants are greentao_sb_fcs and
   greentaod_sb_nbfcsud there. </li>
   <li> And the output-functions are output_greentao2, output_greentao2nd,
   output_greentao and output_greentaod (basic forms), plus output_greentao_sb
   and output_greentaod_sb. </li>
  </ul>

*/

