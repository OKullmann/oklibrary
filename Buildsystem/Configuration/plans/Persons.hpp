// Oliver Kullmann, 3.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Configuration/plans/Persons.hpp
  \brief Plans regarding management of personal data


  \todo Developers information : DONE
  <ul>
   <li> Complete information in "Developers.html". DONE </li>
   <li> Link appropriately to "Developers.html" (from the local
   home page; see Buildsystem/Html/plans/LocalHomePage.hpp). DONE </li>
   <li> Compare with "Developers" in Buildsystem/plans/Release.hpp. DONE </li>
   <li> DONE (decided to produce an html-file "Developers.html" right away
   due to greater flexibility, and it seems we do not automatically process
   this information; we also don't need to distinguish between current
   and past developers --- this might change, and one can see this in
   the repository-information)
   We should have a subdirectory Configuration/Developers, containing files
    <ol>
     <li> current_main </li>
     <li> past_main </li>
     <li> past_external </li>
     <li> current_external </li>
    </ol>
    Each of those files contains lines like
    <ol>
     <li> OK "Oliver Kullmann" O.Kullmann@Swansea.ac.uk </li>
     <li> MH "Matthew Henderson" XXX </li>
     <li> ML "Matthew Lewsey" XXX </li>
    </ol>
    Or is there some "official" file format for such address-data?!
   </li>
   <li> Out of these files an html-file is produced. </li>
  </ul>

*/


