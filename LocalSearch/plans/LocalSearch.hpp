// Oliver Kullmann, 7.11.2006 (Swansea)

/*!
  \file LocalSearch/plans/LocalSearch.hpp
  \brief Plans for the general module on local search

  \todo Improved installation of UBCSAT:
  <ol>
   <li> Some documentation is needed (to Buildsystem/docus/ExternalSources.hpp):
    <ul>
     <li> how to use the binary </li>
     <li> how to use the library files </li>
     <li> what are those library files </li>
     <li> how to use LocalSearch/Ubcsat. </li>
    </ul>
   </li>
   <li> Correct the implementation of ExternalSources/Ubcsat/1-0-0/bin/ubcsat
   for 64-bit platforms (corrected ubcsat-types.h), making some checks:
    <ol>
     <li> ExternalSources/Ubcsat/1-0-0/bin/ubcsat -alg adaptnovelty+
      on csltok: </li>
     <li> ExternalSources/Ubcsat/1-0-0/bin/ubcsat -alg adaptnovelty+
      on cs-wsok: ERROR </li>
    </ol>
    The problem appears to be the bug reported in LocalSearch/Ubcsat/corrected/novelty.c.
   </li>
   <li> Get rid off the temporary build-directory by adding
    <code> -I- -I$(OKsystem)/Transitional/LocalSearch/Ubcsat/corrected </code>
    to the build-compilation. (?) </li>
   <li> No "modified files" anymore, but NEW FILES, with proper change
   documentation, in the OKlibrary; by appropriate settings of variable
   source-libraries the directory with the updated ubcsat-files is dominant
   over the original ubcsat-source-directory. DONE
   (by using LocalSearch/Ubcsat, and a temporary build-directory).
   </li>
   <li> Improve the current build, so that there is exactly one directory containing
   everything offered by Ubcsat, that is, the appropriate src-directory is moved to
   Ubcsat/1-0-0, and possibly the make-variables are updated. DONE </li>
   <li> The source-files from ubcsat should be converted to unix-files (using
   dos2unix): DONE </li>
   <li> Modified .h  files are added to Transitional/AutarkySearch/ which will contain
   definitions of fixed width types from stdint.h instead. DONE </li>
  </ol>

  \todo Create further milestones.

  \todo Move Ubcsat-components from module AutarkySearch to here:
   - commit to cvs : DONE
   - move files : DONE.

*/

/*!
  \namespace OKlib::LocalSearch
  \brief Components for supporting local search
*/

namespace OKlib {
  namespace LocalSearch {
  }
}

