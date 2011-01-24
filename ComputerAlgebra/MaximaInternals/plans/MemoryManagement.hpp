// Oliver Kullmann, 11.10.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/MaximaInternals/plans/MemoryManagement.hpp
  \brief Plans on improved memory managenemt


  \todo Improve default_memory_ecl
  <ul>
   <li> Compare "What memory-bounds?" in
   Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp. </li>
   <li> It seems that set_lisp_stack_ecl(ls) does not work as expected:
   \verbatim
show_memory_ecl();

frame_stack_ecl: 2304 
binding_stack_ecl: 8448 
c_stack_ecl: 557056 
heap_size_ecl: 1073741824 
lisp_stack_ecl: 561408 

  561408

default_memory_ecl();
  262144

show_memory_ecl();

frame_stack_ecl: 16640 
binding_stack_ecl: 131072 
c_stack_ecl: 2129920 
heap_size_ecl: 1073741824 
lisp_stack_ecl: 33849600 

33849600
   \endverbatim
   So apparently by itself Maxima increased the lisp-spack by a large amount.
   </li>
   <li> We should check this again with Ecl 11.1.1.1 and Maxima 5.23.2. </li>
  </ul>

*/
