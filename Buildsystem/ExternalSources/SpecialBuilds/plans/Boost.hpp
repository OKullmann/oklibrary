// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp
  \brief Plans regarding installation of the Boost library


  \todo Update Boost installation
  <ul>
   <li> Many of the following points to be improved relate only to
   our "generic" installation method, however there are also relations
   to the (at least in the past) rather strange boost build-system;
   see "Problems with building Boost" below. </li>
   <li> DONE
   We only provide installation by local gcc. </li>
   <li> DONE
   And no combinations, just the "recommended" boost
   installation using the "recommended" local gcc. </li>
   <li> DONE
   The main point is to clean-up the partial mess. </li>
   <li> Later we can re-introduce these features, if needed
   (this would then likely be the job of oklib). </li>
   <li> DONE
   And we also drop the timestamp-file. </li>
   <li> And we also drop the dependency on the gcc-installation. </li>
   <li> This should simplify drastically the build-makefile. </li>
   <li> Installation of bjam should be improved: Having exactly one
   bjam-installation for each boost-version, and no need to recreate
   it if it's already there. Or, perhaps better: We just leave it in
   the distribution directory --- this seems reasonable: We don't use
   bjam ourselves, and thus by removing the additional steps
   our systems becomes simpler! </li>
  </ul>


  \todo Building version 1_38_0 / 1_39_0 / 1_40_0 / 1_43_0
  <ul>
   <li> DONE (they are all built now)
   Building seems unproblematic, but likely this is due to the fact
   that none of the link-libraries is built. </li>
   <li> DONE
   However the usual problem with their inconsistent names. </li>
   <li> DONE (got rid off this directory at all)
   We use (with "oklib all")
   \verbatim
   -I/${OKPLATFORM}/ExternalSources/Boost/1_38_0+4.1.2/include/boost-1_38_0
   \endverbatim
   but it needs to be "boost-1_38". </li>
   <li> DONE ("Boost" etc. has been removed)
   Also our system for handling Boost is a mess. Where does "Boost"
   get its value? </li>
   <li> DONE (see above)
   The problem of the wrong path "boost-1_38" can be temporarily fixed
   by renaming "include/boost-1_38" to "include/boost-1_38_0" by hand.
   This should then be simply done in the build-script (renaming whatever
   it finds inside "include"). </li>
   <li> We now get a compiler error in Competition/TestSATCompetition.cpp
   (apparently boost/range/size.hpp related). </li>
   <li> Another compiler error in
   Satisfiability/Interfaces/InputOutput/ExtendedDimacsStatistics. </li>
   <li> And ExperimentalThreads.hpp apparently uses now unsupported
   functions; so we need to disable it. </li>
   <li> And Programming/Refactoring/IncludeHandling_Tests.hpp doesn't compile.
   </li>
   <li> For PathDifference.compiler_options "-m32" doesn't work. </li>
   <li> DONE (we only compile single-threaded)
   Apparently now all libraries end with "mt" for multi-threading. Do we
   really want this? </li>
   <li> DONE (works with 1_38_0)
   We get a compilation error with QuantumPhysics/OrthogonalTriples.cpp,
   which seems unjustified:
    <ol>
     <li> The problems are in function template orthogonality_relation. </li>
     <li> First we get the (non-sensical) warning that in
     \verbatim
assert(num_vertices(g) == boost::size(r));
     \endverbatim
     we would compare an unsigned number with a signed number --- according
     to the concepts both types should by unsigned! </li>
     <li> If we replace "boost::size(r)" by "size_type(boost::size(r))"
     then the warning goes away --- however size_type should be the type
     of boost::size(r) ! </li>
     <li> The "real" compilation error then happens in line
     \verbatim
Graph g(ev.begin(), ev.end(), boost::size(r), ev.size());
     \endverbatim
     where in boost/graph/detail/adjacency_list.hpp:2113 the wrong type
     of an argument to pointer dereference "*" is claimed ??? </li>
     <li> Compilation happens with "-ansi -pedantic", however also without
     these options we get the same errors. </li>
     <li> Perhaps the problem is a boost::range problem. </li>
     <li> Hopefully a small example can be extracted, which can be
     shown to the boost mailing list. </li>
     <li> By casting both occurrences of boost::size(r) to size_type,
     and by not compiling with -strict (because of "long long"),
     compilation succeeds. </li>
     <li> Another problem is that (again(!)) the link-libraries have
     been renamed, this time apparently replacing "gcc" in the name
     by "gcc-mt". Renaming (temporarily) the links (created by our
     build-process) to their old forms (without the "-mt") solves
     the problem at least for the graph-library. </li>
     <li> Next problem then with the compilation of Concepts/TestConcepts.cpp,
     where now we get a problem with the concept-checking-class
     ResultElementWithName in Concepts/ResultElements.hpp, due to
     compiler actually wanting to link(!) something for the instruction
     \verbatim
static_cast<string_type>(r.name());
     \endverbatim
     (and which it claims yields an
     "undefined reference to `boost::system::get_system_category()"). </li>
     <li> Could this be a problem with the Boost concept-checking library ??
     </li>
     <li> Similarly we get a link-error for concept-check
     FullyEqualityComparable in Concepts/Basics.hpp related to
     \verbatim
bo = static_cast<bool>(a != y);
     \endverbatim
     (and not for the two similar instructions above!). This seems nonsense.
     </li>
     <li> Temporarily we can get rid off this error by just not linking
     the program "TestConcepts" (compilation of the .o-file works without
     problems). </li>
     <li> Then compilation errors in TestSATCompetition.cpp, apparently
     all related to boost-range. </li>
     <li> Perhaps the critical function is IteratorHandling::range_first
     in General/IteratorHandling.hpp. </li>
     <li> This function needs to be specified (what it is supposed to
     do), and then perhaps we show it to the boost mailing list. </li>
    </ol>
   </li>
  </ul>


  \todo Problems with building Boost
  <ul>
   <li> 1_40 is out, and we need to investigate the changes:
    <ol>
     <li> DONE (we seems to have now a working system)
     Apparently now a standard Unix/Linux build is partially
     supported. </li>
     <li> DONE (it seems we got bjam working)
     It would be great if all our needs (local installation, with
     local gcc-compiler) could be fulfilled in this way. </li>
    </ol>
   </li>
   <li> Report to Boost: How to call the libraries is not documented, %e.g.,
   the only library-name mentioned is "Boost.Python", while its real name
   is "python". (Important for "--without-libraries=python".)
   </li>
   <li> DONE (this is handled by a bjam-configuration-file)
   How to inform bjam about an alternative compiler? What about
   <code> "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" </code> ??
   </li>
   <li> DONE (this should be solved now by the corrected boost-building)
   When building gcc with local versions, we should make sure that
   the system-gcc doesn't interfere (especially important regarding linking).
    <ol>
     <li> On cs-wsok it seems that the system-gcc (4.0.2) interferes;
     perhaps it tries to link with the 32bit-version, can't do that,
     and then falls back to the system version? </li>
     <li> What is the role of LD_LIBRARY_PATH ?? (On cs-wsok it is empty.) </li>
     <li> We should check in general whether building %boost links to the 32bit
     or to the 64bit version. </li>
    </ol>
   </li>
   <li> DONE (to be ignored for now) On cs-ltok (32 bit) we get build-log-messages like
   \verbatim
`.L1119' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_iarchive.o
`.L573' referenced in section `.rodata' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o: defined in discarded section `.gnu.linkonce.t._ZNK5boost7archive17archive_exception4whatEv' of bin.v2/libs/serialization/build/gcc-3.4.3/debug/threading-multi/xml_oarchive.o
   \endverbatim
   Does this indicate something we should worry about ?
   </li>
   <li> DONE (this seems to be solved now --- one needs to avoid having
   multiple gcc's at system level, and for a new system-installation one needs
   to disable the old version by renaming it)
   On cs-wsok (64 bit) we get
   \verbatim
ExternalSources> make boost-1_34_0

...failed updating 8 targets...
...skipped 24 targets...
...updated 5517 targets...
   \endverbatim
   Rerunning reveals
   \verbatim
...failed gcc.link.dll /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/ExternalSources/Boost/1_34_0+4.1.2_Build/boost/bin.v2/libs/wave/build/gcc-4.1.2/debug/threading-multi/libboost_wave-gcc41-mt-d-1_34.so.1.34.0...
gcc.link.dll /h/21/GemeinsameBasis/SAT-Algorithmen/OKplatform/ExternalSources/Boost/1_34_0+4.1.2_Build/boost/bin.v2/libs/graph/build/gcc-4.1.2/debug/threading-multi/libboost_graph-gcc41-mt-d-1_34.so.1.34.0
collect2: ld terminated with signal 11 [Segmentation fault]
   \endverbatim
   So there are linking problems regarding the wave- and the graph-library.
   This seems not to be of urgent concern for now (but the problem must be
   fixed in the future).
   </li>
  </ul>


  \todo Documentation
  <ul>
   <li> Complete Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp.
   </li>
   <li> DONE (we don't use them anymore)
   Mention that the mcp-tools (mln, mmv) need to be installed (available 
   in all distributions). </li>
  </ul>


*/

