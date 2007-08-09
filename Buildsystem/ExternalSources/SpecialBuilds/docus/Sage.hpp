// Oliver Kullmann, 9.8.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Sage.hpp
  \brief Documentation on how to build Sage


  <h1> Installing Sage </h1>


  <h2> On the purpose of Sage </h2>

  Sage is a computer-algebra platform for uniform access to several platform.
  If necessary, we use it besides Maxima and Axiom/Aldor.


  <h2> What the installation yields </h2>

  <ul>
   <li> The main sage-executable (an interactive console) </li>
   <li> Documentation </li>
  </ul>

  <h2> Current state of installation </h2>

  <ul>
   <li> sage-program call = <code>$(sage_call)</code> </li>
   <li> ready: $(sage_call_ready)
    <ul>
     <li> location = $(location_sage_call) </li>
     <li> version = <code>$(version_sage_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(sage_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Only local installation currently.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> sage </code> </td>
    <td> Build the recommended version of sage. </td>
   </tr>
   <tr>
    <td> <code> sage-x.y.z </code> </td>
    <td> Build the version x.y.z of sage (for example
    <code>sage-2.7.2</code>). </td>
   </tr>
  </table>

*/

