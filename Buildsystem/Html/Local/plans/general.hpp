// Matthew Henderson, 14.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010 Oliver Kullmann
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
   <li> This has been acknowledged as a Doxygen-bug, and shall be submitted
   to the Doxygen bug-tracking system. Though we need to check whether with
   newer doxygen-versions the problem might have been solved. </li>
   <li> List of such problems (by the make-variables for the addresses):
    <ol>
     <li> tenchallenges_doxygen_directory </li>
     <li> licence_docu_page: The currently stored path starts with
     "doxygen_html/d9/da9/", however on csltok we have the path
     "doxygen_html/d2/d59" ? Also on cs-wsok there is the same path, so
     perhaps here we just update the path. Check now on csltok. </li>
     <li> main_docu_page: This seems to be a different issue --- on csltok
     the current value, with path
     "doxygen_html/d5/d70/OKlib_2docus_2general_8hpp.html" works for one
     installation, however for another installation we get
     "doxygen_html/d0/dfd/docus_2general_8hpp.html" ? </li>
     <li> main_demo_page: Seems to be the same problem; on csltok the current
     value, with path "doxygen_html/de/d71/OKlib_2demos_2general_8hpp.html"
     works, however for a different installation we get the path
     "doxygen_html/dc/d5b/demos_2general_8hpp.html" ? </li>
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
