// Oliver Kullmann, 21.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/docus/Linux.hpp
  \brief Documentation on techniques regarding Linux (and Bash) which proved useful


  <h1> Linux- and tools-related techniques </h1>

  Here we collect techniques for solving problems we encountered at the
  level of the operating system (Linux) or of the shell (Bash) or of
  the buildtool (Make).


  <h2> Some general Linux-techniques </h2>

  <ul>
   <li> Linker segmentation faults might indicate that a newer version of
   binutils needs to be installed (system-wide standard installation).
   For example installing binutils-2.19.1 solved a linker segmentation fault
   when using Boost. </li>
   <li> Linker failures because of "some symbols can't be read" might be due to
   a incorrect build of the respective link-library, namely that on
   AMD64-platforms the compiler option "-fPIC" is required. </li>
   <li> Runtime link-errors might result in segmentation faults, and might
   be caused by not providing the "biarch" gcc-versions (on 64-bit machines,
   providing 32-bit compatabilit mode). </li>
  </ul>


  <h2> Some general Bash-techniques </h2>

  <ul>
   <li> File-renaming functionality:
    <ol>
     <li> The mmv-package was useful, but is apparently not available for
     download anymore (in a maintained form). </li>
     <li> So we just use <code>find</code> (or just <code>ls</code> together
     with <code>awk</code>) together with <code>sed</code> (and a loop) for
     achieving the results. </li>
     <li> For example, for creating symbolic links for files with
     name-components "gcc43", "gcc34" by using just "gcc", use
     \verbatim
for F in $(ls | awk '/.*gcc[0-9][0-9].*/'); do
  ln - ${F} $(echo ${F} | sed 's/gcc[0-9][0-9]/gcc/'); done
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>

*/


