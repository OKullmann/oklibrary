// Oliver Kullmann, 18.8.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp
  \brief Plans regarding installation of gmp


  \todo Installation
  <ul>
   <li> Create an installation directory (for example, "Gmp/4.2.1"). </li>
   <li> For cleanliness, we copy the documentation to the doc-directory
   (as we do it with Boost). </li>
  </ul>


  \todo Local and system-wide
  <ul>
   <li> We need the possibility to choose local installation
   as well as system-wide installation. </li>
   <li> Perhaps a variable "global_installation" is used, and when
   set, the sudo-command is used and variable install_dir is (if not
   already set) set to "/usr/local"; if not set then (if not already
   set) the value is "gmp_base_directory/version-number". </li>
   <li> This method should be also usable for all other installations. </li>
   <li> As with Boost, in case of a local installation, not only
   the path to the link-library is needed, but also when linking
   the rpath-Option. Perhaps this is simply provided by another
   make-variable. </li>
  </ul>


  \todo C++
  <ul>
   <li> If we enable C++ support, then we must also, similar to Boost,
   enable compilation with the different local compilers. </li>
   <li> And for linking then linking to different versions, according
   to the compiler used, is needed. </li>
   <li> Perhaps first we just use the C functionality, and write
   our own (very simple) basic wrapper? </li>
   <li> On the other hand, this is duplication, and we should avoid this. </li>
   <li> So let's enable the C++ support, and rethink the general scheme
   of linking etc.: This should be exactly the same as with Boost, and thus
   we can rethink the whole approach. </li>
   <li> Instead of making these case-distinctions in generic.mak,
   it should be possible to delegate everything to the use of
   recursive make-variables! </li>
  </ul>
*/

