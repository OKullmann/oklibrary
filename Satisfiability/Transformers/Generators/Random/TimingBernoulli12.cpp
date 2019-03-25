// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Timing of function bernoulli(g)

Example (annotation on next line):

Random> ./TimingBernoulli12 -v
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only

Random> ./TimingBernoulli12 0 3e9 0
# computational level, number N of calls of the generator, and
# number of discards (default values),
# plus sequence of 64-bit seed values (empty by default)
"0,s,min" 3000000000 0
# the arguments (numbers in integer format)
()
# the list of 32-bit seeds (initialising the generator)
1499919941 0.49997331366666666666 0.0034629664979143074932
# the count of results "true", their relative frequency, and the p-value
3e+09 0
# N and discards again, in float80-precision.

Remark: The p-value here is very low: the probability that with 3e9 fair
coin-flips we obtain a deviation of the measured frequency f to the true
probability of at least the given value, i.e.,
  |f-0.5| >= 0.5 - 0.49997331366666666666
holds, is 0.35%, which is a low probability, and one would reject that the
given sequence is "random", due to having too few "true"-results.
This 0/1-sequence is further evaluated in Distributions.hpp.

With one seed-value (a 64-bit 0, yielding two 32-bit 0s):
Random> ./TimingBernoulli12 0 3e9 0 0
"0,s,min" 3000000000 0 0
(0,0)
1500008065 0.50000268833333333336 0.76838185202788818196
3e+09 0

At computational level 2, and with 3 64-bit seed-values:
Random> time ./TimingBernoulli12 2 3e9 0 1 2 3
"2,s,min" 3000000000 0 1 2 3
(1,0,2,0,3,0)
1499999818 0.49999993933333333334 0.99469753731439383899
1500023951 0.38180962301409408641
# number of runs, and p-value
31 30.8151
# longest run, and expected longest run
3e+09 0


Results:

On csltok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:37:29
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
15.97 16.25

Roughly 3e9 / 15.97 ~ 188e6 generations per sec, which is
188 / 225 ~ 84% of pure generation speed.
And this is 188 / 207 ~ 91% of the speed of the previous version,
where just trivial changes took place -- the compilation is "all over
the place".

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:27:32
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
16.08 16.37

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
16.31 16.59

One sees that just adding further options (not touching the above computation)
make the program slower.

Or is it just random? The newest version:
Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            a329a5a355fc552595f155d2234efb7269e83f29
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:01:47
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
15.89 16.15

Now with:

rm TimingBernoulli12
make CXXFLAGS="-fprofile-generate" TimingBernoulli12
./TimingBernoulli12
rm TimingBernoulli12
make CXXFLAGS="-fprofile-use" TimingBernoulli12

Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            429bfa9d1561ca45a276c4ef4eca817eaf1d09d4
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:23:00
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
13.87 14.18

A significant speedup.
XXX The options-output is wrong above, should include CXXFLAGS.
With profiling on the run-level-2 run:
Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            429bfa9d1561ca45a276c4ef4eca817eaf1d09d4
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:55:07
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
18.35 18.70

Worse than the original, unprofiled version.


This picture is consistent over the three platforms:
 - Profiling yields a speed-up of say 20% on the input used for
   profiling.
 - But the speed-up on the other input is much worse.
The question is whether by providing a special profiling-run, which
perhaps just first runs run-level 0, then run-level 1, then run-level 2,
we can gain overal?!?


For comparison, in "full mode":
Random> ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:29:45
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
34.82 35.38

With profiling on the run-level-0 (default) program:
Random> ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            429bfa9d1561ca45a276c4ef4eca817eaf1d09d4
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:23:00
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
57.32 58.04

An enormous slowdown.
Now profiling on the run-level-2 run:
Random> ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            429bfa9d1561ca45a276c4ef4eca817eaf1d09d4
machine name:       csltok.swansea.ac.uk
 bogomips:          4788.21
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:55:07
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
33.06 33.34

Also improving the original runtime.


On cs-wsok:

Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_19_2019 20:41:28
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
11.90 12.00

Roughly 3e9 / 11.9 ~ 252e6 generations per sec, which is
252 / 317 ~ 79.5% of pure generation-speed.
And this is 252 / 291 ~ 87% of the speed of the previous version,

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_21_2019 19:42:15
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
11.87 12.02

Basically the same.

Newer version:
Random> ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:41:40
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
12.08 12.26

Newest version:
Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:09:47
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
12.07 12.24

With profiling:
Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:27:02
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
10.02 10.14

Also a significant speedup.
With profiling on the run-level-2 run:
Random> ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:54:19
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
14.09 14.22

Worse than the original, unprofiled version.


For comparison, in "full mode":
Random> ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_23_2019 16:41:40
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
27.07 27.32

With profiling on the run-level-0 (default) program:
Random> ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:27:02
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
45.23 45.49

An enormous slowdown.
Now profiling on the run-level-2 run:
Random> ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       cs-wsok
 bogomips:          5986.74
compiler version:   g++ 8.3.0
 date:              Mar_24_2019 19:54:19
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
25.74 26.02

Much better, also improving the original runtime.


On csverify:

Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.3.2
 last change:       19.3.2019
 git-id:            0e33bd127e3ef9e78912f8c8a900413d59382c25
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_19_2019 20:42:57
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666
3e+09 0
MIN + MAX user times:
6.10 6.39

Roughly 3e9 / 6.1 ~ 492e6 generations per sec, which is
492 / 568 ~ 86.6% of pure generation-speed.
And this is 492 / 522 ~ 94% of the speed of the previous version,

Newer version:
Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.0
 last change:       21.3.2019
 git-id:            e130527c6a50f5016461088eeffdee7e29961895
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_21_2019 19:45:42
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.13 6.42

Basically the same.

Newer version:
Random$ ./RunTime ./TimingBernoulli12
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_23_2019 16:42:22
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.24 6.50

Newest version:
Random$ ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_24_2019 19:11:22
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
6.29 6.56

With profiling:
Random$ ./RunTime ./TimingBernoulli12
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_24_2019 19:28:18
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
5.72 5.83

Also here a significant speedup.
With profiling on the run-level-2 run:
Random$ ./RunTime ./TimingBernoulli12
1 10.48 MIN MAX
2 10.49 MAX
3 10.49
4 10.48
5 10.50 MAX
6 10.45 MIN
7 10.51 MAX
8 10.48
9 10.43 MIN
10 10.47
11 10.49
12 10.50
13 10.45
14 10.45
15 10.48
16 10.44
17 10.50
18 10.49
19 10.45
20 10.45
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_24_2019 19:58:36
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"0,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
3e+09 0
MIN + MAX user times:
10.43 10.51

Significantly slower than without profiling.


For comparison, in "full mode":
Random$ ./RunTime ./TimingBernoulli12 2
program name:       TimingBernoulli12
 version:           0.4.2
 last change:       23.3.2019
 git-id:            7c8043676e280f5987d26ffb9aadd02958aaa971
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_23_2019 16:42:22
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
19.12 19.34

Surprising, how that little addition of counting runs and the maximum length
increases the runtime by 300%.

With profiling on the run-level-0 (default) program:
Random$ ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_24_2019 19:28:18
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
36.13 36.16

An enormous slowdown.
Now profiling on the run-level-2 run:
Random$ ./RunTime ./TimingBernoulli12 2
author:             "Oliver Kullmann"
 url:               "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp"
 license:           "GPL v3"
program name:       TimingBernoulli12
 version:           0.4.4
 last change:       24.3.2019
 git-id:            52c36b83a4b0c3945ad267a0a67d1cfebe06d0d4
machine name:       csverify
 bogomips:          7183.87
compiler version:   g++ 8.2.0
 date:              Mar_24_2019 19:58:36
 options:           --std=c++17 -pedantic -Ofast -DNDEBUG -march=native -fwhole-program -static   -fno-finite-math-only
Output program:
"2,s,min" 3000000000 0
()
1499919941 0.49997331366666666666 0.0034629664979143074932
1499992345 0.7799646876543496644
30 30.8151
3e+09 0
MIN + MAX user times:
18.02 18.10

Much better, also improving on the original time.


It is also surprising that adding just one small test to the generation
slows it down by ~ 10%: the generation should be a much more involved
computation, and so the effect of adding this test should be smaller?
Apparently the computation of g() is very fast, so that such a small addition
matters. Or it is the compilation.

*/

#include <optional>
#include <tuple>
#include <sstream>
#include <iostream>

#include <cassert>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Tests.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.5",
        "25.3.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TimingBernoulli12.cpp",
        "GPL v3"};

  using namespace RandGen;

  constexpr gen_uint_t N_default = 3e9L;
  constexpr gen_uint_t discard_default = 0;


  // Policy classes OP, CL, OL

  using Environment::read;
  template <typename P>
  using RegistrationPolicies = Environment::RegistrationPolicies<P>;

  using OP = Environment::OP;

  // Computation-level:
  enum class CL {basic=0, runs=1, max=2}; // 0, 1, 2
  std::ostream& operator <<(std::ostream& out, const CL l) {
    return out << int(l);
  }

  // Output-level:
  enum class OL {min=0, mid=1, max=2}; // min, mid, max
}
namespace Environment {
  template <>
  struct RegistrationPolicies<CL> {
   static constexpr int size = int(CL::max) + 1;
   static constexpr std::array<const char*, size> string
      {"0", "1", "2"};
  };
  template <>
  struct RegistrationPolicies<OL> {
    static constexpr int size = int(OL::max) + 1;
    static constexpr std::array<const char*, size> string
      {"min", "mid", "max"};
  };
}
namespace RandGen {
  std::ostream& operator <<(std::ostream& out, const OL o) {
    return out << RegistrationPolicies<OL>::string[int(o)];
  }


  // The output specification:
  typedef std::tuple<CL,OP,OL> output_t;
  constexpr char sep = ',';

  std::ostream& operator <<(std::ostream& out, const output_t o) {
    return out << "\"" << std::get<CL>(o) << sep << std::get<OP>(o) << sep << std::get<OL>(o) << "\"";
  }

  output_t translate(const std::string& arg) noexcept {
    output_t res;
    for (const std::string& item : Environment::split(arg,sep)) {
      if (item.empty()) continue;
      {const auto cl = read<CL>(item);
       if (cl) { std::get<CL>(res) = *cl; continue; }}
      {const auto ot = read<OP>(item);
       if (ot) { std::get<OP>(res) = *ot; continue; }}
      {const auto ol = read<OL>(item);
       if (ol) { std::get<OL>(res) = *ol; continue; }}
    }
    return res;
  }


  Count_true frequency(const gen_uint_t N, RandGen_t& g) noexcept {
    Count_true count;
    for (gen_uint_t i = 0; i < N; ++i) count(bernoulli(g));
    return count;
  }

  CountRuns runs(const gen_uint_t N, RandGen_t& g) noexcept {
    CountRuns count(bernoulli(g));
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    return count;
  }

  LongestRun longest(const gen_uint_t N, RandGen_t& g) noexcept {
    LongestRun count(bernoulli(g));
    for (gen_uint_t i = 1; i < N; ++i) count(bernoulli(g));
    return count;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  int index = 1;
  const output_t choices = (argc <= index) ? output_t{} : translate(argv[index++]);
  if (std::get<OP>(choices) == OP::rh) {
    std::cout << Environment::Wrap(proginfo, OP::rh);
    std::cout << "TO BE IMPLEMENTED\n";
    return 0;
  }
  const gen_uint_t N = (argc <= index) ? N_default : FloatingPoint::toUInt(argv[index++]);
  assert(N >= 1);
  const gen_uint_t discard = (argc <= index) ? discard_default : FloatingPoint::toUInt(argv[index++]);
  vec_eseed_t seeds64;
  if (argc >= 4) {
    seeds64.reserve(argc-3);
    for (int i = index; i < argc; ++i)
      seeds64.push_back(FloatingPoint::toUInt(argv[i]));
  }


  const vec_seed_t seeds = transform(seeds64);
  RandGen_t g(seeds);
  g.discard(discard);

  using FloatingPoint::float80;
  using FloatingPoint::Wrap;
  std::cout << choices << " " << N << " " << discard;
  for (const auto x : seeds64) std::cout << " " << x;
  std::cout << "\n";
  out_seeds(std::cout, seeds);
  std::cout << std::endl;

  switch (std::get<CL>(choices)) {
  case CL::basic : {
    const auto ct = *frequency(N, g);
    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    break;
  }
  case CL::runs : {
    const auto [cr, ct] = *runs(N, g);
    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    std::cout << cr << " " << Wrap(runstest(ct, N, cr)) << "\n";
    break;
  }
  default : {
    const auto [lr, cr, ct] = *longest(N, g);

    std::cout << ct << " " << Wrap(float80(ct) / N) << " " << Wrap(monobit(ct, N)) << "\n";
    std::cout << cr << " " << Wrap(runstest(ct, N, cr)) << "\n";
    std::cout << lr << " " << expectedlongestrun(N) << "\n";
  }}

  std::cout << float80(N) << " " << float80(discard) << "\n";

}
