// Oliver Kullmann, 1.8.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Ocaml.hpp
  \brief Documentation on how to build Ocaml


  <h1> Installing Ocaml </h1>


  <h2> On the purpose of Ocaml </h2>

  XXX programming language in which Coq is implemented


  <h2> What the installation yields </h2>

  3 directories:
  <ul>
   <li> with binaries (only needed here) </li>
   <li> with link-libraries </li>
   <li> with man-pages. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version: $(ocaml_recommended_version_number) </li>
   <li> The three provided installation directories:
    <ol>
     <li> binaries in $(ocaml_bin_dir) </li>
     <li> link-libraries in $(ocaml_lib_dir) </li>
     <li> man-pages in $(ocaml_man_dir). </li>
    </ol>
   </li>
   <li> Checking the installation:
    <ol>
     <li> ocaml-program call = <code>$(ocaml_call)</code> </li>
     <li> ready: $(ocaml_call_ready)
      <ul>
       <li> location = $(location_ocaml_call) </li>
       <li> version = <code>$(version_ocaml_call)</code> </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  <h2> How to install </h2>

  Only local installation currently. Ocaml comes with Linux distributions, so
  you could simply install them with your distribution tool and reset the
  directories for the binaries (this is all what is needed for Coq).

  When installing Ocaml by our build system, then libraries "tk-devel, tcl-devel"
  need to be installed (these are their names for the Suse Linux distribution).

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> ocaml </code> </td>
    <td> Build the recommended version of ocaml. </td>
   </tr>
   <tr>
    <td> <code> ocaml-x.y.z </code> </td>
    <td> Build the version x.y.z of ocaml, using ocaml ??? (for example
    <code>ocaml-3.09.3</code>). </td>
   </tr>
  </table>

*/

