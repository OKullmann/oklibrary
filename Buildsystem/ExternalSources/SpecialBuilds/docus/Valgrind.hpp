// Oliver Kullmann, 6.9.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Valgrind.hpp
  \brief Documentation on how to build Valgrind


  <h1> Installing Valgrind </h1>


  <h2> On the purpose of Valgrind </h2>

  Valgrind is a debugging tool used by the OKlibrary. XXX


  <h2> What the installation yields </h2>

  <ul>
   <li> The executable <code>valgrind</code>, a command-line tool for running
   programs in a monitored way (checking for example access to undefined
   variables, and memory access violations). </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> valgrind-program call = <code>$(valgrind_call)</code> </li>
   <li> ready: $(valgrind_call_ready)
    <ul>
     <li> location = $(location_valgrind_call) </li>
     <li> version = <code>$(version_valgrind_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(valgrind_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Only system-wide installation currently.

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

*/

