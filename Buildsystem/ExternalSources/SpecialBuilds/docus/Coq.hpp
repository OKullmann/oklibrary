// Oliver Kullmann, 31.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Coq.hpp
  \brief Documentation on how to build Coq


  <h1> Installing Coq </h1>


  <h2> On the purpose of Coq </h2>

  XXX


  <h2> What the installation yields </h2>

  <ul>
   <li> The coq interpreter
    <ol>
     <li> the <code>coqtop</code> program: XXX </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> coq-program call = <code>$(coq_call)</code> </li>
   <li> ready: $(coq_call_ready)
    <ul>
     <li> location = $(location_coq_call) </li>
     <li> version = <code>$(version_coq_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(coq_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Only system-wide installation currently.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> coq </code> </td>
    <td> Build the recommended version of coq, using ocaml ???. </td>
   </tr>
   <tr>
    <td> <code> coq-x.y.z </code> </td>
    <td> Build the version x.y.z of coq, using ocaml ??? (for example
    <code>coq-8.1</code>). </td>
   </tr>
  </table>

*/

