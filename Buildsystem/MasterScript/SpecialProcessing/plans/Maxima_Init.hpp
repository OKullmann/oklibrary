// Oliver Kullmann, 12.9.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/MasterScript/SpecialProcessing/plans/Maxima_Init.hpp
  \brief Plans regarding initialisation of Maxima


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
