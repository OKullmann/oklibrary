// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/milestones.hpp
  
  \module_version Buildsystem/ExternalSources/SpecialBuilds 0.2.3.5 (23.8.2011)


  \par Version 0.2.4 (updating especially Gcc and the existing SAT solvers,
  and installing CSP-software)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp the following
   topics are handled:
    - Update to 11.1.1.2

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp the following
   topics are handled:
    - Installing and using Sbcl
    - Installing other Lisp's
    - Installing and using GCL : DONE
    - Installing and using Cmucl : DONE

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
   the following topics are handled:
    - Supporting Java
    - Complete documentation
    - Patches

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Cryptography.hpp the
   following topics are handled:
    - Grain of salt
    - Terminology
    - New library (replacing Mhash)
    - DES generator
    - Argo DES instances
    - Kreuzer DES instances

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp the following
   topics are handled:
    - Update to version 1_48_0 : DONE
    - Linking error (on csoberon) : DONE
    - Defect boost-installation for fixed gcc-4.1.2 : DONE
    - Missing documentation for Boost
    - Update Boost installation
    - Documentation
    - distance versus size
    - Stale renaming instructions : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp the following
   topics are handled:
    - Application RankPrimes is slower with version 4.3.0
    - Improve installation of Mpfr
    - Installation of Mpfr : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp the following
   topics are handled:
    - Failure building gcc412/gcc due to internal compiler error : DONE
    - Gcj
    - Providing gcc 4.1.2
    - Install GCC 4.5.3 : DONE
    - texi2dvi (texinfo 1.13a) fails to build gcc.texi on some systems : DONE
    - Local Gmp/Mpfr are not used (appropriately) : DONE
    - How to use local Gmp + Mpfr? : DONE
    - Local build of Fortran fails (for 4.1.2) : DONE
    - Putting "configure" under our version control : DONE
    - GCC 4.1.2 will not build on systems without GMP with MPFR support : DONE
    - Install GCC 4.2.4 : DONE
    - Install GCC 4.3.5 : DONE
    - Install GCC 4.4.5 : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp the following
   topics are handled:
    - Failing tests
    - Installing 2.12.1 : DONE
    - Local installation of gfortran : DONE
    - Can't output graphics : DONE
    - Improving the docus-page
    - Building dvi,pdf,info-documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Mhash.hpp the following
   topics are handled:
    - MHash segmentation fault
    - Complete MHash documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp the following
   topics are handled:
    - Install Relsat
    - Parallel SAT solvers
    - SAT solvers
    - (P)Lingeling
    - Grasp uses system C++ libraries
    - Improvements of CryptoMiniSat
    - Glucose
    - Add todos for SAT-Race 2010 SAT solvers
    - Fuzzing
    - Write docus for March
    - Write docus for Satz
    - Write docus for Minisat
    - Update of GRASP
    - Installation in general
    - SAT4J
    - Minisat
    - Picosat
    - Precosat
    - March solvers
    - Argo
    - Satz
    - Belief and Survey propagation
    - Correct solvers with poor handling of trivial CNFs : DONE
    - Glucose build error : DONE
    - False Glucose build : DONE
    - CryptoMiniSat : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp the following
   topics are handled:
    - Update to version 1.2.0
    - UBCSAT 1.0.0 reports wrong minimum number of falsified clauses
    - UBCSAT 1.0.0 segfaults with weighted MaxSAT instances
    - Make clear the required package for dos2unix

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Doxygen.hpp the following
   topics are handled:
    - Doxygen uses system C++ libraries
    - Install version 1.7.2

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
   the following topics are handled:
    - False build for Espresso

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/PseudoBoolean.hpp
   the following topics are handled:
    - %Tools for PB -> SAT translation
    - minisat+ input format
    - BoolVar/PB

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp
   the following topics are handled:
    - Overview
    - Constraint modelling languages
    - Sugar
    - Gecode
    - Minion
    - Mistral
    - Choco
    - Mozart
    - TAILOR

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp the following
   topics are handled:
    - Update to newer Sage : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp the following
   topics are handled:
    - Update to CLisp version 2.49 : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Installation of version 5.23.2 : DONE
    - Installation of version 5.22.1 : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp the following
   topics are handled:
    - Update to 11.1.1 : DONE

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Valgrind.hpp the
   following topics are handled:
    - Install version 3.6.0 : DONE


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
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp the following
   topics are handled:
    - Update to 12.2.1
    - Documentation

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp the following
   topics are handled:
    - Memory restrictions
    - External documentation
    - Install xgettext
    - Database of integer sequences
    - Additional packages
    - System-wide installation
    - Handling of redefined load-function

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp the following
   topics are handled:
    - Libffcall not installing on AB's 64-bit Xeon machine

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
   the following topics are handled:
    - Improve documentation for Espresso

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp the following
   topics are handled:
    - Documentation for CryptoMiniSat

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp
   the following topics are handled:
    - Eclipse
    - N Queens
    - Sudoku
    - Local search

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp the following
   topics are handled:
    - Loop-optimisation


  \par Version 0.2.6 (completion of general services, and installing SAT)

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
   the following topics are handled:
    - Enable local/global installation for all packages
    - Organisation of links

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp the
   following topics are handled:
    - Installing SAT solvers

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Valgrind.hpp the
   following topics are handled:
    - Update


  \par Version 0.2.7 (cleaning up installations)

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp the following
   topics are handled:
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
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp the following
   topics are handled:
    - General C++ graph libraries
    - Graph isomorphism
    - Graph drawing
    - Treewidth
    - Travelling salesman
    - Graph colouring
    - Cliques

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/RamseyTheory.hpp
   the following topics are handled:
    - Software by Aaron Robertson


  \par Version 0.2.9

  \par
   In OKlib/Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp the
   following topics are handled:
    - Optimisation

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

  \par
   In Buildsystem/ExternalSources/SpecialBuilds/plans/Games.hpp the following
   topics are handled:
    - Parity games



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.2   : 16.7.2007; initial version, several special systems can be build (the special builds are the stronger parts of module ExternalSources, while the general structure is lagging behind).
   - 0.2.1 : 10.8.2007; built Sage and Coq (and Boost 1_34_1), and extended substantially the documentation system (docus pages, functionality checks, and linking).
   - 0.2.2 : 21.5.2009; complete updates for many packages (Gmp, R, CMake, MHash), especially much work on installing Maxima and related software (CLisp, Ecl, gnuplot, etc.), making Boost 1_34_1 installing and working on all platforms, some smaller updates (continously Git; attempts with Sage), working out the new general style of build-makefiles (just providing one version "at at time") and update of documentation; several SAT-solvers installed and updated/patched (Ubcsat, Grasp, Satz215, Minsat+, March, SP).
   - 0.2.3 : 24.12.2010; update to newest Boost version, updates, corrections and improvements regarding Maxima (and ECL), update of Gcc-building, updates and extensions for R-building, various improvements to Ubcsat, and various other new installation or updates (Espresso, Doxygen).

*/
