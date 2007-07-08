// Oliver Kullmann, 27.6.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp
  \brief Plans regarding installation of gcc

  \todo Gcc
  <ul>
   <li> Linking to the gcc-documentation (main info-page) should be checked
   for completeness. </li>
   <li> Instead of, %e.g., "make gcc-4.1.2", wouldn't it be more consistent with
   building Boost to use instead "make gcc gcc-version=4.1.2" ? </li>
   <li> If variable "gcc-version" is set, then it should have one of the
   allowed values (while otherwise we get an error). </li>
   <li> We must understand, how gcc interacts with 32- and 64-bit
   environments, and how to take control of this. </li>
   <li> There should be make-variables, which allow control over
   some settings for the build of gcc. </li>
   <li> Shouldn't file external_sources_versions.mak be placed in
   subdirectory Buildsystem/ExternalSources ? </li>
   <li> texti2pdf is a script, which can be put into ~/bin for example,
   and is needed for building the gcc-documentation --- how to provide it?
   Such small utilities could be put under version control (OKlibrary) ? </li>
   <li> Installation of R requires a Fortran compiler. Therefore the 
    system-installation of Gcc should allow for enabling of Fortran
    language support. </li>
   <li> Enabling Fortran language support in Gcc requires that the MPFR
    and GMP libraries are installed. Should we install these also locally?
    <ol>
     <li> GMP is also of interest to us regarding big-number-types, so we need full
     control about it, and perhaps a local installation is warranted (ignoring the
     system installation). </li>
    </ol>
   </li>
   <li> If the filestamp does already exist, then we want nothing to happen ---
   however yet the build-directory will be rebuilt if not existent, since
   it is a prerequisite of the rule for the "tag-paths". So it seems
   necessary to remove the build-directory from the prerequisite-list,
   however then it seems impossible to create the build-directory, if actually
   gcc *is* to be build, via the target-mechanism. DONE (it seems reasonable
   to remove the build-directory from the prerequisite-list and to build it
   "manually") </li>
  </ul>

*/
