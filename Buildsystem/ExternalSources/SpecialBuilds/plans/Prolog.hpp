// Oliver Kullmann, 10.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Prolog.hpp
  \brief Plans regarding installation of Prolog systems


  \todo Swi Prolog
  <ul>
   <li> http://www.swi-prolog.org/ </li>
   <li> It seems we should install the latest stable version 5.6.63. </li>
   <li> DONE (see INSTALL file in package)
   On their web page I can't find any information on how to install
   the package? </li>
   <li> For example it uses Gmp, and we want to use our locally installed
   Gmp. </li>
   <li> Manual installation:
   \verbatim
builds/Prolog> tar -xzf ../../sources/Prolog/Swi/pl-5.6.63.tar.gz
builds/Prolog/pl-5.6.63/src> ./configure --prefix=Path-to-ExternalSources/builds/Prolog/
builds/Prolog/pl-5.6.63/src> make
builds/Prolog/pl-5.6.63/src> make check
builds/Prolog/pl-5.6.63/src> make install
   \endverbatim
   works.
   </li>
  </ul>


  \todo Gnu Prolog
  <ul>
   <li> http://www.gprolog.org/ </li>
  </ul>

*/

