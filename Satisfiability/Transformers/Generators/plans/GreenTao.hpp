// Oliver Kullmann, 11.6.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/GreenTao.hpp
  \brief Plans for generators for Green-Tao problems

  This covers GreenTaoCNF.cpp, GreenTaoGCNF.cpp, GTTransversals and
  GTTransversalsInc.


  \todo Connections and scope
  <ul>
   <li> Generators for GreenTao etc.-problems should be located in this module
   (as satisfiability problems). </li>
   <li> The specifications are to be provided in module
   ComputerAlgebra/Satisfiability/Lisp/Generators; see
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/GreenTaoProblems.hpp.
   </li>
   <li> While hypergraph generators should be located in module
   Combinatorics/Hypergraphs/Generators/plans/GreenTao.hpp. </li>
   <li> In module Applications/RamseyTheory one finds applications for
   investigating the associated numbers, hypergraphs and SAT problems. </li>
  </ul>


  \todo The basic generators
  <ul>
   <li> The basic generators are greentao2_fcs, greentao2nd_fcs,
   greentao_nbfcsud and greentaod_nbfcsud in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/GreenTaoProblems.mac.
   </li>
   <li> The symmetry-breaking variants are greentao_sb_fcs and
   greentaod_sb_nbfcsud there. </li>
   <li> And the output-functions are output_greentao2, output_greentao2nd,
   output_greentao and output_greentaod (basic forms), plus output_greentao_sb
   and output_greentaod_sb. </li>
   <li> The script "GTSat" covers output_greentao2nd and output_greentao;
   yet the symmetry-breaking clauses have to be added manually. </li>
   <li> DONE (GTdSat)
   Likely we should also provide a wrapper script for the diagonal cases. </li>
  </ul>


  \todo Improving SAT translations
  <ul>
   <li> See the todos in Generators/GreenTaoCNF.cpp. </li>
   <li> See the todos in Generators/GreenTaoGCNF.cpp. </li>
   <li> It seems better not to have GreenTaoCNF-O3-DNDEBUG and
   GreenTaoGCNF-O3-DNDEBUG available in the public bin-directory, since the
   scripts GTSat and GTdSat shall do the job. </li>
  </ul>


  \todo Improving GTTransversals
  <ul>
   <li> We should use the C++ hypergraph generator GreenTao.cpp instead of
   the Maxima generator. </li>
   <li> A little problem is that the C++ program doesn't standardise at all,
   while the Maxima version standardises the vertex set of the (whole)
   hypergraph. </li>
  </ul>


  \todo Improving GTTransversalsInc
  <ul>
   <li> DONE
   If no additional hyperedge has been added, then we should just transfer
   the result, without running the solver. </li>
  </ul>

*/
