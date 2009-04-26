// Oliver Kullmann, 22.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-3-k.hpp
  \brief On investigations into vdW-numbers vdw_3(3,3,k)
  
  
  \todo Literature overview
  <ul>
   <li> Apparently nothing is known about vdw_3(3,3,k) ? Could there also be
   a conjecture that it's polynomially bounded? </li>
   <li> The known values:
   \verbatim
create_list(vanderwaerden33k(k),k,1,6);
 [9,14,27,51,80,unknown]
   \endverbatim
   </li>
   <li> [Ahmed 2009] states
    <ol>
     <li> vanderwaerden33k(6) > 105 </li>
     <li> vanderwaerden33k(7) > 142 </li>
     <li> vanderwaerden33k(8) > 160 </li>
    </ol>
   </li>
  </ul>

*/

