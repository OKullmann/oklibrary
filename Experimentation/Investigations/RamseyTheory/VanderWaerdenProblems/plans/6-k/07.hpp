// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/6-k/07.hpp
  \brief On investigations into vdw_2(6,7)


  \todo Best complete solver for palindromic problems


  \todo Best local-search solver for palindromic problems
  <ul>
   <li> Best ubcsat-algorithm:
   \verbatim
> PdVanderWaerdenCNF-O3-DNDEBUG 6 7 1155

> E=run_ubcsat("VanDerWaerden_pd_2-6-7_1155.cnf",runs=100,cutoff=1000000)

saps
  2 498 499 504 505 506 507 508 509 510 511 512 513 514 515 516 517 518 519 520
  1   1   1   2   2   1   3   3   6   6   4   3  11   4   4   7  14   9   7   2
521 522
  5   4
gwsat
432 438 440 442 443 444 445 446 447 448 449 450 451 452 453 454 455 456 457 458
  1   1   2   1   1   1   3   1   9   3   4   8   1   2   2   4   4   3   6   6
459 460 461 462 463 464 465 466 467 468 469 470 471 473 474 476 479 481
  3   3   5   2   2   1   1   3   4   1   1   3   2   1   1   1   1   2
rots
444 445 447 449 451 452 453 454 455 456 457 458 459 460 461 462 463 464 466 467
  1   2   1   2   4   1   4   1   2   2   2   4   7   4   5   6   6   4   3   6
468 469 470 471 472 473 474 475 478 480 481 482 483 489 490 496
  5   2   4   2   2   3   2   4   2   1   1   1   1   1   1   1
ag2wsat
444 447 448 449 450 451 452 453 455 456 457 458 459 460 461 462 463 464 465 466
  1   1   1   1   2   1   3   3   2   3   6   5   3   5   4   6  12   9   8   5
467 468 469 470 471 476
  5   5   4   3   1   1
samd
448 450 454 462 463 464 465 466 467 468 469 472 473 474 475 477 478 479 480 481
  1   1   1   2   1   1   3   4   1   1   2   2   3   5   4   4   4   4   1   2
482 483 484 485 487 488 489 490 491 492 493 494 495 496 497 498 499 502 506 507
  5   5   2   2   1   4   2   1   5   4   3   3   5   1   1   2   1   1   1   2
512 525
  1   1
gsatt
450 451 454 458 459 460 461 462 464 466 467 469 470 471 472 473 474 475 476 477
  1   1   1   1   1   2   1   1   4   2   1   4   3   1   2   3   3   3   4   5
478 479 480 481 482 483 484 485 486 487 489 490 491 492 493 496 497 498 499 501
  4   3   4   3   7   6   3   2   3   3   3   1   2   2   2   2   1   1   1   2
505
  1
irots
452 456 459 460 461 462 463 464 465 466 467 468 469 470 471 472 473 474 475 476
  1   1   3   2   1   2   1   1   4   4   4   4   7   7   4   8   9   7   6   4
477 478 479 480 483 484
  5   6   2   5   1   1
ddfw
455 460 461 463 464 465 466 467 468 469 470 471 472 473 474 475 476 477 478 479
  1   1   1   2   1   2   1   5   3   4   5   1   8   4  10  14   8   6   7  10
480 481
  3   3

> E=run_ubcsat("VanDerWaerden_pd_2-6-7_1155.cnf",runs=100,cutoff=10000000)
ag2wsat
  0  88 363 368 369 372 375 439 442 443 444 445 446 447 448 449 450 451 452 453
  1   1   1   1   2   1   2   1   1   3   2   2   7   5   3   2  10   5   6   6
454 455 456 457 458 459 461 462
  5   6   9   4   8   1   3   2
rsaps
  0  97 487 489 496 497 498 499 500 502 503 504 505 506 507 508 509 510 511 512
  1   1   1   1   3   3   3   1   5  10   7   5   5   8   7  11   9   7   7   1
513 514
  3   1
sapsnr
  0 100 104 496 497 498 499 500 501 502 503 504 505 506 507 508 509 510 511 512
  1   1   1   3   1   3   2   2   4   4   5   6   5   5  11   6  13  14   2   3
513 514
  7   1
irots
372 374 376 381 437 450 451 453 454 455 456 457 458 459 460 461 462 463 464 465
  1   1   1   1   1   2   1   1   2   3   3   4   4   4   9   3   6   9   4  11
466 467 468 469 470 471
 10   2   4   9   3   1
g2wsat
376 465 469 470 471 472 473 474 475 476 477 478 479 480 481 482 483 484 485 486
  1   1   1   1   1   3   3   1   4   6   4  11   9  12   2   8  12   8   7   1
487 488
  2   2
   \endverbatim
   Perhaps ag2wsat scales better than rsaps and sapsnr? </li>
  </ul>


  \todo vdw_2^pd(6,7) >= (591,1156)
  <ul>
   <li> minisat-2.2.0:
    <ol>
     <li> Last solved problem for n=374. </li>
     <li> Do we have an easy-hard pattern based on parity?
     <li> Getting the data from
     Exp_PdVanderWaerdenC_2-6-7_minisat-2.2.0_2011-03-07-194459:
     \verbatim
> E = read_minisat_outputs(Sys.glob("Exp_PdVanderWaerdenC_2-6-7_minisat-2.2.0_2011-03-07-194459/VanDerWaerden_pd_2-6-7_*.cnf_OUT"))
> plot(E$n[E$n>=175], E$decisions[E$n>=175])
> lines(E$n[E$n>=175], E$decisions[E$n>=175])
     \endverbatim
     </li>
     <li> That that with that table, n is the number of variables in the SAT
     problem, not the original number of vertices. </li>
     <li> We have the problem that the original number of vertices is not in
     the table, and so we always have two identical n-values for the even and
     the corresponding odd number of vertices. </li>
     <li> Thus the pattern is not as visible as it should be. The number of
     vertices must be extracted from the file name. </li>
     <li> Due to the ordering according to filename, in most cases (but see
     "Improved handling of file-names" in
     ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp) odd comes before
     even. Unclear whether there is a pattern (if so, then it is unstable).
     </li>
     <li> There are slightly more cases with odd harder than cases with even
     harder, with the biggest difference for n=185:
     \verbatim
> E[E$n==185,]
      n     c restarts  decisions propagations    mem       time sat
296 185 11887  1086969 1242578530  23942560994 123.66 179851.000   1
298 185 12166     8191    6237556    116377530  29.83    418.978   1
                                                                                         filename
296 Exp_PdVanderWaerdenC_2-6-7_minisat-2.2.0_2011-03-07-194459/VanDerWaerden_pd_2-6-7_369.cnf_OUT
298 Exp_PdVanderWaerdenC_2-6-7_minisat-2.2.0_2011-03-07-194459/VanDerWaerden_pd_2-6-7_370.cnf_OUT
     \endverbatim
     </li>
    </ol>
   </li>
   <li> Certificates:
    <ol>
     <li> n=590:
     \verbatim
6,9,11,13,14,21,22,24,26,27,
28,29,31,35,38,39,43,44,45,49,
54,57,62,63,64,65,66,69,70,73,
74,75,76,77,82,85,90,94,95,96,
98,100,101,104,108,110,111,112,113,115,
117,118,119,122,124,125,127,128,130,133,
134,135,137,139,140,141,142,144,148,151,
152,154,156,157,158,162,167,170,175,176,
177,178,179,182,183,186,187,188,189,190,
195,198,203,207,208,209,211,213,214,217,
221,223,224,225,226,228,230,231,232,235,
237,238,240,241,243,246,247,248,250,252,
253,254,255,257,261,264,265,267,269,270,
271,275,280,283,288,289,290,291,292,295
     \endverbatim
     </li>
     <li> n=1155:
     \verbatim
7,12,13,20,22,23,24,25,26,28,
33,37,38,44,47,51,53,54,55,59,
61,62,64,66,67,70,71,74,76,79,
81,82,83,87,89,90,91,93,94,96,
97,98,99,100,103,104,105,107,108,109,
110,112,118,121,122,123,124,126,128,129,
130,131,134,140,142,143,144,145,147,148,
149,152,153,154,155,156,158,159,161,162,
163,165,169,170,171,173,176,178,181,182,
185,186,188,190,191,193,197,198,199,201,
205,208,214,215,219,224,226,227,228,229,
230,232,233,238,239,240,245,246,248,249,
250,251,252,254,259,263,264,270,273,277,
279,280,281,285,287,288,290,292,293,296,
297,300,302,305,307,308,309,313,315,316,
317,319,320,322,323,324,325,326,329,330,
331,333,334,335,336,338,344,347,348,349,
350,352,354,355,356,357,360,366,368,369,
370,371,373,374,375,378,379,380,381,382,
384,385,387,388,389,391,395,396,397,399,
402,404,407,408,411,412,414,416,417,419,
423,424,425,427,431,434,440,441,445,450,
452,453,454,455,456,458,459,464,466,471,
472,474,475,476,477,478,480,485,489,490,
496,499,503,505,506,507,511,513,514,516,
518,519,522,523,526,528,531,533,534,535,
539,541,542,543,545,546,548,549,550,551,
552,555,556,557,559,560,561,562,564,570,
573,574,575,576,578
     \endverbatim
    </ol>
   </li>
   <li> "RunPdVdWk1k2 6 7 adaptg2wsat 100 4000000" yields
   \verbatim
Break point 1: 393
Break point 2: 594
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 6 7 adaptg2wsat 100 20000000" yields
   \verbatim
Break point 1: 587
Break point 2: 1156
   \endverbatim
   </li>
   <li> "RunPdVdWk1k2 6 7 adaptg2wsat 100 40000000" yields
   \verbatim
Break point 1: 591
Break point 2: 1156
   \endverbatim
   where all solutions were found within the first 5 rounds. </li>
  </ul>

*/
