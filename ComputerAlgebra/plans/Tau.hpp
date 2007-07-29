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

chi(t,x) := apply("+", x^(-t));

/* The tau-function: */
tau(t) :=
  block( [ lb : float(length(t)^(1/mean(t))), ub : float(length(t)^(1/lmin(t))) ],
    if (signum(chi(t,lb)-1) * signum(chi(t,ub)-1) >= 0) then
      return(ub)
    else
      return(find_root(chi(t,x)=1, x, lb, ub))
  );
taun([t]) := tau(t);
tau2(x,y) := taun(x,y);
tau3(x,y,z) := taun(x,y,z);
/* tau([1,2]) = taun(1,2) = tau2(1,2) = 1.618... */

/* The probability distribution derived from a branching tuple: */
tprob(t) := tau(t)^(-t);
tprobn([t]) := tprob(t);
tprob2(x,y) := tprobn(x,y);
tprob3(x,y,z) := tprobn(x,y,z);
/* tprob([5,5,5]) = tprobn(5,5,5) = tprob3(5,5,5) = [1/3, 1/3, 1/3] */

/* The total differential for the tau-function */
Dtau(t) := block( [ tv : tau(t)], block( [pv : tv^(-t) ] ,
  return((- tv * log(tv) / apply("+", t * pv)) * pv)
));
Dtaun([t]) := Dtau(t);
Dtau2(x,y) := Dtaun(x,y);
Dtau3(x,y,z) := Dtaun(x,y,z);

/* Symbolic differentiation of tau2 */
Dtau2s(x,y) := block( [ tv : tau2s(x,y)], block( [pv : tv^(-[x,y]) ] ,
  return((- tv * log(tv) / apply("+", [x,y] * pv)) * pv)
));
gradef(tau2s(x,y), Dtau2s(x,y)[1], Dtau2s(x,y)[2]);
   \endverbatim
    <ol>
     <li> How to define examples for functions? </li>
     <li> And it should be possible to define a variation which doesn't
     take a list but a variable number of arguments (for the tuple-values). DONE (define the
     n-ary function taun, and for plotting the fixed 2-ary resp. 3-ary function tau2, tau3) </li>
     <li> One has to do something here in case lower and upper bound are already very
     close. DONE (using the sign-check) </li>
     <li> Perhaps one should define chi(t) as a function x -> apply("+", x^(-t)). DONE </li>
    </ol>
   </li>
   <li> Using the faster Newton-method:
   \verbatim
load (descriptive)$ 
load (newton1)$
taunt(t,eps) := newton(apply("+", x^(-t))-1, x, length(t)^(1/mean(t)), eps);
   \endverbatim
    <ol>
     <li> Again, one should define a variant without lists (but variadic). </li>
     <li> Are there reasonable default values for eps? </li>
     <li> Can a higher precision be used? Apparently not. </li>
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

