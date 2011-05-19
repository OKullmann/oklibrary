// Oliver Kullmann, 9.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Git.hpp
  \brief Plans regarding installation of Git


  \todo Prerequisites
  <ul>
   <li> We need to discuss these in the Git installation information. </li>
   <li> What to do about xmlto --- is this available in recent
   distributions, so that we don't need to provide an installation
   ourselves (only documentation)?
    <ol>
     <li> Suse 10.0 has it. </li>
    </ol>
   </li>
   <li> curl + libcurl and expat development library (for pulling/pushing
   from/to  a git repository via the http protocol) is another prerequisite,
   which can be installed by the Linux distribution. </li>
   <li> openssl ? </li>
   <li> wish (as part of the Tcl/Tk system) ? </li>
   <li> DONE
   The build system can install asciidoc if needed; put it into
   the documentation. (Is it available in recent distributions?) </li>
  </ul>


  \todo Asciidoc
  <ul>
   <li> Make the installation of asccidoc local, with placement in
   OKplatform/bin.
    <ol>
     <li> Unfortunately, asciidoc doesn't allow local installation. </li>
     <li> But at least we should provide some documentation. </li>
     <li> And perhaps we should have direct access to the installation page
     (not just through Git). </li>
    </ol>
   </li>
   <li> http://www.methods.co.nz/asciidoc/ is the home page. </li>
  </ul>


  \todo Info- and man-pages
  <ul>
   <li> Using system-wide installation, are man-pages provided by our
   installation? </li>
   <li> And how to use these pages with a local installation?
    <ol>
     <li> With "more" or "less" these pages can be viewed. </li>
    </ol>
   </li>
  </ul>


  \todo Git book
  <ul>
   <li> Likely we should make the git-book directly available (via a
   link, not just mentioning the documentation directory at the Git
   installation page). </li>
  </ul>


  \todo Installation process
  <ul>
   <li> There are several build-possibilities (listed in the INSTALL-file
   of the git-package); the one we used was the only one working: Is this
   still true? What are the reasons? </li>
  </ul>


*/

