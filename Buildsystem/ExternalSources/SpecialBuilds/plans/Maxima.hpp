// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Install xgettext
  <ul>
   <li> To create new Maxima packages, xgettext, available at
   http://www.gnu.org/software/gettext/ , is needed </li>
  </ul>

  
  \todo Communicate with Maxima/Ecl to fix load behaviour with symlinks on path
  <ul>
   <li> The issue is that load-command fails (%e.g., "load(descriptive)").
   </li>
   <li> The problem occurs if one has a symbolic link on the path during the
   building of maxima and ecl. </li>
   <li> MG is communicating with the Maxima mailing list on the issue and
   hopefully this will be resolved in the next Maxima. </li>
   <li> For now, the symlink to OKplatform can simply be removed, as it is
   only a convenience. </li>
   <li> Additionally, if one wishes to use a symlink on the build path,
   the "file_search_maxima" variable may be "fixed" at runtime by using an
   old "hard-coded" version of a particular lisp function for listing the
   directories (Thanks to Robert Dodier):
   \verbatim
:lisp (setf (symbol-function 'share-subdirs-list) (symbol-function 'default-share-subdirs-list))
:lisp (set-pathnames)
   \endverbatim
   and then load functionality works fine. </li>
  </ul>


  \todo Database of integer sequences
  <ul>
   <li> We have to see whether the sequences are becoming (publically)
   available. </li>
   <li> If this is not the case, then likely we shouldn't contribute to
   that database. </li>
  </ul>


  \todo Additional packages
  <ul>
   <li> Real algebraic geometry
    <ol>
     <li> The book [Algorithms in Real Algebraic Geometry; Basu, Pollack, Roy]
     is available at http://www.math.gatech.edu/~saugata/bpr-posted1.html.
     </li>
     <li> Also maxima-code for various chapters is available. </li>
    </ol>
   </li>
  </ul>


  \todo System-wide installation
  <ul>
   <li> The build-system should be extended, taking for example the
   Gmp-installation as an example, so that we can also produce/use in the
   standard way system-wide installations of Libsigsegv, CLisp and Maxima.
   </li>
  </ul>


  \todo Handling of redefined load-function
  <ul>
   <li> See "How to eliminate the annotation of lists" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> And see the e-mail thread (May 2008)
   "how to stop the annotation of lists by filenames?" on the Maxima
   mailing list. </li>
   <li> We need documentation on this regarding the build-system:
    <ol>
     <li> We need to watch the Maxima distribution, whether the
     two functions involved, "load" and "add-lineinfo", ever are
     changed, and then we need to incorporate these changes. </li>
     <li> Hopefully, this is not a big thing, since these functions
     are rather small. </li>
     <li> The good point about this solution is its non-intrusiveness:
     The Maxima system doesn't know about these changes. </li>
    </ol>
   </li>
  </ul>

*/
