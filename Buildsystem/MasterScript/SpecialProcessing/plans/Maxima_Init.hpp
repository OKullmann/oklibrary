// Oliver Kullmann, 12.9.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp
  \brief Plans regarding initialisation of Maxima


  \todo Reasonable memory and stack limits
  <ul>
   <li> We should enhance the various limits, appropriate for the minimum
   memory size we require for our machines (2 GB). </li>
   <li> Compare "Hypergraphs of arithmetic progressions" in
   ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/general.hpp. </li>
   <li>
   \verbatim
:lisp (ext:set-limit 'ext:frame-stack 8192)
:lisp (ext:set-limit 'ext:binding-stack 65536)
:lisp (ext:set-limit 'ext:c-stack 1048576)
:lisp (ext:set-limit 'ext:heap-size 1073741824)
:lisp (ext:set-limit 'ext:lisp-stack 131072)
   \endverbatim
   should be more reasonable than the default values. </li>
   <li> These commands are for Ecl --- do they also work for CLisp? </li>
   <li> How to use Maxima variables in this context?
   \verbatim
frame_stack : 2^13;
:lisp (ext:set-limit 'ext:frame-stack $frame_stack)
   \endverbatim
   seems to work. </li>
   <li> So we should use variables "frame_stack, binding_stack, c_stack,
   heap_size, list_stack". If they only work for Ecl, then we should add
   the suffix "_ecl". </li>
   <li> One also has the command-line parameters "--heap-size, --lisp-stack,
   --frame-stack, --c-stack", but it seems they are not useful here. </li>
  </ul>

*/
