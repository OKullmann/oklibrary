// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007. 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/PostgreSQL.hpp
  \brief Plans regarding installation of the PostgreSQL package


  \todo Missing pgsql/libpq-fe.h
  <ul>
   <li> Install the source-libraries for postgresql, and use them for
   the compilation of the OKlibrary. </li>
   <li> Then re-enable compilation of Database/CreateStatistics.cpp (and
   the other programs there). </li>
   <li> And re-enable testing of General/DatabaseHandler01.hpp. </li>
  </ul>


  \todo Improvements
  <ul>
   <li> %Test the updated PostgreSQL installation. </li>
   <li> "make initialise-database" should work with the recommended version (and no specification
   of the pgsql-version should be needed). </li>
   <li> A detailed user-manual is needed (on how to use the installed database). </li>
  </ul>

*/

