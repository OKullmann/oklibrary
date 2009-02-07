// Oliver Kullmann, 3.2.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/GreenTao_2-4-5.hpp
  \brief Investigations on greentao_2(4,5)


  \todo Finding the best local search solver
  <ul>
   <li> It seems that for the more constraint problems greentao_2(3,6)
   adaptnovelty+ is best, while here, as for for the diagonal problems,
   rnovelty+ still is best. </li>
   <li> It seems just looking at the average behaviour for cutoff = 100*10^3
   is enough. </li>
   <li> Best is rnovelty+:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -i GreenTao_2_4_5_4225.cnf
Clauses = 100155
Variables = 4225
TotalLiterals = 410604
FlipsPerSecond = 149813
BestStep_Mean = 83155.970000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 81.050000
BestSolution_Median = 82.000000
BestSolution_Min = 47.000000
BestSolution_Max = 110.000000
   \endverbatim
   </li>
   <li> Second is adaptnovelty+
   \verbatim
> ubcsat-okl -alg adaptnovelty+ -runs 100 -i GreenTao_2_4_5_4225.cnf
Clauses = 100155
Variables = 4225
TotalLiterals = 410604
FlipsPerSecond = 156715
BestStep_Mean = 88499.750000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 88.860000
BestSolution_Median = 89.000000
BestSolution_Min = 67.000000
BestSolution_Max = 105.000000
   \endverbatim
   </li>
   <li> Then walksat-tabu with nonull-flips:
   \verbatim
> ubcsat-okl -alg walksat-tabu -v nonull -runs 100 -i GreenTao_2_4_5_4225.cnf
Clauses = 100155
FlipsPerSecond = 233046
BestStep_Mean = 88304.290000
Steps_Mean = 100000.000000
Steps_Max = 100000.000000
PercentSuccess = 0.00
BestSolution_Mean = 93.780000
BestSolution_Median = 95.000000
BestSolution_Min = 75.000000
BestSolution_Max = 122.000000
   \endverbatim
   </li>
  </ul>


  \todo greentao_2(4,5) > 4215
  <ul>
   <li> n=2000 trivial for ubcsat-rnovelty+. </li>
   <li> n=4000 found satisfiable by ubcsat-rnovelty+ with
   msteps=8376482 and seed=2773039201. </li>
   <li> n=4100 found satisfiable by ubcsat-rnovelty+ with
   msteps=7230012 and seed=3310338573. </li>
   <li> n=4150 found satisfiable by ubcsat-rnovelty+ with
   msteps=17897909 and seed=1856779838. </li>
   <li> n=4164 found satisfiable by ubcsat-rnovelty+ with
   msteps=39295024 and seed=1480959763. </li>
   <li> n=4166 found satisfiable by ubcsat-rnovelty+ with
   msteps=83573644 and seed=871144102. </li>
   <li> n=4167 found satisfiable by ubcsat-rnovelty+ with
   msteps=891150901 and seed=1913694368. </li>
   <li> n=4168 seems unsatisfiable (but isn't): 58 runs with rnovelty+ and
   cutoff 100000000 yielded often min=1, but no satisfying assignment
   was found. But
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 1000000000 -i GreenTao_2_4_5_4168.cnf
       sat  min     osteps     msteps       seed
      1 0     1  738268145 1000000000  373431756
      2 0     1  160332016 1000000000 1761599948
      3 1     0  936705634  936705634  313191661
      4 0     1  325465137 1000000000 1569292939
      5 0     1  671883843 1000000000 3451537904
      6 1     0  193351757  193351757  679157717
      7 0     2  146929304 1000000000 1406719143
      8 0     1   94854828 1000000000 1206648114
      9 1     0  399415279  399415279 3266707576
   \endverbatim
   So a cutoff of 10^9 is needed to progress further. </li>
   <li> n=4175 found satisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 100000000 -i GreenTao_2_4_5_4175.cnf
Clauses = 98055
Variables = 4175
TotalLiterals = 401986
FlipsPerSecond = 304718
BestStep_Mean = 52672891.020000
Steps_Mean = 99224814.270000
Steps_Max = 100000000.000000
PercentSuccess = 3.00
BestSolution_Mean = 2.210000
BestSolution_Median = 2.000000
BestSolution_Min = 0.000000
BestSolution_Max = 6.000000
   \endverbatim
   (quickest solution: msteps=40542872, seed=2849343332).
   </li>
   <li> n=4188 found satisfiable by ubcsat-rnovelty+ with
   msteps=89754713 and seed=1408284365. </li>
   <li> n=4194 found satisfiable by ubcsat-rnovelty+ with
   msteps=922634517 and seed=1864969928. </li>
   <li> n=4197 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 1000000000 -i GreenTao_2_4_5_4197.cnf
       sat  min     osteps     msteps       seed
      1 0     1  340323691 1000000000  744588579
      2 0     2   14686272 1000000000 3518539282
      3 0     1  408985275 1000000000  945832409
      4 0     1  110211777 1000000000  573383613
      5 0     1  267528773 1000000000 1177614520
      6 0     2   74866438 1000000000 2491079329
      7 0     3   21999403 1000000000 1319388202
      8 0     2  120329369 1000000000 1210367914
      9 0     1  806760625 1000000000 1611973161
     10 0     1  135781486 1000000000 2074714037
     11 0     1  536129169 1000000000 2952378764
   \endverbatim
   </li>
   <li> n=4200 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 100 -cutoff 100000000 -i GreenTao_2_4_5_4200.cnf
Clauses = 99125
Variables = 4200
TotalLiterals = 406380
FlipsPerSecond = 294132
BestStep_Mean = 55051674.390000
Steps_Mean = 100000000.000000
Steps_Max = 100000000.000000
PercentSuccess = 0.00
BestSolution_Mean = 3.210000
BestSolution_Median = 3.000000
BestSolution_Min = 1.000000
BestSolution_Max = 6.000000
   \endverbatim
   </li>
   <li> However it is satisfiable: 20 runs with cutoff 10^9 neither did
   find a solution, however 111 runs with cutoff 200*10^6 yielded one
   solution: msteps=95907147 and seed=4212480219. </li>
   <li> n=4206 found satisfiable by ubcsat-rnovelty+ with
   msteps=452878347 and seed=1359137308 (12th run with cutoff=10^9). </li>
   <li> n=4209 found satisfiable by ubcsat-rnovelty+ with
   msteps=198794445 and seed=1704826917 (10th run with cutoff=10^9). </li>
   <li> n=4210 found satisfiable by ubcsat-rnovelty+ with
   msteps=427443936 and seed=1253862184 (7th run with cutoff=2*10^9; also the
   42th run with cutoff=10^9 founda solution: msteps=866468275,seed=
   4038795093). </li>
   <li> n=4211
    <ol>
     <li> Found satisfiable by ubcsat-rnovelty+ with
     msteps=917466182 and seed=287655397 (29th run with cutoff=2*10^9). </li>
     <li> Over 100 runs with cutoff=10^9 didn't find a solution. </li>
     <li> Additionally with Walksat:
     \verbatim
> cat GreenTao_2_4_5_4211.cnf | walksat -cutoff 100000000 -rnovelty+ -tries 100
average flips per second = 137669
average length successful tries = 0
     \endverbatim
     It yields a lot of data, but completely unclear what it means, or whether
     it's important (as usual, the optimum value reached is not mentioned!).
     It seems slower by a factor 2 (at least) compared to ubcsat. </li>
    </ol>
   </li>
   <li> n=4212 found satisfiable by ubcsat-rnovelty+ in 43the run with
   msteps=100302888 and seed=3413331300. </li>
   <li> n=4213
    <ol>
     <li> 26 runs with cutoff=10^9 yield 1 run with min=3, 14 runs with min=2,
     and 11 runs with min=3. </li>
     <li> The 113th run with cutoff=10^9 found a solution: msteps=182964555
     and seed=1134622576, and the 51th run with cutoff=2*10^9 found a
     solution: msteps=1659276857 and seed=1773541139. </li>
     <li> But
     \verbatim
> cat GreenTao_2_4_5_4213.cnf | walksat -super -cutoff 10000000 -rnovelty+ -tries 1000
     \endverbatim
     didn't find a solution. </li>
    </ol>
   </li>
   <li> n=4215 found satisfiable by ubcsat-rnovelty+ with msteps=909712453
   and seed=43139898:
    <ol>
     <li> 122 runs with cutoff=10^9 and 40 runs with cutoff=2*10^9 didn't
     find a solution. </li>
     <li> Neither did walksat with "-super -cutoff 20000000 -rnovelty+". </li>
     <li> But then the first run with cutoff=4*10^9 found the above
     solution. </li>
    </ol>
   </li>
   <li> n=4217 </li>
   <li> n=4225
    <ol>
     <li> cutoff 200*10^6 produces in 87 runs no min=1, while with cutoff=10^9
     roughly 25% of the runs reach min=1, so let's increase the cutoff to
     300 * 10^6. And for the experiment with high cutoff 10^9, since
     18 runs didn't find a solution, let's increase it to 2*10^9. </li>
     <li> cutoff=2*10^9 produced in 26 runs no solutions, and neither did
     cutoff=4*10^9 in 6 runs. So let's assume it's unsatisfiable. </li>
    </ol>
   </li>
   <li> n=4250
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0    10    5899907   10000000 1673200738
      2 0     5    6814994   10000000 1380530593
      3 0    10    7817348   10000000 1800085398
      4 0    12    9804471   10000000   81344402
      5 0    12    3880176   10000000  432120567
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0     6   94885885  100000000  282232988
      2 0     4   88868758  100000000 3253876323
      3 0     6   66409455  100000000  366509890
      4 0     7   43725056  100000000 2558278115
      5 0     5   42205257  100000000  564216858
      6 0     7   75727900  100000000  382452377
      7 0     8   15438757  100000000 2144637212
      8 0     6   56511298  100000000 2375147134
      9 0     5   76323464  100000000  847932947
     10 0     6   69876640  100000000 2763622722
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 1000000000 -i GreenTao_2_4_5_4250.cnf
       sat  min     osteps     msteps       seed
      1 0     3  957789520 1000000000 1347268192
      2 0     4   51871287 1000000000 2848316142
      3 0     2  998189178 1000000000 1876955045
      4 0     4  365650041 1000000000 1659925539
      5 0     4  153451341 1000000000 2857726654
      6 0     3  434671408 1000000000  159827785
      7 0     2  775236975 1000000000 1138200854
      8 0     3  429842430 1000000000 3112078298
      9 0     4  729284027 1000000000  737916580
     10 0     2  636479752 1000000000 3888433655
     11 0     3  458473821 1000000000 2600521927
     12 0     4  473667998 1000000000 1903692056
     13 0     6   15416778 1000000000 2974956240
     14 0     3  523470787 1000000000 2717980127
   \endverbatim
   </li>
   <li> n=4500 perhaps is unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_4500.cnf
       sat  min     osteps     msteps       seed
      1 0    63    9763671   10000000 1769275251
      2 0    64    7217982   10000000 1761999770
      3 0    50    7495970   10000000    2286273
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_4500.cnf
       sat  min     osteps     msteps       seed
      1 0    32   52302516  100000000  404695228
      2 0    34   92977284  100000000 2884982016
   \endverbatim
   </li>
   <li> n=5000 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   198    7643948   10000000  220666133
      2 0   202    2445858   10000000 2677059783
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_5000.cnf
       sat  min     osteps     msteps       seed
      1 0   171   25452928  100000000  576886492
      2 0   170   61069864  100000000 1322237633
   \endverbatim
   (remarkable the controllability of rnovelty+ on these instances!)
   </li>
   <li> n=6000 looks unsatisfiable:
   \verbatim
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 10000000 -i GreenTao_2_4_5_6000.cnf
       sat  min     osteps     msteps       seed
      1 0   572    8204482   10000000  987022785
> ubcsat-okl -alg rnovelty+ -runs 20 -cutoff 100000000 -i GreenTao_2_4_5_6000.cnf
       sat  min     osteps     msteps       seed
      1 0   548   71357965  100000000 1291665640
      2 0   544   98261120  100000000 2085465932
   \endverbatim
   </li>
  </ul>


  \todo Survey propagation
  <ul>
   <li> Doesn't seem to be successful:
   \verbatim
builds/SAT/SurveyPropagation/sp-1.4> ./sp -l GreenTao_2_4_5_4100.cnf
...
<bias>:nan
contradiction
   \endverbatim
   </li>
   <li> One could try the parameter "-R rho", where apparently the default
   is rho=0, and 0 <= rho <= 1 is possible (for rho=1 we apparently get
   ordinary belief progagation). </li>
  </ul>

*/

