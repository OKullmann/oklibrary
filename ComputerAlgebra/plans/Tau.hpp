// Oliver Kullmann, 21.7.2007 (Swansea)

/*!
  \file ComputerAlgebra/plans/Tau.hpp
  \brief Plans regarding the re-implementation of tau.mup
  (compare ComputerAlgebra/Mupad/plans/general.hpp)
  
  
  \todo Maxima
  <ul>
   <li> Defining tau using the precise but slow interval-halving:
   \verbatim
load (descriptive)$ 
tau(t) := find_root(apply("+", x^(-t))=1, x, length(t)^(1/mean(t)), length(t)^(1/lmin(t))); 
   \endverbatim
    <ol>
     <li> One has to do something here in case lower and upper bound are already very
     close. </li>
     <li> And it should be possible to define a variation which doesn't
     take a list but a variable number of arguments (for the tuple-values). </li>
    </ol>
   </li>
   <li> Perhaps one should define chi(t) as a function x -> apply("+", x^(-t)). </li>
   <li> Using the faster Newton-method:
   \verbatim
load (newton1);
taun(t,eps) := newton(apply("+", x^(-t))-1, x, length(t)^(1/mean(t)), eps);
   \endverbatim
    <ol>
     <li> Again, one should define a variant without lists (but variadic). </li>
     <li> Are there reasonable default values for eps? </li>
     <li> Can a higher precision be used? Apparently now. </li>
     <li> If all arguments are integers, then actually an expression is returned.
     So well. </li>
     <li> Does the newton-procedure know how to differentiate the expression?
     Don't think so, but apparently one can't tell the newton-procedure
     about it? </li>
    </ol>
   </li>
   <li> How to make these definitions available? </li>
  </ul>

*/

