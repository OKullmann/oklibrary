// Oliver Kullmann, 28.7.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/3-k/TawSolverVdw.hpp
  \brief On tawSolver computing vdW-numbers pdvdw_2(3,k)


  \todo Version 1.0
  <ul>
   <li> On cswsok (3 GHz):
   \verbatim
for F in *.cnf; do echo $F; ./ta_solver75 $F | tee -a Output_ta75; done

VanDerWaerden_2-3-12_134.cnf
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593
Running time: 44.09 seconds

VanDerWaerden_2-3-12_135.cnf
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366
Running time: 46.64 seconds

VanDerWaerden_2-3-13_159.cnf
N_UNITS: 821693, N_BRANCHES: 65615, N_BACK: 32796
Running time: 2.01 seconds

VanDerWaerden_2-3-13_160.cnf
N_UNITS: 180658356, N_BRANCHES: 13722975, N_BACK: 6861487
Running time: 462.84 seconds

VanDerWaerden_2-3-14_185.cnf
N_UNITS: 1002639318, N_BRANCHES: 69102886, N_BACK: 34551429
Running time: 3030.05 seconds

VanDerWaerden_2-3-14_186.cnf
N_UNITS: 1498228640, N_BRANCHES: 102268511, N_BACK: 51134255
Running time: 4576.51 seconds

VanDerWaerden_2-3-15_217.cnf
N_UNITS: 4924868287, N_BRANCHES: 309583287, N_BACK: 154791621
Running time: 18378.29 seconds

VanDerWaerden_2-3-15_218.cnf
N_UNITS: 12442138772, N_BRANCHES: 774872707, N_BACK: 387436353
Running time: 47005.83 seconds

VanDerWaerden_2-3-16_238.cnf
N_UNITS: 134192977183, N_BRANCHES: 8120609615, N_BACK: 4060304807
Running time: 532416.20 seconds
   \endverbatim
   </li>
  </ul>


  \todo From version 1.0 to the version before the improved heuristics
  <ul>
   <li> On cswsok (3 GHz):
   \verbatim
# version b45bc81344b378d0130138af761e397f2b3a87c7

> tawSolver VanDerWaerden_2-3-12_134.cnf
c running_time(sec)                     37.13
c number_of_nodes                       1721206
c number_of_binary_nodes                860593
c number_of_1-reductions                20977611
c max_number_changes                    9912
> tawSolver VanDerWaerden_2-3-12_135.cnf
c running_time(sec)                     39.46
c number_of_nodes                       1790733
c number_of_binary_nodes                895366
c number_of_1-reductions                21978048
c max_number_changes                    10029
> tawSolver VanDerWaerden_2-3-13_159.cnf
c running_time(sec)                     1.68
c number_of_nodes                       65615
c number_of_binary_nodes                32796
c number_of_1-reductions                821693
c max_number_changes                    13797
> tawSolver VanDerWaerden_2-3-13_160.cnf
c running_time(sec)                     388.87
c number_of_nodes                       13722975
c number_of_binary_nodes                6861487
c number_of_1-reductions                180658356
c max_number_changes                    14061
> tawSolver VanDerWaerden_2-3-14_185.cnf
c running_time(sec)                     2545.72
c number_of_nodes                       69102886
c number_of_binary_nodes                34551429
c number_of_1-reductions                1002639318
c max_number_changes                    18792
> tawSolver VanDerWaerden_2-3-14_186.cnf
c running_time(sec)                     3859.84
c number_of_nodes                       102268511
c number_of_binary_nodes                51134255
c number_of_1-reductions                1498228640
c max_number_changes                    19139
> tawSolver VanDerWaerden_2-3-15_217.cnf
c running_time(sec)                     15252.00
c number_of_nodes                       309583287
c number_of_binary_nodes                154791621
c number_of_1-reductions                4924868287
c max_number_changes                    25796
> tawSolver VanDerWaerden_2-3-15_218.cnf
c running_time(sec)                     39229.11
c number_of_nodes                       774872707
c number_of_binary_nodes                387436353
c number_of_1-reductions                12442138772
c max_number_changes                    26075
> tawSolver VanDerWaerden_2-3-16_238.cnf
c running_time(sec)                     456846.11
c number_of_nodes                       8120609615
c number_of_binary_nodes                4060304807
c number_of_1-reductions                134192977183
c max_number_changes                    30874


# ID d72626596917101a573c05cfb90cda9ab25f4214
# Now new basis-weight 3.0.

> tawSolver VanDerWaerden_2-3-12_134.cnf
c running_time(sec)                     28.37
c number_of_nodes                       1498256
c number_of_binary_nodes                749117
c number_of_1-reductions                18113899
c max_number_changes                    9959
> tawSolver VanDerWaerden_2-3-12_135.cnf
c running_time(sec)                     33.00
c number_of_nodes                       1713023
c number_of_binary_nodes                856511
c number_of_1-reductions                20916887
c max_number_changes                    10052
> tawSolver VanDerWaerden_2-3-13_159.cnf
c running_time(sec)                     2.44
c number_of_nodes                       107389
c number_of_binary_nodes                53680
c number_of_1-reductions                1336239
c max_number_changes                    13804
> tawSolver VanDerWaerden_2-3-13_160.cnf
c running_time(sec)                     335.17
c number_of_nodes                       13358805
c number_of_binary_nodes                6679402
c number_of_1-reductions                175121629
c max_number_changes                    14060
> tawSolver VanDerWaerden_2-3-14_185.cnf
c running_time(sec)                     2153.95
c number_of_nodes                       66557682
c number_of_binary_nodes                33278826
c number_of_1-reductions                960346804
c max_number_changes                    18970
> tawSolver VanDerWaerden_2-3-14_186.cnf

# ID 6a22ecdde554f19b56cbf7faaaa40429cf3d650d
# Basically final implementation performance-wise.

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c running_time(sec)                     24.86
c number_of_nodes                       1498256
c number_of_binary_nodes                749117
c number_of_1-reductions                18113899
c max_number_changes                    9959
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c running_time(sec)                     28.93
c number_of_nodes                       1713023
c number_of_binary_nodes                856511
c number_of_1-reductions                20916887
c max_number_changes                    10052
c file_name                             VanDerWaerden_2-3-12_135.cnf

c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c running_time(sec)                     2.12
c number_of_nodes                       107389
c number_of_binary_nodes                53680
c number_of_1-reductions                1336239
c max_number_changes                    13804
c file_name                             VanDerWaerden_2-3-13_159.cnf

c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c running_time(sec)                     290.38
c number_of_nodes                       13358805
c number_of_binary_nodes                6679402
c number_of_1-reductions                175121629
c max_number_changes                    14060
c file_name                             VanDerWaerden_2-3-13_160.cnf

c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c running_time(sec)                     1852.41
c number_of_nodes                       66557682
c number_of_binary_nodes                33278826
c number_of_1-reductions                960346804
c max_number_changes                    18970
c file_name                             VanDerWaerden_2-3-14_185.cnf

c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c running_time(sec)                     2823.03
c number_of_nodes                       99576555
c number_of_binary_nodes                49788277
c number_of_1-reductions                1450713952
c max_number_changes                    19126
c file_name                             VanDerWaerden_2-3-14_186.cnf

c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c running_time(sec)                     11666.76
c number_of_nodes                       313770776
c number_of_binary_nodes                156885365
c number_of_1-reductions                4945351409
c max_number_changes                    25728
c file_name                             VanDerWaerden_2-3-15_217.cnf

c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c running_time(sec)                     29014.19
c number_of_nodes                       763714403
c number_of_binary_nodes                381857201
c number_of_1-reductions                12163387462
c max_number_changes                    26148
c file_name                             VanDerWaerden_2-3-15_218.cnf
   \endverbatim
   </li>
  </ul>


  \todo Improving the heuristics (and the implementation)
  <ul>
   <li> On cswsok (3 GHz):
   \verbatim
# ID c730dc830fb5df241fdc81697c389afcce6ba117
# Now with product as projection, and weight-basis 2.

c running_time(sec)                     15.52
c number_of_nodes                       957164
c number_of_binary_nodes                478575
c number_of_1-reductions                10859220
c max_number_assignments                134
c max_number_changes                    9780
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     18.45
c number_of_nodes                       1122651
c number_of_binary_nodes                561325
c number_of_1-reductions                12787692
c max_number_assignments                130
c max_number_changes                    9900
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     47.02
c number_of_nodes                       2242579
c number_of_binary_nodes                1121280
c number_of_1-reductions                27564612
c max_number_assignments                157
c max_number_changes                    13715
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     140.48
c number_of_nodes                       6634485
c number_of_binary_nodes                3317242
c number_of_1-reductions                81937917
c max_number_assignments                157
c max_number_changes                    13866
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     74.68
c number_of_nodes                       2616032
c number_of_binary_nodes                1308005
c number_of_1-reductions                35647489
c max_number_assignments                184
c max_number_changes                    18300
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     1223.29
c number_of_nodes                       44231759
c number_of_binary_nodes                22115879
c number_of_1-reductions                602435895
c max_number_assignments                181
c max_number_changes                    18714
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     1534.47
c number_of_nodes                       39433610
c number_of_binary_nodes                19716795
c number_of_1-reductions                584526688
c max_number_assignments                217
c max_number_changes                    25185
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     12776.23
c number_of_nodes                       336982019
c number_of_binary_nodes                168491009
c number_of_1-reductions                5027250007
c max_number_assignments                217
c max_number_changes                    25808
c file_name                             VanDerWaerden_2-3-15_218.cnf

# ID 215c1f65d3368dab47052b512de966741940ab94
# The weights are now w_2=7.5, w_3=1, w_4=2^-1, w_5=2^-2, ...

c running_time(sec)                     14.06
c number_of_nodes                       857366
c number_of_binary_nodes                428676
c number_of_1-reductions                9983978
c max_number_assignments                134
c max_number_changes                    9815
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     16.82
c number_of_nodes                       1003777
c number_of_binary_nodes                501888
c number_of_1-reductions                11774917
c max_number_assignments                132
c max_number_changes                    9884
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     39.88
c number_of_nodes                       1875227
c number_of_binary_nodes                937604
c number_of_1-reductions                23829208
c max_number_assignments                157
c max_number_changes                    13710
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     124.75
c number_of_nodes                       5811929
c number_of_binary_nodes                2905964
c number_of_1-reductions                74119386
c max_number_assignments                157
c max_number_changes                    13848
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     63.27
c number_of_nodes                       2229826
c number_of_binary_nodes                1114902
c number_of_1-reductions                31487282
c max_number_assignments                184
c max_number_changes                    18247
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     1059.86
c number_of_nodes                       38107857
c number_of_binary_nodes                19053928
c number_of_1-reductions                533296743
c max_number_assignments                182
c max_number_changes                    18748
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     906.07
c number_of_nodes                       23469490
c number_of_binary_nodes                11734735
c number_of_1-reductions                363222334
c max_number_assignments                217
c max_number_changes                    25253
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     9155.65
c number_of_nodes                       243443409
c number_of_binary_nodes                121721704
c number_of_1-reductions                3761900684
c max_number_assignments                213
c max_number_changes                    25535
c file_name                             VanDerWaerden_2-3-15_218.cnf

# ID 7da84e6c2727eb51d61117aea7a7938092d5712c

c running_time(sec)                     13.63
c number_of_nodes                       857366
c number_of_binary_nodes                428676
c number_of_1-reductions                9983978
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     16.31
c number_of_nodes                       1003777
c number_of_binary_nodes                501888
c number_of_1-reductions                11774917
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     38.85
c number_of_nodes                       1875227
c number_of_binary_nodes                937604
c number_of_1-reductions                23829208
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     122.10
c number_of_nodes                       5811929
c number_of_binary_nodes                2905964
c number_of_1-reductions                74119386
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     61.67
c number_of_nodes                       2229826
c number_of_binary_nodes                1114902
c number_of_1-reductions                31487282
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     1039.53
c number_of_nodes                       38107857
c number_of_binary_nodes                19053928
c number_of_1-reductions                533296743
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     891.87
c number_of_nodes                       23469490
c number_of_binary_nodes                11734735
c number_of_1-reductions                363222334
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     8972.42
c number_of_nodes                       243443409
c number_of_binary_nodes                121721704
c number_of_1-reductions                3761900684
c file_name                             VanDerWaerden_2-3-15_218.cnf

# ID 4d644cd9007a1351d8c263db7dfbb50df33db946
# weight_2=7.0, weight_4=0.31, weight_5=0.19, basis_open = 1.70

c running_time(sec)                     13.53
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                10105278
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     15.73
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11522820
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     36.00
c number_of_nodes                       1771688
c number_of_binary_nodes                885835
c number_of_1-reductions                22477004
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     117.12
c number_of_nodes                       5642269
c number_of_binary_nodes                2821134
c number_of_1-reductions                72084484
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     39.91
c number_of_nodes                       1476187
c number_of_binary_nodes                738082
c number_of_1-reductions                20813471
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     950.33
c number_of_nodes                       35499881
c number_of_binary_nodes                17749940
c number_of_1-reductions                500275401
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     503.95
c number_of_nodes                       13876432
c number_of_binary_nodes                6938206
c number_of_1-reductions                215404699
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     7108.35
c number_of_nodes                       198353325
c number_of_binary_nodes                99176662
c number_of_1-reductions                3078439723
c file_name                             VanDerWaerden_2-3-15_218.cnf

c running_time(sec)                     59385.40
c number_of_nodes                       1534688845
c number_of_binary_nodes                767344422
c number_of_1-reductions                24555644336
c file_name                             VanDerWaerden_2-3-16_238.cnf

# ID 3a87d8d3cccb7056bb23597a2c6fcfb5eddc68ca

c running_time(sec)                     11.99
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     13.78
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     31.62
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     102.23
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     34.64
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     834.54
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     435.86
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     6176.06
c file_name                             VanDerWaerden_2-3-15_218.cnf

c running_time(sec)                     51988.62
c file_name                             VanDerWaerden_2-3-16_238.cnf

# ID 7c0e6b352e43e587a9d8d113387d940402253029
# Version 1.9.1

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     11.73
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                10105278
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     13.58
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11522820
c file_name                             VanDerWaerden_2-3-12_135.cnf

c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c number_of_literal_occurrences         31398
c running_time(sec)                     30.83
c number_of_nodes                       1771688
c number_of_binary_nodes                885835
c number_of_1-reductions                22477004
c file_name                             VanDerWaerden_2-3-13_159.cnf

c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     100.09
c number_of_nodes                       5642269
c number_of_binary_nodes                2821134
c number_of_1-reductions                72084484
c file_name                             VanDerWaerden_2-3-13_160.cnf

c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c number_of_literal_occurrences         42542
c running_time(sec)                     34.03
c number_of_nodes                       1476187
c number_of_binary_nodes                738082
c number_of_1-reductions                20813471
c file_name                             VanDerWaerden_2-3-14_185.cnf

c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c number_of_literal_occurrences         43014
c running_time(sec)                     816.46
c number_of_nodes                       35499881
c number_of_binary_nodes                17749940
c number_of_1-reductions                500275401
c file_name                             VanDerWaerden_2-3-14_186.cnf

c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c number_of_literal_occurrences         58617
c running_time(sec)                     425.88
c number_of_nodes                       13876432
c number_of_binary_nodes                6938206
c number_of_1-reductions                215404699
c file_name                             VanDerWaerden_2-3-15_217.cnf

c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c number_of_literal_occurrences         59166
c running_time(sec)                     6027.82
c number_of_nodes                       198353325
c number_of_binary_nodes                99176662
c number_of_1-reductions                3078439723
c file_name                             VanDerWaerden_2-3-15_218.cnf

c number_of_variables                   238
c number_of_clauses                     15812
c maximal_clause_length                 16
c number_of_literal_occurrences         70446
c running_time(sec)                     50810.66
c number_of_nodes                       1534688845
c number_of_binary_nodes                767344422
c number_of_1-reductions                24555644336
c file_name                             VanDerWaerden_2-3-16_238.cnf

# ID d8cab850756bfc0d1a8e7c1d61012630aa79f3d3
# Version 1.10.0
# cswsok with one other process running.

c running_time(sec)                     11.40
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                9949777
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     13.11
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11324097
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     30.42
c number_of_nodes                       1771688
c number_of_binary_nodes                885835
c number_of_1-reductions                22100927
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     100.51
c number_of_nodes                       5642269
c number_of_binary_nodes                2821134
c number_of_1-reductions                70960525
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     34.30
c number_of_nodes                       1476187
c number_of_binary_nodes                738082
c number_of_1-reductions                20254109
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     815.40
c number_of_nodes                       35499881
c number_of_binary_nodes                17749940
c number_of_1-reductions                488455067
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     429.64
c number_of_nodes                       13876432
c number_of_binary_nodes                6938206
c number_of_1-reductions                207943615
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     6122.86
c number_of_nodes                       198353325
c number_of_binary_nodes                99176662
c number_of_1-reductions                2988176312
c file_name                             VanDerWaerden_2-3-15_218.cnf

# ID ce0dd7e1da0f70bfe0c8b560a74e0110a01e2099
# Version 1.10.1

c running_time(sec)                     9.62
c file_name                             VanDerWaerden_2-3-12_134.cnf

c running_time(sec)                     11.19
c file_name                             VanDerWaerden_2-3-12_135.cnf

c running_time(sec)                     25.70
c file_name                             VanDerWaerden_2-3-13_159.cnf

c running_time(sec)                     84.94
c file_name                             VanDerWaerden_2-3-13_160.cnf

c running_time(sec)                     29.10
c file_name                             VanDerWaerden_2-3-14_185.cnf

c running_time(sec)                     690.75
c file_name                             VanDerWaerden_2-3-14_186.cnf

c running_time(sec)                     366.86
c file_name                             VanDerWaerden_2-3-15_217.cnf

c running_time(sec)                     5197.32
c file_name                             VanDerWaerden_2-3-15_218.cnf
   \endverbatim
   </li>
   <li> version 2 (on cswsok (3 GHz)):
# Version 2:
# cswsok, with one other process running

tawSolver:
 author: Tanbir Ahmed
 url: http://sourceforge.net/projects/tawsolver/
 Changes by Oliver Kullmann
 Version: 1.10.2
 Last change date: 14.7.2013
 Clause-weight parameters: 7, 1.7
  Mapping k -> weight for weights specified at compile-time:
   2->7  3->1  4->0.31  5->0.19
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 14 2013 12:53:10
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 6d82c68830eccc0a5f3490bfddbdc24ea5fa8ce7

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     9.52
c number_of_nodes                       864314
c number_of_binary_nodes                432150
c number_of_1-reductions                10105278
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     11.09
c number_of_nodes                       977415
c number_of_binary_nodes                488707
c number_of_1-reductions                11522820
c file_name                             VanDerWaerden_2-3-12_135.cnf

s SATISFIABLE
c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c number_of_literal_occurrences         31398
c running_time(sec)                     25.79
c number_of_nodes                       1771688
c number_of_binary_nodes                885835
c number_of_1-reductions                22477004
c file_name                             VanDerWaerden_2-3-13_159.cnf

s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     84.30
c number_of_nodes                       5642269
c number_of_binary_nodes                2821134
c number_of_1-reductions                72084484
c file_name                             VanDerWaerden_2-3-13_160.cnf

s SATISFIABLE
c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c number_of_literal_occurrences         42542
c running_time(sec)                     28.77
c number_of_nodes                       1476187
c number_of_binary_nodes                738082
c number_of_1-reductions                20813471
c file_name                             VanDerWaerden_2-3-14_185.cnf

s UNSATISFIABLE
c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c number_of_literal_occurrences         43014
c running_time(sec)                     688.16
c number_of_nodes                       35499881
c number_of_binary_nodes                17749940
c number_of_1-reductions                500275401
c file_name                             VanDerWaerden_2-3-14_186.cnf

s SATISFIABLE
c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c number_of_literal_occurrences         58617
c running_time(sec)                     365.22
c number_of_nodes                       13876432
c number_of_binary_nodes                6938206
c number_of_1-reductions                215404699
c file_name                             VanDerWaerden_2-3-15_217.cnf

s UNSATISFIABLE
c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c number_of_literal_occurrences         59166
c running_time(sec)                     5195.18
c number_of_nodes                       198353325
c number_of_binary_nodes                99176662
c number_of_1-reductions                3078439723
c file_name                             VanDerWaerden_2-3-15_218.cnf

s UNSATISFIABLE
c number_of_variables                   238
c number_of_clauses                     15812
c maximal_clause_length                 16
c number_of_literal_occurrences         70446
c running_time(sec)                     43659.21
c number_of_nodes                       1534688845
c number_of_binary_nodes                767344422
c number_of_1-reductions                24555644336
c file_name                             VanDerWaerden_2-3-16_238.cnf


# Version 2.2.2
# New weights

tawSolver:
 author: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.2.2
 Last change date: 29.7.2013
 Clause-weight parameters: 5.4, 1.6
  Mapping k -> weight for weights specified at compile-time:
   2->5.4  3->1  4->0.31  5->0.13
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
 Compiled without TAU_ITERATION
 Compiled without PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 29 2013 12:07:31
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 8f849791377c512a2a34286892f7fa62e198d64d

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     9.06
c number_of_nodes                       829473
c number_of_binary_nodes                414729
c number_of_1-reductions                9709393
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     10.87
c number_of_nodes                       977215
c number_of_binary_nodes                488607
c number_of_1-reductions                11510241
c file_name                             VanDerWaerden_2-3-12_135.cnf

s SATISFIABLE
c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c number_of_literal_occurrences         31398
c running_time(sec)                     24.80
c number_of_nodes                       1751320
c number_of_binary_nodes                875651
c number_of_1-reductions                22197746
c file_name                             VanDerWaerden_2-3-13_159.cnf

s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     81.85
c number_of_nodes                       5602715
c number_of_binary_nodes                2801357
c number_of_1-reductions                71495948
c file_name                             VanDerWaerden_2-3-13_160.cnf

s SATISFIABLE
c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c number_of_literal_occurrences         42542
c running_time(sec)                     36.35
c number_of_nodes                       1888773
c number_of_binary_nodes                944376
c number_of_1-reductions                26490710
c file_name                             VanDerWaerden_2-3-14_185.cnf

s UNSATISFIABLE
c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c number_of_literal_occurrences         43014
c running_time(sec)                     671.41
c number_of_nodes                       34897521
c number_of_binary_nodes                17448760
c number_of_1-reductions                491620130
c file_name                             VanDerWaerden_2-3-14_186.cnf

s SATISFIABLE
c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c number_of_literal_occurrences         58617
c running_time(sec)                     341.54
c number_of_nodes                       13225184
c number_of_binary_nodes                6612582
c number_of_1-reductions                204899659
c file_name                             VanDerWaerden_2-3-15_217.cnf

s UNSATISFIABLE
c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c number_of_literal_occurrences         59166
c running_time(sec)                     5003.41
c number_of_nodes                       193215865
c number_of_binary_nodes                96607932
c number_of_1-reductions                2997164937
c file_name                             VanDerWaerden_2-3-15_218.cnf

s UNSATISFIABLE
c number_of_variables                   238
c number_of_clauses                     15812
c maximal_clause_length                 16
c number_of_literal_occurrences         70446
c running_time(sec)                     41657.60
c number_of_nodes                       1477863157
c number_of_binary_nodes                738931578
c number_of_1-reductions                23641449915
c file_name                             VanDerWaerden_2-3-16_238.cnf

# Node count got a bit better, and apparently also the implementation got a
# bit faster.
   \endverbatim
   </li>
  </ul>


  \todo With tau-projection
  <ul>
   <li> The following data is on cswsok (3GHz). </li>
   <li>
   \verbatim
tawSolver:
 author: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.1.2
 Last change date: 28.7.2013
 Clause-weight parameters: 7, 1.7
  Mapping k -> weight for weights specified at compile-time:
   2->7  3->1  4->0.31  5->0.19
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 28 2013 15:17:12
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = 742c25c36779fedcd73e8a8b4dfb13f5559cccea

c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     30.60
c number_of_nodes                       778208
c number_of_binary_nodes                389095
c number_of_1-reductions                9157137
c number_of_pure_literals               1229
c file_name                             VanDerWaerden_2-3-12_134.cnf

c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     38.01
c number_of_nodes                       953179
c number_of_binary_nodes                476589
c number_of_1-reductions                11285589
c number_of_pure_literals               1317
c file_name                             VanDerWaerden_2-3-12_135.cnf

c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c number_of_literal_occurrences         31398
c running_time(sec)                     32.39
c number_of_nodes                       711278
c number_of_binary_nodes                355629
c number_of_1-reductions                8948827
c number_of_pure_literals               1238
c file_name                             VanDerWaerden_2-3-13_159.cnf

c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     280.49
c number_of_nodes                       5868913
c number_of_binary_nodes                2934456
c number_of_1-reductions                75154015
c number_of_pure_literals               7859
c file_name                             VanDerWaerden_2-3-13_160.cnf

c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c number_of_literal_occurrences         42542
c running_time(sec)                     1370.75
c number_of_nodes                       24254885
c number_of_binary_nodes                12127431
c number_of_1-reductions                342553391
c number_of_pure_literals               18204
c file_name                             VanDerWaerden_2-3-14_185.cnf

c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c number_of_literal_occurrences         43014
c running_time(sec)                     2056.91
c number_of_nodes                       35661749
c number_of_binary_nodes                17830874
c number_of_1-reductions                509135733
c number_of_pure_literals               22890
c file_name                             VanDerWaerden_2-3-14_186.cnf

c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c number_of_literal_occurrences         58617
c running_time(sec)                     9320.52
c number_of_nodes                       133763548
c number_of_binary_nodes                66881763
c number_of_1-reductions                2087408170
c number_of_pure_literals               46862
c file_name                             VanDerWaerden_2-3-15_217.cnf

c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c number_of_literal_occurrences         59166
c running_time(sec)                     14057.18
c number_of_nodes                       200199573
c number_of_binary_nodes                100099786
c number_of_1-reductions                3151926052
c number_of_pure_literals               59107
c file_name                             VanDerWaerden_2-3-15_218.cnf
   \endverbatim
   </li>
   <li> Perhaps one can say that there are no relevant changes to the
   node-counts. Of course, run-time increased much. </li>
   <li> With new weights and improved tau-computation:
   \verbatim
tawSolver:
 authors: Tanbir Ahmed and Oliver Kullmann
 url's:
  http://sourceforge.net/projects/tawsolver/
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/TawSolver/tawSolver.cpp
 Version: 2.3.1
 Last change date: 29.7.2013
 Mapping k -> weight, for clause-lengths k specified at compile-time:
   2->5.4  3->1  4->0.31  5->0.13
 Divisor for open weights: 1.6
 Macro settings:
  LIT_TYPE = std::int32_t (with 31 binary digits)
  UCP_STRATEGY = 1
  TAU_ITERATION = 5
 Compiled with PURE_LITERALS
 Compiled with NDEBUG
 Compiled with optimisation options
 Compilation date: Jul 29 2013 22:52:12
 Compiler: g++, version 4.7.3
 Provided in the OKlibrary http://www.ok-sat-library.org
 Git ID = cc8bbb4f92750e728ca1c1461034903a3cff1e40

s SATISFIABLE
c number_of_variables                   134
c number_of_clauses                     5172
c maximal_clause_length                 12
c number_of_literal_occurrences         22266
c running_time(sec)                     19.06
c number_of_nodes                       916403
c number_of_binary_nodes                458194
c number_of_1-reductions                10821105
c number_of_pure_literals               1418
c file_name                             VanDerWaerden_2-3-12_134.cnf

s UNSATISFIABLE
c number_of_variables                   135
c number_of_clauses                     5251
c maximal_clause_length                 12
c number_of_literal_occurrences         22611
c running_time(sec)                     19.66
c number_of_nodes                       928273
c number_of_binary_nodes                464136
c number_of_1-reductions                11010516
c number_of_pure_literals               1196
c file_name                             VanDerWaerden_2-3-12_135.cnf

s SATISFIABLE
c number_of_variables                   159
c number_of_clauses                     7216
c maximal_clause_length                 13
c number_of_literal_occurrences         31398
c running_time(sec)                     17.29
c number_of_nodes                       707721
c number_of_binary_nodes                353850
c number_of_1-reductions                8890756
c number_of_pure_literals               1172
c file_name                             VanDerWaerden_2-3-13_159.cnf

s UNSATISFIABLE
c number_of_variables                   160
c number_of_clauses                     7308
c maximal_clause_length                 13
c number_of_literal_occurrences         31804
c running_time(sec)                     147.94
c number_of_nodes                       5820247
c number_of_binary_nodes                2910123
c number_of_1-reductions                74694837
c number_of_pure_literals               7440
c file_name                             VanDerWaerden_2-3-13_160.cnf

s SATISFIABLE
c number_of_variables                   185
c number_of_clauses                     9689
c maximal_clause_length                 14
c number_of_literal_occurrences         42542
c running_time(sec)                     581.78
c number_of_nodes                       19064245
c number_of_binary_nodes                9532112
c number_of_1-reductions                269527793
c number_of_pure_literals               13841
c file_name                             VanDerWaerden_2-3-14_185.cnf

s UNSATISFIABLE
c number_of_variables                   186
c number_of_clauses                     9795
c maximal_clause_length                 14
c number_of_literal_occurrences         43014
c running_time(sec)                     1090.64
c number_of_nodes                       34959819
c number_of_binary_nodes                17479909
c number_of_1-reductions                499800655
c number_of_pure_literals               22110
c file_name                             VanDerWaerden_2-3-14_186.cnf

s SATISFIABLE
c number_of_variables                   217
c number_of_clauses                     13239
c maximal_clause_length                 15
c number_of_literal_occurrences         58617
c running_time(sec)                     3368.09
c number_of_nodes                       87132812
c number_of_binary_nodes                43566393
c number_of_1-reductions                1360532192
c number_of_pure_literals               29192
c file_name                             VanDerWaerden_2-3-15_217.cnf

s UNSATISFIABLE
c number_of_variables                   218
c number_of_clauses                     13362
c maximal_clause_length                 15
c number_of_literal_occurrences         59166
c running_time(sec)                     7594.19
c number_of_nodes                       195349245
c number_of_binary_nodes                97674622
c number_of_1-reductions                3083110517
c number_of_pure_literals               55684
c file_name                             VanDerWaerden_2-3-15_218.cnf

s UNSATISFIABLE
c number_of_variables                   238
c number_of_clauses                     15812
c maximal_clause_length                 16
c number_of_literal_occurrences         70446
c running_time(sec)                     60092.35
c number_of_nodes                       1441795623
c number_of_binary_nodes                720897811
c number_of_1-reductions                23548317181
c number_of_pure_literals               1098005
c file_name                             VanDerWaerden_2-3-16_238.cnf

# Slightly improved node-counts, and much improved running-time.
   \endverbatim
   </li>
  </ul>


  \todo Performance evaluation
  <ul>
   <li> From version 1.0 to version 1.8 a big improvement; when comparing the
   run-times and node-counts:
   \verbatim
> round(c(47,463,4577,47006,532416)/c(16,117,950,7108,59385), digits=1)
[1] 2.9 4.0 4.8 6.6 9.0
> round(c(1790733,13722975,102268511,774872707,8120609615)/c(977415,5642269,35499881,198353325,1534688845), digits=1)
[1] 1.8 2.4 2.9 3.9 5.3
   \endverbatim
   </li>
   <li> This solver is the fastest single solver, and in the new version even
   faster than Cube&Conquer via OKsolver&minisat22 (but basically comparable).
   </li>
   <li> We should also try C&C with this solver XXX. </li>
  </ul>

*/
