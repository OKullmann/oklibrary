// Oliver Kullmann, 22.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
  \brief Plans regarding building packages related to boolean functions

  See OKlib/Satisfiability/FiniteFunctions/plans/general.hpp for
  the central module regarding boolean functions (and generalisations).


  \todo BDD's
  <ul>
    <li> CUDD http://vlsi.colorado.edu/~fabio/CUDD/ </li>
    <li> Buddy http://sourceforge.net/projects/buddy </li>
  </ul>


  \todo Aiger
  <ul>
   <li> AIGER http://fmv.jku.at/aiger contains tools for handling AIGs
   (special boolean circuits). </li>
  </ul>


  \todo ABC
  <ul>
   <li> http://www.eecs.berkeley.edu/~alanmi/abc/ </li>
   <li> Seems to be rather comprehensive. </li>
  </ul>


  \todo Translations to SAT
  <ul>
   <li> MNI Proposition Analyzer (MPA)
   http://homepages.fh-giessen.de/~hg11260/hp_p7_de.html
   seems to be a smaller program, translating various forms of propositional
   SAT problems into CNF, and/or solving them directly (based on SAT4J). </li>
   <li> See "BAT" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/ModelChecking.hpp. </li>
  </ul>


  \todo Boolean functions
  <ul>
    <li> DONE (now automatically installed)
    There is an R-package with Quine/McCluskey etc.
     <ol>
      <li> Installation (from CRAN)
      \verbatim
> oklib --R
# Within R environment
> install.packages("QCA")
      \endverbatim
      This draws the package from external CRAN sources. The lpSolve package 
      dependency seems to be already installed for R in oklib.
      </li>
      <li> Installation (from source tarball)
      \verbatim
ExternalSources/Installations/R> oklib --R
# Within R environment (with  QCA_0.5-0.tar.gz in the current directory)
> install.packages("QCA_0.5-0.tar.gz", repos=NULL)
      \endverbatim
      Setting the "repos" parameter to null seems to force it to treat the 
      package arguments as file paths to tar.gz packages.
      </li>
     </ol>
    </li>
    <li> http://www.dei.isep.ipp.pt/~ACC/bfunc/ seems not to be open source.
    </li>
    <li> http://www.tcs.hut.fi/~tjunttil/circuits/index.html has translators
    and
    generators for boolean circuits. </li>
  </ul>


  \todo Logic synthesis
  <ul>
   <li> http://vlsicad.eecs.umich.edu/BK/Slots/slots/LogicSynthesis.html </li>
   <li> Scherzo
    <ol>
     <li> Developed by Olivier Coudert (http://www.ocoudert.com/). </li>
     <li> Uses implicit BDD representation for primes and reduction
     techniques to avoid blow up in size of problem due to large
     numbers of prime implicates. </li>
     <li> Coudert has provided the code to MG, although it is not
     clear whether he would be willing to make it open-source. </li>
     <li> The source builds with some minor Makefile changes but
     segfaults. This seems likely to be an issue with how MG is building
     it and the difference in architectures (Scherzo seems to have been
     developed for systems other than standard x86 / x86_64). </li>
     <li> MG needs to e-mail Coudert again to check whether he would
     be willing to open-source the application. </li>
    </ol>
   </li>
   <li> BOOM
    <ol> 
     <li> Developed by Petr Fiser 
     (http://service.felk.cvut.cz/vlsi/prj/BOOM/). </li>
     <li> Non-exact boolean minimisation, but allows iterative
     minimisation. </li>
     <li> According to experimental results on the project site,
     the software is highly competitive. </li>
     <li> License requires authors reference particular papers if
     one uses the software, and one must provide details to download
     the software (MG hasn't done this). </li>
    </ol>
   </li>
   <li> Using genetic programming:
    <ol>
     <li> There are quite a few papes on finding (digital) circuits via
     genetic programming, but apparently no software. </li>
    </ol>
   </li>
   <li> Grigory Yaroslavtsev and Alexander Kulikov have written some software
   to find short circuits (over the full binary base for example). </li>
   <li> egntott http://code.google.com/p/eqntott/ </li>
  </ul>


  \todo Improve documentation for Espresso
  <ul>
   <li> We should improve the source-code, so that the option "--version"
   is recognised. </li>
   <li> At the ExternalSources page we need an index-page for the various
   pieces of documentation we have. </li>
   <li> We need to investigate what is in espresso-book-examples.tar.gz:
    <ul>
     <li> This tarball contains a variety of different PLA files, which
     don't have any documentation along with them. </li>
     <li> There is a book "Logic Minimization Algorithms for VLSI Synthesis"
     by Robert Brayton (the author of Espresso). </li>
     <li> Given the name of the tarball, "Logic Minimization Algorithms for
     VLSI Synthesis" seems a likely candidate for the "book" the filename
     refers to (MG has ordered a copy). </li>
    </ul>
   </li>
   <li> The PLA format:
    <ul>
     <li> The "PLA format" is documented at
     ExternalSources/sources/Boolean/Espresso/espresso.5.html with additional
     information in
     ExternalSources/sources/Boolean/Espresso/man_octtools_espresso.html. </li>
     <li> Finite functions with boolean variables:
      <ul>
       <li> A PLA represents a partial finite function,
       f : {0,1}^n -> {0,1}^m, by encoding the DNF of the relational
       viewpoint of f. </li>
       <li> By the relational viewpoint, we mean representing the boolean
       function which takes both input and output bits and returns true iff
       f maps the given input to the given output. More precisely,
       representing f' : {0,1}^(n+m) -> {0,1} such that f'((I,O)) = 1 if
       f(I) = O. </li>
       <li> The "header" of the PLA file:
        <ul>
         <li> The number of input and output variables are
         specified first with lines of the form:
         \verbatim
.i n
.o m
         \endverbatim
         where n and m are the positive integer number of input and output
         variables. </li>
         <li> Additional lines such as ".type" (discussed below), and
         others discussed in the documentation, may also be specified in
         the "header" of the file. </li>
        </ul>
       </li>
       <li> The "clauses":
        <ul>
         <li> Each line after the header is:
          <ol>
           <li> a sequence of characters I in {1,0,-} of size n; followed by </li>
           <li> a space; followed by </li>
           <li> a sequence of characters O in {1,0,-} of size m. </li>
          </ol>
         where:
          <ul>
           <li> Each line encodes a DNF clause which is part of a DNF
           representation for the partial finite function using the
           relational viewpoint. </li>
           <li> That is, each line encodes that f(I') = O', where I' and O'
           are the boolean vectors corresponding to the strings I and O for
           that line. </li>
           <li> A "-" in I indicates that f(I') = O' for both values of the
           corresponding variable. </li>
           <li> A "-" in O indicates that f(I') = O' but the output value,
           for the variable the "-" corresponds to, is undefined.  </li>
          </ul>
       </li>
       <li> Additional options:
        <ul>
         <li> ".type X": specifies how to interpret total assignments left
         unspecified, where X is one of the following:
          <ul>
           <li> "f": unspecified and assignments with undefined ouputs are
           assumed to be false (0). </li>
           <li> "r": unspecified and assignments with undefined outputs are
           assumed to be true (1). </li>
           <li> "fd": unspecified assignments are assumed to be false
           (the default for espresso). </li>
           <li> "fr": unspecified assignments are considered to be
           "don't care" values. </li>
           <li> "fdr": there should be no unspecified values. </li>
          </ul>
         </li>
         <li> The PLA standard also allows one to provide names for variables,
         as well as other options which give Espresso information on how
         to minimise the finite function provided. These options are discussed
         in </li>
        </ul>
       </li>
       <li> Lines in a PLA file starting with the "#" symbol are comments. </li>
       <li> Note here that there is a difference between "undefined" and
       "unspecified". </li>
       <li> One can use "-" for an output bit to explicitly *specify* that
       the partial finite function defined by the PLA is undefined for the
       corresponding input bits. A total assignment is unspecified if there
       is no espresso line which specifies its value. </li>
       <li> Examples:
        <ul>
         <li> The DNF {{1,2,3},{-1,4,-5}} becomes
         \verbatim
.i 5
.o 1
111-- 1
0--10 1
         \endverbatim
         </li>
         <li> The two bit adder (from the espresso documentation):
         \verbatim
# 2-bit by 2-bit binary adder (with no carry input)
.i 4
.o 3
0000  000
0001  001
0010  010
0011  011
0100  001
0101  010
0110  011
0111  100
1000  010
1001  011
1010  100
1011  101
1100  011
1101  100
1110  101
1111  110
         \endverbatim
         </li>
        </ul>
       </li>
       <li> At the maxima level:
        <ul>
         <li> DONE (see output_fcl2pla in
         ComputerAlgebra/Satisfiability/Lisp/ClauseSets/BasicOperations.mac)
         We should provide functions to take a formal clause-list F
         and output a PLA file. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> Finite functions with multi-valued variables:
      <ul>
       <li> The PLA format allows one to specify that certain boolean
       variables should be considered together as a single multi-valued
       variable. </li>
       <li> Rather than using ".i" and ".o" to specify the number of input
       and output variables, one specifies:
       \verbatim
.mi n nb d1 ... dm
       \endverbatim
       where n is the total number of variables, nb is the number of boolean
       variables (always given first in the variable ordering), and di
       is the number of bits used to represent the i'th multi-valued variable.
       </li>
       <li> The function is then specified as before, but the bits in
       multi-valued variables are separated by "|" and the last multi-valued
       variable is the output value (no space-separation). </li>
       <li> For example (from the manual):
       \verbatim
.mv 8 5 2 3 4
0-010|10|100|0000
10-10|10|010|1000
       \endverbatim
       </li>
       <li> We need to better understand multi-valued PLAs and relate it
       to our own considerations of non-boolean clause-sets etc. </li>
      </ul>
     </li>
     <li> All remaining options allowed in a PLA file should be documented
     here and then this later moved to docus. </li>
    </ul>
   </li>
   <li> We need to fully specify (in our system, using our language) what
   "PLAs" are, and create documentation for that. Perhaps we also need that
   at Maxima-level. </li>
  </ul>


  \bug DONE (added additional entries to the configuration specific
  to the source directories)
  False build for Espresso
  <ul>
   <li> The directory names do not follow our standards. </li>
   <li> In particular, espresso has the package name
   "espresso-ab-1.0-2.3.tar.gz" and directory names
   "espresso-ab-1.0", and yet it's executable name is
   "espresso2.3", all of which are very different. </li>
   <li> This initial decision was made so as not to change
   the version information too much from the original (keeping
   with the authors original version information (1.0)) but adding
   more reasonable version information at the same time (the
   version number as printed by the executable). </li>
   <li> To bring things inline with other packages, espresso
   should simply be repackaged to "espresso-2.3.tar.bz2" with
   "espresso-2.3" as the directory name and "espresso2.3" as
   the executable name. </li>
  </ul>

*/

