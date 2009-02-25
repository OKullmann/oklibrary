// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo ubcsat-okl
  <ul>
   <li> See ExperimentSystem/plans/RunUBCSAT.hpp for plans on extending
   the functionality. </li>
   <li> The idea is that ubcsat-okl creates some form of complete output,
   which is directly readable by R (into a dataframe, without further
   processing). </li>
   <li> By appropriate formatting one also might make it better readable
   for the human reader. </li>
   <li> Since we reformat the output, the wrapper should likely reformat
   every single output line individually (as it comes, so that one can
   read into R also partial results (from the intermediate file)). </li>
   <li> The following seems reasonable as a "ubcsat-okl" script:
   \verbatim
# Work out algorithm argument
ALG_ARG_P=1; for arg_p in `seq 1 $#`; do 
  if [[ ${!arg_p} == "-alg" ]] ; then ALG_ARG_P=`expr $arg_p + 1`; fi;
done
ALG=`printf '%34s' ${!ALG_ARG_P}`
echo "       sat  min     osteps     msteps       seed                                alg"
ubcsat -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] $* | sed -e "s/^\\(\\( \\+[0-9]\\+\\)\\{6\\} *\\)$/\\1${ALG}/"
   \endverbatim
   This appends the algorithm as a column to the data, line by line. </li>
   <li> DONE
   A problem appending columns line by line to ubcsat output is that 
   ubcsat version 1.0.0 doesn't flush data, and so one would have to wait until
   ubcsat has finished before viewing results. See "Ubcsat does not flush the
   output-buffer" below for how to repair this. </li>
   <li> Another point is how to handle statistics as they are only printed
   at the end. </li>
  </ul>


  \todo DONE (we wait for the successor of 1.1.0)
  Estimating the problem with version 1.1.0
  <ul>
   <li> Most important seems to obtain a clear idea on how much work
   is needed to
    <ol>
     <li> first get Ubcsat compiling and running correctly (independent of
     the machine type); </li>
     <li> and second to make the step counter 64 bit. </li>
    </ol>
   </li>
   <li> Depending on this estimation we need then to decide how to
   proceed. </li>
   <li> Regarding the first step, we should see what we did with version 1.0.0,
   and compare it with the new version. </li>
   <li> We should also run the newest gcc on it, with all warnings turned on.
   </li>
   <li> Perhaps also the flint-tool can help. </li>
   <li> The problematic points should be bit-operations and pointer-operations
   (and also index-operations). </li>
   <li> If the first step can be done with reasonable effort then we should
   do it. </li>
   <li> Of course, a question is whether just small corrections are sufficient,
   or whether larger changes are needed, in which case to fork the whole
   Ubcsat code might be an option. </li>
  </ul>
  

  \bug DONE (perhaps this had to do with not having "biarch"-gcc versions;
  in any way we wait for 1.2.0)
  Also "corrected" Ubcsat-executable segfaults (with gcc 4.3.1)
  <ul>
   <li> After building Ubcsat (1.0.0) with gcc 4.3.1 we get
   \verbatim
OKplatform> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_41.cnf
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12: 28315 Segmentation fault      ubcsat -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] $*
   \endverbatim
   </li>
   <li> Strange that also after replacing the ubcsat-executable with one
   built on a 64-bit machine (cs-wsok) and which works, we get a similar
   problem:
   \verbatim
> ubcsat-okl -alg samd -cutoff 20000000 -runs 100 -i Ramsey_5_5_2_41.cnf
/compsci/saturn/staff/csoliver/OKplatform/bin/ubcsat-okl: line 12: 28461 Segmentation fault      ubcsat -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] $*
   \endverbatim
   ??? </li>
   <li> One would guess that this reveals an error which will not show up
   on every platform? Perhaps on that "cscharon" (the machine above) actually
   no 32-bit compatibility mode is run, but true 64-bit ?? </li>
  </ul>


  \bug DONE (changing the permissions after unpacking)
  Ubcsat source files are read-only
  <ul>
   <li> The permissions of the sources files in "src/" for the Ubcsat source
   tarball are "444":
    <ol>
     <li> this causes issues when they are copied during any
     rebuild (therefore keeping the same permissions) to the 
     "Installations/SAT/Ubcsat" directory, as the user then doesn't have 
     permission to overwrite a read-only file. </li>
     <li> More precisely, the issue at hand is the behaviour of "dos2unix",
     which apparently on some machines creates writeable files, on some
     others keeps the original permissions. </li>
    </ol>
   </li>
   <li> It seems two solutions here are to either 
    <ol>
     <li> Use "cp -f" rather than "cp" to force removal of the files first 
     rather than simply overwriting them. </li>
     <li> "chmod" the files during the build to 644. </li>
    </ol>
   </li>
  </ul>


  \bug Cutoff value etc. should be 64 bits on a 64-bit machine
  <ul>
   <li> Yet "unsigned int" is used for example for the cutoff-value,
   not allowing big experimentation. </li>
   <li> Tell the Ubcsat people. </li>
  </ul>


  \todo Investigating the weak performance of Ubcsat on OK's 32-bit machine
  <ul>
   <li> See "Bounds on NR([q,q],2)" in
   Experimentation/Investigations/plans/RamseyProblems.hpp on some data
   on the problem. </li>
   <li> We should send the Ramsey-instance to the Ubcsat-people, to see
   whether they think the performance on csltok is pretty bad (or perhaps
   the performance on cs-wsok is pretty good). </li>
  </ul>


  \todo Building Ubcsat (OK, ML)
  <ul>
   <li> The Ubcsat installation seems pretty up-to-date, but we should
   check the details (only concerning 1.0.0).
    <ol>
     <li> Make-variables should be renamed, replacing "-" by "_". </li>
    </ol>
   </li>
   <li> See "Installation of UBCSAT completed" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> DONE (updated to the new system, where only the "recommended"
   version number is provided)
   Yet the installation of a different version than the "recommended"
   version wouldn't work, since the directory-names contain the recommended
   version number (and not the one dictated by the target).
    <ol>
     <li> One could leave it like that --- if a version different from
     the recommended shall be used then the recommended version needs
     to be overridden. </li>
     <li> But then we don't need to provide different targets. </li>
    </ol>
   </li>
   <li> DONE Remove the hardcoded version-numbers. </li>
   <li> DONE (removed)
   What is the meaning of "tag" and "tag_" ??? The names should tell. </li>
   <li> No file modification in the external sources. DONE </li>
  </ul>


  \todo DONE (waiting for 1.2.0)
  Update to version 1.1.0
  <ul>
   <li> First we try what happens on 32-bit and 64-bit machines.
    <ol>
     <li> On cs-wsok (64bit): The provided executable seems to work, but
     the compiled executable yields an error:
     \verbatim
builds/SAT/Ubcsat> ./ubcsat -alg rnovelty+ -i ~/SAT-Algorithmen/OKplatform/GreenTao_2_5_33500.cnf
#
# UBCSAT version 1.1.0 (Sea to Sky Release)
#
# http://www.satlib.org/ubcsat
#
# ubcsat -h for help
#
Error: Invalid Literal [-31473] in clause [0]
     \endverbatim
     Likely this is due to incorrect assumptions on integer types. </li>
    </ol>
   </li>
   <li> We should be able to instruct the compiler to build for a 32-bit
   machine?! Perhaps "-m32"? The executable provided with the
   1.1.0-distribution seems to work mostly (see below) on all 32- and
   64-bit machines. </li>
   <li> And apparently the executable compiled on cs-wsok (64-bit, AMD) seems
   to work on "all" machines. </li>
   <li> Though "-m32" works on cs-wsok (64-bit, AMD), but we get a compilation
   error
   \verbatim
/usr/lib64/gcc/x86_64-suse-linux/4.3/../../../../x86_64-suse-linux/bin/ld: skipping incompatible /usr/lib64/gcc/x86_64-suse-linux/4.3/libgcc.a when searching for -lgcc
/usr/lib64/gcc/x86_64-suse-linux/4.3/../../../../x86_64-suse-linux/bin/ld: cannot find -lgcc
collect2: ld returned 1 exit status
   \endverbatim
   on an another 64-bit machine (Intel)? </li>
   <li> So perhaps for now we just use the provided executable: We get a
   segfault when finally analysing the data (and computing the maximum
   number of steps), but this is not such a big problem (the data analysis
   then is done using R). </li>
   <li> We should download the now available documentation. </li>
   <li> OK and MG must get in contact with the Ubcsat-group. </li>
   <li> See "Contact the developers of Ubcsat" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
  </ul>


  \todo Speed
  <ul>
   <li> We should find out about the speed of the various versions (1.0.0
   versus 1.1.0, the provided executable versus the one build by our system
   versus one build with "-m32", 32-bit versus 64-bit). </li>
   <li> Yet it seems the only difference is between 32 and 64 bits. </li>
   <li> Let's use
   \verbatim
ubcsat -alg samd -i Ramsey_5_2_40.cnf -runs 10 -cutoff 5000 
   \endverbatim
   as the benchmark (reading off the "FlipsPerSecond" (FPS)). </li>
   <li> On csltok (32 bit) we have the problem that, apparently due to heating
   problems (real or fake), the processor runs with various clock-frequencies,
   and this not controllable (under Suse 9.2). But it seems that the slowest
   execution is constantly ~ 270 FPS, while the fastest execution is
   constantly ~ 1200 FPS (which should correspond to 1700 MHz). </li>
   <li> On cs-wsok (64 bit) we have between 2400 and 2800 FPS for our
   executable (1.0.0), between 2100 and 2200 FPS both for 1.1.0 with -m32
   or as provided with the distribution --- so native compilation yields
   better results (between 10%-30%)! </li>
   <li> On cscarme (64-bit, 2.8 GHz) we have around 2500 FPS (for 1.1.0-
   executable). </li>
  </ul>


  \todo Complete docu-file : DONE


  \todo DONE (see above)
  Check the new Ubcsat version. ?? There doesn't seem to exist one??


  \bug Ubcsat does not flush the output-buffer DONE
  <ul>
   <li> Flushing the output buffer therefore is left to the operating system. 
   </li>
   <li> Problems then occur when redirecting output to a file (i.e the output
   buffer is not flushed at all until the end). </li>
   <li> Ubcsat seems to have a "-rflush" option, which flushes the output 
   buffer after each run, but this was introduced in version 1.1.0 (See
   http://www.satlib.org/ubcsat/revisions.txt ). </li>
   <li> DONE The solution is simply to find the place in the Ubcsat source code
   where the result line is output, and adding a buffer-flush. </li>
  </ul>

*/

