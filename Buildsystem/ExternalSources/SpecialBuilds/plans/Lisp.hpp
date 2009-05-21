// Oliver Kullmann, 30.8.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp
  \brief Plans regarding installation of Lisp compiler/interpreters othen then Ecl (the current default)


  \bug CLisp doesn't use libsigsegv (though specified)
  <ul>
   <li> According to http://www.clisp.org/impnotes/faq.html#faq-with-prefix
   the build of libsigsegv has to be adopted on 64-bit machines by adding
   "--libdir=='${exec_prefix}/lib64'" as configure-parameter:
    <ol>
     <li> For that we need a way to figure out whether the platform is 32-bit
     or 64-bit. </li>
     <li> Perhaps for 64-bit platforms we also need to distinguish between
     "bi-arch" and "mono-arch"? </li>
     <li> The above faq-link contains more information. </li>
     <li> The new libsigsegv-version is 2.6 --- perhaps this solves the problem
     already? No, still the same problem. </li>
    </ol>
   </li>
   <li> Isn't there a similar problem with libffcall ? </li>
   <li> We should try Clisp 2.46 : DONE (also with 2.47 the same problem)
    <ol>
     <li> Seems alright on cs-wsok (warnings about non-portable linking between
     static and shared libraries, but uses the local libraries). </li>
     <li> Now needs to be tested on cs-oksvr: Same problem again! </li>
    </ol>
   </li>
   <li> DONE (Ecl is now the default)
   Thus apparently we need to use another Lisp; let's see whether Ecl
   installs properly on all our systems. </li>
  </ul>


  \todo Installing other Lisp's
  <ul>
   <li> CMUCL http://www.cons.org/cmucl/ looks reasonable. </li>
   <li> SBCL http://www.sbcl.org/ looks alright (and maintained).
    <ol>
     <li> However, it needs another Lisp to be compiled! </li>
     <li> So let's try the other's first. </li>
    </ol>
   </li>
   <li> GCL http://savannah.gnu.org/projects/gcl seems a bit outdated, but
   we should try.
    <ol>
     <li> However, if the argument-length is really only 64, then it's
     not usable. </li>
    </ol>
   </li>
  </ul>

*/

