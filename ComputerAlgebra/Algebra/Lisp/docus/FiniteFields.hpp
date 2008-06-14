// Matthew Gwynne, 14.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/docus/FiniteFields.hpp
  \brief User documentation for Finite Fields in Lisp/Maxima


  <h2> Overview </h2>

  <ul>
   <li> Finite field functions within the OKlibrary provide wrappers to the
   underlying "gf" package, which is distributed as contribution along with
   Maxima. </li>
   <li> The abstraction provided by the functions in this library, allows one to
   specify finite fields as a 3-tuple list [p,n,m], where the field, being
   described then, is GF(p^n)[x]/m where "m" is the modulo polynomial. </li>
   <li> Such a tuple is then passed as the first argument to each function, to
   specify precisely which field the operation should be performed in. </li>
  </ul>

  <h2> Usage </h2>

  <ul>
   <li> Adding two elements in GF(2)[x]/x, a simple bit field with "xor" and
   "and" as the addition and multiplication operators :
   \verbatim
egf_add([2,1,x],0,1);
1
egf_add([2,1,x],1,1);
0
   \endverbatim
   </li>
   <li> Multiply two elements in GF(2^8)[x]/(x^8+x^4+x^3+x+1) (the Rijndael byte
   field from ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/ ) :
   \verbatim
egf_mul([2,8,x^8+x^4+x^3+x+1],x^5+x^3+x+1, x^7+x^3+x^2);
x^7+x^5+x^4+x+1
   \endverbatim
   </li>
   <li> Matrix operations use the usual maxima matrix operations to generate the
   matrix :
   \verbatim
matOne : matrix([0,1],[x,x+1])$
matTwo : matrix([x+1,x],[x,x^2])$
egf_matmul([3,3,x^3+x^2+x+2],matOne,matTwo);
matrix([x,x^2],[-x^2-x,x^2-x+1])
egf_matinv([3,3,x^3+x^2+x+2],matOne);
matrix([-x^2-x+1,x^2+x+1],[1,0])
   \endverbatim
   </li>
   <li> Operations on many different fields can be performed, in series, 
   without the need to specifically set the field globally, as was the case in
   the "gf" package :
   \verbatim
egf_add([2,1,x],0,1);
1
egf_add([2,1,x],1,1);
0
egf_mul([2,8,x^8+x^4+x^3+x+1],x^5+x^3+x+1, x^7+x^3+x^2);
x^7+x^5+x^4+x+1
matOne : matrix([0,1],[x,x+1])$
matTwo : matrix([x+1,x],[x,x^2])$
egf_matmul([3,3,x^3+x^2+x+2],matOne,matTwo);
matrix([x,x^2],[-x^2-x,x^2-x+1])
egf_matinv([3,3,x^3+x^2+x+2],matOne);
matrix([-x^2-x+1,x^2+x+1],[1,0])
   \endverbatim
   </li>
  </ul>

  <h2> Issues </h2>

  <ul>
   <li> Due to the way the underlying "gf" package handles the polynomials
   representing the field elements, when using the matrix functions 
   "egf_matmul" and "egf_matinv", the polynomials returned within the resultant
   matrix will be in an optimised form, called a "CRE" (Canonical Rational 
   Expression). To convert such a polynomial back to a basic polynomial,
   for example for comparison of equality, one may apply the maxima function
   "totaldisrep" to the result, to regain the result with simple maxima
   polynomials. </li>
   <li> Some finite field functions may return polynomials with negative 
   coefficients due to behaviour in the underlying "gf" package. These 
   polynomials will be correct, modulo the characteristic of
   the field in question, but one should be aware of this, as it may not
   necessarily be what the user is expecting, nor does such behaviour seem 
   clearly defined. 
   \verbatim
egf_add([3,1,3],2,0);
-1
egf_add([3,3,x^3+x^2+x+2],2,0);
-1
egf_add([3,3,x^3+x^2+x+2],2*x,0);
-x
egf_add([3,3,x^3+x^2+x+2],2*x^2,0);
-x^2
egf_add([3,3,x^3+x^2+x+2],x^2,0);
x^2
   \endverbatim
   </li>
  </ul>

*/
