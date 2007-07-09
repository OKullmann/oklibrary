// Oliver Kullmann, 28.6.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/PostgreSQL.hpp
  \brief Plans regarding installation of the PostgreSQL package


  \bug Missing pgsql/libpq-fe.h
   - For MG, pgsql/libpq-fe.h was not found when using "make postgresql".
   - Obviously, the postgresql-installation shouldn't need anything
   from postgresql already being installed?


  \todo Improvements
  <ul>
   <li> %Test the updated PostgreSQL installation. </li>
   <li> "make initialise-database" should work with the recommended version (and no specification
   of the pgsql-version should be needed). </li>
   <li> A detailed user-manual is needed (on how to use the installed database). </li>
  </ul>

*/

