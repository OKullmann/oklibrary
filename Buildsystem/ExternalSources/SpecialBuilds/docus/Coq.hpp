// Oliver Kullmann, 31.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Coq.hpp
  \brief Documentation on how to build Coq


  <h1> Installing Coq </h1>


  <h2> On the purpose of Coq </h2>

  Proof assistant for upper bound proofs (and extraction of
  related programs).


  <h2> What the installation yields </h2>

  <ul>
   <li> The coq interpreter
    <ol>
     <li> the <code>coqtop</code> program: The interactive console. </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version: $(coq_recommended_version_number) </li>
   <li> coq-program call = <code>$(coq_call)</code> </li>
   <li> ready: $(coq_call_ready)
    <ul>
     <li> location = $(location_coq_call) </li>
     <li> version = <code>$(version_coq_call)</code>
     </li>
    </ul>
   </li>
   <li> The installation directory: $(coq_installation_dir) </li>
   <li> documentation: $(coq_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Only local installation currently. </li>
   <li> The programming language Ocaml needs to be installed; see
   the installation page $(ocaml_docu_page_tag). </li>
   <li> The default is to use the local (%OKlib) installation of Ocaml. </li>
   <li> For Coq version 8.1 the latest version
   of Ocaml which can be used is 3.09.3 (the successor version 3.10.0
   has removed the deprecated usage of "pa_ifdef.cmo" used by Coq). </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> coq </code> </td>
    <td> Build the recommended version of coq. </td>
   </tr>
   <tr>
    <td> <code> coq-x.y.z </code> </td>
    <td> Build the version x.y.z of coq (for example
    <code>coq-8.1</code>). </td>
   </tr>
  </table>

*/

