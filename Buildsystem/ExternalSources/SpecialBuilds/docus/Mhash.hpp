// Oliver Kullmann, 6.9.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Mhash.hpp
  \brief Documentation on how to build and use Mhash


  <h1> Installing and using Mhash </h1>


  <h2> On the purpose of Mhash </h2>

  Mhash is a C library with cryptographic tools XXX


  <h2> What the installation yields </h2>

  <ul>
   <li> The mhash header files and the corresponding link libraries XXX </li>
   <li> Documentation XXX </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> To include a mhash-header use for example
   \code
#include <mhash/XXX>
   \endcode
   together with "${mhash_include_option}" XXX (currently "$(Mhash)" XXX). </li>
   <li> To link with %Mhash, use "${mhash_link_option}" XXX for each sub-library separately XXX. </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version = <code>$(mhash_recommended_version_number)</code> </li>
   <li> %mhash include option = <code>$(mhash_include_option)</code> XXX </li>
   <li> %mhash link option = <code>$(mhash_link_option)</code> XXX </li>
   <li> ready: $(mhash_ready)
    <ul>
     <li> mode = <code>$(mhash_default_install)</code> </li>
     <li> version = <code>$(version_mhash)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(mhash_html_documentation_index_location_tag) </li>
   <li> list of supported versions XXX </li>
  </ul>


  <h2> How to install </h2>

  <ul>
   <li> Local and system-wide installation possible; the default is local
   installation if not configure otherwise. XXX yet only local installation XXX </li>
   <li> Set variable <code>mhash_default_install</code> to value "system" to switch. XXX </li>
   <li> If some required local installation of gcc is not available, then it is build first. XXX </li>
  </ul>

  <h3> Make targets </h3>

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

*/

