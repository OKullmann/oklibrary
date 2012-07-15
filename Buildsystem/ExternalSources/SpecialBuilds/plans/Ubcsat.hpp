// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo Update to version 1.2.0
  <ul>
   <li> The target "ubcsat-new" should be removed (just using "ubcsat"). </li>
   <li> Update to beta-17 (ID 7ef9cabdc47e33e2fce4ce0e0dca04a7853b4adb is our
   merge). </li>
   <li> DONE (meanwhile we merged beta-10 with our own changes; ID
   07d6baf978c02c2c63af6143096f085cee83179a, last change 29.10.2011)
   We created a new package "ubcsat-1-2-0-beta.tar.bz2", cloning
   https://github.com/dtompkins/ubcsat.git, checking out branch beta. </li>
   <li> Likely we stop supporting version 1.0.0:
    <ol>
     <li> See "Shall we abort this development?" in
     Autarkies/Search/plans/AnalyseTotalAssignment.hpp. </li>
     <li> Then also the dos2unix-problems (see
     "Make clear the required package for dos2unix" below) disappears (at least
     for now). </li>
    </ol>
   </li>
   <li> Segmentation faults for saps, rsaps, sapsnr:
    <ol>
     <li>
     \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 38 1376

> valgrind ExternalSources/Installations/SAT/Ubcsat/1-2-0-beta/bin/ubcsat-1-2-0-beta -alg saps -i VanDerWaerden_2-3-38_1376.cnf
...
#       F  Best                 Step                Total 
#   Run N Sol'n                   of               Search 
#   No. D Found                 Best                Steps 
# 
==25749== 
==25749== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==25749==  General Protection Fault
==25749==    at 0x4390B3: InitClausePenaltyFL (in /home/kullmann/OKplatform/ExternalSources/Installations/SAT/Ubcsat/1-2-0-beta/bin/ubcsat-1-2-0-beta)
==25749==    by 0x441AFA: ubcsatmain (in /home/kullmann/OKplatform/ExternalSources/Installations/SAT/Ubcsat/1-2-0-beta/bin/ubcsat-1-2-0-beta)
==25749==    by 0x50A2B7C: (below main) (in /lib64/libc-2.11.2.so)
     \endverbatim
     Same with rsaps and sapsnr (but not for any other algorithm).
     </li>
     <li> The error doesn't occur with the debug-version. </li>
     <li> After further type-corrections it seems these errors have been
     resolved. </li>
     <li> But on cs-oksvr we still have a segmentation-fault for saps. </li>
     <li> On cspcmg the issue is resolved (no segfaults on this instance for
     any algorithm).</li>
    </ol>
   </li>
   <li> DONE
   We also changed "#define MAXPARMLINELEN 4096" to
   "#define MAXPARMLINELEN 16384". </li>
   <li> "new-ubcsat-okl" is the wrapper-script to access the new version
   (besides the old version). </li>
   <li> Update to a newer version via
   \verbatim
ubcsat-1-2-0-beta> git pull https://github.com/dtompkins/ubcsat.git beta
   \endverbatim
   inside the extracted archive (then archiving it again). </li>
   <li> Speed issues:
    <ol>
     <li> Version 1-2-0 is slower than our version 1-0-0:
     \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 3 36 1256

> ubcsat-okl -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
...
FlipsPerSecond = 15225

> new-ubcsat-okl -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
...
FlipsPerSecond = 14102
     \endverbatim
     This needs to be checked with our newest version, and on different
     machines. </li>
     <li> There are enormous differences (a factor up to 80) on different
     machines; see
     Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/38.hpp.
     </li>
     <li> With the newest ubcsat-version, on cs-oksvr we get
     \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 3 24 569
> cat VanDerWaerden_pd_2-3-24_569.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
285 43132 192671 0 192671 1 1
 length count
2 378
3 39704
12 12
13 23
24 3015


> E=run_ubcsat("VanDerWaerden_pd_2-3-24_569.cnf", cutoff=50000,runs=10)

OKplatform> exp_dir="ubcsat_tmp_VanDerWaerden_pd_2-3-24_569.cnf_2011-01-14-203622/"; for F in ${exp_dir}/*.run_ubcsat_stats; do echo -ne "$(basename $F .run_ubcsat_stats)\t"; grep "Flips" $F; done
ag2wsat FlipsPerSecond = 62735
anovpp  FlipsPerSecond = 37936
anovp   FlipsPerSecond = 39277
cwalk   FlipsPerSecond = 294118
cwalks  FlipsPerSecond = 265957
dano    FlipsPerSecond = 36075
ddfw    FlipsPerSecond = 22624
dwalk   FlipsPerSecond = 156740
g2wsat  FlipsPerSecond = 60024
gsat    FlipsPerSecond = 134409
gsats   FlipsPerSecond = 137363
gsatt   FlipsPerSecond = 92593
gwsat   FlipsPerSecond = 65963
hsat    FlipsPerSecond = 143266
hwsat   FlipsPerSecond = 100402
irots   FlipsPerSecond = 68213
novpc   FlipsPerSecond = 51867
novp_p  FlipsPerSecond = 35137
novpp   FlipsPerSecond = 36311
novp    FlipsPerSecond = 36657
nov     FlipsPerSecond = 36819
paws    FlipsPerSecond = 57737
rgsat   FlipsPerSecond = 43403
rnovp   FlipsPerSecond = 36049
rnov    FlipsPerSecond = 39277
rots    FlipsPerSecond = 83472
rsaps   FlipsPerSecond = 49751
samd    FlipsPerSecond = 90253
sapsnr  FlipsPerSecond = 51282
saps    uwalk   FlipsPerSecond = 211864
vw1     FlipsPerSecond = 55928
vw2     FlipsPerSecond = 56497
wsat    FlipsPerSecond = 56306
wsattn  FlipsPerSecond = 53996
wsatt   FlipsPerSecond = 55804

> cat /proc/cpuinfo
processor       : 0
vendor_id       : AuthenticAMD
cpu family      : 15
model           : 5
model name      : AMD Opteron(tm) Processor 250
stepping        : 10
cpu MHz         : 2400.000
cache size      : 1024 KB
fpu             : yes
fpu_exception   : yes
cpuid level     : 1
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 syscall nx mmxext lm 3dnowext 3dnow rep_good
bogomips        : 4782.32
TLB size        : 1024 4K pages
clflush size    : 64
cache_alignment : 64
address sizes   : 40 bits physical, 48 bits virtual
power management: ts fid vid ttp

processor       : 1
vendor_id       : AuthenticAMD
cpu family      : 15
model           : 5
model name      : AMD Opteron(tm) Processor 250
stepping        : 10
cpu MHz         : 2400.000
cache size      : 1024 KB
fpu             : yes
fpu_exception   : yes
cpuid level     : 1
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 syscall nx mmxext lm 3dnowext 3dnow rep_good
bogomips        : 4782.32
TLB size        : 1024 4K pages
clflush size    : 64
cache_alignment : 64
address sizes   : 40 bits physical, 48 bits virtual
power management: ts fid vid ttp
     \verbatim
     segmentation fault with saps; these flips-per-second seem roughly to
     correspond to the run-times. </li>
     <li> On csoberon we get
     \verbatim
> E=run_ubcsat("VanDerWaerden_pd_2-3-24_569.cnf", cutoff=50000,runs=10, include_algs=names(new_run_ubcsat_cnf_algs), ubcsat_wrapper="new-ubcsat-okl")

ag2wsat FlipsPerSecond = 114155
anovpp  FlipsPerSecond = 74627
anovp   FlipsPerSecond = 74405
cwalk   FlipsPerSecond = 396825
cwalks  FlipsPerSecond = 393701
dano    FlipsPerSecond = 74850
ddfw    FlipsPerSecond = 36232
dwalk   FlipsPerSecond = 119904
g2wsat  FlipsPerSecond = 122549
gsat    FlipsPerSecond = 154799
gsats   FlipsPerSecond = 193050
gsatt   FlipsPerSecond = 168350
gwsat   FlipsPerSecond = 146199
hsat    FlipsPerSecond = 213675
hwsat   FlipsPerSecond = 151515
irots   FlipsPerSecond = 134771
novpc   FlipsPerSecond = 114679
novp_p  FlipsPerSecond = 60024
novpp   FlipsPerSecond = 75415
novp    FlipsPerSecond = 73206
nov     FlipsPerSecond = 72569
paws    FlipsPerSecond = 124688
rgsat   FlipsPerSecond = 86505
rnovp   FlipsPerSecond = 74627
rnov    FlipsPerSecond = 74963
rots    FlipsPerSecond = 144092
rsaps   FlipsPerSecond = 130890
samd    FlipsPerSecond = 170068
sapsnr  FlipsPerSecond = 123457
saps    FlipsPerSecond = 95238
uwalk   FlipsPerSecond = 423729
vw1     FlipsPerSecond = 105263
vw2     FlipsPerSecond = 104603
wsat    FlipsPerSecond = 106383
wsattn  FlipsPerSecond = 106610
wsatt   FlipsPerSecond = 106610

> cat /proc/cpuinfo
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 23
model name      : Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
stepping        : 10
cpu MHz         : 2003.000
cache size      : 6144 KB
physical id     : 0
siblings        : 2
core id         : 0
cpu cores       : 2
apicid          : 0
initial apicid  : 0
fdiv_bug        : no
hlt_bug         : no
f00f_bug        : no
coma_bug        : no
fpu             : yes
fpu_exception   : yes
cpuid level     : 13
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe nx lm constant_tsc arch_perfmon pebs bts aperfmperf pni dtes64 monitor ds_cpl vmx smx est tm2 ssse3 cx16 xtpr pdcm sse4_1 xsave lahf_lm tpr_shadow vnmi flexpriority
bogomips        : 6000.00
clflush size    : 64
cache_alignment : 64
address sizes   : 36 bits physical, 48 bits virtual
power management:

processor       : 1
vendor_id       : GenuineIntel
cpu family      : 6
model           : 23
model name      : Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
stepping        : 10
cpu MHz         : 3003.000
cache size      : 6144 KB
physical id     : 0
siblings        : 2
core id         : 1
cpu cores       : 2
apicid          : 1
initial apicid  : 1
fdiv_bug        : no
hlt_bug         : no
f00f_bug        : no
coma_bug        : no
fpu             : yes
fpu_exception   : yes
cpuid level     : 13
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe nx lm constant_tsc arch_perfmon pebs bts aperfmperf pni dtes64 monitor ds_cpl vmx smx est tm2 ssse3 cx16 xtpr pdcm sse4_1 xsave lahf_lm tpr_shadow vnmi flexpriority
bogomips        : 5999.64
clflush size    : 64
cache_alignment : 64
address sizes   : 36 bits physical, 48 bits virtual
power management:
     \endverbatim
     Again, the flips-per-second seem correct measurements. Apparently the
     bigger cache makes it quite a bit faster. </li>
     <li> With the newest version beta-9 on csltok still we get segmentation
     faults with rsaps and sapsnr. And again with the debug-version we don't
     get the error. </li>
     <li> While the above numbers seem reasonable, for the computations
     w.r.t VanDerWaerden_2-3-39_1415.cnf and VanDerWaerden_2-3-38_1376.cnf we
     get values well over a million (cscharon, csoberon) which can't be right.
     </li>
    </ol>
   </li>
   <li> DONE (we didn't check out the beta-version)
   The current version is not usable at all; e-mail sent to
   ubcsat-developers. </li>
   <li>  DONE (corrected the bug)
   The new version still seems not usable; bug report sent to
   ubcsat-developers. </li>
   <li> DONE (32-bit types were defined as "long", which is 64 bits)
   After update the problem looks like that:
   \verbatim
ubcsat-1-2-0-beta> make ubcsat_debug
ubcsat-1-2-0-beta> valgrind --track-origins=yes ./ubcsat_debug -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
==32044== Conditional jump or move depends on uninitialised value(s)
==32044==    at 0x40AAFD: PickRoTS (rots.c:123)
==32044==    by 0x454A38: ubcsatmain (ubcsat.c:95)
==32044==    by 0x454EBA: main (ubcsat.c:149)
==32044==  Uninitialised value was created by a heap allocation
==32044==    at 0x4B1B16E: malloc (vg_replace_malloc.c:236)
==32044==    by 0x41D60D: AllocateRAM (ubcsat-mem.c:71)
==32044==    by 0x41D949: SetString (ubcsat-mem.c:98)
==32044==    by 0x41BD51: SetupUBCSAT (ubcsat-internal.c:1847)
==32044==    by 0x4544B2: ubcsatmain (ubcsat.c:33)
==32044==    by 0x454EBA: main (ubcsat.c:149)
   \endverbatim
   </li>
  </ul>


  \bug UBCSAT 1.0.0 reports wrong minimum number of falsified clauses
  <ul>
   <li> Using the test file:
   \verbatim
> cat test.cnf 
p cnf 2 2
1 2 0
1 -2 0
   \endverbatim
   and running UBCSAT with "gsat", "rnovelty+" or possibly other
   solvers, yields 3 falsified clauses as the "best" or "min" 
   despite the fact that there are only 2 clauses in the clause
   set AND those runs are said to have found a satisfying assignment:
   \verbatim
> ubcsat-okl -alg gsat -i test.cnf -seed 1845608494
# -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] -runs 10 -cutoff 100000 -alg gsat -i test.cnf -seed 1845608494
       sat  min     osteps     msteps       seed
      1 1     0          2          2 1845608494 
      2 1     3          0          1 2475891066 
      3 1     3          0          1 1321093004 
      4 1     3          0          1 3320110952 
      5 1     0          2          2 3736165511 
      6 1     0          2          2 2062463429 
      7 1     3          0          1 3199736854 
      8 1     0          2          2 3920935571 
      9 1     3          0          1  495225504 
     10 1     0          2          2 3620363783 


Clauses = 2
Variables = 2
TotalLiterals = 4
FlipsPerSecond = inf    
BestStep_Mean = 1.000000
Steps_Mean = 1.500000
Steps_Max = 2.000000
PercentSuccess = 100.00
BestSolution_Mean = 1.500000
BestSolution_Median = 1.500000
BestSolution_Min = 0.000000
BestSolution_Max = 3.000000
   \endverbatim
   </li>
  </ul>


  \bug UBCSAT 1.0.0 segfaults with weighted MaxSAT instances
  <ul>
   <li> Given the input file ("test.wcnf")
   \verbatim
p wcnf 2 4
200 1 2 0
200 1 -2 0
1 1 0
1 2 0 
   \endverbatim
   and running
   \verbatim
ubcsat-okl -alg gsat -w -i test.wcnf
   \endverbatim
   or
   \verbatim
ubcsat -alg gsat -w -i test.wcnf
   \endverbatim
   segfaults on MG's machine.
   </li>
   <li> Using ubcsat 1.1.0 doesn't segfault. </li>
   <li> Typically this behaviour depends on the algorithms, so gsat can't be
   used, but hopefully other algorithms. </li>
   <li> With ubcsat version 1.2 we hope that this problem disappears. </li>
  </ul>


  \todo Make clear the required package for dos2unix
  <ul>
   <li> Some linux distributions (such as Ubuntu, and (therefore)
   presumably Debian) do not include dos2unix by default
   and the only packaged versions of the utility they have (in 
   particular the tofrodos package which until recently created
   dos2unix and unix2dos aliases - 
   http://www.thefreecountry.com/tofrodos/index.shtml) do not match the 
   "correct" version, as used by the OKlibrary. </li>
   <li> As such alternative packages, even when creating aliases oneself,
   do not have certain options available, as well as the fact that the 
   "correct" dos2unix package is not clear from a standard google search, 
   we should provide a link to the package home page somewhere within the 
   documentation or plans to make it clear where to obtain this package. </li>
   <li> The correct package is available at 
   http://www.xs4all.nl/~waterlan/dos2unix.html . </li>
  </ul>

*/

