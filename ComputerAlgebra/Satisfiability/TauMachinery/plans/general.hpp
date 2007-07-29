// Oliver Kullmann, 21.7.2007 (Swansea)

/*!
  \file ComputerAlgebra/Satisfiability/TauMachinery/plans/general.hpp
  \brief Plans regarding the tau function (see SAT-handbook article of OK)
  
  
  \todo Tau for Maxima
  <ul>
   <li> Defining tau using the precise but slow interval-halving:
    <ol>
     <li> The demo-file could be called "demos/Basic.hpp", using
     doxyen comments?! </li>
     <li> How to define examples for functions? DONE (creating demo-files,
     loading them by "batch")
     </li>
     <li> And it should be possible to define a variation which doesn't
     take a list but a variable number of arguments (for the tuple-values). DONE (define the
     n-ary function taun, and for plotting the fixed 2-ary resp. 3-ary function tau2, tau3) </li>
     <li> One has to do something here in case lower and upper bound are already very
     close. DONE (using the sign-check) </li>
     <li> Perhaps one should define chi(t) as a function x -> apply("+", x^(-t)). DONE </li>
    </ol>
   </li>
   <li> Using the faster Newton-method:
    <ol>
     <li> Can a higher precision be used? </li>
     <li> What is the precision of float? It seems that actually 64-bit numbers
     are used? </li>
     <li> Does the newton-procedure know how to differentiate the expression?
     Perhaps. </li>
     <li> If all arguments are integers, then actually an expression is returned.
     So well. DONE (the "default"-version tau_nwt converts the input
     into floating point) </li>
     <li> Are there reasonable default values for eps? Perhaps the maximal
     precision? DONE (used the apparently maximal precision) </li>
     <li> Again, one should define a variant without lists (but variadic). DONE </li>
    </ol>
   </li>
   <li> How to make these definitions available? DONE (it seems that explicit load-instructions have
   to be issued for each Maxima-session) </li>
  </ul>


  \todo Reimplement the remaining functionality from Mupad/tau.mup in Maxima
  <ol>
   <li> tau : DONE
   <li> probability </li>
  </ol>

*/

