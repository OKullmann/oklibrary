// Oliver Kullmann, 5.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/Maxima.hpp
  \brief Central docus-file regarding Maxima/Lisp


  <h1> Maxima/Lisp in the OKlibrary </h1>


  <h2> Configuration </h2>

  <ul>
   <li> Via <code>oklib --maxima</code> (see
   Buildsystem/MasterScript/docus/general.hpp for general information on the
   masterscript "oklib") a Maxima-console is opened, with
   support for loading Maxima-components from the OKlibrary:
    <ol>
     <li> By <code>oklib_load_all();</code> then all Maxima-functions in the
     OKlibrary are loaded. </li>
     <li> More general, via <code>oklib_load(filename)</code> a specific
     module is loaded:
      <ul>
       <li> "filename" here is, as with the C++ system, a relative path
       starting with "Transitional". </li>
       <li> At each level files "include.mac" are provided to include
       whole (super-)modules. </li>
       <li> So "oklib_load_all()" just is <code>oklib_load("Transitional/ComputerAlgebra/include.mac")</code>. </li>
       <li> But also specific Maxima-files can be loaded. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Inside a Maxima-file (in the context of the OKlibrary) inclusion of
   other Maxima-files happens via <code>oklib_include(filename)</code>,
   where filename is the relative path starting with "Transitional". </li>
   <li> Access to configuration variables:
    <ol>
     <li> From a Maxima-session, via "system(string_with_shell_code)" one can
     perform system calls, however to obtain the value of a variable,
     one has to use 'system("echo ${Var}")' and copy the output
     from the screen. </li>
     <li> In this way all configuration-variables from the OKlibrary
     build system are available. </li>
     <li> For convenience <code>OKplatform</code> and <code>OKsystem</code>
     are directly defined as variables. </li>
    </ol>
   </li>
  </ul>


  <h2> Documentation in general </h2>

  <ul>
   <li> For a tutorial see ComputerAlgebra/docus/MaximaTutorial.hpp. </li>
   <li> The demonstrations (in <code>demos</code>-subdirectories) can be run via
   <code>oklib_demo(filename)</code> (as to be found in the preamble of the
   respective demos-file). After showing each expression to be evaluated you need
   to press the cr-key. </li>
   <li> Another possibility to load a file with protocolling of the expressions
   to be executed is to use <code>oklib_batch</code>, which, different from
   oklib_demo, does not pause after each expression. </li>
  </ul>


  <h2> Overviews on specific modules </h2>

  <ul>
   <li> %Graphs : ComputerAlgebra/Graphs/Lisp/docus/general.hpp </li>
   <li> Linear algebra : ComputerAlgebra/LinearAlgebra/Lisp/docus/general.hpp </li>
   <li> Satisfiability : ComputerAlgebra/Satisfiability/Lisp/docus/general.hpp
   </li>
  </ul>

*/

