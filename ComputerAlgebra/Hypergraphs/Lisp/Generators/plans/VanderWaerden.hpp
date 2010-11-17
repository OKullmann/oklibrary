// Oliver Kullmann, 16.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/VanderWaerden.hpp
  \brief Plans regarding generators for vdW-hypergraphs


  \todo palindromise_vdw_ohg
  <ul>
   <li> DONE
   Order of hyperedges:
    <ol>
     <li> Currently the order is directly derived from the colexicographical
     ordering on the original hyperedges. </li>
     <li> For example palindromise_vdw_ohg(arithprog_ohg(3,7)) =
     [[1,2,3,4],[{1,2,3},{2,3,4},{1,3},{3,4},{2,4},{1,4}]]. </li>
     <li> Perhaps colexicographical ordering would be better, which would be
     here [{1,3},{1,2,3},{1,4},{2,4},{3,4},{2,3,4}]. </li>
     <li> In ComputerAlgebra/Combinatorics/Lisp/Enumeration/Order.mac we have
     colex_lessp_l, however this functions needs to be generalised to handle
     lists of different lengths. </li>
    </ol>
   </li>
   <li> DONE
   Perhaps subsumption-elimination should be performed. For the above
   example we would get  [{1,3},{1,4},{2,4},{3,4}]. </li>
   <li> Counting the number of hyperedges: a formula for the number of
   hyperedges should be developed. </li>
  </ul>


  \todo DONE (now the implementation is non-recursive, and in case more
  memory is needed, use default_memory_ecl())
  Hypergraphs of arithmetic progressions
  <ul>
   <li> The current implementation of arithprog_ohg/arithprog_hg can only
   handle small values of n --- otherwise we get a stack-size error!
   \verbatim
C-STACK overflow at size 139456. Stack can probably be resized.
   \endverbatim
   (this on a 32-bit machine with 2GB memory). </li>
   <li> Perhaps this is due to Ecl --- can we grow the stack size?!
    <ol>
     <li> ulimit reports that there are no restrictions from the bash-side.
     </li>
     <li> There are various Ecl memory limits which are discussed in the
     manual -
     http://ecls.sourceforge.net/new-manual/re34.html#table.memory.limits . 
     </li>
     <li> In this case the C-STACK is overflowing with a current limit of 
     128KB. This can be adjusted to say 1MB in the following way:
     \verbatim
:lisp (ext:set-limit 'ext:c-stack 1048576)
     \endverbatim
     </li>
     <li> MG: Please make the documentation of Ecl
     available (locally), document all the memory option (apparently
     there are five: "frame-stack, binding-stack, c-stack, heap-size,
     lisp-stack"; they can also be set on the command line), together
     with appropriate values, and move this important documentation into its 
     right (general) place. Likely the OKlibrary should provide meta-commands,
     which work for all Lisps supported; perhaps just asking for
     one argument, the amount of memory available for Maxima,
     and then calculating the approriate values.
     One also needs to find out how to pass command-line arguments to
     ecl (for example "--frame-stack 4096"). </li>
     <li> Compare "Reasonable memory and stack limits" in
     Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp. </li>
    </ol>
   </li>
   <li> On the other hand, a non-recursive solution is also very easy to
   produce. </li>
   <li> However, such little problems shouldn't pose a problem! </li>
  </ul>

*/
