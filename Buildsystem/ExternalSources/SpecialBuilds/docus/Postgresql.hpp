// Oliver Kullmann, 6.9.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Postgresql.hpp
  \brief Documentation on how to build Postgresql


  <h1> Installing Postgresql </h1>


  <h2> On the purpose of Postgresql </h2>

  Postgresql is a powerful SQL database, used by the OKlibrary to store experimental data.


  <h2> What the installation yields </h2>

  <ul>
   <li> Several executables
    <ol>
     <li> XXX </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> postgresql interface call = <code>$(postgresql_call)</code> </li>
   <li> ready: $(postgresql_call_ready)
    <ul>
     <li> location = $(location_postgresql_call) </li>
     <li> version = <code>$(version_postgresql_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(postgresql_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

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

