// Oliver Kullmann, 16.8.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Git.hpp
  \brief Documentation on how to build Git


  <h1> Installing Git </h1>


  <h2> On the purpose of Git </h2>

  Git is a distributed source control system, used by the OKlibrary.


  <h2> What the installation yields </h2>

  <ul>
   <li> Thee executables
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
   <li> git-program call = <code>$(git_call)</code> </li>
   <li> ready: $(git_call_ready)
    <ul>
     <li> location = $(location_git_call) </li>
     <li> version = <code>$(version_git_call)</code>
     </li>
    </ul>
   </li>
   <li> documentation: $(git_html_documentation_index_location_tag) </li>
  </ul>


  <h2> How to install </h2>

  Only system-wide installation currently.

  <h3> Make targets </h3>
  
  <table>
   <tr>
    <td> <code> git </code> </td>
    <td> Build the recommended version of git, using the system-gcc. </td>
   </tr>
   <tr>
    <td> <code> git-a.b.c.d </code> </td>
    <td> Build the version a.b.c.d of git, using the system-gcc (for example
    <code>git-1.5.2.5</code>). </td>
   </tr>
  </table>

*/

