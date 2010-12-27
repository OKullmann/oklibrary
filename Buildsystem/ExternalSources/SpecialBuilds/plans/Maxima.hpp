// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Installation of version 5.23.0
  <ul>
   <li> Installation fails:
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
   <li> This happens on cs-wsok, where command-lines can't be very long; on
   cs-oksvr the build succeeds, however then no loading works:
   \verbatim
cs-oksvr:~/OKplatform> maxima_recommended_version_number_okl=5.23.0 oklib --maxima
Maxima 5.23.0 http://maxima.sourceforge.net
using Lisp ECL 10.4.1
(%i1) load(graphs);
file_search1: "grcommon.lisp" not found in file_search_maxima,file_search_lisp.
   \endverbatim
   </li>
   <li> According to Raymond Toy the precison-problems below are likely not
   solved with this new version. </li>
  </ul>


  \todo Installation of version 5.22.1
  <ul>
   <li> Floating-point problem:
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
     <li> Examples, contrasting 5.21.1 with 5.22.1:
     \verbatim

testf1(n) := float(apply("+",create_list(log(i)*(-1)^i,i,1,n)));
testf1c(n):=block([fpprec:30],bfloat(apply("+",create_list(log(i)*(-1)^i,i,1,n))));
testf1f(n) := apply("+",create_list(float(log(i))*(-1)^i,i,1,n));

21:

(%i24) testf1(10000);
Evaluation took 79.0520 seconds (125.7410 elapsed)
(%o24) 4.830986538632788
(%i25) testf1c(10000);
Evaluation took 75.6320 seconds (141.3070 elapsed)
(%o25) 4.8309865386327771337329138576b0
(%i31) testf1f(10000);
Evaluation took 0.3380 seconds (0.4770 elapsed)
(%o31) 4.830986538632788

(%i26) testf1(20000);
Evaluation took 301.3270 seconds (546.0920 elapsed)
(%o26) 5.177547628912792
(%i27) testf1c(20000);
Evaluation took 326.1730 seconds (542.8570 elapsed)
(%o27) 5.17754762891278624677437887665b0
(%i32) testf1f(20000);
Evaluation took 0.6810 seconds (1.0060 elapsed)
(%o32) 5.177547628912792

(%i28) testf1(40000);
Evaluation took 1244.4280 seconds (2258.0720 elapsed)
(%o28) 5.524114969192852
(%i29) testf1c(40000);
Evaluation took 1256.1750 seconds (2077.7460 elapsed)
(%o29) 5.52411496919276345877464646714b0
(%i33) testf1f(40000);
Evaluation took 1.3840 seconds (2.1210 elapsed)
(%o33) 5.524114969192852

22:

(%i19) testf1(10000);
Evaluation took 68.2900 seconds (123.3390 elapsed)
(%o19) 4.83098653863282
(%i20) testf1c(10000);
Evaluation took 76.8890 seconds (123.0450 elapsed)
(%o20) 4.8309865386327771337329138576b0
(%i26) testf1f(10000);
Evaluation took 0.3280 seconds (0.6130 elapsed)
(%o26) 4.83098653863282

(%i21) testf1(20000);
Evaluation took 289.1520 seconds (468.5160 elapsed)
(%o21) 5.17754762891287
(%i22) testf1c(20000);
Evaluation took 286.7340 seconds (559.2420 elapsed)
(%o22) 5.17754762891278624677437887665b0
(%i27) testf1f(20000);
Evaluation took 0.7100 seconds (1.2810 elapsed)
(%o27) 5.17754762891287

(%i23) testf1(40000);
Evaluation took 1175.0270 seconds (2232.6280 elapsed)
(%o23) 5.524114969192786
(%i24) testf1c(40000);
Evaluation took 1206.8040 seconds (1911.2210 elapsed)
(%o24) 5.52411496919276345877464646714b0
(%i28) testf1f(40000);
Evaluation took 1.3790 seconds (2.1250 elapsed)
(%o28) 5.524114969192786

     \endverbatim
     There are differences, but no clear picture. </li>
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
