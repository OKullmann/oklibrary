// Oliver Kullmann, 6.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/Auxiliary.hpp
  \brief Plans regarding helper functions with number-theoretical character


  \bug binv2hexstr mishandles vectors with non-multiple-of-4 lengths
  <ul>
   <li> We have:
   \verbatim
maxima> binv2hexstr([0,0,0,1,1]);
"11"
   \endverbatim
   </li>
   <li> The expected result is:
   \verbatim
maxima> binv2hexstr([0,0,0,1,1]);
"03"
   \endverbatim
   </li>
   <li> Either that or the documentation for binv2hexstr should specify it's
   behaviour in such instances. </li>
   <li> To fix this, there are several options:
    <ul>
     <li> In binv2hexstr: pad the input list out to a multiple of 4, from the
     left before applying other operations. </li>
     <li> Create two functions for partition_elements:
      <ul>
       <li> lpartition_elements(L,n): "leftover" elements are to
       the left of the list:
       \verbatim
maxima> lpartition_elements([0,0,0],2);
[[0],[0,0]]
       \endverbatim
       </li>
       <li> rpartition_elements(L,n): "leftover" elements are to the left of
       the list:
       \verbatim
maximma> rpartition_elements([0,0,0],2);
[[0,0],[0]]
       \endverbatim
       </li>
      </ul>
      then lpartition_elements instead of partition_elements in
      binv2hexstr.
     </li>
    </ul>
   </li>
   <li> It is better to just pad binv2hexstr, as the underlying assumption
   one would like to make when converting a binary vector to a hexidecimal
   string is that the vector length is a multiple of 4.  We should validate
   this assumption as soon as possible. </li>
  </ul>


  \todo Floors of logarithms
  <ul>
   <li> Currently we have fld, which is a special case of floorlog, can
   only handle integers, and which is also slower, but which can handle
   arguments of arbitrary size. </li>
   <li> Should "floorlog" be changed to "flog"? </li>
   <li> floorlog could be made excepting arbitrary large inputs if arbitrary
   precision would be used --- perhaps a case distinction should be performed?
   </li>
   <li> The use of numeric logarithms will always involve a risk of false
   results, so precise algorithms are sought. </li>
  </ul>


  \todo Positional (polyadic) representations of numbers
  <ul>
   <li> Complete the tests. </li>
   <li> We duplicated some Maxima functionality (see "ibase" and "obase"),
   and we should comment on that.
    <ol>
     <li> For example that it's quite confusing --- if one doesn't know what
     obase is, it's a bit hard to find out, and to change ibase we need to know
     its current value! </li>
     <li> So one better uses it only locally, but this is also not completely
     trivial:
     \verbatim
(%i6) block([obase : 16], print(16));
10
Evaluation took 0.0000 seconds (0.0004 elapsed) using 1,016 bytes.
(%o6) 16
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE
   The whole current system is a bad hack --- strings are not appropriate
   mathematical objects, but lists of integers!
    <ol>
     <li> Strings should only result from converting lists of integers. </li>
     <li> DONE So all these functions need to be rewritten. </li>
     <li> DONE We also need better names than "base_n". </li>
    </ol>
   </li>
   <li> DONE
   Currently, integer conversion functions don't handle negative numbers.
    <ol>
     <li> Example : "int2base_n(-10,3);" errors. </li>
    </ol>
    Likely this limitation should be removed.
   </li>
   <li> DONE (polyadicstr2int(str,0) equals the rightmost digit)
   Should base_n2int("SOMETHING",0) = 0 ? </li>
  </ul>


  \todo ext_integer_partitions
  <ul>
   <li> That Maxima yields "integer_partitions(0) = {}" is a bug:
    <ol>
     <li> DONE
     Tell the Maxima mailing list, and report the bug. </li>
     <li> The documentation states "Thus 0 has no partitions.";
     this is false (since [] is a partition of 0). </li>
     <li> Accordingly, also "integer_partitions(0,0) = {}" is false;
     it must be {[]}. </li>
     <li> Furthermore the documentation states that n can be an
     integer, but for example "integer_partitions(-1) = int_partitions(-1)".
     </li>
    </ol>
   </li>
   <li> We should also correct "integer_partitions(0,0)", but how to overload
   ext_integer_partitions on the number of arguments? </li>
   <li> If the Maxima function would be corrected, then our function
   ext_integer_partitions shall be eliminated. </li>
   <li> See "Document important programming techniques" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
  </ul>


  \todo Unordered integer partitions
  <ul>
   <li> Is there a more direct way of computing unordered integer partitions?
   (without using integer partitions)?
    <ol>
     <li> Given the proposed formula below, it should be possible to convert
     the subsets of {1,...,n-1} into unordered integer partitions. </li>
    </ol>
   </li>
   <li> The Maxima-function permutations is very inefficient for example for
   lists like create_list(1,i,1,n) --- are there better algorithms? </li>
   <li> The number of unordered partitions of n seems to be 2^(n-1) ---
   is this correct? </li>
  </ul>

*/

