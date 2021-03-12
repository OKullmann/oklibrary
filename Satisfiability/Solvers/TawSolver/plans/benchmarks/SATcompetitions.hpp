// Oliver Kullmann, 30.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/benchmarks/SATcompetitions.hpp
  \brief The TawSolver on benchmarks from the SAT competitions


  \todo SAT 2014
  <ul>
   <li> The natural starting point is the SAT competition
   http://www.satcompetition.org/ , and all instances should be run, with
   a time-out of say 3600s (or shorter, if this takes too long). </li>
   <li> Currently tawSolver does not allow a timeout, but it can be achieved
   as follows:
   \verbatim
> timeout --signal=SIGINT 3600 tawSolver File
   \endverbatim
   (interrupt here after 3600s).
   </li>
   <li> Running through the crafted instances from SAT2014, with timeout of
   600sec; data/History (sc14_craftet_600). Solved instances:
    <ol>
     <li> Satisfiable: "289-*", "Chvatal-*", "Composite-*", "mod2-*",
     "prime-*", "Q3inK11-*", "rbsat-*", "rnd-*", "sgen3-*", "sgen4-*",
     "toughsat-*" , "VanDerWaerden-*". </li>
     <li> Unsatisfiable: "edges-*", "sgen6-*". </li>
    </ol>
   </li>
   <li> Now running the same with ttawSolver. XXX
   </li>
   <li> Running through the applications-instances from SAT2014, with timeout
   of 600 sec (sc14_app_600). Only satisfiable instances solved:
   "complete-*", "stable-*". </li>
   <li> Now running the same with ttawSolver. XXX </li>
  </ul>

  \todo SAT Competitions 2011-2020
  <ul>
   <li> 16313 instances from SAT Competitions 2011-2020 were collected to
        in repository DataSolvers and analysed.
   <li> BCP was applied to all instances to make ones without unit clauses.
   <li> Instances were split into 209 families, see their descriptions at
        DataSolvers/docus/SATCompetition/families.
   <li> tawSolver 2.8.2 was compared with ttawSolver 2.11.1 on these families,
        the timelimit was 1000 seconds on the cs-kullmann-server, the results
        are in DataSolvers: sc*_tawSolver_2.8.2_1000 and sc*_ttawSolver_2.11.1_1000
   <li> The script AnalyseSolversResults.R from OKlib/Statistics/R/ was used
        to compare the solvers.
   <li> On 57 families out of 209 either solver solved at least one instance.
   <li> On 11 families out of 57 ttawSolver solved >= UNSAT instances than tawSolver.
   <li> On 8 families out of 11 ttawSolver had lower mean UCPs and number of nodes:
        "sc11-GreenTao_2-*"; "sc11-battleship-*"; "sc12-sgen4-*; sc13-MUS-v3*";
        "sc16-sokoban-*.sas.*"; "sc16-modgen-n*"; "sc19-eq*"; "sc20-Steiner-*".
   <li> On 2 families of of 8 also the mean runtime was lower on ttawSolver:
        "sc16-sokoban-*.sas.*"; "sc16-modgen-n*".
   <li> On 24 families out of 57 ttawSolver solved >= SAT instances than tawSolver.
   <li> On 13 families out of 24 ttawSolver had lower mean UCP and number of nodes:
        "sc11-aes_*"; "sc11-GreenTao_2-*"; "sc11-rnd_1*"; "sc11-sgen3-n*";
        "sc12-sgen4-*"; "sc15-manthey_DimacsSorterHalf*";
        "sc15-manthey_DimacsSorter_*"; "sc17-mp1-klieber2017s*"; "sc18-fla-komb-*";
        "sc18-fla-qhid-*"; "sc18-2018032*"; "sc18-satcoin-genesis-SAT-*";
        "sc20-ncc_none_*".
   <li> On 9 families of of 13 also the mean runtime was lower on ttawSolver:
        "sc11-aes_*"; "sc11-GreenTao_2-*"; "sc11-rnd_1*";
        "sc15-manthey_DimacsSorterHalf*"; "sc15-manthey_DimacsSorter_*";
        "sc17-mp1-klieber2017s*"; "sc18-2018032*"; "sc18-satcoin-genesis-SAT-*";
        "sc20-ncc_none_*".
   <li> ttawSolver showed particular good results (compared with tawSolver) on the
        following families: "sc15-manthey_DimacsSorterHalf*" - solved 9 instances
        with mean runtime 173.81 seconds, while 5 / 208.85 for tawSolver;
        "sc15-manthey_DimacsSorter_*" - 13 / 184.00 vs 11 / 308.33;
        "sc18-2018032*" - 9 / 321.88 vs 5 /409.8; "sc18-satcoin-genesis-SAT-*" -
        14 / 71.785 vs 9 / 153.77
  </ul>
*/
