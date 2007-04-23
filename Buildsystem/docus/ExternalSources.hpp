// Oliver Kullmann, 9.4.2007 (Swansea)

/*!
  \file Buildsystem/docus/ExternalSources.hpp
  \brief General documentation for the makefile Buildsystem/ExternalSources.mak, responsible for external builds
  
  <H1> General overview </H1>
  
  XXX base directory OKplatform/ExternalSources
  
  XXX version numbers in Buildsystem/external_sources_versions.mak

  Buildsystem/ExternalSources.mak is the central makefile
  linked to by ExternalSources/makefile. Here are defined the main
  targets for building and cleaning of external libraries.

  Buildsystem/ExternalSources.mak includes 
  Buildsystem/external_sources_versions.mak for variable definitions of
  supported and recommended version numbers and for version
  number dependendent variable definitions.

  Buildsystem/ExternalSources.mak includes 
  Buildsystem/ExternalSources/definitions_.mak for additional variable 
  and function definitions.

  Transitional/Buildsystem/ExternalSources.mak includes:
  <ul>
  <li>Buildsystem/ExternalSources/mhash.mak
  <li>Buildsystem/ExternalSources/gcc.mak
  <li>Buildsystem/ExternalSources/doxygen.mak
  <li>Buildsystem/ExternalSources/boost.mak
  <li>Buildsystem/ExternalSources/postgresql.mak
  <li>Buildsystem/ExternalSources/valgrind.mak
  <li>Buildsystem/ExternalSources/ubcsat.mak
  </ul>
  Each of these files contains the definitions required to build an
  external library. 


  <H1> Installations </H1>
  
  
  <H2> GCC </H2>
  
  Only local installation.
  
  <H3> Make targets </H3>

  <table>
   <tr>
    <td> <code> gcc </code> </td>
    <td> Build the recommended gcc-version (using the system-gcc). </td>
   </tr>
   <tr>
    <td> <code> gcc_x.y.z </code> </td>
    <td> Build the gcc version x.y.z (for example <code>gcc_4.1.2</code>; using the system-gcc). </td>
   </tr>
   <tr>
    <td> <code> gcc_all </code> </td>
    <td> Build all supported gcc-versions (using the system-gcc). </td>
   </tr>
  </table>

  <H3> Usage </H3>

  XXX setting the link-path XXX


  <H2> Boost </H2>

  Only local installation. If some required local installation of gcc is not available, then it is build first.

  <H3> Make targets </H3>

  The targets are <code>%boost</code>, <code>boost_all</code> and <code>boost_gcc_all</code>, with modifiers <code>boost_recommended=</code> and <code>gcc-version=</code>.

  <table>
   <tr>
    <td> <code> %boost </code> </td>
    <td> Build the recommended Boost-version, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> %boost boost_recommended=V </code> </td>
    <td> Build the Boost-version V, using the system-gcc (using for example <code>boost_recommended=1_33_1</code>). </td>
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
    <td> <code> %boost boost_recommended=V gcc-version=x.y.z </code> </td>
    <td> Build the Boost-version V, using the local gcc-version x.y.z. </td>
   </tr>
   <tr>
    <td> <code> %boost boost_recommended=V gcc-version=all </code> </td>
    <td> Build the Boost-version V, for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> boost_gcc_all </code> </td>
    <td> Build all supported Boost-versions for all supported local gcc-versions as well as the system-gcc. </td>
   </tr>
  </table>


  <H2> UBCSAT </H2>

  Only local installation.

  <H3> Make targets </H3>
  
  <table>
   <tr>
    <td> <code> ubcsat </code> </td>
    <td> Builds the source-library, the link-library and the ubcsat-program in
    ExternalSources/Ubcsat/1-0-0. </td>
   </tr>
  </table>

  <H3> Build explanations </H3>

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

  <H3> Usage explanations </H3>

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


  <H2> Mhash </H2>

  Only local installation.
  
  <H3> Make targets </H3>
  
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


  <H2> Doxygen </H2>

  Only system-wide installation.

  <H3> Make targets </H3>
  
  <table>
   <tr>
    <td> <code> doxygen </code> </td>
    <td> Build the recommended version of doxygen, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> doxygen-x.y.z </code> </td>
    <td> Build the version x.y.z of doxygen, using the system-gcc (for example
    <code>doxygen-1.5.2</code>). </td>
   </tr>
  </table>


  <H2> Valgrind </H2>

  Only system-wide installation.

  <H3> Make targets </H3>
  
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


  <H2> PostgreSQL </H2>

  Only local installation.

  <H3> Make targets </H3>
  
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

  
