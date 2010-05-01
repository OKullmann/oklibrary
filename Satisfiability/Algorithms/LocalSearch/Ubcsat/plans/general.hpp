// Oliver Kullmann, 1.5.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp
  \brief Plans regarding Ubcsat


  \todo Using Ubcsat as a library
  <ul>
   <li> Once the successor of version 1.1.0 is out (and we successfully
   installed it), we need to re-evaluate the possibility of using Ubcsat as
   an ordinary library. </li>
   <li> Relevant build-system-variables are ubcsat_installsrc_okl,
   ubcsat_include_option_okl, ubcsat_link_okl and ubcsat_link_option_okl. </li>
   <li> Yet we have to use the deprecated gcc-option "-I-" (see variable
   ubcsat_include_option_okl; the application using this is
   Satisfiability/Algorithms/Autarkies/Search/AnalyseTotalAssignment.cpp).
   </li>
  </ul>


  \todo Installation of UBCSAT completed
  <ol>
   <li> See "Building Ubcsat" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
   <li> Some documentation is needed (goes to
   Buildsystem/docus/ExternalSources.hpp):
    <ul>
     <li> how to use the binary </li>
     <li> how to use the library files </li>
     <li> what are those library files </li>
     <li> how to use LocalSearch/Ubcsat. </li>
    </ul>
   </li>
   <li> Temporary build-directory:
    <ol>
     <li> We could get rid off the temporary build-directory by adding
     \code
-I- -I$(OKsystem)/OKlib/LocalSearch/Ubcsat/corrected
     \endcode
     to the build-compilation. </li>
     <li> However this option is deprecated with gcc version 4.2. </li>
     <li> We should try to convince the Ubcsat-developers to use a standard
     directory structure. </li>
    </ol>
   </li>
  </ol>


  \todo Contact the developers of Ubcsat
  <ul>
   <li> See "Update to version 1.1.0" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
   <li> Wish-list for a new Ubcsat-release 1.2.0:
    <ol>
     <li> 64 bit:
      <ol>
       <li> Native 64-bit compilation should be available. </li>
       <li> It seems that our version (with native 64-bit compilation) is
       10%-30% faster than the executable provided by version 1.1.0. </li>
       <li> Higher cutoffs are needed: Yet "unsigned int" is used for example
       for the cutoff-value, not allowing big experimentation. </li>
       <li> Perhaps by default on 64-bit machines such counters should use
       64 bits. </li>
       <li> And one should also have the option to specify the counter size
       (so that on 32-bit machines one can also get 64-bit counters). </li>
      </ol>
     </li>
     <li> Weak performance on 32-bit machines:
      <ol>
       <li> An example is given by
       \verbatim
> Ramsey-O3-DNDEBUG 5 5 2 40 | ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_2-5-5-40.cnf
> ls -l Ramsey_2-5-5-40.cnf
-rw-r--r--  1 kullmann users 60031461 2010-05-01 22:04 Ramsey_2-5-5-40.cnf
       \endverbatim
       </li>
       <li> Considering "FlipsPerSecond" (FPS), on a 32-bit machine the
       performance is much weaker (nearly a factor of 2) than on 64-bit
       machines, where experiments are performed by e.g.
       \verbatim
> ubcsat-okl -alg samd -i Ramsey_2-5-5-40.cnf -runs 10 -cutoff 5000
       \endverbatim
       </li>
       <li> This is all, as everything here, about version 1.0.0 in our
       adoptation. </li>
      </ol>
     </li>
     <li> Improved directory structure:
      <ol>
       <li> Currently the includes use the " ... " form, however they
       should use the form
       \verbatim
#include <dir/file>
       \endverbatim
       </li>
       <li> Only in this way is it possible just by using an include-option
       for gcc to replace certain files by others (for example by corrected
       files in Ubcsat/corrected). </li>
       <li> Currently one can use option "-I-", but this option is deprecated
       with gcc version 4.2.0, without a proper replacement. </li>
      </ol>
     </li>
     <li> Input of large numbers
      <ol>
       <li> Especially for the cutoff value it would be good if number
       representations like "100*10^6" (or "55*11^7") could be used. </li>
       <li> It's awkward to count the zeros. </li>
      </ol>
     </li>
     <li> DIMACS return codes should be available (10 for SAT or 0 for
     unknown). </li>
     <li> Signal SIGINT should be caught, all remaining output should be
     performed, and only then the computation is aborted. </li>
     <li> Every output should be flushed (there is no reason not to flush
     always everything --- a user thinking he doesn't need it will experience
     the the day where he needs to abort a long computation without access to
     the output, and will realise that it was a trap). </li>
     <li> It should be possible to specify a partial assignment to start with.
     </li>
     <li> Option "-solve" is important, and shouldn't be coupled with the
     output of a satisfying assignment, but that should be handled by a
     differerent option (which also should allow to output it into a file).
     </li>
     <li> Table output:
      <ol>
       <li> We need some simple output format, which simply outputs all
       available data per run in table format, while omitting all summary
       statistics. </li>
       <li> One could use an option "--table-output". </li>
       <li> The aim is that from R with a simple read.table-command the whole
       data becomes available. </li>
       <li> So comment-lines start with "#". </li>
       <li> The first line should be a comment-line showing all the parameters
       (for reproduction purposes). </li>
       <li> Then we have a line with the names of the columns (without the
       first counter column), formatted in such a way that it reads nicely.
       </li>
       <li> And then comes the data. </li>
       <li> A simple example (the table-format should contain *all* data):
       \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 12000000 -i GreenTao_N_3-4-4-4_5300.cnf -solve | tee -a GreenTao_N_3-4-4-4_5300.cnf_OUT
# -alg rnovelty+ -runs 100 -cutoff 12000000 -i GreenTao_N_3-4-4-4_5300.cnf -solve
       sat  min     osteps     msteps       seed
      1 0    26    7723050   12000000 1068430341
      2 0    23    8864255   12000000  563811609
      3 0    13   11538566   12000000 3234664768
      4 0    13   10518459   12000000 2599548647
       \endverbatim
       <li> If there is additional output, then in comment-form. </li>
      </ol>
     </li>
     <li> Usage as library:
      <ol>
       <li> We have currently one application, namely
       Satisfiability/Algorithms/Autarkies/Search/AnalyseTotalAssignment.cpp.
       </li>
       <li> See changes in Ubcsat/local. </li>
       <li> Include-guards are needed. </li>
       <li> The ODR (One-Definition-Rule) needs to be obeyed, so variables
       need to be properly declared as extern. </li>
       <li> See our build-system
       (Buildsystem/ExternalSources/SpecialBuilds/ubcsat.mak). </li> 
      </ol>
     </li>
     <li> Use of C99: nowadays every C-compiler is a C99 compiler, and using
     the C99 possibilities (like the new parts of the library) would enhance
     the code-quality (and our user-experience). </li>
    </ol>
   </li>
  </ul>

*/
