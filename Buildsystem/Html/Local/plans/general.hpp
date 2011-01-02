// Matthew Henderson, 14.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Html/Local/plans/general.hpp
  \brief Plans for the local html pages (around the local home page)


  \todo Checking all links
  <ul>
   <li> We need to run from time to time a tool for checking
   the links on the local html pages. </li>
   <li> And this on different types of machines. </li>
  </ul>


  \todo Doxygen problems regarding stability of links
  <ul>
   <li> Directory-links are not the same on 32-bit and 64-bit platforms;
   see "Reference to 10Challenges-directory does not work" in
   Buildsystem/Html/Local/plans/TenChallenges.hpp. </li>
   <li> DONE (apparently nothing has changed with version 1.6.3)
   This has been acknowledged as a Doxygen-bug, and shall be submitted
   to the Doxygen bug-tracking system. Though we need to check whether with
   newer doxygen-versions the problem might have been solved. </li>
   <li> List of link-problems (using the make-variables for the addresses):
    <ol>
     <li> tenchallenges_doxygen_directory (see above) </li>
     <li> licence_docu_page:
      <ol>
       <li> On cs-wsok and csltok we have
       doxygen_html/d2/d59/OKlib_2System_2LegalIssues_2docus_2general_8hpp.html
       </li>
       <li> On cspcab2 and cs-oksvr we have
       doxygen_html/d3/dae/System_2LegalIssues_2docus_2general_8hpp.html
       </li>
      </ol>
     </li>
     <li> main_docu_page:
      <ol>
       <li> On csltok and cs-wsok we have
       doxygen_html/d5/d70/OKlib_2docus_2general_8hpp.html
       </li>
       <li> On another installation on csltok we have
       doxygen_html/d0/dfd/docus_2general_8hpp.html
       </li>
       <li> Also for a single installation on another 64-bit machine and
       on cs-oksvr we get the same address. </li>
       <li> Still the same problem with doxygen 1.7.1. </li>
      </ol>
     </li>
     <li> main_demo_page:
      <ol>
       <li> On csltok and cs-wsok we have
       doxygen_html/de/d71/OKlib_2demos_2general_8hpp.html
       </li>
       <li> For a different installation on csltok we get
       doxygen_html/dc/d5b/demos_2general_8hpp.html
       </li>
       <li> The same path also on the other 64-bit-machine already mentioned
       above and on cs-oksvr. </li>
       <li> Still the same problem with doxygen 1.7.1. </li>
      </ol>
     </li>
     <li> Interesting that these two variations seem to be stable (there seem
     to be *only* these two possibilities)?! </li>
     <li> DONE (the problem shouldn't be related to having multiple
     installations)
     Since we are using the local doxygen-installation (due to using
     the variable doxygen_call_okl), there should be no interference between
     the two installations. </li>
    </ol>
   </li>
   <li> When introducing the new page
   Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp, then the
   address for Buildsystem/ExternalSources/SpecialBuilds/docus/Ecl.hpp
   changed, so there is a problem with files with the same names.
   Notify the Doxygen mailing list. </li>
  </ul>


  \todo Better general documentation
  <ul>
   <li> Also a special overview and introduction would be needed;
   ideal a video. </li>
  </ul>

*/
