// Oliver Kullmann, 9.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Gap.hpp
  \brief General plans regarding the computer algebra system Gap (and SAT)


  \todo General impression
  <ul>
   <li> The system seems to work very well, it seems very powerful,
   and has definitely many functions which are very useful for us. </li>
   <li> Perhaps the programming language is kind of a special-purpose
   language, supporting efficient implementations in the domain of
   algebra, but perhaps a bit specialised:
    <ol>
     <li> It seems that there are no lambda-terms? </li>
     <li> And also no other mechanisms to support some form of
     polymorphism? </li>
     <li> And apparently there are no floating-point numbers.
    </ol>
   </li>
   <li> The central question is how GAP fits into our system.
    <ol>
     <li> Perhaps most exchange is simply by files. </li>
     <li> One can also have direct connections, via pipes and streams;
     one needs to find out how complicated this is.
    </ol>
   </li>
  </ul>


  \todo Command line
  <ul>
   <li> On both my system (OK) the keys to go to the end-of-line and to
   the begin-of-line don't work? As far as I know, this is a "readline"-issue,
   but this library is available? </li>
  </ul>


  \todo Customising the system
  <ul>
   <li> Likely there is a standard-file read by Gap on start-up.
    <ol>
     <li> We should load all the additional packages we use. </li>
     <li> Perhaps there are some special mechanisms to specify these
     preloaded packages? </li>
    </ol>
   </li>
   <li> Shall we provide "oklib --gap" ? Perhaps after some time. </li>
   <li> Then, similar to Maxima, we need to take care that the configuration
   files don't interact. </li>
   <li> It would be nice to have a timing, like with Maxima. </li>
  </ul>


  \todo Package Grape
  <ul>
   <li> It seems that e.g.,
   \verbatim
gap> Size(AutGroupGraph(JohnsonGraph(100,2)));
933262154439441526816992388562667004907159682643816214685929638952175999932299
15608941463976156518286253697920827223758251185210916864000000000000000000000000
gap> time;
125927
77718
40426
117816
42861
144090
   \endverbatim
   is a lot faster on cs-wsok than on csltok --- how does this come?
   Is somehow here 64-bit much faster than 32-bit ? </li>
   <li> Very strange the big variations in reported run-times (the above
   numbers show consecutive runs)?? </li>
  </ul>

*/

