// Oliver Kullmann, 18.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp
  \brief Plans regarding installation of gmp


  \todo Update
  <ul>
   <li> Update make-variable names to the "new standard". </li>
  </ul>


  \todo Installation of Mpfr
  <ul>
   <li> Since apparently mpfr belongs to gmp, could it be automatically
   installed with gmp? </li>
   <li> Update mpfr-version. </li>
   <li> Update the mpfr-installation to the new standard. </li>
   <li> We should now by default provide local installation:
    <ol>
     <li> First we need to find out who needs mpfr. </li>
     <li> R needs it (see "Prerequisites" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp). </li>
     <li> Also the gcc Fortran compiler need it (see "Installation in general"
     in Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp). </li>
    </ol>
   </li>
  </ul>


  \todo C++
  <ul>
   <li> If we enable C++ support, then we must also, similar to Boost,
   enable compilation with the different local compilers. </li>
   <li> And for linking then linking to different versions, according
   to the compiler used, is needed. </li>
   <li> Perhaps first we just use the C functionality, and write
   our own (very simple) basic wrapper? </li>
   <li> On the other hand, this is duplication, and we should avoid this. </li>
   <li> So let's enable the C++ support, and rethink the general scheme
   of linking etc.: This should be similar as with Boost, and thus
   we can rethink the whole approach. </li>
   <li> For setting the local compiler, we need to use CC_FOR_BUILD and
   CPP_FOR_BUILD for configure. </li>
   <li> Instead of making these case-distinctions in OKlibBuilding/Makefile,
   it should be possible to delegate everything to the use of
   recursive make-variables! </li>
  </ul>


  \todo Write docus page : DONE
  <ul>
   <li> System-installation default: DONE
   <ol>
    <li> gmh.h in include </li>
    <li> libgmp.a and variations in lib </li>
    <li> gmp.info, gmp.info-1, gmp.info-2 in info </li>
   </ol>
  <li>
  <li>  DONE (sufficient for now)
  Describe how to use the gmp-library under the different circumstances. </li>
  <li> DONE (sufficient for now)
  Describe usage of make-variable gmp_default_install, from the
  command line and from the configuration file. </li>
  </ul>


  \todo Local and system-wide : DONE
  <ul>
   <li> DONE This method should be also usable for all other installations
   (once finished here, update Buildsystem/ExternalSources/plans/general.hpp). </li>
   <li> DONE (not tested yet, but provided with gmp_link_option)
   As with Boost, in case of a local installation, not only
   the path to the link-library is needed, but also when linking
   the rpath-option. Perhaps this is simply provided by another
   make-variable. </li>
   <li> DONE We need the possibility to choose local installation
   as well as system-wide installation. </li>
   <li> DONE (used a variable gmp_default_install)
   Perhaps a variable "global_installation" is used, and when
   set, the sudo-command is used and variable install_dir is (if not
   already set) set to "/usr/local"; if not set then (if not already
   set) the value is "gmp_base_directory/version-number". </li>
  </ul>


  \todo Installation : DONE
  <ul>
   <li> Create an installation directory (for example, "Gmp/4.2.1"). </li>
   <li> For cleanliness, we copy the documentation to the doc-directory
   (as we do it with Boost). </li>
  </ul>

*/

