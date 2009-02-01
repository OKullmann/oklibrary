// Oliver Kullmann, 1.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file EmptyProgram/docus/general.hpp
  \brief Documentation on the empty program


  <h1> The empty program </h1>


  <h2> The minimal ingrediences for a C/C++ program </h2>

  <ul>
   <li> A C/C++ "program" is a collection of "compilation units", containing
   exactly one definition of a function called "main". </li>
   <li> The function "main" is treated specially:
    <ul>
     <li> Its return-type must be "int". </li>
     <li> If no return-value is specified, then 0 is returned. </li>
     <li> Its argument-list can only take two forms, either no argument,
     as in this case, or two arguments for the options from the command-line.
     </li>
    </ul>
   </li>
  </ul>


  <h2> The basic build system </h2>

  <ul>
   <li> Call "make" (or "make EmptyProgram") to compile the program. </li>
   <li> Call "make cleanall" for removing everything. </li>
  </ul>


  <h2> Using the program </h2>

  <ul>
   <li> To run the program, and print out the return value, use
   \verbatim
./EmptyProgram; echo $?
   \endverbatim
   </li>
  </ul>
 
*/
