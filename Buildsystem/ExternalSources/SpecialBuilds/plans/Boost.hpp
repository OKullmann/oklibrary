// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp
  \brief Plans regarding installation of the Boost library


  \bug Defect boost-installation for fixed gcc-4.1.2 : DONE
  <ul>
   <li> The installation for gcc-4.1.2 (falsely) uses some general
   gcc-variables, which relate to the current gcc-version. </li>
  </ul>


  \bug Missing documentation for Boost
  <ul>
   <li> The installation page needs to be updated ("XXX" should be eliminated).
   </li>
   <li> DONE (Included new "index.html" file in copy operation during
   boost build)
   The link to the local documentation on the ExternalSources page does
   not work. </li>
  </ul>


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


  \todo Documentation
  <ul>
   <li> Complete Buildsystem/ExternalSources/SpecialBuilds/docus/Boost.hpp.
   </li>
   <li> DONE (we don't use them anymore)
   Mention that the mcp-tools (mln, mmv) need to be installed (available 
   in all distributions). </li>
  </ul>


  \todo distance versus size
  <ul>
   <li> Using a specially designed C++ program to test the speed difference
   between using s.size() and std::distance(s) for sets, shows that there
   is a considerable difference. For instance, consider
   \verbatim
#include <fstream>
#include <iostream>
#include <string>
#include <boost/range.hpp>
#include <set>

#ifdef USE_SIZE
#define SIZE(A) A.size()
#else
#define SIZE(A) boost::distance(A)
#endif

const long size = 10000000;

int main(const int argc, const char* const argv[]) {
  std::set<long> s;

  for (long i = 0; i < size; i++) {
    s.insert(i);
  }

  long x = 0;
  for (long i = 0; i < 1000000; i++) {
    x += SIZE(s);
  }

  std::cout << "Output = " << x << std::endl;
}
   \endverbatim
   Setting "size" to 10000000 and defining "USE_SIZE" results in a run
   time of "1.57s" on my machine, however with a size of only "10000", leaving
   USE_SIZE undefined, results in a run time of more than 2 minutes. 
   This is a clear demonstration that there may be potential issues with
   using boost::distance instead of boost:size. MG has sent an e-mail
   to the boost-users mailing list regarding this issue. </li>
  </ul>


  \bug DONE (Removed stale instructions and tested)
  Stale renaming instructions
  <ul>
   <li> The renaming of the link-libraries is likely not needed anymore.
   </li>
  </ul>

*/

