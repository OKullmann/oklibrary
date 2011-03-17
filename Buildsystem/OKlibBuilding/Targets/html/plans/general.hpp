// Matthew Henderson, 15.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/OKlibBuilding/Targets/html/plans/general.hpp
  \brief Plans for the documentation-extraction from the source-code


  \todo Problems with version 1.7.3
  <ul>
   <li> With an older Konqueror version pages are not shown correctly. </li>
   <li> Under Firefox, the notes about OKlibrary at page-bottom is not
   shown.
    <ol>
     <li> Actually it shows up at the very bottom, very small, and hardly
     readable. </li>
     <li> We need to investigate this, whether it is a bug of Doxyen, or
     whether we need to change something in our system. </li>
     <li> Has this to do with the old Firefox-version? </li>
    </ol>
   </li>
   <li> Under Firefox, the menus on the main-page extend to full depth and
   length, making them unusable.
    <ol>
     <li> Hopefully this can be configured. </li>
    </ol>
   </li>
   <li> Firefox takes a long time to open pages.
    <ol>
     <li> This is a rather old version --- is this better with newer versions?
     </li>
    </ol>
   </li>
   <li> So we wait at least until ok cs-wsok a new Linux is installed, and
   until the above problems have been solved. </li>
  </ul>


  \todo Move
  <ul>
   <li> Currently a part of Buildsystem/OKlibBuilding, the make-functionality
   under target "html" will move to Buildsystem/Html; see
   Buildsystem/plans/MasterScript.hpp. </li>
   <li> The target "html" will then be split into targets for the local home
   page and for the doxygen-system; this sub-module here will be responsible
   for the doxygen-system. Or not? See below. </li>
   <li> One the other hand, yet it's easier to handle html-goals via
   sub-makefiles of the OKlibBuilding-makefile, and also it's easier
   to have the rules for the local home page and for the doxygen-system? </li>
   <li> But compiling programs etc. has not much to do with creating
   documentation, so documentation building should become an independent
   makefile in Buildsystem/Html; this means then that the oklib-masterscript
   needs to distinguish between "normal" and "html" goals (for calling
   the appropriate makefiles) ? Hm, this might collide with the general
   rule, that oklib just hands down all targets? </li>
  </ul>


  \todo The role of documents_dir
  <ul>
   <li> The last action for target "documentation_index" is to copy
   the directory $(documents_dir) to $(doc_dir). </li>
   <li> This fills directory $(doc_dir)/documents. </li>
   <li> Why do we need $(doc_dir)/documents ? </li>
   <li> Currently the local home page gets the documents from this directory,
   which is "documents_html_dir". </li>
   <li> However, why don't we just use "documents_dir" ? </li>
  </ul>


  \todo Doxygen error messages:
  <ul>
   <li> Perhaps the output of doxygen could be configured (only error
   messages, all output, etc.). </li>
   <li> DONE Move aux/DoxygenErrorMessages to directory
   system_directories/log (and add suffix .txt; this filename
   is a configuration-variable). </li>
  </ul>


  \todo Doxygen problems
  <ul>
   <li> In OKDatabase/CreateStatistic.cpp we have, within an unnamed
   namespace, the private member v of class Densities --- nevertheless
   doxygen claims, that at every other place, where an object is called v,
   this private member is referenced??? This looks like a clear doxygen-bug
   to me. </li>
   <li> How to avoid false links (create by doxygen)?
    <ol>
     <li> For example in satz215.2.c a struct "node" is defined --- now we
     get a link to that class wherever the word "node" occurs --- for example
     it occurs here: node. </li>
     <li> For C++ we have unnamed namespaces, where the members should not
     be referenced --- we should check this (since it appears that all
     .cpp-files are considered as linked together?). </li>
     <li> However for the example "node", we want to have this documented
     for the file satz215.2.c --- but nowhere else! How to achieve this? </li>
     <li> Obviously, using "%node" everywhere is not a solution. Contact the
     doxygen mailing list. </li>
     <li> Another, definitely nonsensic case, is the use of "w.r.t.", for
     example here: w.r.t. In this case, a *private* data member called "t"
     is referenced (as here: x.t)! </li>
    </ol>
   </li>
   <li> There are errors regarding stability of links; see "Doxygen problems
   regarding stability of links" in Buildsystem/Html/Local/plans/general.hpp.
   </li>
  </ul>
  

  \todo Doxygen general
  <ul>
   <li> Instead of defining the exceptional macros by Doxyfile-tag PREDEFINED,
   we should use the tag EXPAND_AS_DEFINED (since we do not alter the
   definitions), however this does not work? </li>
   <li> Can doxygen tell which other files include a file (not in graph form,
   but in text form)? </li>
   <li> For functions there is a "callergraph" --- shall we use it? </li>
   <li> How to avoid that a leading "include" in a Doxygen-comment is
   interpreted as a doxygen-command? And how to avoid that apparently certain
   "keywords" are captured by doxygen? The keyword-interpretation can be
   avoided by prefixing them with "%". </li>
   <li> How to obtain general statistics: About the number of classes, lines
   of code, etc. (best with some statistics on the change over time) and then
   also on the version numbers of the modules (again with changes over time).
   If doxygen does not support it yet, then we should submit some feature
   request, and postpone this item. See what' in 1.5.3. </li>
   <li> Documentation for other programming languages?
    <ol>
     <li> makefiles ? </li>
     <li> cmake-files ? </li>
     <li> computer algebra ? </li>
    </ol>
    A simple way would be
     <ol>
      <li> just to allow verbatim processing of certain files </li>
      <li> additionally, one could have the possibility to define
      the doxygen-comments. </li>
     </ol>
   </li>
   <li> What is the role of the doxygen configuration tag BUILTIN_STL_SUPPORT?
     Should we enable this? : Enabled; it should help --- check. </li>
   <li> Can we have nested lists? Numbered lists? These things should go into
     our general documentation --- or we have some example files, which
     demonstrate our use of doxygen :
     For simple lists we do it as here, otherwise we use the html-elements.
     Documented in the main documentation-document.  However, how do they
     description-lists work?
   </li>
   <li> Doxygen can create "man pages" --- what does this %mean? </li>
   <li> See the doxygen-related problems in Messages (should be solvable by
   expanding those macros) : DONE </li>
   <li> How to integrate a *general* todo list into Doxygen? With xrefitem.
   However it seems we no longer need it : DONE </li>
   <li> Can doxygen extract a list with all modules and their current version
   numbers? Installed module_version : DONE </li>
   <li> We should get the newest configuration file, and transfer our
   information from the old configuration file : DONE </li>
  </ul>


  \todo Doxygen menu and main page review
  <ul>
   <li> Sorting of file/directory lists should distinguish between capital and
   small first letters (as it is possible with Unix/Linux), so that "plans"
   etc. comes at the end. </li>
   <li> Is the file list still needed (with version 0.1.4 of OKlib it
     was kind of useful to see for example how many files are documented)?
     Otherwise dropping it could make the system more user-friendly, by
     restricting the attention to the most appropriate choice? </li>
   <li> Sometimes it would be good to show also other files (especially for
   Buildsystem). This is mainly the issue of how to get doxygen showing
   makefiles etc. (perhaps just as plain text files, without any special
   formatting). </li>
   <li> The page "Module versions" should be sortable by version numbers (to
   see the most mature modules). Or how else can we (automatically) display
   the levels of development? </li>
   <li> DONE (doxygen_project_name now only contains the name, not the
   version number)
   On the main page the link list shows now the full name (with doxygen version
   1.5.4) in front of the page name (like "Class List"): How do we get back the
   old behaviour, which only shows the page name? </li>
  </ul>


  \todo Docus pages
  <ul>
   <li> Docus-pages should get a somehow distinguished design. </li>
   <li> For example the filename as title is not needed, and also not
   the brief explanation. </li>
   <li> Perhaps we should use the "\page"-directive ? </li>
  </ul>


  \todo Search engine
  <ul>
   <li> Doxygen can also use a "searchengine" (see activation in the
   "Doxyfile"): This would be very useful, but apparently needs some
   php-stuff? </li>
   <li> Moreover, a web server needs to be started? And then --- I installed
   php and appache on csltok, and put all pages to /srv/www/htdocs, but still
   konqueror just returns the search.php-file instead of doing something with
   it --- how should konqueror know that also for local files the web server
   should be used? </li>
  </ul>


  \todo Other code
  <ul>
   <li> We should also integrate (somehow) documentation of other code (for
   example all the computer-algebra code) into the doxygen-system (that is,
   giving access also to the code, not just to the plans). </li>
   <li> At least a simple "dump" should be possible ("as is"). </li>
   <li> Can doxygen be extended? We need some easy solutions. </li>
  </ul>

*/
