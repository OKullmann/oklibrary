// Oliver Kullmann, 3.12.2010 (Swansea)
/* Copyright 2010, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/docus/Espresso.hpp
  \brief Documentation on how to build and use Espresso


  <h1> Installing Espresso </h1>


  <h2> On the purpose of Espresso </h2>

  Espresso is a program for the computation of small CNF/DNF representations
  of boolean functions.


  <h2> Package history </h2>

  <ul>
   <li> The original Espresso home page is
   http://embedded.eecs.berkeley.edu/pubs/downloads/espresso/index.htm . </li>
   <li> We use a slightly updated version from
   http://code.google.com/p/eqntott/downloads/detail?name=espresso-ab-1.0.tar.gz .
   </li>
  </ul>


  <h2> What the installation yields </h2>

  <ul>
   <li> The executable <code>$(espresso_public_call_okl)</code>. </li>
   <li> Some documentation in directory <code>$(espresso_doc_dir_okl)</code>.
   </li>
  </ul>


  <h2> Current state of installation </h2>

  <ul>
   <li> Recommended version =
   <code>$(espresso_recommended_version_number_okl)</code> </li>
   <li> Path to the executable:
    <ol>
     <li> espresso program call = <code>$(espresso_call_okl)</code> </li>
     <li> A symbolic link <code>$(espresso_public_call_okl)</code> is provided
     in the public bin-directory (<code>$(public_bin_dir_okl)</code>). </li>
    </ol>
   </li>
   <li> Checking the installation:
    <ul>
     <li> Ready: $(espresso_call_ready_okl) </li>
     <li> location = $(location_espresso_call_okl) </li>
     <li> version = <code>$(version_espresso_call_okl)</code>
     </li>
    </ul>
   </li>
  </ul>


  <h2> How to install </h2>

  <h3> Make targets </h3>

  <table>
   <tr>
    <td> <code> espresso </code> </td>
    <td> Build the recommended version of espresso, using the local gcc. </td>
   </tr>
   <tr>
    <td> <code> cleanespresso </code> </td>
    <td> Remove the build directory. </td>
   </tr>
   <tr>
    <td> <code> cleanallespresso </code> </td>
    <td> Remove build, installation and documentation directory. </td>
   </tr>
  </table>


  <h3> Configuration </h3>

  <ul>
   <li> To install a different version, set variable
   <code>espresso_recommended_version_number_okl</code> accordingly. </li>
  </ul>


  <h2> Usage </h2>

  <ul>
   <li> Use <code>$(espresso_exec_okl) filename</code>, where the file contains
   a boolean function in the (extended) PLA-format. </li>
   <li> An example is to use the following "test.pla" file (see below)
   with espresso
   \verbatim
.i 3
.o 1
110 1
111 1
010 1
001 0
000 0
100 1
011 0
101 0
   \endverbatim
   by calling
   \verbatim
> espresso2.3 test.pla
.i 3
.o 1
.p 3
1-0 1
-10 1
11- 1
.e
  \endverbatim
   </li>
   <li> The PLA file is a format for describing finite functions
   with a particular emphasis on boolean functions. </li>
   <li> For simple usage one can assume that the ".i" line specifies the
   number of input variables, the ".o" line the number of output variables,
   and then each line specifies the entry in a truth table describing the
   associated boolean function. </li>
   <li> If an entry is marked with a "-", this is simply shorthand for the
   expansion into multiple truth table entries where the "-" values take every
   possibility but the rest of the values remain unchanged. </li>
   <li> For more information on the PLA format, see below or refer
   to the Espresso documentation at
   ExternalSources/sources/Boolean/Espresso/espresso.5.html . </li>
  </ul>


  <h2> The PLA format </h2>

  <ul>
   <li> A PLA file represents a partial finite function,
   f : {0,1}^n -> {0,1}^m, by encoding the DNF of the relational
   viewpoint of f. </li>
   <li> By the relational viewpoint, we mean representing the boolean
   function which takes both input and output bits and returns true iff
   f maps the given input to the given output. More precisely,
   representing f' : {0,1}^(n+m) -> {0,1} such that f'((I,O)) = 1 if
   f(I) = O. </li>
   <li> The "header" of the PLA file:
    <ul>
     <li> The number of input and output variables are
     specified first with lines of the form:
     \verbatim
.i n
.o m
     \endverbatim
     where n and m are the positive integer number of input and output
     variables. </li>
     <li> Additional lines such as ".type" (discussed below), and
     others discussed in the documentation, may also be specified in
     the "header" of the file. </li>
    </ul>
   </li>
   <li> The "clauses":
    <ul>
     <li> Each line after the header is:
      <ol>
       <li> a sequence of characters I in {1,0,-} of size n; followed by </li>
       <li> a space; followed by </li>
       <li> a sequence of characters O in {1,0,-} of size m. </li>
      </ol>
     where:
      <ul>
       <li> Each line encodes a DNF clause which is part of a DNF
       representation for the partial finite function using the
       relational viewpoint. </li>
       <li> That is, each line encodes that f(I') = O', where I' and O'
       are the boolean vectors corresponding to the strings I and O for
       that line. </li>
       <li> A "-" in I indicates that f(I') = O' for both values of the
       corresponding variable. </li>
       <li> A "-" in O indicates that f(I') = O' but the output value,
       for the variable the "-" corresponds to, is undefined.  </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Additional options:
    <ul>
     <li> ".type X": specifies how to interpret total assignments left
     unspecified, where X is one of the following:
      <ul>
       <li> "f": unspecified assignments and those with undefined ouputs are
       assumed to be false (0). </li>
       <li> "r": unspecified assignments and those with undefined outputs are
       assumed to be true (1). </li>
       <li> "fd": unspecified assignments are assumed to be false
       (the default for espresso). </li>
       <li> "fr": unspecified assignments are considered to be
       "don't care" values. </li>
       <li> "fdr": there should be no unspecified values. </li>
      </ul>
     </li>
     <li> The PLA standard also allows one to provide names for variables,
     as well as other options which give Espresso information on how
     to minimise the finite function provided. These options are discussed
     in </li>
    </ul>
   </li>
   <li> Lines in a PLA file starting with the "#" symbol are comments. </li>
   <li> Note here that there is a difference between "undefined" and
   "unspecified". </li>
   <li> One can use "-" for an output bit to explicitly <em>specify</em> that
   the partial finite function defined by the PLA is undefined for the
   corresponding input bits. A total assignment is <em>unspecified</em> if
   there is no line in the PLA which defines its value (or "undefinedness"). </li>
   <li> The function <code> output_fcl2pla </code> is available at
   the Maxima level to translate a clause-set to a PLA file. </li>
   <li> The PLA format also supports defining "multi-valued functions". </li>
   <li> For more information, see the "the Espresso documentation at
   ExternalSources/sources/Boolean/Espresso/espresso.5.html with additional
   information in
   ExternalSources/sources/Boolean/Espresso/man_octtools_espresso.html. </li>
  </ul>


  <h2> Examples </h2>

  <ul>
   <li> The DNF {{1,2,3},{-1,4,-5}} becomes
   \verbatim
.i 5
.o 1
111-- 1
0--10 1
   \endverbatim
   </li>
   <li> The two bit adder (from the espresso documentation):
   \verbatim
# 2-bit by 2-bit binary adder (with no carry input)
.i 4
.o 3
0000  000
0001  001
0010  010
0011  011
0100  001
0101  010
0110  011
0111  100
1000  010
1001  011
1010  100
1011  101
1100  011
1101  100
1110  101
1111  110
   \endverbatim
   </li>
  </ul>


*/

