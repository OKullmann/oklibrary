// Oliver Kullmann, 21.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/BranchingTuples/plans/general.hpp
  \brief Plans regarding the tau function (see SAT-handbook article of OK)
  
  
  \todo Tau for Maxima
  <ul>
   <li> The demo-file could be called "demos/Basic.hpp", using doxyen comments?!
   </li>
   <li> Defining tau using the precise but slow interval-halving:
    <ol>
     <li> How can "find_root" be instrumented to use bfloat? Or should we
     implement find_root ourselves? </li>
     <li> How to define examples for functions? DONE (creating demo-files,
     loading them by "batch") </li>
     <li> And it should be possible to define a variation which doesn't
     take a list but a variable number of arguments (for the tuple-values). DONE
     (define the n-ary function taun, and for plotting the fixed 2-ary resp.
     3-ary function tau2, tau3) </li>
     <li> One has to do something here in case lower and upper bound are already
     very close. DONE (using the sign-check) </li>
     <li> Perhaps one should define chi(t) as a function x -> apply("+", x^(-t)).
     DONE </li>
    </ol>
   </li>
   <li> Using the faster Newton-method:
    <ol>
     <li> Can a higher precision be used?
      <ol>
       <li> The problem seems to be that "newton" only uses type float ? </li>
      </ol>
     </li>
     <li> What is the precision of float? It seems that actually 64-bit numbers
     are used? </li>
     <li> Does the newton-procedure know how to differentiate the expression?
     Perhaps. </li>
     <li> Initial value: DONE (correct as given)
      <ol>
       <li> Can cyclic behaviour happen for our use of the initial value,
       given by the arithmetic mean? </li>
       <li> The problem in using tau_up is that the power-means are numerically
       complicated to compute. </li>
       <li> An alternative simple upper bound is obtained by scaling the tuple
       to minimal value 1, where then the width of the tuple is an upper
       bound on the tau-value.
       \verbatim
tau_eps(t,eps) := block([l : length(t)], 
 if l = 0 then inf elseif l = 1 then 1 else
  block([m : lmin(t), s, xl], s : t / m,
   return(newton(chi(s,xl)-1, xl, l, eps)^(1/m))))$
       \endverbatim
       </li>
       <li> However already for t=[1,2,3,4] this yields a
       "floating point overflow" ???
       \verbatim
newton(chi([1,2,3,4],x)-1,x,4,10^(-1));
Maxima encountered a Lisp error:
floating point overflow
       \endverbatim
       </li>
       <li> Perhaps actually in the given representation of the function as
       monotically decreasing, we actually should start with a lower bound?!!
       YES, that's the case: Due to the convexity monotonic convergence
       is guaranteed, while for an initial value larger than the root
       the derivative is close to zero, and the Newton-method becomes
       unusable.
       </li>
      </ol>
     </li>
     <li> If all arguments are integers, then actually an expression is returned.
     So well. DONE (the "default"-version tau_nwt converts the input
     into floating point) </li>
     <li> Are there reasonable default values for eps? Perhaps the maximal
     precision? DONE (used the apparently maximal precision) </li>
     <li> Again, one should define a variant without lists (but variadic). DONE
     </li>
    </ol>
   </li>
   <li> The general convention should be, that for example the branching tuples
   contain real terms, and then perhaps via some special naming convention we
   indicate that for this version all arguments are automatically evaluated?
   </li>
   <li> The experience we have made already should be extracted:
    <ol>
     <li> To the FullDocumentation-file? </li>
     <li> There are many questions, so a more open format is needed --- perhaps
     ComputerAlgebra/docus/Maxima.hpp. </li>
    </ol>
   </li>
   <li> How to make these definitions available? DONE (it seems that explicit
   load-instructions have to be issued for each Maxima-session) </li>
  </ul>


  \todo Reimplement the remaining functionality from Mupad/tau.mup in Maxima
  <ul>
   <li> tau : DONE
   <li> tree probability distributions:
    <ol>
     <li> tpd_flatten : DONE </li>
     <li> tpd_moment : DONE </li>
     <li> tpd_tuples </li>
     <li> td_combine </li>
     <li> tpd_ccombine </li>
     <li> tpd_ccmoment </li>
    </ol>
   </li>
  </ul>


  \todo Trees
  <ul>
   <li> Some of the tree functions should go to a general module
   on trees. </li>
   <li> For example these functions can be applied to splitting trees
   (see ComputerAlgebra/Satisfiability/Lisp/Backtracking/SplittingTrees.mac).
   </li>
  </ul>


  \todo Branching tuples : DONE
  <ul>
   <li> DONE We should develop some general helper functions for branching tuples t,
   and reimplement the tau-functions using those.
    <ol>
     <li> DONE A branching tuple is a list of positive real numbers,
     where in case of width 1 also 0 is allowed (excluded for "non-zero
     branching tuples"). </li>
     <li> DONE There are power means and their special cases;
     do we need special functions here? Like  bt_mean(t) := mean(t) ? </li>
     <li> bt_min(t) := lmin(t) DONE </li>
     <li> bt_max(t) := lmax(t) DONE </li>
     <li> bt_sum(t) := apply("+", t) DONE </li>
     <li> bt_width(t) := length(t) DONE </li>
     <li> DONE (those functions can be used, but there is no must ---
     we haved fixed the representation once and for all)
     Is it worth to introduce all the above special abbreviations ?
      <ul>
       <li> It adds a certain meaning. </li>
       <li> But perhaps it is better handled by implicit conversion of
       branching tuples into lists, and then applying the standard functions. </li>
       <li> So perhaps we do not introduce min, max, sum, width here, only
       in the documentation we mention them. </li>
       <li> However always having to write "apply("+", t)" is ugly, and so
       we should introduce a general convenience function here (doesn't it
       already exist??). </li>
      </ul>
     </li>
     <li> DONE (not needed)
     Perhaps we have checks
      <ol>
       <li> is_bt(a) (all entries are real and > 0, or a = (0)) </li>
       <li> is_nz_bt(a) (not (0)) </li>
       <li> is_ne_bt(a) (not empty) </li>
       <li> is_s_bt(a) (strict bt, i.e., nz and ne). </li>
      </ol>
     </li>
     <li> DONE The concatenation of branching tuples is just the concatenation of
     lists, so again we do not need to introduce a special function. </li>
     <li> DONE However composition of branching tuples a, b at position i of
     a is needed: bt_composition(a,b,i). </li>
    </ol>
   </li>
   <li> DONE (called now "Basic.mac")
   Creating a new file "BranchingTuples.maxima". </li>
   <li> DONE Perhaps the module should be called "BranchingTuples" instead of
   "TauMachinery". </li>
  </ul>

*/

