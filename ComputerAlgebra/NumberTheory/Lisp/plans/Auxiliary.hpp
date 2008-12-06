// Oliver Kullmann, 6.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/Auxiliary.hpp
  \brief Plans regarding helper functions with number-theoretical character


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


  \todo Positional representations of numbers
  <ul>
   <li> Currently, integer conversion functions don't handle negative numbers.
    <ol>
     <li> Example : "int2base_n(-10,3);" errors. </li>
    </ol>
    Likely this limitation should be removed. </li>
    <li> Should base_n2int("SOMETHING",0) = 0 ? </li>
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

*/

