// Oliver Kullmann, 7.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/general.hpp
  \brief General plans for number theory in the Maxima computer algebra system


  \todo ext_integer_partitions
  <ul>
   <li> That Maxima yields "integer_partitions(0) = {}" is a bug;
   tell the Maxima mailing list.
    <ol>
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
  </ul>

*/

