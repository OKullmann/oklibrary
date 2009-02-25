// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/milestones.hpp
  
  \module_version Buildsystem/ExternalSources/SpecialBuilds 0.2.1.15 (25.2.2009)


  \par Version 0.2.2 (documentation, mathematical packages, starting with SAT,
  and some updates)

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
   the following topics are handled:
    - Install the mmv package : DONE
    - Documentation
    - Handling of different versions : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp the following
   topics are handled:
    - Installation of Mpfr

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Building of Maxima-packages

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp the following
   topics are handled:
    - Estimating the problem with version 1.1.0 : DONE
    - Also "corrected" Ubcsat segfaults (with gcc 4.3.1) : DONE
    - Ubcsat source files are read-only : DONE
    - Update to version 1.1.0 : DONE
    - Ubcsat does not flush the output-buffer : DONE
    - Complete docu-file : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp the following
   topics are handled:
    - We need to update the created sage-script : DONE
    - Update the Sage installation : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp the following
   topics are handled:
    - Remove application of mln : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp the following
   topics are handled:
    - Installing Ecl : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp the following
   topics are handled:
    - Querying ecl : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Inconsistent installation of Maxima : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp the following
   topics are handled:
    - Update to version 2.8.1 : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Update to Maxima 5.17.1 : DONE
    - Update to Maxima 5.16.x : DONE
    - Enabling use of ECL : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp the following
   topics are handled:
    - Improving building Boost : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/GAP.hpp the following
   topics are handled:
    - Installing GAP : DONE
    - Monoids : DONE
    - GRAPE : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Git.hpp the following
   topics are handled:
    - Update the installation process : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp the following
   topics are handled:
    - R-documentation : DONE
    - Documentation-page : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Installing libsigsegv and CLisp 2.45 locally : DONE
    - Install CLisp : DONE
    - Install Maxima : DONE
    - Install Maxima 5.13 on csltok : DONE
    - Install Maxima 5.13 on cs-wsok : DONE
    - Install clisp (version 2.43) on csltok : DONE
    - Install clisp (version 2.43) on cs-wsok (system-wide) : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp the following
   topics are handled:
    - Write docus page : DONE
    - Local and system-wide : DONE
    - Installation : DONE


  \par Version 0.2.3 (updating Boost, Gcc, the existing SAT solvers, R)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp the following
   topics are handled:
    - Update our gcc-installation-system to the new standard
    - Providing gcc 4.1.2

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp the following
   topics are handled:
    - Update Boost installation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp the following
   topics are handled:
    - Cutoff value etc. should be 64 bits on a 64-bit machine
    - Investigating the weak performance of Ubcsat on OK's 32-bit machine
    - Building Ubcsat

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp the following
   topics are handled:
    - bug: GRASP cannot be compiled with gcc version 4.3
    - Grasp : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp the following
   topics are handled:
    - Local installation of gfortran
    - Can't output graphics


  \par Version 0.2.4 (installing e-mail services)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Apache.hpp the following
   topics are handled:
    - Building Apache
    - Installation script
    - Writing docus page

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Mailman.hpp the following
   topics are handled:
    - Building Mailman on freshly setup machine
    - Building Mailman in general
    - Update above instructions
    - Building Mailman on cs-wsok (for testing)
    - Testing
    - Installation script
    - Writing docus page

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Postfix.hpp the following
   topics are handled:
    - Building Postfix on a freshly setup test machine
    - Installation script
    - Writing docus page


  \par Version 0.2.5 (installing computer algebra)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Aldor.hpp the following
   topics are handled:
    - Install Aldor

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Axiom.hpp the following
   topics are handled:
    - Install Axiom
    - Install OpenAxiom
    - FriCAS

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp the following
   topics are handled:
    - Update to Sage later than 3.2.3

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp the following
   topics are handled:
    - Documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp the following
   topics are handled:
    - bug CLisp doesn't use libsigsegv (though specified)


  \par Version 0.2.6 (completion of general services, and installing SAT)

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
   the following topics are handled:
    - Complete documentation
    - Enable local/global installation for all packages
    - Organisation of links

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp the
   following topics are handled:
    - Installing SAT solvers

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Doxygen.hpp the following
   topics are handled:
    - Version 1.5.8

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Valgrind.hpp the
   following topics are handled:
    - Update to version 3.4.0


  \par Version 0.2.7 (cleaning up installations)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp the following
   topics are handled:
    - Building version 1_38_0
    - Problems with building Boost
    - Documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp the following
   topics are handled:
    - Install GCC 4.2.4
    - Installation in general

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/PostgreSQL.hpp the
   following topics are handled:
    - Missing pgsql/libpq-fe.h
    - Improvements

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp the following
   topics are handled:
    - Packages
    - Improving the docus-page
    - Building dvi,pdf,info-documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Database of integer sequences
    - System-wide installation
    - Handling of redefined load-function


  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp the following
   topics are handled:
    - Update

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/GAP.hpp the following
   topics are handled:
    - Improve installation


  \par Version 0.2.8 (new external sources for computer algebra and graphs)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/ComputerAlgebra.hpp the
   following topics are handled:
    - Systems

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Additional packages

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp the following
   topics are handled:
    - General C++ graph libraries
    - Graph isomorphism
    - Graph drawing
    - Treewidth
    - Travelling salesman
    - Graph colouring
    - Cliques


  \par Version 0.2.9

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp the following
   topics are handled:
    - Optimisation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/CMake.hpp the following
   topics are handled:
    - CMake-documentation building

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp the following
   topics are handled:
    - C++

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Coq.hpp the following
   topics are handled:
    - Improve Coq installation
    - Improve ocaml installation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Git.hpp the following
   topics are handled:
    - Prerequisites
    - Asciidoc
    - Info- and man-pages
    - Git book
    - Installation process



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.2   : 16.7.2007; initial version, several special systems can be build (the special builds are the stronger parts of module ExternalSources, while the general structure is lagging behind).
   - 0.2.1 : 10.8.2007; built Sage and Coq (and Boost 1_34_1), and extended substantially the documentation system (docus pages, functionality checks, and linking).

*/
