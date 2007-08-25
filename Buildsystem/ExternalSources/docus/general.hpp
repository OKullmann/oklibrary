// Oliver Kullmann, 9.4.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/docus/general.hpp
  \brief General documentation for the makefile "Buildsystem/ExternalSources/Makefile", responsible for external builds
  
  <h1> General overview </h1>


  <h2> Configuration </h2>

  Enter into OKplatform/.oklib/override.mak the overriding definitions, for example
  \code
ocaml_bin_dir := /usr/local/bin
  \endcode
  to use the system-wide ocaml-installation instead of the local installation.
  (This prevents these variables from being defined by the configuration system.)

  
  <h2> Installations </h2>

  <ol>
   <li> Boost (Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp) </li>
   <li> Coq (Buildsystem/ExternalSources/SpecialBuilds/docus/Coq.hpp) </li>
   <li> Doxygen (Buildsystem/ExternalSources/SpecialBuilds/docus/Doxygen.hpp) </li>
   <li> Gcc (Buildsystem/ExternalSources/SpecialBuilds/docus/Gcc.hpp) </li>
   <li> Git (Buildsystem/ExternalSources/SpecialBuilds/docus/Git.hpp) </li>
   <li> Gmp (Buildsystem/ExternalSources/SpecialBuilds/docus/Gmp.hpp) </li>
   <li> Ocaml (Buildsystem/ExternalSources/SpecialBuilds/docus/Ocaml.hpp) </li>
   <li> Sage (Buildsystem/ExternalSources/SpecialBuilds/docus/Sage.hpp) </li>
   <li> Ubcsat (Buildsystem/ExternalSources/SpecialBuilds/docus/Ubcsat.hpp) </li>
  </ol>
  

  <h2> Mhash </h2>

  Only local installation.
  
  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> mhash </code> </td>
    <td> Build the recommended version of mhash, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> mhash-a.b.c.d </code> </td>
    <td> Build version a.b.c.d of mhash, using the system-gcc (using for
    example <code>mhash-0.9.7.1</code>). </td>
   </tr>
   <tr>
    <td> <code> mhash gcc-version=x.y.z </code> </td>
    <td> Build the recommended version of mhash, using the local gcc-version x.y.z (for example use <code>gcc-version=4.1.2</code>). </td>
   </tr>
   <tr>
    <td> <code> mhash gcc-version=all </code> </td>
    <td> Build the recommended version of mhash for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> mhash-a.b.c.d gcc-version=x.y.z </code> </td>
    <td> Build version a.b.c.d of mhash, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> mhash-a.b.c.d gcc-version=all </code> </td>
    <td> Build version a.b.c.d of mhash for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> mhash_gcc_all </code> </td>
    <td> Build all supported versions of mhash for all all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
  </table>


  <h2> Valgrind </h2>

  Only system-wide installation.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> valgrind </code> </td>
    <td> Build the recommended version of valgrind, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> valgrind-x.y.z </code> </td>
    <td> Build the version x.y.z of valgrind, using the system-gcc (for example
    <code>valgrind-3.2.3</code>). </td>
   </tr>
  </table>


  <h2> PostgreSQL </h2>

  Only local installation.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> postgresql </code> </td>
    <td> Build server and client for the recommended version of postgresql, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> postgresql-x.y.z </code> </td>
    <td> Build server and client for postgresql version x.y.z, using the system-gcc (for example <code>postgresql-8.0.3</code>). </td>
   </tr>
   <tr>
    <td> <code> initialise-database </code> </td>
    <td> Initialise the database and start the server, using the recommended
    postgresql-version (x.y.z) and the default location
    ExternalSources/Postgresql/x.y.z/data of the database. </td>
   </tr>
   <tr>
    <td> <code> initialise-database pgsql-version=x.y.z </code> </td>
    <td> Initialise the database and start the server, using the postgresql-version
    x.y.z and the location ExternalSources/Postgresql/x.y.z/data of the database. </td>
   </tr>
   <tr>
    <td> <code> initialise-database pgdata=P </code> </td>
    <td> Initialise the database and start the server, using the recommended
    postgresql-version and the location of the database given by the absolute
    path P. </td>
   </tr>
   <tr>
    <td> <code> initialise-database pgdata=P pgsql-version=x.y.z </code> </td>
    <td> Initialise the database and start the server, using the postgresql-version
    x.y.z and the location of the database given by the absolute path P. </td>
   </tr>
  </table>

*/

