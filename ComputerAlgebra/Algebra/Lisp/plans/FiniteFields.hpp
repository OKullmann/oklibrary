// Oliver Kullmann, 5.7.2008 (Swansea)
/* Copyright 2008, 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/plans/FiniteFields.hpp
  \brief Plans on handling finite fields


  \todo Move to Ringframes/Fields.


  \todo Write tests for all functions provided


  \todo Complete docus
  <ul>
   <li> All functions need to be covered, using prime fields GF(p)
   and more complex fields GF(p^n). </li>
  </ul>


  \todo Demos


  \todo Maxima bugs
  <ul>
   <li> All items below need to be updated w.r.t. the new Maxima version
   5.29 (which is a complete revision). </li>
   <li> Register all gf-bugs we are aware of with the Maxima bug-tracker,
   and also send a summarising e-mail to the Maxima mailing list.
    <ol>
     <li> Consider also the kind-of-bugs as in, e.g., okltest_gf_stand. </li>
     <li> In any way, the gf-documentation should discuss the representation
     issue. </li>
    </ol>
   </li>
   <li> A list of the known bugs here, which can be marked as the bugs are
   reported to the Maxima bug-tracker and mailing list.
    <ul>
     <li> "gf_matmul" can not multiply square matrices of dimension 1. </li>
     <li> This should be solved with Maxima 5.29.1:
     "gf_exp" returns 1 for "gf_exp(p,-1)" for any "p", i.e the handling
     of negative exponents is incorrect. 
     \verbatim
(%i2) gf_set(2,2,x^2+x+1); 
Evaluation took 0.0040 seconds (0.0060 elapsed) using 75.617 KB.
(%o2) true
(%i3) gf_exp(x,-1);
Evaluation took 0.0000 seconds (0.0003 elapsed) using 2.305 KB.
(%o3) 1
     \endverbatim
     </li>
     <li> "gf_exp" is defined both as a variable internal to the "gf" package
     and also as the exponential function. This leads to problems when using
     "gf_exp" with higher order functions
     \verbatim
(%i2) gf_set(2,1,x);
Evaluation took 0.0000 seconds (0.0007 elapsed) using 15.203 KB.
(%o2) true
(%i3) map(gf_exp,[1],[1]);
gf_exp evaluates to 1
Improper name or value in functional position.
 -- an error.  To debug this try debugmode(true);
(%i4) map('gf_exp,[1],[1]);
Evaluation took 0.0000 seconds (0.0004 elapsed) using 2.961 KB.
(%o4) [1]
     \endverbatim
     </li>
     <li> DONE (gf_primitive() works now)
     "gf_findprim" produces an error when called on the field
     with 2 elements, however 1 is a primitive root of this field
     \verbatim
(%i2) gf_set(2,1,x);
Evaluation took 0.0000 seconds (0.0007 elapsed) using 15.203 KB.
(%o2) true
(%i3) gf_findprim();
Use `fasttimes' only on CRE polynomials with same varlists
#0: gf_binpower(p=x,n=2)(gf.mac line 241)
#1: mainpowers()(gf.mac line 343)
#2: gf_findprim()(gf.mac line 435)
 -- an error.  To debug this try debugmode(true);
      \endverbatim
     </li>
     <li> gf_set(2,1,x) followed by gf_primitive_p(1) yields a Lisp-error.
     </li.
     <li> Setting "largefield" to false seems to break "gf_set" for some fields
     \verbatim
(%i2) largefield : false; gf_set(2,2,x^2+x+1);
Evaluation took 0.0000 seconds (0.0000 elapsed) using 104 bytes.
(%o2) false
Use `fasttimes' only on CRE polynomials with same varlists
#0: gf_binpower(p=x,n=2)(gf.mac line 603)
#1: mainpowers()(gf.mac line 343)
#2: gf_findprim()(gf.mac line 435)
 -- an error.  To debug this try debugmode(true);
     \endverbatim
     The same with the example from the documentation, 
     "gf_set(2,4,x^4+x+1);". 
     </li>
     <li> Perhaps we should set "GF_IRREDUCIBILITY_CHECK : false".
      <ol>
       <li> With 5.15.0 the default value is "false". </li>
       <li> And actually it seems that whenever it is set to true, and
       gf-operations are performed, then it is reset to false!. This looks
       like a bug, which might be corrected in 5.16.3. </li>
      </ol>
     </li>
     <li> Perhaps also "largefield : false" should be used, but then we
     should not permanently reset the field.
      <ol>
       <li> With 5.15.0 the default value is "true". </li>
       <li> We should ask for the possibility to store and re-store actively
       the values computed by gf_set. </li>
       <li> Setting largefield to false results in 5.15.0 in the error
       "Use `fasttimes' only on CRE polynomials with same varlists" (when
       adding 0 to a polynomial). </li>
      </ol>
     </li>
    </ul>
   </li>
   <li> Neither gf_test.mac nor gf_hard_test.mac contains
   tests for example for findprim. </li>
  </ul>


  \todo Missing functionality
  <ul>
   <li> The standard Maxima functions have the field of complex numbers
   apparently hardcoded? </li>
   <li> So apparently everything needs to be re-implemented. </li>
   <li> How to do linear algebra over finite fields?
    <ol>
     <li> Only gf_matmul and gf_matinv exist. </li>
     <li> We need to check for linear independence; or, more powerful, we
     need the rank. </li>
    </ol>
   </li>
  </ul>

*/

