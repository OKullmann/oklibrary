// Oliver Kullmann, 16.8.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Git.hpp
  \brief Documentation on how to build Git


  <h1> Installing Git </h1>


  <h2> On the purpose of Git </h2>

  Git is the distributed source control system used by the OKlibrary.


  <h2> What the installation yields </h2>

  <ul>
   <li> Three executables
    <ol>
     <li> the <code>git</code> program: command-line tool for complete access to the source control system; </li>
     <li> the <code>git-gui</code> program: simple and useful graphical interface to git-functionality related to commits; </li>
     <li> the <code>gitk</code> program: simple and useful graphical interface to git-functionality related to browsing the history. </li>
    </ol>
   </li>
   <li> Documentation </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(git_recommended_version_number_okl)</code> </li>
   <li> Paths to the three executables:
    <ol>
     <li> git program call = <code>$(git_call_okl)</code> </li>
     <li> gitk program call = <code>$(gitk_call_okl)</code> </li>
     <li> git-gui program call = <code>$(gitgui_call_okl)</code> </li>
    </ol>
    Note that symbolic links are provided in $(public_bin_dir_okl).
   </li>
   <li> Ready: $(git_call_ready_okl)
    <ul>
     <li> location = $(location_git_call_okl) </li>
     <li> version = <code>$(version_git_call_okl)</code>
     </li>
    </ul>
   </li>
   <li> Documentation: $(git_html_documentation_index_location_tag_okl) </li>
   <li> Paths to the auxiliary programs needed on remote hosts for push
   and pull:
    <ol>
     <li> git-upload-pack = <code>$(git_upload_pack_okl)</code> </li>
     <li> git-receive-pack = <code>$(git_receive_pack_okl)</code> </li>
    </ol>
    Note that symbolic links are provided in $(public_bin_dir_okl).
   </li>
  </ul>


  <h2> How to install </h2>

  <h3> Prerequisites </h3>

  <ul>
   <li> <code>gcc</code> is needed (as for many packages). </li>
   <li> <code>asciidoc</code> (often already installed; see
   Buildsystem/ExternalSources/SpecialBuilds/docus/Asciidoc.hpp). </li>
   <li> The <code>curl</code> development libraries for connecting to a remote
   git repository via the http protocoll. </li>
  </ul>


  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> git </code> </td>
    <td> Build the recommended version of git, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanallgit </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> Local versus system-wide installation:
    <ol>
     <li> The default is local installation. </li>
     <li> Set variable <code>git_default_install_okl</code> to value
     "system" to switch. </li>
    </ol>
   </li>
   <li> To install a different version, set variable
   <code>git_recommended_version_number_okl</code> accordingly. </li>
  </ul>

*/

