// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp
  \brief Plans regarding installation of the Boost library


  \todo Update Boost installation
  <ul>
   <li> We only provide installation by local gcc. </li>
   <li> And no combinations, just the "recommended" boost
   installation using the "recommended" local gcc. </li>
   <li> The main point is to clean-up the partial mess. </li>
   <li> Later we can re-introduce these features, if needed
   (this would then likely be the job of oklib). </li>
   <li> And we also drop the timestamp-file. </li>
   <li> This should simplify drastically the build-makefile. </li>
   <li> Since Boost seems to be the only user of the mmv-package,
   perhaps with the Boost installation also a local installation
   of mmv is provided.
    <ol>
     <li> Perhaps here we could check via "which mln" the existence,
     and only install if not existing. </li>
     <li> But better not (this introduces another unknown):
     We install it in any case! </li>
     <li> But we use then "allboost", similar to "allmaxima" etc. </li>
     <li> Installation to OKplatform/bin. </li>
    </ol>
   </li>
  </ul>


  \todo Building version 1_35_0
  <ul>
   <li> We should now skip 1_35_0 and try 1_36_0 : Perhaps this is a
   bit better (though the build-documentation still seems pretty
   weak). </li>
   <li> Building seems unproblematic, but likely this is due to the fact
   that none of the link-libraries is built. </li>
   <li> However the usual problem with their inconsistent names. </li>
   <li> We use (with "oklib all")
   \verbatim
   -I/${OKPLATFORM}/ExternalSources/Boost/1_35_0+4.1.2/include/boost-1_35_0
   \endverbatim
   but it needs to be "boost-1_35". </li>
   <li> Also our system for handling Boost is a mess. Where does "Boost"
   get its value? </li>
   <li> The problem of the wrong path "boost-1_35" can be temporarily fixed
   by renaming "include/boost-1_35" to "include/boost-1_35_0" by hand.
   This should then be simply done in the build-script (renaming whatever
   it finds inside "include"). </li>
   <li> We get a compilation error with QuantumPhysics/OrthogonalTriples.cpp,
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


  \todo Improving building Boost
  <ul>
   <li> Like with gcc, the build-directory and the bjam-directory should not be a
   prerequisite (so that an unnecessary "make boost" is a noop). </li>
   <li> It should be possible to say "gcc-version=recommended". </li>
   <li> Instead of "boost boost_recommended=boost-1_33_1" we should use
   "boost boost-version=1_33_1". </li>
   <li> The default for OKlibBuilding/Makefile is to use the recommended <strong>local</strong>
   gcc-installation --- shouldn't this then be also for building %boost
   the default ? </li>
   <li> Installation of bjam should be improved: Having exactly one
   bjam-installation for each boost-version, and no need to recreate
   it if it's already there. Or, perhaps better: We just leave it in
   the distribution directory --- this seems reasonable: We don't use
   bjam ourselves, and thus by removing the additional steps
   our systems becomes simpler! </li>
   <li> What happens with copying the documentation- files and directories
   if the target- files and/or directories are already there? DONE ("cp -r"
   copies directories/files which are not already present, and actually
   replaces them if they are already there (in any case!)) </li>
   <li> Building %boost should include copying the documentation to doc
   (in the subdirectory boost-1_33_1 for example). DONE </li>
   <li> In the long run, it seems that actually supporting different versions
   of Boost is not feasible (the library will likely always use the newest
   version), so finally supporting different Boost version should be dropped
   (but the general machinery is worth keeping)?!? DONE Very likely most of the times we will support only one boost-version --- but we need the machinery for the transition to newer version (then for some times 2 versions are around)! And we might try out beta-versions etc. So we need the boost-build-machinery. </li>
  </ul>


  \todo Problems with building Boost
  <ul>
   <li> Report to Boost: How to call the libraries is not documented, %e.g.,
   the only library-name mentioned is "Boost.Python", while its real name
   is "python". (Important for "--without-libraries=python".)
   </li>
   <li> How to inform bjam about an alternative compiler? What about
   <code> "-sGCC_ROOT_DIRECTORY=$(gcc-base-directory)/$(2)" </code> ??
   </li>
   <li> When building gcc with local versions, we should make sure that
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
   <li> DONE (to be ignored for now) On cs-wsok (64 bit) we get
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
   <li> Complete Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp. </li>
   <li> Mention that the mcp-tools (mln, mmv) need to be installed (available in all distributions). </li>
  </ul>

*/

