// Oliver Kullmann, 3.7.2007 (Swansea)
/* Copyright 2007, 2010, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/Configuration/ExternalSources/plans/general.hpp
  \brief Plans for handling of configuration data regarding external sources


  \todo Handling of version numbers
  <ul>
   <li> DONE
   Names like
   <code> doxygen_supported_not_recommended_version_numbers_okl </code>
   are misleading (these versions are not "not recommended"), and
   they are too long. Proposal: <code> doxygen_other_versions_okl </code>
   </li>
   <li> And e.g. "gcc_recommended_version_number_okl" should become
   "gcc_version_okl". </li>
   <li> E.g. "gcc_recommended_package_name_okl" should become
   "gcc_package_name_okl". </li>
   <li> And all other uses of "recommended" should be eliminated. </li>
  </ul>


  \todo Remove usage of xxx_targets_prefix_okl : DONE
  <ul>
   <li> This is a relict of the old system, to be removed. </li>
   <li> Except where still the old system is used. </li>
  </ul>


  \todo Update to the new system
  <ul>
   <li> Coq </li>
   <li> Ocaml </li>
   <li> Postgresql </li>
  </ul>


  \todo Software management
  <ul>
   <li> All information about whether software (like doxygen, gcc etc.)
   needed is installed locally or system-wide, and where to find it,
   must be found in the configuration directory (in an easily accesible
   way). </li>
   <li> It must be possible to update the system (installing a new
   gcc, for example), perhaps by setting variables about supported and
   recommended versions. </li>
  </ul>


  \todo Configuration/ExternalSources/all.mak
  <ul>
   <li> All definitions for one external source go to for example
   Buildsystem/ExternalSources/gcc.mak, while ExternalSources/all.mak includes
   all these files. </li>
   <li> How we call our internal targets must be decoupled from
   how packages are called etc. </li>
   <li> There are several inline-comments in ExternalSources/all.mak,
   typically referring to apparently stale variables. </li>
   <li> The variables related to the targets all (should) follow
   the same scheme, and perhaps should be defined in a general
   way in the main external-sources makefile (perhaps using
   a list for all external sources supported, and then generating
   these variables) ?! </li>
   <li> The role of Configuration/all.mak must
   be clarified. DONE (contains all variable setting for making
   special sources which are general and relate to the version
   number and/or to the documentation and web pages and other
   resources provided by the external source) </li>
  </ul>


  \todo Configuration/ExternalSources/tests.mak
  <ul>
   <li> Why do we have to use
   \verbatim
$(shell (type -P $(gpp_system_call)))
   \endverbatim
   and can't use just
  \verbatim
$(shell type -P $(gpp_system_call))
   \endverbatim
   ???
   </li>
   <li> Perhaps we should process all shell-calls in one go? </li>
   <li> We should improve parsing of version numbers:
    <ol>
     <li> Shall we use shell pattern matching or awk? </li>
     <li> If using awk, how can we extract the field with the matching
     content? </li>
     <li> We need bash version 3.0 or newer for the regular expression
     matching operator "=~"; perhaps this is common now. </li>
     <li> Newer versions of bash seem to have "[:digit:]", but
     3.00.0 doesn't have it, so we better use "[0-9]" instead. </li>
    </ol>
   </li>
  </ul>

*/  

