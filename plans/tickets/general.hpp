// Oliver Kullmann, 4.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/plans/tickets/general.hpp
  \brief Plans regarding the ticket system


  \todo Ranking
  <ul>
   <li> The running numbers gives a rough indication on the order of
   processing the tickets. </li>
   <li> In the fourth column we can use precise priority-specifications:
    <ol>
     <li> "A" means urgent, must be done as the next step (on a daily basis --
     this is work which can be done rather quickly, and on which other work
     depends). </li>
     <li> "B" means is also urgent, but might use a bit more time, since
     also some planning is involved. </li>
     <li> "C" means to just follow the numerical order of the tickets. </li>
    </ol>
   </li>
  </ul>


  \todo Estimated time needed
  <ul>
   <li> Instead of "A" we write for example "A1.5h" to indicate, that roughly
   1 1/2 hours should be spend. </li>
   <li> Such effort estimation are needed to give a better idea what the
   task should involve, and also to see the total requirement for a
   developer. </li>
  </ul>

*/
