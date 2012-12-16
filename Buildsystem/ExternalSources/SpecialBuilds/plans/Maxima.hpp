// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Memory restrictions
  <ul>
   <li> Why does the actual value of "Lisp stack limit" differ so much
   from the value of lisp_stack_ecl_okl ? </li>
   <li> And what are reasonable values for an 8 GB machine? </li>
  </ul>


  \todo External documentation
  <ul>
   <li> At the documentation page for Maxima we should make available more
   additional documentation. </li>
   <li> The book by Ted Woollett seems out-of-date. </li>
   <li> Foreign-language documentation:
    <ol>
     <li> http://maxima.sourceforge.net/docs/manual/de/maxima.html </li>
    </ol>
   </li>
  </ul>


  \todo Installation of version 5.23.2 : DONE
  <ul>
   <li> DONE (we need to update cs-wsok)
   Installation of 5.23.0 and 5.23.2 fails on cs-wsok:
   \verbatim
;;;   Invoking external command:
;;;   /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gcc/4.1.2/bin/gcc "-I/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Ecl/10.4.1/include/"  -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -g -O2 -fPIC -I/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gmp/4.1.2/5.0.1/include -Dlinux -O -w -c "/tmp/ECLINITyBHG4P.c" -o "/tmp/ECLINITyBHG4P.o"
;;; 
;;; Note:
;;;   Invoking external command:

XXX this line has 36861 characters XXX -Wl,-rpath=/home/csoliver/SAT-AlgorithmAn error occurred during initialization:
(SYSTEM "/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gcc/4.1.2/bin/gcc -o XXX this line has 37734 characters XXX -lecl  -lgmp -ldl  -lm ") returned non-zero value 127.
en/OKplatform/ExternalSources/Installations/Ecl/10.4.1/lib -Wl,-rpath,/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gmp/4.1.2/5.0.1/lib -L/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gmp/4.1.2/5.0.1/lib -lecl  -lgmp -ldl  -lm 
;;; make[2]: *** [binary-ecl/maxima] Error 1
make[2]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.23.0/src'
make[1]: *** [all-recursive] Error 1
   \endverbatim
   </li>
   <li> DONE (bug resolved)
   This happens on cs-wsok, where command-lines can't be very long; on
   cs-oksvr the build succeeds, however then no loading works:
   \verbatim
cs-oksvr:~/OKplatform> maxima_recommended_version_number_okl=5.23.0 oklib --maxima
Maxima 5.23.0 http://maxima.sourceforge.net
using Lisp ECL 10.4.1
(%i1) load(graphs);
file_search1: "grcommon.lisp" not found in file_search_maxima,file_search_lisp.
   \endverbatim
   </li>
   <li> DONE (we decreased the deficiency-demands)
   According to Raymond Toy the precison-problems below are likely not
   solved with this new version (see "Installation of version 5.22.1" below).
   </li>
  </ul>


  \todo Installation of version 5.22.1 : DONE
  <ul>
   <li> DONE (moved to ComputerAlgebra/TestSystem/Lisp/plans/general.hpp)
   Floating-point problem:
    <ol>
     <li> Failure of okltest_probsatrand(probsatrand) due to less precise
     computation:
     \verbatim
F : weak_php(2,3)$
p : probsatrand(F);
  1953/2048
float(p);
  0.95361328125
pa : exp(logprobrand(F));
  0.9536132812500007
assert_float_equal(p, pa);
  ASSERT: Expression " 6.661338147750939e-16 < 1/5000000000000000 " does not 
    evaluate to true.
float(1/5000000000000000);
  2.e-16
     \endverbatim
     </li>
     <li> Already below we experienced the diminished precision. How does this
     come? </li>
     <li> With version 5.21.1 (the current version) exp(logprobrand(F)) has
     the value 0.95361328125. This is quite a difference. </li>
     <li> Ask on the Maxima mailing list whether this is expected. </li>
     <li> A simple solution would be to raise the error-tolerance for comparing
     floating-point numbers from 2*10^-16 to 10^-15. </li>
     <li> This problem as well as the problem below has to do with logarithms
     and exponentials; is there some special problem here? </li>
     <li> With this change to assert_float_equal then we get no further
     test-failures with 5.22.1. </li>
    </ol>
   </li>
   <li> DONE Set problem:
    <ol>
     <li> The correct way of referring to the underlying order of a set is by
     orderlessp, not by <. </li>
     <li> This problem occurs likely at several places, but it's hard to search
     for them. </li>
     <li> So we just handle these errors as they come. </li>
    </ol>
   </li>
   <li> DONE Floating-point problem:
    <ol>
     <li> We get a test failure:
     variable_heuristics_tau([{1,2,3},{1,2,3,4}],identity) in
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/tests/ConstraintSatisfaction.mac
     should yield [1,[1,2,3]]), however it yields (now) [2,[1,2,3,4]] ? </li>
     <li> It seems that this is due to a changed behaviour of "sort". </li>
     <li> No, this is not the problem, but that with 5.22.1 two equal
     tau-values now compare as "strictly less". </li>
     \verbatim
> oklib --maxima

m0 : log(3*4);
t1 : tau(m0 - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0 - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

m0p : log(3) + log(4);
t1 : tau(m0p - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0p - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

> maxima_recommended_version_number_okl=5.22.1 oklib --maxima
m0 : log(3*4);
t1 : tau(m0 - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0 - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

m0p : log(3) + log(4);
t1 : tau(m0p - [log(4),log(4),log(4)]);
  2.718281828459046
t2 : tau(m0p - [log(3),log(3),log(3),log(3)]);
  2.718281828459046
is (t1 < t2);
  false
is (t2 < t1);
  true
is (t1 = t2);
  false

     \endverbatim
     </li>
     <li> 5.22.1 computes with less precision here:
     \verbatim
> oklib --maxima
t1 : tau([log(3),log(3),log(3)]);
  2.718281828459045
t2 : tau([log(4),log(4),log(4),log(4)]);
  2.718281828459045
is (t1 = t2);
  true
tau_hp([log(3),log(3),log(3)], 15);
  2.71828182845905b0
tau_hp([log(3),log(3),log(3)], 30);
  2.71828182845904523536028747135b0

> maxima_recommended_version_number_okl=5.22.1 oklib --maxima
t1 : tau([log(3),log(3),log(3)]);
  2.718281828459046
t2 : tau([log(4),log(4),log(4),log(4)]);
  2.718281828459046
is (t2 < t1);
  true
tau_hp([log(3),log(3),log(3)], 15);
  2.71828182845905b0
tau_hp([log(3),log(3),log(3)], 30);
  2.71828182845904523536028747135b0
     \endverbatim
     </li>
     <li> I think a reasonable solution is to compute more precise at
     Maxima-level, and so to use now for variable_heuristics_tau instead of
     sum_log_dom_size(dom) the value log_prod_dom_size(dom). </li>
    </ol>
   </li>
  </ul>


  \todo Install xgettext
  <ul>
   <li> To create new Maxima packages, xgettext, available at
   http://www.gnu.org/software/gettext/ , is needed </li>
  </ul>
  

  \todo Using other Lisps
  <ul>
   <li> CLisp is very slow, while Ecl has certain unprofessional aspects
   (inconsistent buildsystem, not reacting to e-mails). </li>
   <li> See "Installing other Lisp's" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp. </li>
  </ul>


  \todo Database of integer sequences
  <ul>
   <li> We have to see whether the sequences are becoming (publically)
   available. </li>
   <li> If this is not the case, then likely we shouldn't contribute to
   that database. </li>
  </ul>


  \todo Additional packages
  <ul>
   <li> Real algebraic geometry
    <ol>
     <li> The book [Algorithms in Real Algebraic Geometry; Basu, Pollack, Roy]
     is available at http://www.math.gatech.edu/~saugata/bpr-posted1.html.
     </li>
     <li> Also maxima-code for various chapters is available. </li>
    </ol>
   </li>
  </ul>


  \todo System-wide installation
  <ul>
   <li> The build-system should be extended, taking for example the
   Gmp-installation as an example, so that we can also produce/use in the
   standard way system-wide installations of Libsigsegv, CLisp and Maxima.
   </li>
  </ul>


  \todo Handling of redefined load-function
  <ul>
   <li> See "How to eliminate the annotation of lists" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> And see the e-mail thread (May 2008)
   "how to stop the annotation of lists by filenames?" on the Maxima
   mailing list. </li>
   <li> We need documentation on this regarding the build-system:
    <ol>
     <li> We need to watch the Maxima distribution, whether the
     two functions involved, "load" and "add-lineinfo", ever are
     changed, and then we need to incorporate these changes. </li>
     <li> Hopefully, this is not a big thing, since these functions
     are rather small. </li>
     <li> The good point about this solution is its non-intrusiveness:
     The Maxima system doesn't know about these changes. </li>
    </ol>
   </li>
  </ul>

*/
