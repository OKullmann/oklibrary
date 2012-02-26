// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp
  \brief Plans regarding installation of the Boost library


  \todo DONE (updated path to bjam-sources)
  Update to version 1_48_0
  <ul>
   <li> The build-process changed, now lo longer directly based on bjam, but
   on "b2". </li>
   <li> So we need to update our build-process. OK sent an e-mail to the
   Boost e-mail list. </li>
  </ul>


  \bug DONE (explicite linking to libboost_regex required, when using the
  Graphviz components of the Graph library)
  Linking error (on csoberon)
  <ul>
   <li> We get for "oklib new_check"
   \verbatim
time -p /compsci/saturn/staff/csoliver/OKplatform/system_directories/bin/tests/BoostSupport basic "cerr|ofstream-w=/compsci/saturn/staff/csoliver/OKplatform/system_directories/aux/tests/BoostSupport/Error|ofstream-w=/compsci/saturn/staff/csoliver/OKplatform/system_directories/aux/tests/BoostSupport/Log" "cout|ofstream-w=/compsci/saturn/staff/csoliver/OKplatform/system_directories/aux/tests/BoostSupport/Message|ofstream-w=/compsci/saturn/staff/csoliver/OKplatform/system_directories/aux/tests/BoostSupport/Log" "ofstream-w=/compsci/saturn/staff/csoliver/OKplatform/system_directories/aux/tests/BoostSupport/Log" 0 0 0 en_GB en_GB en_GB
/compsci/saturn/staff/csoliver/OKplatform/system_directories/bin/tests/BoostSupport: error while loading shared libraries: libboost_regex.so.1.44.0: cannot open shared object file: No such file or directory

OKplatform> ldd system_directories/bin/tests/BoostSupport
        linux-gate.so.1 =>  (0xffffe000)
        libboost_graph.so.1.44.0 => /compsci/saturn/staff/csoliver/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_graph.so.1.44.0 (0xb7774000)
        libstdc++.so.6 => /compsci/saturn/staff/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib/libstdc++.so.6 (0xb7681000)
        libm.so.6 => /lib/libm.so.6 (0xb763c000)
        libgcc_s.so.1 => /compsci/saturn/staff/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib/libgcc_s.so.1 (0xb7630000)
        libc.so.6 => /lib/libc.so.6 (0xb74c5000)
        libboost_regex.so.1.44.0 => not found
        librt.so.1 => /lib/librt.so.1 (0xb74bb000)
        libpthread.so.0 => /lib/libpthread.so.0 (0xb749f000)
        /lib/ld-linux.so.2 (0xb77de000)
   \endverbatim
   </li>
   <li> On other machines it links to the the system boost:
    <ol>
     <li> On the old cs-wsok (Suse 10.0) we had
     \verbatim
> ldd system_directories/bin/tests/BoostSupport
        libboost_graph.so.1.44.0 => /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_graph.so.1.44.0 (0x00002aaaaabc2000)
        libstdc++.so.6 => /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib64/libstdc++.so.6 (0x00002aaaaae2f000)
        libm.so.6 => /lib64/tls/libm.so.6 (0x00002aaaab165000)
        libgcc_s.so.1 => /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib64/libgcc_s.so.1 (0x00002aaaab2bc000)
        libc.so.6 => /lib64/tls/libc.so.6 (0x00002aaaab4ca000)
        libboost_regex.so.1.44.0 => /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_regex.so.1.44.0 (0x00002aaaab6f5000)
        librt.so.1 => /lib64/tls/librt.so.1 (0x00002aaaab9e4000)
        libpthread.so.0 => /lib64/tls/libpthread.so.0 (0x00002aaaabaed000)
        /lib64/ld-linux-x86-64.so.2 (0x00002aaaaaaab000)
     \endverbatim
     </li>
     <li> However with newer installation (Suse 11.4, cs-wsok and csltok) it
     links to the system-boost (and thus only works if there is a compatible
     system-boost):
     \verbatim
        libboost_graph.so.1.44.0 => /home/csoliver/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_graph.so.1.44.0 (0x00007f8170fbe000)
        libboost_regex.so.1.44.0 => /usr/lib64/libboost_regex.so.1.44.0 (0x00007f81701c1000)

        libboost_regex.so.1.44.0 => /usr/lib64/libboost_regex.so.1.44.0 (0x00007f8d63e00000)
     \endverbatim
     </li>
    </ol>
   </li>
   <li> On cspcmg it links to the OKlibrary boost:
   \verbatim
> ldd system_directories/bin/tests/BoostSupport
     linux-vdso.so.1 =>   (0x00007fff547ff000)
     libboost_graph.so.1.44.0 =>  /home/csmg/Work/OKlibrary/NewOKlib/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_graph.so.1.44.0
(0x00007fa2415f5000)
     libstdc++.so.6 =>  /home/csmg/Work/OKlibrary/NewOKlib/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib64/libstdc++.so.6
(0x00007fa2412e8000)
     libm.so.6 =>  /lib/x86_64-linux-gnu/libm.so.6 (0x00007fa24103d000)
     libgcc_s.so.1 =>  /home/csmg/Work/OKlibrary/NewOKlib/OKplatform/ExternalSources/Installations/Gcc/4.1.2/lib64/libgcc_s.so.1
(0x00007fa240e30000)
     libc.so.6 =>  /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa240a9b000)
     libboost_regex.so.1.44.0 =>  /home/csmg/Work/OKlibrary/NewOKlib/OKplatform/ExternalSources/Installations/Boost/4.1.2/1_44_0/lib/libboost_regex.so.1.44.0
(0x00007fa2407a9000)
     librt.so.1 =>  /lib/x86_64-linux-gnu/librt.so.1 (0x00007fa2405a1000)
     libpthread.so.0 =>  /lib/x86_64-linux-gnu/libpthread.so.0
(0x00007fa240382000)
     /lib64/ld-linux-x86-64.so.2 (0x00007fa241864000)
   \endverbatim
   </li>
   <li> System information:
    <ol>
     <li> csoberon/csnereid:
     \verbatim
> uname -a
Linux csnereid 2.6.37.1-1.2-desktop #1 SMP PREEMPT 2011-02-21 10:34:10 +0100 i686 i686 i386 GNU/Linux
> ld --version
GNU ld (GNU Binutils; openSUSE 11.3) 2.20.0.20100122-6
     \endverbatim
     </li>
     <li> On cs-wsok:
     \verbatim
> uname -a
Linux cs-wsok 2.6.37.6-0.7-desktop #1 SMP PREEMPT 2011-07-21 02:17:24 +0200 x86_64 x86_64 x86_64 GNU/Linux
> ld --version
GNU ld (GNU Binutils; openSUSE 11.4) 2.21
     \endverbatim
     </li>
     <li> On csltok:
     \verbatim
> uname -a
Linux csltok 2.6.39.3-0.5-desktop #1 SMP PREEMPT Sun Jul 31 02:04:11 BST 2011 x86_64 x86_64 x86_64 GNU/Linux
> ld --version
GNU ld (GNU Binutils; openSUSE 11.4) 2.21
     \endverbatim
     </li>
     <li> On cspcmg:
     \verbatim
> uname -a
Linux cspcmg 2.6.38-11-generic #48-Ubuntu SMP Fri Jul 29 19:02:55 UTC 2011 x86_64 x86_64 x86_64 GNU/Linux
> ld --version
GNU ld (GNU Binutils for Ubuntu) 2.21.0.20110327
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Why does it want to link to libboost_regex at all?
    <ol>
     <li> OK has sent an e-mail to boost-users@lists.boost.org, under heading
     "linking with libboost_graph not enough, but libboost_regex required for
     simple graph application?". </li>
     <li> According to the Boost mailing-list, if Graphviz is involved, then
     explicit linking with libboost_regex is needed. </li>
     <li> So apparently the behaviour on cspcmg is not guaranteed (or
     possibly even wrong). </li>
    </ol>
   </li>
  </ul>


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

