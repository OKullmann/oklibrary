// Oliver Kullmann, 21.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/plans/general.hpp
  \brief Plans in general for algebra functionality via Maxima/Lisp


  \todo Docus

  \todo Demos

  \todo Field Element Representation
  <ul>
   <li> Some field elements are represented as negative values rather than 
   their positive counterpart. For example
   \verbatim
(%i59) egf_add([3,3,x^3+x^2+x+1], 2,0);
Evaluation took 0.0080 seconds (0.0097 elapsed) using 105.633 KB.
(%o59) -1
(%i60) egf_add([3,3,x^3+x^2+x+1], 2*x,0);
Evaluation took 0.0080 seconds (0.0098 elapsed) using 105.945 KB.
(%o60) -x
   \endverbatim
   Is this an issue? Such values are valid as they are the equal to their 
   positive counterparts modulo the characteristic of the field but it would 
   seem more intuitive if all coefficients of the polynomials representing
   the field elements were positive. </li> 
   <li> This should certainly be kept in mind. </li>
  </ul>

  \todo Finite Field operations
  <ol>
   <li> The gf_set operation in the finite field package takes a considerable
   amount of time to complete and doesn't seem to cache 
   any of the results in any way and so each call takes the same amount of 
   time and so simply calling gf_set inside wrappers, as has been done, 
   causes issues where these functions are called often (such as in the case 
   of egf_add).
   <ol>
    <li> Perhaps a potential solution would be to keep track of which field
    is currently being used, and only if the field being used is different 
    to the last call would gf_set be called? (although in general, this 
    concept of keeping global state in such a way seems like it will cause 
    problems) .</li>
    <li> It seems the best way to approach this is to provide an
    egf_set function that calls gf_set, which keeps track of
    the finite field being used in a global variable. Then each of the
    other functions then offer the option to pass in the field. If the field
    is passed in, then the operation is done in that field, but the overall
    global variable is not changed so any other operations are still done
    in the field set with egf_set, otherwise the operation is done in the 
    field set with egf_set. This would allow one to set a default field but then
    perform other operations in other fields if necessary without 
    interrupting the flow of code. </li>
    <li> Calling egf_set once in a file causes problems as then if that file 
    is loaded and then other operations are performed then this original set
    can not be relied on, but clearly there is an overhead (of around 2 
    seconds or more on a current machine) to this operation. </li>
    <li> Perhaps have this operation check to see if the field being set is
    different to the current field and only then call gf_set? </li>
    <li> It seems the only real option is to pass the field being used in
    with every call to "egf_add" etc, otherwise we can potentially end up in
    an inconsistent state if "gf_set" is called outside of the "egf" functions.
    (MG: I believe the overall slowness attributed to use of "gf_set" 
    originally was due to my use of an older version of the "gf" package and
    the speed issues have now been resolved, since by default it no longer
    checked that the modulo polynomial is irreducible when gf_set is called
    (this can be enabled by setting a particular option). </li>
   </ol>
   </li>
   <li> Move the interface to the gf-package into a seperate module / .mac
   file. This should be moved to Algebra. MG : Should a new module 
   "ComputerAlgebra/Algebra" be created or does this mean LinearAlgebra? DONE 
   </li>
   <li> DONE Since we need the package, and it 
   doesn't come with Maxima, we need to handle it as an ExternalSource. 
    <ul>
     <li> What is the procedure for this?
      <ol>
       <li> At least make the package (pyhsically) available. </li>
       <li> Installation will at least unpack it, and put it to a right place. </li>
       <li> There should be configuration variables which help us using the package. </li>
      </ol>
     </li>
     <li> When doing this, it might be beneficial to add some basic functionality
     such as a gf_matadd (matrix addition within the set field) and so on which
     isn't currently offered but seems trivial to implement using the functions 
     defined in gf.mac, although whether this is useful would depend on the design. </li>
    </ul>
   </li>
   <li> Create an interface to the gf-package. DONE </li>
   <li> Create field-identification list [p, n, poly]. DONE </li>
   <li> Field-operation-functions take such an "object" as additional argument,
   to specify the field. DONE </li>
   <li> Finite fields? Possibly through a third party package. (see
   http://www.math.utexas.edu/pipermail/maxima/2006/003255.html), 
   although this looks untested. DONE (started using it) </li>
  </ol>


  \bug DONE Finite Fields package function doesn't terminate
  <ul>
   <li> OKlibrary now uses maxima 5.15 and this has been fixed in this 
   version. </li>
   <li> This has been raised on the maxima bug tracker as #1913047 
   https://sourceforge.net/tracker/?func=detail&atid=104933&aid=1913047&group_id=4933
   </li>
   <li> This has been fixed in the latest cvs version of maxima (5.15) and the 
   upcoming release candidate. 5.15 should be available in the next few weeks.
   </li>
   <li> Additionally an email entitled "Setting GF(2) in share/contrib/gf.mac 
   doesn't terminate?" has now been sent to the mailing list. </li>
   <li> The current distributor of the package seems to be Jacopo Daurizio? Is
   there some way to contact him (googling him and a quick search of the 
   mailing list archives don't seem to find anything)? </li>
   <li> "gf_set" doesn't terminate when passed 1 as an argument for the degree. 
   </li>
   <li> "gf_set(2,1,[x]);" leads to non-termination (MG : or at least it takes
   much longer than expected) </li>
   <li> DONE This should be moved when the wrapper functions are moved into a 
   seperate module </li>
  </ul>


  \todo DONE Tests for Finite Fields
  <ul>
   <li> DONE Extensive tests with larger fields at higher tests levels </li>
   <li> DONE Perhaps for egf_mul check known generators for certain fields 
   (Mostly likely at oklib_test_level > 0) </li>
   <li> DONE Simple tests (oklib_test_level = 0) </li>
  </ul>

  
  \todo DONE Create milestones


  \todo DONE Move the finite-fields functions here. 

*/

