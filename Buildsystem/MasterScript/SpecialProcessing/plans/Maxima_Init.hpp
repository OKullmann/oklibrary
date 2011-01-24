// Oliver Kullmann, 12.9.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp
  \brief Plans regarding initialisation of Maxima


  \todo DONE (our default values are now set when calling Maxima)
  Reasonable memory and stack limits
  <ul>
   <li> DONE (we assume that the current default values are appropriate)
   We should enhance the various limits, appropriate for the minimum
   memory size we require for our machines (4 GB). </li>
   <li> DONE
   We now have default_memory_ecl() in
   ComputerAlgebra/MaximaInternals/MemoryManagement.mac. </li>
   <li> DONE
   See ComputerAlgebra/MaximaInternals/plans/MemoryManagement.hpp. </li>
   <li> DONE
   So if enhanced memory is needed, then this function is to be called.
   It seems difficult to do this directly at initialisation-time. </li>
   <li> DONE
   Compare "Hypergraphs of arithmetic progressions" in
   ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/general.hpp. </li>
   <li> DONE
   \verbatim
:lisp (ext:set-limit 'ext:frame-stack 8192)
:lisp (ext:set-limit 'ext:binding-stack 65536)
:lisp (ext:set-limit 'ext:c-stack 1048576)
:lisp (ext:set-limit 'ext:heap-size 1073741824)
:lisp (ext:set-limit 'ext:lisp-stack 131072)
   \endverbatim
   should be more reasonable than the default values. Remark:
   apparently with Ecl version 9.12.3 heap-size is already 2^30. </li>
   <li> DONE (apparently these commands are Ecl-specific)
   These commands are for Ecl --- do they also work for CLisp? </li>
   <li> DONE
   How to use Maxima variables in this context?
   \verbatim
frame_stack : 2^13;
:lisp (ext:set-limit 'ext:frame-stack $frame_stack)
   \endverbatim
   seems to work. </li>
   <li> DONE
   So we should use variables "frame_stack, binding_stack, c_stack,
   heap_size, lisp_stack". Since they only work for Ecl, then we should add
   the suffix "_ecl". </li>
   <li> DONE
   This yields
   \verbatim
frame_stack_ecl : 2^13;
binding_stack_ecl : 2^16;
c_stack_ecl : 2^20;
heap_size_ecl : 2^30;
lisp_stack_ecl : 2^17;
:lisp (ext:set-limit 'ext:frame-stack $frame_stack_ecl)
:lisp (ext:set-limit 'ext:binding-stack $binding_stack_ecl)
:lisp (ext:set-limit 'ext:c-stack $c_stack_ecl)
:lisp (ext:set-limit 'ext:heap-size $heap_size_ecl)
:lisp (ext:set-limit 'ext:lisp-stack $lisp_stack_ecl)
   \endverbatim
   </li>
   <li> DONE (available in ComputerAlgebra/MaximaInternals/MemoryManagement.lisp)
   Now we should define Maxima functions "set_frame_stack_ecl", etc.,
   which perform these Lisp-instructions --- how to do this?
    <ol>
     <li> As in ComputerAlgebra/DataStructures/Lisp/HashMaps.lisp, we have to
     create five Lisp-functions in a file MemoryManagement.lisp, encapsulating
     the above instructions. This can then be included by oklib_include. </li>
     <li> The Maxima functions "set_frame_stack_ecl" etc. are simple wrappers
     for these Lisp-functions (so that also from the command-line we can use
     these functions without using ":lisp"). </li>
     <li> We simply call these Lisp functions "set_frame_stack_ecl_" etc. </li>
     <li> But these Lisp functions likely can't use easily the Maxima-variables
     like "frame_stack", so we need to provide these variables as
     Lisp-variables, which are then read and write from Maxima. </li>
     <li> Perhaps then we don't use global variables, but just setter and
     getter functions? </li>
     <li> 
    </ol>
   </li>
   <li> DONE (we don't need them anymore)
   One also has the command-line parameters "--heap-size, --lisp-stack,
   --frame-stack, --c-stack", but it seems they are not useful here. </li>
  </ul>


  \todo What memory-bounds?
  <ul>
   <li> Compare "Improve default_memory_ecl" in
   ComputerAlgebra/MaximaInternals/plans/MemoryManagement.hpp. </li>
   <li> MG mentioned that increasing certain of the above values has a
   negative effect on others due to increased memory-consumption; MG should
   explore this. </li>
   <li> Is frame_stack_ecl_okl too large? </li>
  </ul>

*/
