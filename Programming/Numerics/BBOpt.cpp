// Oliver Kullmann, 20.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

Numerics> time echo -e "-4.5 -4.5 -2e0 4.5 4.5\n-4.5 -4.5 -2e0 4.5 4.5" | ./BBOpt +4 1 1 1 /dev/stdin "0" 0
(1.9938489681631693068,0),0.70785177945559613424
real	0m0.014s
user	0m0.003s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 -2e0 4.5 4.5\n-4.5 -4.5 -2e0 4.5 4.5" | ./BBOpt -4 1 1 1 /dev/stdin "0" 0
(2.25,0),0.703125
real	0m0.014s
user	0m0.003s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 -2e0 4.5 4.5\n-4.5 -4.5 -2e0 4.5 4.5" | ./BBOpt 4 1 1 1 /dev/stdin "0" 0
(2.190497721257697504,0),0.66911985446985311851
real	0m0.019s
user	0m0.003s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 0 1000 1000 1 /dev/stdin "" 0
(0,0),14.203125
real	0m0.180s
user	0m0.176s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 0 1000 1000 1 /dev/stdin "0" 0
(2.9999999999999999993,0.49999999999999999984),5.8774717541114375398e-38
real	0m0.378s
user	0m0.373s
sys	0m0.005s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005866),1.1656921948007753119e-16
real	0m3.494s
user	0m3.495s
sys	0m0.001s
OLD:
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.437s
user	0m2.438s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(2.9999999944298000466,0.49999999845846909008),5.558866614159696022e-18
real	0m3.391s
user	0m3.392s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005866),1.1656921948007753119e-16
real	0m3.467s
user	0m3.458s
sys	0m0.011s
OLD:
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m2.445s
user	0m2.446s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(-3.750572657504313355,1.2175990261172870535),0.81728202994738042176
real	0m3.399s
user	0m3.399s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "1" 0
(2.999999995370998804,0.49999999877908325797),3.5528835712481896098e-18
real	0m3.398s
user	0m3.398s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(2.9999999742122268674,0.49999999294281005866),1.1656921948007753119e-16
real	0m4.581s
user	0m5.494s
sys	0m0.377s
OLD:
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.498s
user	0m3.980s
sys	0m0.388s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(2.9999999944298000466,0.49999999845846909008),5.558866614159696022e-18
real	0m4.699s
user	0m5.250s
sys	0m0.217s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
size=1
(2.9999999742122268674,0.49999999294281005866),1.1656921948007753119e-16
real	0m3.444s
user	0m3.444s
sys	0m0.001s
OLD:
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m3.442s
user	0m3.997s
sys	0m0.352s

Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(-3.7365384226717407658,1.2182842932992600933),0.81850338032673788621
real	0m3.430s
user	0m3.430s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 0 4.5 4.5\n-4.5 -4.5 0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 0
(2.9999999951756807154,0.49999999867330022986),4.1175798878053400249e-18
real	0m3.404s
user	0m3.405s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 0
(0.023263908576965331872,-4.5),9.392948308870454943
real	0m3.585s
user	0m3.582s
sys	0m0.005s
OLD:
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m2.488s
user	0m2.486s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 0
(0.023263909013964370279,-4.4999999999999997723),9.3929483088704531658
real	0m3.474s
user	0m3.475s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "20" 0
(0.02326390900919802842,-4.4999999999999999219),9.3929483088704531241
real	0m3.389s
user	0m3.389s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
(0.023263908576965331872,-4.5),9.392948308870454943
real	0m4.715s
user	0m5.584s
sys	0m0.439s
OLD:
(0.023263908576965331871,-4.5),9.3929483088704549413
real	0m3.497s
user	0m3.960s
sys	0m0.397s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(0.023263909013964370279,-4.4999999999999997723),9.3929483088704531658
real	0m4.698s
user	0m5.184s
sys	0m0.283s

Numerics> time echo -e "-4.5 -4.5 -3.0 4.5 4.5\n-4.5 -4.5 -3.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "20" 0
(0.02326390900919802842,-4.4999999999999999219),9.3929483088704531241
real	0m4.686s
user	0m5.210s
sys	0m0.306s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
size=4
(-2.5019914169311523434,1.3027376276992034913),0.97098619904576047193
real	0m3.702s
user	0m14.720s
sys	0m0.001s
OLD:
(-2.5019914169311523438,1.3027376276992034913),0.97098619904576047182
real	0m4.448s
user	0m17.202s
sys	0m1.685s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
(-4.4999999999999977253,1.1864290570530033295),0.76206965089231612005
real	0m3.735s
user	0m14.885s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 0
size=9
(2.9999999742122268674,0.49999999294281005866),1.1656921948007753119e-16
real	0m4.438s
user	0m37.891s
sys	0m0.009s
OLD:
(2.9999999742122268674,0.49999999294281005833),1.1656921949001919658e-16
real	0m6.779s
user	0m47.483s
sys	0m4.981s

Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=9
(-3.480072761827384739,1.2316301025976710268),0.8423486744273232522
real	0m4.586s
user	0m38.444s
sys	0m0.001s
OLD:
(-3.480072761827384739,1.2316301025976710268),0.8423486744273232522
real	0m5.271s
user	0m45.609s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 2 4.5 4.5\n-4.5 -4.5 2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 0
size=9
(2.9999999959857859223,0.49999999907559742763),2.6954407021551495249e-18
real	0m4.472s
user	0m37.851s
sys	0m0.000s
OLD:
(2.9999999959857859223,0.49999999907559742763),2.6954407021551495249e-18
real	0m5.109s
user	0m43.990s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 +2 4.5 4.5\n-4.5 -4.5 +2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=9
(-4.4999999999999999866,1.186429057100484102),0.76206965089231597856
real	0m4.582s
user	0m38.813s
sys	0m0.009s
Numerics> time echo -e "-4.5 -4.5 +2 4.5 4.5\n-4.5 -4.5 +2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 0
size=9
(2.999999995715336563,0.49999999880927455446),3.3178953528071279438e-18
real	0m4.536s
user	0m38.358s
sys	0m0.018s

Numerics> time echo -e "-4.5 -4.5 -2 4.5 4.5\n-4.5 -4.5 -2 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=9
((2.999999995588427009,0.49999999879576539899),3.3957238848041963029e-18
real	0m4.556s
user	0m38.483s
sys	0m0.012s

Numerics> time echo -e "-4.5 -4.5 +2e0 4.5 4.5\n-4.5 -4.5 +2e0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=3
(-4.4999999999999996969,1.1864290570343664571),0.76206965089231599704
real	0m3.436s
user	0m10.284s
sys	0m0.004s

Numerics> time echo -e "-4.5 -4.5 +6e0 4.5 4.5\n-4.5 -4.5 +6e0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=7
(2.9999999956757200008,0.49999999881111618281),3.3066075719262833302e-18
real	0m4.452s
user	0m27.731s
sys	0m0.024s

Numerics> time echo -e "-4.5 -4.5 +6e0 4.5 4.5\n-4.5 -4.5 +5e0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 0
size=42
(2.9999999981750257174,0.49999999957922865173),5.5635543739593960907e-19
real	0m18.715s
user	3m19.187s
sys	0m0.028s


Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m3.550s
user	0m3.544s
sys	0m0.009s
OLD:
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m2.546s
user	0m2.546s
sys	0m0.001s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 1
(1.7999999737464820132,0.1999999828748160792),84.000000000000102654
real	0m3.460s
user	0m3.460s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m4.568s
user	0m5.457s
sys	0m0.553s
OLD:
(1.7999999553680419921,0.19999997005462646464),84.000000000000300725
real	0m3.542s
user	0m4.075s
sys	0m0.437s

Numerics> time echo -e "-4.5 -4.5 0.0 4.5 4.5\n-4.5 -4.5 0.0 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(1.7999999737464820132,0.1999999828748160792),84.000000000000102654
real	0m4.785s
user	0m5.457s
sys	0m0.248s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "" 1
(0,-0.99999999961853027375),3.0000000000000000625
real	0m3.589s
user	0m3.584s
sys	0m0.007s
OLD:
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m2.544s
user	0m2.544s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 1 /dev/stdin "0" 1
(-1.2356096963899761264e-10,-1.0000000000763654306),2.999999999999999966
real	0m3.477s
user	0m3.477s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
(0,-0.99999999961853027375),3.0000000000000000625
real	0m4.590s
user	0m5.489s
sys	0m0.465s
OLD:
(-1.3100751963206599058e-19,-0.99999999961853027375),3.0000000000000000503
real	0m3.546s
user	0m4.083s
sys	0m0.429s

Numerics> time echo -e "-4.5 -4.5 0.1 4.5 4.5\n-4.5 -4.5 -1.1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(-1.2356096963899761264e-10,-1.0000000000763654306),2.999999999999999966
real	0m4.729s
user	0m5.377s
sys	0m0.328s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
size=4
(-1.7618285302889447053e-19,-0.99999999961853027353),3.0000000000000000659
real	0m3.838s
user	0m15.253s
sys	0m0.025s
OLD:
(-1.8070024064261784141e-19,-0.99999999961853027353),3.0000000000000000659
real	0m2.753s
user	0m10.944s
sys	0m0.000s

Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
(2.3779029580190141691,0.57860198575223903303),183.83119303446519176
real	0m3.829s
user	0m15.221s
sys	0m0.000s
Numerics> time echo -e "-4.5 -4.5 1 4.5 4.5\n-4.5 -4.5 1 4.5 4.5" | ./BBOpt 10000 30 20 12 /dev/stdin "2" 1
(-3.0719426545660699433,-3.0450466614003354083),17055.473852189535046
real	0m3.779s
user	0m15.093s
sys	0m0.000s

Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "" 1
size=100
(0,-1),3
real	0m42.263s
user	7m56.275s
sys	0m0.025s
OLD:
(-5.1499516920708510749e-19,-1.0000000000000000007),2.9999999999999999844
real	0m39.121s
user	6m24.722s
sys	0m0.012s

Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "0" 1
size=100
(1.8000000488356253292,0.20000003377648644698),84.000000000000382451
real	0m45.171s
user	8m29.437s
sys	0m0.032s
OLD:
(1.8000000488356253292,0.20000003377648644698),84.000000000000382451
real	0m57.233s
user	9m5.433s
sys	0m0.020s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 10000 30 20 12 /dev/stdin "1" 1
size=100
(1.8000000535460977032,0.20000003559951966956),84.0000000000004294
real	0m45.410s
user	8m33.637s
sys	0m0.040s
OLD:
(1.8000000535460977032,0.20000003559951966956),84.0000000000004294
real	0m57.493s
user	9m7.023s
sys	0m0.020s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "0" 1
(3.7558884747816771266e-09,-0.99999999728495941062),3.0000000000000045276
real	0m1.651s
user	0m15.150s
sys	0m0.000s
Numerics> time echo -e "-10 -10 9 10 10\n-10 -10 9 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "1" 1
size=100
(1.8000011131121099098,0.20000075674461296895),84.000000000190756036
real	0m1.662s
user	0m14.890s
sys	0m0.001s

Numerics> time echo -e "-10 -10 +99e0 10 10\n-10 -10 +99e0 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "0" 1
size=100
(1.1644572960566255994e-09,-0.99999999917432248026),3.0000000000000004155
real	0m1.311s
user	0m14.716s
sys	0m0.000s
Numerics> time echo -e "-10 -10 +99e0 10 10\n-10 -10 +99e0 10 10" | ./BBOpt 100 100 20 12 /dev/stdin "20" 1
size=100
(3.7130807399262040653e-10,-0.9999999994368396159),3.0000000000000001203
real	0m1.314s
user	0m14.744s
sys	0m0.001s

Numerics> time echo -e "-10 -10 -99e0 10 10\n-10 -10 -99e0 10 10" | ./BBOpt 100 10 20 12 /dev/stdin "0" 1
size=100
(-6.541272850573429904e-09,-1.0000000094066096625),3.0000000000000357238
real	0m0.135s
user	0m1.441s
sys	0m0.004s

Numerics> time echo -e "-10 -10 -9e0 10 10\n-10 -10 10e0 10 10" | ./BBOpt 100 10 20 12 /dev/stdin "0" 1
size=110
(1.804381624173732293,0.20287742907597270097),84.00288114333849928
real	0m0.146s
user	0m1.577s
sys	0m0.000s
Numerics> time echo -e "-10 -10 -9e0 10 10\n-10 -10 10e0 10 10" | ./BBOpt 100 10 20 12 /dev/stdin "1" 1
size=110
(-1.5253580492109164236e-08,-1.0000000097631327605),3.0000000000000676787
real	0m0.149s
user	0m1.574s
sys	0m0.012s


BUGS:

*/

#include <iostream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Optimisation.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.9.1",
        "14.9.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE seeds [parameters for opt]\n\n"
      " - [+-]M = partition-size\n"
      " - R = number of rounds (running through all coordinates once) >= 1\n"
      " - S = number of shrinkings (on top of the rounds) >= 1\n"
      " - T = number of threads >= 1\n"
      " - optFILE : contains a line with 5 numbers per coordinate (allows formatting)\n"
      " - seeds   : either empty string or seed-sequence\n"
      " - all following arguments are passed to the functor to be optimised.\n"
 ;
    return true;
  }

  // Prototype:
  struct Func_triv {
    void init([[maybe_unused]] const int, const char* const []) noexcept {}
    Optimisation::y_t func(const Optimisation::vec_t&) noexcept {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      return 0;
    }
  };

  struct Func {
    typedef Optimisation::x_t x_t;
    typedef Optimisation::y_t y_t;
    typedef Optimisation::vec_t vec_t;
    typedef std::function<y_t(const vec_t&)> f_t;

    f_t f;
    void init([[maybe_unused]] const int argc,
              const char* const argv[]) noexcept {
      assert(argc == 1);
      const std::string arg = argv[0];
      if (arg == "0") f = Optimisation::bealef;
      else f = Optimisation::goldsteinpricef;
    }
    Optimisation::y_t func(const vec_t& v, y_t) noexcept {
      return f(v);
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  FloatingPoint::fullprec_float80(std::cout);
  const auto result = Optimisation::bbopt_rounds_app(argc, argv, Func());
  std::cout << result << "\n";
}
