// Oliver Kullmann, 9.4.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/docus/general.hpp
  \brief General documentation for the makefile Buildsystem/ExternalSources/Makefile, responsible for external builds
  
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
   <li> Gcc (Buildsystem/ExternalSources/SpecialBuilds/docus/Gcc.hpp) </li>
   <li> Doxygen (Buildsystem/ExternalSources/SpecialBuilds/docus/Doxygen.hpp) </li>
  </ol>
  

  <h2> Boost </h2>

  Only local installation. If some required local installation of gcc is not available, then it is build first.

  <h3> Make targets </h3>

  The targets are <code>%boost</code>, <code>boost_all</code> and <code>boost_gcc_all</code>, with modifiers <code>boost_recommended=</code> and <code>gcc-version=</code>.

  <table>
   <tr>
    <td> <code> %boost </code> </td>
    <td> Build the recommended Boost-version, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost-V </code> </td>
    <td> Build the Boost-version V, using the system-gcc (using for example <code>boost-1_33_1</code>). </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V </code> </td>
    <td> Same as <code>%boost-V</code>. </td>
   </tr>
   <tr>
    <td> <code> boost_all </code> </td>
    <td> Build all supported Boost-versions, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost gcc-version=x.y.z </code> </td>
    <td> Build the recommended Boost-version, using the local gcc-version x.y.z (using for example <code>gcc-version=4.1.2</code>). </td>
   </tr>
   <tr>
    <td> <code> %boost gcc-version=all </code> </td>
    <td> Build the recommended Boost-version for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V gcc-version=x.y.z </code> </td>
    <td> Build the Boost-version V, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> %boost-V+x.y.z </code> </td>
    <td> Build the Boost-version V, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> %boost %boost_recommended=boost-V gcc-version=all </code> </td>
    <td> Build the Boost-version V, for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> boost_gcc_all </code> </td>
    <td> Build all supported Boost-versions for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
  </table>


  <h2> UBCSAT </h2>

  Only local installation.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> ubcsat </code> </td>
    <td> Builds the source-library, the link-library and the ubcsat-program in
    ExternalSources/Ubcsat/1-0-0. </td>
   </tr>
  </table>

  <h3> Build explanations </h3>

  In ExternalSources/Ubcsat/1-0-0 one finds the extracted archive ubcsat-1-0-0.tar.gz,
  which contains pre-compiled binaries for Unix/Linux systems and Windows systems.
  There are no changes, except that the source-files in sub-directory src are converted
  to Unix/Linux line-endings.

  The build adds sub-directories bin and lib:
  <ol>
   <li> In bin the ubcsat-executable is compiled with corrected sources. </li>
   <li> In lib one finds the corresponding link-library, also compiled with the
   corrected source, and now defining the macro ALTERNATEMAIN, so that no main
   function is created. </li>
  </ol>

  The corrected sources come from Transitional/LocalSearch/Ubcsat/corrected.

  <h3> Usage explanations </h3>

  Calling the binary with "ubcsat -h" will show the list of options.

  To use Ubcsat-components from inside the OKlibrary, in the ".link_libraries"-file the specification
  \code echo -L${Ubcsat_dir}/1-0-0/lib -lubcsat \endcode
  is needed, which enables linking with all functions and variables provided by the ubcsat-library. For the source-libraries add
  \code -I- -I$(OKsystem)/Transitional/LocalSearch/Ubcsat/local -I$(OKsystem)/Transitional/LocalSearch/Ubcsat/corrected $(Ubcsat) \endcode
  to the definition of <code>source_libraries</code> in <code>definitions.mak</code>:
  This activates the localised version of header files from the Ubcsat-library, and,
  if not already overwritten by a localised version, the corrected files (while
  the other files are taken from ExternalSources/Ubcsat/1-0-0/src).

  The main header file for the Ubcsat-library is
  Transitional/LocalSearch/Ubcsat/local/ubcsat.h.


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

