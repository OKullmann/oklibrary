// Matthew Gwynne, 14.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Algebra/Lisp/docus/FiniteFields.hpp
  \brief User documentation for finite fields in Lisp/Maxima


  <h2> The Maxima package "gf", and simple wrappers </h2>

  <ul>
   <li> This $(maxima_gf_manual_tag_okl) documents the Maxima package. </li>
   <li> Finite field functions within the OKlibrary provide simple convenience
   wrappers to basic functions of the underlying "gf" package:
    <ol>
     <li> The purpose is to provide the ability to specify the field together
     with the operation. </li>
     <li> While the underlying gf-functions rely on globally setting the field
     (once). </li>
     <li> So within a given block, our convenience "egf"-functions can be used
     to specify the field, while after this the original gf-functions can be
     used. </li>
    </ol>
   </li>
   <li> For the basic functions from the <code>gf</code>-package, which always
   have prefix "gf_", we provide "extended" versions with corresponding prefix
   "egf_", which just offers an additional first argument, specifying the
   field.
    <ol>
     <li> Finite fields are specified by a triple [p,n,m]:
      <ul>
       <li> p is a prime number; </li>
       <li> n a natural number; </li>
       <li> m an irreducible polynomial of degree n, in the unknown x and with
       integer coefficients understood modulo p. </li>
      </ul>
      The degree n is redundant and can be left out; and if only p is specified,
      then the prime field GP(p) is meant.
     </li>
     <li> Such tuples are passed as the first argument to each of our function, to
     specify which field the operation should be performed in. </li>
    </ol>
   </li>
   <li> The field, simply denoted by GF(p^n), has as elements the congruence
   classes of polynomials modulo m.
    <ol>
     <li> The unknown is always "x". </li>
     <li> To specify which of the finite fields GF(p^n) of order p^n we
     are speaking about, we use the notation "GF(p^n)_m", where m is the
     irreducible polynomial used. </li>
    </ol>
   </li>
   <li> Field elements are represented as follows:
    <ol>
     <li> Inputs of the functions are integral polynomials in
     the unknown x, interpreted as representing the class of all equivalent
     polynomials. </li>
     <li> The output of the functions is again such an integral polynomial,
     but using a standard representation for the equivalence class. </li>
    </ol>
    So equality of field elements represented by polynomials p(x) and q(x)
    happens by first converting p(x), q(x) into their standard representations,
    for which then we can use ordinary equality testing. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> Addition and multiplication of elements in GF(2)_x, a simple (2-element)
   bit field with "xor" and "and" as the addition and multiplication operators:
   \verbatim
egf_add([2,1,x],0,1);
  1
egf_add([2,x],0,1);
  1
egf_add([2,x],1,1);
  0
g2 : [2,1,x];
egf_mul(g2,1,1);
  1
egf_mul(g2,1,x);
  0
   \endverbatim
   </li>
   <li> The same additions, but using different representations (recall: here
   the polynomial x is equivalent to zero):
   \verbatim
egf_add(g2,x,3+x^2);
  1
egf_add(g2,5-2*x+x^4,7+6*x^10);
  0
   \endverbatim
   <li> For converting a polynomial into the standard representation, use
   addition with 0:
   \verbatim
egf_add(g2,0,x^3);
  0
egf_add(g2,0,x^3+5);
  1
   \endverbatim
   </li>
   <li> Multiply two elements in GF(2^8)_(x^8+x^4+x^3+x+1), the "Rijndael byte
   field" from ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael:
   \verbatim
egf_mul([2,8,x^8+x^4+x^3+x+1], x^5+x^3+x+1, x^7+x^3+x^2);
  x^7+x^5+x^4+x+1
egf_mul(rijndael_byte_field, x^5+x^3+x+1, x^7+x^3+x^2);
  x^7+x^5+x^4+x+1
   \endverbatim
   </li>
   <li> Matrix operations use the usual Maxima matrix operations to generate the
   matrix (recall, the field-elements are just represented by integer polynomial, 
   only the special matrix-operations interprete them):
   \verbatim
gf27 : [3,3,x^3+x^2+x+2]$
mat1 : matrix([0,1],[x,x+1])$
mat2 : matrix([x+1,x],[x,x^2])$
egf_matmul(gf27,mat1,mat2);
  matrix([x,x^2],[-x^2-x,x^2-x+1])
mat1_inv : egf_matinv(gf27,mat1);
  matrix([-x^2-x+1,x^2+x+1],[1,0])
egf_matmul(gf27,mat1_inv, mat1);
  matrix([1,0],[0,1])
   \endverbatim
   </li>
   <li> Since our little wrappers call the underlying <code>gf_set</code>
   function with each call, after the first call one may just use the
   "non-extended", original versions (provided, the field didn't change):
   \verbatim
egf_add(g2,0,x^3);
  0
gf_add(0,x^3);
  0
   \endverbatim
   </li>
  </ul>


  <h2> To keep in mind </h2>

  <ul>
   <li> When using the matrix functions 
   "egf_matmul" and "egf_matinv", the polynomials returned within the resulting
   matrix will be in an optimised form, called a "CRE" (Canonical Rational 
   Expression). To convert such a polynomial back to a basic polynomial
   expression, for example for comparison of equality, one may apply the
   maxima function "totaldisrep" to the result, to regain the result with
   simple maxima polynomials.
   OK: this is completely unclear.
   </li>
   <li> The polynomials returned by the field operations uniquely represent,
   as polynomials, the field element (for the same field element always the
   same polynomial, as polynomial over the integers, is returned). However
   which polynomial is chosen is implementation-defined. For example in the
   field with 27 elements:
   \verbatim
gf27 : [3,3,x^3+x^2+x+2]$
egf_add(gf27,2,0);
  -1
egf_add(gf27,2*x,0);
  -x
egf_add(gf27,2*x^2,0);
  -x^2
egf_add(gf27,x^2,0);
  x^2
   \endverbatim
   The representation for characteristic p chooses the element from
   {-(p-1), ..., 0, ..., p-1} with minimal absolute value. </li>
   <li> By default, when setting a field then irreduciblity of the polynomial
   is not checked; checking can be enabled by setting
   <code>GF_IRREDUCIBILITY_CHECK</code> to <code>true</code> (the default-value
   is <code>false</code>). </li>
  </ul>


  <h2> Problems </h2>

  <ul>
   <li> Currently (Maxima version 5.15) the following gf-functions to not work
   properly:
    <ol>
     <li> If largefield is set to false, then gf_set doesn't work anymore. </li>
     <li> findprim, primep don't work with prime-fields. </li>
     <li> ord doesn't seem to work at all. </li>
    </ol>
   </li>
  </ul>

*/
