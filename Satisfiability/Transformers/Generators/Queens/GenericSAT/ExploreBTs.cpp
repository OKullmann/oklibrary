// Oliver Kullmann, 2.1.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Analysis of the ltau-computation

USAGE:

> ExploreBts

gives information on input and output.

> ExploreBTs "H"
a b ltau N rp eam1 eb sum ldiff ltau2 N2 rp2 ub

prints the header line for the output:
 - a, b the inputs, sorted in ascending order
 - ltau = ln(tau(a,b))
 - N is the number of iterations
 - rp is the return-place
 - eam1 = exp(-a*ltau)-1
 - eb = exp(-b*ltau)
 - sum = eam1 + eb
 - ldiff = ln(b) - ln(a)
 - ltau2 uses the improved lower bound
 - N2 the number of iterations for that
 - rp2 the return-place for that
 - ub is the upper bound (via Lambert-W0)
 - lbd is the difference (lower-bound from W-upper-bound) - (W-lower-bound)
 - ltau3 uses the lower-bound from the W-upper-bound
 - N3, rp3 are as above.

> ExploreBTs "L"
la lltau N rp

 - la = ln(a)
 - lltau = -ln(ln(tau(a,b)))
 - N, rp as above

> ExploreBTs N

Creates N random 1 <= a,b <= 2^64, and determines the maximum number of reached
iterations for ltau(a,b,true), ltau(1,a,true), ltau(1,b,true), printing
the maximum reached with a, b, and at the end the average.
N is read as a long double, and then converted to an unsigned 64-bit integer.

When using the form "+N", then use double instead of FloatingPoint::float80.

> ExploreBTs a b

prints the results of the computation for ln(tau(a,b)).

> ExploreBTs begin end N

where "begin" and "end" are of type float80, and N is an UInt_t,
creates the data for ltau(1,x), x running from begin to end in steps of
(begin-end)/N, as left-closed right-open interval, so leaving out end
for begin < end (and creating exactly N numbers).

> Explore begin end N L

now prints the data fitting to "ExploreBTs L" above.

TODOS:

1. Analyse ltau:

   (a) Are there cases where the current implementation does not terminate?
       Apparently not.
   (b) Are there cases where one of the asserts triggers (of course, not
       considering the two asserts for the arguments)?
       Apparently not.
   (c) The computation of fpx0 = a*Am1 + a + b*B could also be formulated as
       fpx0 = a*(Am1 + 1) + b*B, however the current form seems numerically
       better -- is this true? One would assume that the form with "+1"
       is slightly faster.
        (1) Addition +1 introduces an error, and then we multiplied with that
            accumulated error.
        (2) The current form not only avoids this, but is then also able to
            use fma.
   (d) How many interations are used? Where is the maximum reached, and
       how big is it? First the old way of computation:
        (1) A surprisingly good approximation of the number N of iterations is
            ln(b/a) (assuming b >= a), that is, ln(b)-ln(a).
            Below we get for a=1 and 1 <= b <= 10^10 the numerical relation
            N ~ 4.1579086 + 0.9009774 * ln(b).
        (2) Apparently due to the capping of the precision, this is shortcut
            at 11400 iterations.
        (3) How is the relation for float (32 bits) and double (64 bits)?
       Now for the new form, using the new lower bound (ltau_Wlb):
        (4) Maximum reached: 9th iteration, for inputs
            16703907943236820288 10311623104440942929  a/b ~ 1.6199 rp=3
            1066418399878483031  2412617782618577230   b/a ~ 2.2624 rp=3
            16589867186399607565 9683439302147830244   a/b ~ 1.7132 rp=3
            10160342482430904122 4299059295903343351   a/b ~ 2.3634 rp=3

            Experiment on cs-wsok:
> time ./ExploreBTs 1e10
6 15802936068377499781 183160344130469849
7 16791642383913292757 4065916805604311715
8 4218619374332161423 9979003554481616949
9 16589867186399607565 9683439302147830244
N=10000000000, seed=3588235421, mean=5.1369
real    1733m52.295s
user    1732m46.498s
sys     0m0.990s
            On csverify:
$ time ./ExploreBTs 1e10
5 18440059746480114549 6759161357241715963
7 12825578832803872753 6472497884121332112
8 12020428015117746044 3901439736596916880
9 16703907943236820288 10311623104440942929
N=10000000000, seed=197189146, mean=5.1369
real    1802m47.240s
user    1802m47.281s
sys     0m0.000s

Strange that this is slower than on cs-wsok.
Possibly that indicates that there is no hardware support for
extended precision. But it is hard to find any information on that.
But the E%-1650 seems to have the "usual" floating point unit.

The computations on csltok+cs-wsok seem more precise than those on csverify
(fewer iterations are needed)?!? On csverify we use gcc-7.3, on csltok and
cs-wsok gcc-7.4?!?

            Indeed these values are lower for the debug-version, and when
            option -fno-unsafe-math-optimizations is used. One needs to see
            the timing difference.

            Results without unsafe math-operations, on cs-wsok:
> time ./ExploreBTs 1e10
5 6459024372447303798 2187477526226035575
6 13543334970715116462 1376347560552655926
7 11875697068296926189 4145087500940396498
8 5696276450391395622 1917406421658972516
9 4093032196074975294 2536792807261028352
N=10000000000, seed=1763899552, mean=5.10343
real    1732m13.461s
user    1731m9.920s
sys     0m0.088s
A small improvement concerning the average number of iterations.

            On csverify:
$ time ./ExploreBTs 1e10
5 12633253454754184719 14850399898711484516
6 4557406752452909230 6372797450074436622
7 7908702694756119661 11668787067555285171
8 1118134130687922072 2929970240154195279
9 5560627170217329473 2411714478917914189
N=10000000000, seed=2736620509, mean=5.1042
real    1814m58.175s
user    1814m58.187s
sys     0m0.004s
Also here a small improvement regarding iterations, although the maximum
here didn't improve, and we have on cs-ltok:
> ./ExploreBTs_debug 5560627170217329473 2411714478917914189
2411714478917914189 5560627170217329473 1.843050860497241533e-19 6 4 -0.35884961818693126429 0.35884961818693126429 0 0.83537300568309661927 1.843050860497241533e-19 6 4 1.8927821296146979158e-19 1.0365087291720272451e-20 1.843050860497241533e-19 5 4

            so using fewer iterations on the csverify-worst-case example.
            But as we can see below, quite possibly these are just random
            effects.
            On cs2-irfan536:
> time ./ExploreBTs 1e10
5 8769548207261893368 6788518202430336776
6 5927180028089639353 11097756775733187085
7 12070406334308512019 16196084228413581294
8 2150617391099105820 6237675884950930639
9 3789684961899778285 11470176591059096699
N=10000000000, seed=3756793289, mean=5.1042
real    1866m22.876s
user    1866m17.951s
sys     0m1.764s
            This looks very much like csverify.
            However run-time is even slower than csverify, where csverify
            has 7200 bogomips, and cs2-irfan586 has even 8000 bogomips!

            The lower bound based on the upper bound further reduces the
            number of iterations.
            On cs-wsok:
> time ./ExploreBTs 1e10
4 6218384555933453483 17305954648218371190
5 9225588074932873385 6527989728751999091
6 4123191031370163407 12482591276829692264
7 1895226193488067233 5657613003858569223
8 4002072974446389365 10702458202723523369
N=10000000000, seed=999275244, lb-method=LB::upper_bound, mean=3.40508
real    1384m48.796s
user    1383m56.399s
sys     0m0.703s
            A good improvement.
            On csverify:
$ time ./ExploreBTs 1e10
4 12394744782733381523 11084237573018212577
5 14748760050579134984 13820300575811078
6 7645345993129863363 2789184717206972873
7 14648695332041264827 3237861517221203681
8 10477259421966178173 4053651587672455781
N=10000000000, seed=1485535054, lb-method=LB::upper_bound, mean=3.4055
real    1432m49.335s
user    1432m49.322s
sys     0m0.000s
            Also here a good improvement; still a bit worse regarding
            iterations, but very little. Still strange that this faster
            processor is slower.
            On csltok:
> time ./ExploreBTs 1e10
4 1171329530288452743 3396028536542990353
5 309619181024487346 2066397496836013903
6 18209792607008828317 10746168270293205077
7 3993442149533922254 10458138670794155821
8 4870252760889516499 2092193973604611863
N=10000000000, seed=3768564672, lb-method=LB::upper_bound, mean=3.40509
real    1622m26.857s
user    1615m34.799s
sys     2m14.001s
            On cs2-irfan536:
$ time ./ExploreBTs 1e10
5 5314223773156965489 15887457227195830191
6 1422277535878073103 2564199399909358389
7 6544607814807669852 1985377728455160974
8 10414385934317866077 4005028799608241113
N=10000000000, seed=3363349879, lb-method=LB::upper_bound, mean=3.40551
real	1490m30.361s
user	1490m15.300s
sys	0m2.912s
            Also here a good improvement; similar to csverify -- also in that
            it's slower than cs-wsok, although it has more bogomips (and again
            this is slower than even csverify).

            Finally using double instead of float80.

            On cs-wsok:
> time ./ExploreBTs +1e10
4 3.9899065277272648e+18 1.1487711439182703e+19 9.8119442545958894e-20 4 3
5 8.2214829177761344e+17 1.1788743629079091e+19 1.7190135965163698e-19 5 3
6 1.2586346670870692e+19 4.8025083641187809e+18 8.605707489632824e-20 6 3
7 1.0097742426723113e+19 4.4143785602083425e+18 1.0114223339330483e-19 7 3
N=10000000000, seed=1895013578, mean=3.32375
real    526m53.909s
user    526m33.472s
sys     0m0.919s
            Indeed more than twice as fast; compared with the improvements
            below, however that appears quite slow. Repeated:
> time ./ExploreBTs +1e10
3 7.6379731899964017e+18 1.1396721458549891e+19 7.3838323332397374e-20 3 4
4 9.6430871194899825e+18 1.6074208183850688e+17 3.1270772678780027e-19 4 3
5 1.984276985970079e+18 7.4619910841852754e+18 1.6855948911030839e-19 5 4
6 1.1139610458408959e+18 3.0598477722718648e+18 3.6113012802200034e-19 6 3
7 4.3008486154689946e+18 1.1091639948326775e+19 9.6984165507262634e-20 7 3
N=10000000000, seed=606087831, mean=3.32375
real    527m36.638s
user    527m16.903s
sys     0m0.134s
            Would be interested to know why cs-wsok is so slow (relatively)
            with double; it's an Intel i5-2320. One should try whether
            allowing unsafe math-operations has an effect (though previous
            experiments indicated that this would increase the number of
            iterations). So now without "-fno-unsafe-math-optimizations
            -fno-associative-math":
> time ./ExploreBTs +1e10
3 9.6586457478083482e+18 1.4746757660421857e+19 5.7681978513837548e-20 3 3
4 2.6990281680227272e+18 1.1326042122028667e+19 1.1599545085416598e-19 4 3
5 1.6683495095334705e+19 1.8943446526000712e+18 1.0346068173536853e-19 5 3
6 4.742859002832002e+18 1.5100197321969699e+19 7.7839215919126298e-20 6 3
7 1.1823814486450979e+19 4.0171998353761403e+18 9.6214853887738079e-20 7 3
N=10000000000, seed=2770162746, mean=3.32375
real    509m8.097s
user    508m49.280s
sys     0m0.019s
            A bit faster but not much.
            Additionally also without "-fno-reciprocal-math":
> time ./ExploreBTs +1e10
4 1.2540066882987131e+19 6.0804516553133496e+18 7.7806205374817266e-20 3 3
5 1.5941610767015365e+19 2.4265805501763748e+18 9.7622965182557469e-20 5 3
6 5.9209177332526776e+18 2.2688524929209585e+18 1.8260274432050532e-19 6 3
7 5.4875840512587233e+18 2.0809458745318075e+18 1.9790324919985981e-19 7 3
N=10000000000, seed=364126268, mean=3.32375
real    525m49.698s
user    525m29.411s
sys     0m0.783s
            That backfired apparently (perhaps now more iterations?).
            Anyway, it seems that the influence of these options is not
            decisive.
            On csverify:
$ time ./ExploreBTs +1e10
4 1.4329538684250612e+19 2.7535403574411028e+18 9.9620729328971207e-20 4 5
5 7.9078491129514557e+18 1.3921781245759247e+18 1.8655188840460962e-19 5 3
6 5.1156561087820134e+18 1.4106482076320109e+19 7.8462031993441247e-20 6 3
7 5.798356912282665e+18 2.0714257693638136e+18 1.9210092504275167e-19 7 3
N=10000000000, seed=1016341000, mean=3.32376
real    98m7.253s
user    98m7.253s
sys     0m0.000s
            That's now more than a factor of 10 faster!
            To check, run again (ID 993aec4bcaab45eaa716a6479b901ddc48e28397):
$ time ./ExploreBTs +1e10
4 1.3229055439200643e+19 1.7564902936726264e+19 4.5331518195678847e-20 4 4
5 1.0272071845308379e+19 1.2739965079603858e+18 1.6297043614279429e-19 5 3
6 4.1743655104046331e+18 1.746547061001601e+19 7.5133726594726637e-20 6 3
7 3.6637319951800899e+18 1.2311940138205958e+19 9.7610476185331203e-20 7 3
N=10000000000, seed=2169362946, mean=3.32375
real    96m57.345s
user    96m57.347s
sys     0m0.000s
            On csltok:
> time ./ExploreBTs +1e10
5 7.3790307146034063e+18 3.2597124221836534e+18 1.3777590669009523e-19 4 4
6 1.252482580344868e+19 3.1937717715705672e+18 1.0212070193137451e-19 6 3
7 1.0048541055070646e+19 4.4910288642192261e+18 1.0065841183974645e-19 7 3
N=10000000000, seed=4216303640, mean=3.32375
real    547m55.161s
user    515m35.400s
sys     0m34.859s
            (the system went sleeping several times); repeated:
> time ./ExploreBTs +1e10
3 5.6359166203908465e+18 6.8919985179129979e+18 1.1104434625789965e-19 3 4
4 9.5628301945423032e+18 1.6937349879854176e+19 5.3787297442522992e-20 3 4
5 7.961556946116969e+17 1.0020855779545934e+19 1.9400303702640179e-19 5 3
6 5.9872324700840909e+18 4.0925221003382387e+17 3.406120362400838e-19 6 3
7 1.0430973497985411e+19 4.5598523532539459e+18 9.7913019514869153e-20 7 3
N=10000000000, seed=2960566555, mean=3.32375
real    521m9.398s
user    521m35.421s
sys     1m7.061s
            Interesting that that is even faster than cs-wsok.
            On cs2-irfan536:
$ time ./ExploreBTs +1e10
4 1.4189883206158383e+19 1.3950838196057983e+19 4.9264161061139476e-20 2 3
5 8.2101949714370304e+18 1.7858593895582085e+18 1.658164531553144e-19 5 3
6 3.5658834474822031e+18 1.2552453856523786e+19 9.7567792821451892e-20 6 3
7 4.3468850073795553e+18 9.7704848466922353e+18 1.0373035300773566e-19 7 3
N=10000000000, seed=4096252409, mean=3.32375
real	114m11.254s
user	114m10.984s
sys	0m0.128s
            More than a factor of 10 faster. Again similar to csverify:
            Slow on float80, fast on double.

            So it's actually worth to provide both versions.

            Concerning speed, the time for computing the random numbers
            isn't insignificant, and so the simple program MeasureTau
            for measuring the run-times just scans an interval.
            Actually, the time seems for producing the random numbers seems
            (relatively) insignificant.
   (e) Some approximations of the error, perhaps in dependency of ln(b/a),
       are needed.
        (1) We should check whether we have quadratic convergence (and
            what that means), as predicted by the general results for
            Newton's method. Perhaps the convergence speed should be
            measured.
            https://en.wikipedia.org/wiki/Rate_of_convergence
        (2) For the example
> ./ExploreBTs_debug 4002072974446389365 10702458202723523369
4002072974446389365 10702458202723523369 1.0208002254053642389e-19 7 3 -0.33537459609578309383 0.33537459609578309375 -8.1315162936412832551e-20 0.98366098316464228857 1.0208002254053642389e-19 7 3 1.0591102939438040969e-19 7.7453621314834096271e-21 1.0208002254053642389e-19 8 3
            on csltok+cs-wsok we see that although the level-3 lower bound is
            better than the level-2 one (by 7.7e-21), more iterations are
            needed.
            On csverify we get
$ ./ExploreBTs_debug 4002072974446389365 10702458202723523369
4002072974446389365 10702458202723523369 1.0208002254053642388e-19 6 4 -0.3353745960957830938 0.3353745960957830938 0 0.98366098316464228857 1.0208002254053642388e-19 6 4 1.0591102939438040969e-19 7.7453621314834096271e-21 1.0208002254053642388e-19 5 4
            So here csverify behaves better.

            First it should be proven, that when the lower bound is better,
            that then also all Newton-steps are better (this should follow
            from convexity).
            If this is the case, is the above then just some random
            rounding-accident? Seems to be the case.
            We needed a form of ltau(a,b) which outputs all xi.
        (3) A stability analysis, computing the condition number
            https://en.wikipedia.org/wiki/Condition_number
            would be good to have.
        (4) One guess is that ltau(a,b) yields correct results except
            of the last decimal place for 10^-10 <= a, b <= 10^10.
            Are the most extreme cases most unprecise?
> ./ExploreBTs_debug 1e10 1e-10
1e-10 10000000000 4.2306755091738393851e-09 47 5 -4.2306755091738393843e-19 4.2306755091738393921e-19 7.7582627154270975526e-37 46.051701859880913682 4.2306755091738393851e-09 5 5 4.2307777597888414993e-09 4.3236297699972521429e-12 4.2306755091738393851e-09 3 5
ltau_hp([1b10,1b-10],1000);
(%o24) 4.2306755091738393847 95249967023141348709918
            Perhaps we should say that a precision of 0.5e-19 is achieved
            in this range?
        (5) In the range 1e-1000 to 1e1000 still good precision
            seems to be achieved:
> ./ExploreBTs_debug 1e-1000 1e1000
9.9999999999999999999e-1001 9.9999999999999999997e+999 4.5967370839868754998e-997 4602 3 -4.5967370839868754997e-1997 4.596737083986875179e-1997 -3.2076126754312066537e-2013 4605.1701859880913679 4.5967370839868754998e-997 4 3 4.5967370841863848969e-997 9.1709215305932042007e-1004 4.5967370839868754998e-997 2 3
ltau_hp([1b-1000,1b1000],2000);
Evaluation took 176.6050 seconds (177.6470 elapsed)
(%o27) 4.596737130709444299 4529627922553595163
ltau_hp([1b-1000,1b1000],4000);
Evaluation took 788.4770 seconds (792.2250 elapsed)
(%o28) 4.596737083986875501 070893889512919999863
ltau_hp([1b-1000,1b1000],6000);
Evaluation took 1965.4000 seconds (1973.6220 elapsed)
(%o29) 4.596737083986875501 070893889512919999863

(2) Alternative methods
   (a) When we start below the tau-value, we have strictly monotonically
       increasing convergence.
       Indeed, for any start value x0 # lt := ltau(a,b), due to
       strict convexity of the lchi-function, the next Newton-value x1
       will yield a value x1 < lt, and if x0 < lt, so x1 > x0.
       So from the upper-bound via one Newton-step we get a lower bound --
       is is worth it?
       It seems that this lower bound, obtained by ltau_down(a,b), is always
       better than ltau_Wlb(a,b),
   (b) The Newton-Fourier method
       https://en.wikipedia.org/wiki/Newton%27s_method#Newton–Fourier_method
       yields lower and upper bounds at the same time, using a somewhat
       changed second strand.
   (c) Via Halley's method
       https://en.wikipedia.org/wiki/Halley%27s_method
       we should obtain cubic convergence.

(3) Better approximation of ltau(1,a) for a >= 1.

    Question asked at
    https://mathoverflow.net/questions/320584/a-certain-generalisation-of-the-golden-ratio
    --- this yielded a very good upper and lower bounds, both asymptotically
   precise (BranchingTuples:: ltau_Wlb and ltau_Wub).
   Numerically this seems very good (and we have asymptotic equality):
     ltau(a,b) >= 1/b * (q - log(q) + 1/2 log(q) / q)
     for q := log(b/a) = log(b) - log(a).
   This yields
     ltau(1,a) >= 1/a * (log(a) - log(log(a)) + 1/2 log(log(a)) / log(a)),
   which asymptotically is log(a) / a.
   Thus ltau(a,b) (always assuming a <= b) is asymptotically
     log(b/a) / b, for b/a going to infinity.
   This solves shis question; for historical reasons keeping for now some
   of the older approaches:

   (a) In principle, handling of this case is enough, via
       ltau(a,b) = 1/a ltau(1,b/a) for a <= b.
   (b) ltau(1,1) = ln(2), ltau(1,2) = ln(1+sqrt(5)) - ln(2).
       At Maxima we see formulas via radicals for tau(1,3), tau(1,4), while
       the method fails for integers a >= 5 -- is this provably?
       tau(1,5): x^-1 + x^-5 = 1, <=> x^4 + 1 = x^5 <=> x^5 - x^4 - 1 = 0 ---
       is it provable that this polynomial has no solutions via radicals?
       Algorithms (implemented) should exist to decide this question.
   (c) The general polynomial is x^a - x^(a-1) - 1 = 0. Having more information
       on this for natural numbers a would be helpful.
       The related reccurrences are
       f(n) = f(n-1) + f(n-a). In Maxima, for a=5:
       f5(n) := if n<5 then n else f5(n-1) + f5(n-5)
       for n >= 0. It seems that this sequence is not in the OEIS.
         create_list(f5(n),n,0,20);
         [0,1,2,3,4,4,5,7,10,14,18,23,30,40,54,72,95,125,165,219,291]
   (e) The general bounds we have are,
       using the general bounds
         mtau(a,b) := ln(2) / ltau(a,b)
         sqrt(a*b) <= mtau(a,b) <= (a+b)/2,
       which yields
         ltau(1,a) >= 2ln(2) / (a+1)
         ltau(1,a) <= ln(2) / sqrt(a).

       One would guess that the upper bound is closer to the truth?
       Considering the quotients, that's only true for parameter a not too big
       (and then it changes).

       For the curve-fitting as above, one would guess an approach
         f(x) ~ alpha / (x+gamma)^beta
       for 1/2 < gamma < 1, should yield a reasonable fit.
       That makes
         ln(f(x)) ~ ln(alpha) - beta * ln(x+gamma).
       Numerical evaluation below yields
         f(x) ~ exp(1.024) / (x+7.29)^0.9065
       but other models (also with three parameters) are better.
       HOWEVER, as seen above, f(x) ~ ln(x) / x.

Numerical data:

First the direct data (non-logarithmic):

> ./ExploreBTs H > Data.txt
> for (( e=0; e<=9; )); do a1="1e${e}"; ((++e)); a2=" 1e${e}"; ./ExploreBTs ${a1} ${a2} 10000 >> Data.txt; done

> E=read.table("Data.txt", header=TRUE)
> x=E$b
> y=E$ltau
> plot(log(x), log(y))
> M=nls(log(y) ~ alpha * (log(x+gamma))^beta, start=list(alpha=-0.6,beta=1.1,gamma=1))
> summary(M)
Parameters:
        Estimate Std. Error  t value Pr(>|t|)
alpha -6.174e-01  3.839e-05 -16081.9   <2e-16 ***
beta   1.110e+00  2.173e-05  51062.5   <2e-16 ***
gamma  1.462e+00  1.813e-03    806.2   <2e-16 ***
Residual standard error: 0.02384 on 99997 degrees of freedom
Number of iterations to convergence: 4
Achieved convergence tolerance: 2.901e-06

So f(x) ~ exp(-0.6174 * log(x + 1.462)^1.11).

Alternatively (it seems nicest to consider log(x) versus -log(y)):

> M2=nls(-log(y) ~ alpha * log(x)^beta + gamma, start=list(alpha=1,beta=1,gamma=0))
> summary(M2)
Parameters:
       Estimate Std. Error t value Pr(>|t|)
alpha 5.567e-01  1.333e-04  4176.9   <2e-16 ***
beta  1.139e+00  7.262e-05 15686.2   <2e-16 ***
gamma 2.629e-01  4.793e-04   548.5   <2e-16 ***
Residual standard error: 0.03879 on 99997 degrees of freedom
Number of iterations to convergence: 4
Achieved convergence tolerance: 4.07e-07
> plot(log(x), -log(y))
> lines(log(x), predict(M2))

So f(x) ~ exp(-0.5567 * log(x)^1.139 + 0.2629).

Considering

> plot(log(log(x)), log(-log(y)))

it seems that the function becomes linear asymptotically.
What kind of role model is appropriate here?

Considering the above guess:
> M3=nls(log(y) ~ alpha - beta * log(x+gamma), start=list(alpha=1,beta=1,gamma=0))
> summary(M3)
Parameters:
       Estimate Std. Error t value Pr(>|t|)
alpha 1.024e+00  1.060e-03   966.3   <2e-16 ***
beta  9.065e-01  7.193e-05 12603.4   <2e-16 ***
gamma 7.290e+00  2.024e-02   360.2   <2e-16 ***
Residual standard error: 0.119 on 99997 degrees of freedom
Number of iterations to convergence: 8
Achieved convergence tolerance: 3.179e-06

In all cases we have the initial behaviour, closer to the upper bound,
which then turns to being close to the lower bound.


Concerning the number of iterations:

> summary(log(x)-E$ldiff)
      Min.    1st Qu.     Median       Mean    3rd Qu.       Max.
-3.553e-15  0.000e+00  0.000e+00  1.500e-19  0.000e+00  3.553e-15
> L=lm(E$N ~ E$ldiff)
> summary(L)
Residuals:
    Min      1Q  Median      3Q     Max
-4.1579 -0.3915 -0.1040  0.2898  2.9700
Coefficients:
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 4.1579086  0.0034782    1195   <2e-16 ***
E$ldiff     0.9009774  0.0002549    3534   <2e-16 ***
Residual standard error: 0.5352 on 99998 degrees of freedom
Multiple R-squared:  0.9921,    Adjusted R-squared:  0.9921
F-statistic: 1.249e+07 on 1 and 99998 DF,  p-value: < 2.2e-16


Now the logarithmic data (large):

> ./ExploreBTs L > Data.txt
> for (( e=0; e<=999; )); do a1="1e${e}"; ((++e)); a2=" 1e${e}"; ./ExploreBTs ${a1} ${a2} 1000 L >> Data.txt; done

> E=read.table("Data.txt", header=TRUE)
> L=lm(E$lltau ~ E$la)
> summary(L)
Residuals:
    Min      1Q  Median      3Q     Max
-0.4101 -0.3388 -0.1315  0.1946  5.5797
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -5.213e+00  1.018e-03   -5122   <2e-16 ***
E$la         9.987e-01  7.654e-07 1304780   <2e-16 ***
Residual standard error: 0.5088 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 1.702e+12 on 1 and 999998 DF,  p-value: < 2.2e-16

> L2=lm(E$N ~ E$la)
> summary(L2)
Residuals:
    Min      1Q  Median      3Q     Max
-1.1350 -0.4728 -0.1217  0.3320  6.3422
Coefficients:
              Estimate Std. Error  t value Pr(>|t|)
(Intercept) -1.761e-01  1.394e-03   -126.4   <2e-16 ***
E$la         9.986e-01  1.048e-06 952722.6   <2e-16 ***
Residual standard error: 0.6967 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 9.077e+11 on 1 and 999998 DF,  p-value: < 2.2e-16

> L3=lm(E$lltau ~ E$N)
> summary(L3)
Residuals:
    Min      1Q  Median      3Q     Max
-2.3077 -0.2354  0.0666  0.3674  5.4029
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -5.036e+00  9.514e-04   -5294   <2e-16 ***
E$N          1.000e+00  7.165e-07 1395739   <2e-16 ***
Residual standard error: 0.4756 on 999998 degrees of freedom
Multiple R-squared:      1,     Adjusted R-squared:      1
F-statistic: 1.948e+12 on 1 and 999998 DF,  p-value: < 2.2e-16

Considering g(x) := -ln(ltau(1,x)) - ln(x) for x >= 1:

 - g(1) > 0
 - g is strictly decreasing, with g(x)=0 for
   x0 ~ 2.18019225601615510012.
 - g(x) goes to -infinity, but very slowly, e.g.
   g(10^1000) ~ -7.738421, g(10^10000) ~ -10.04.
 - So for x >= x0 we have
     -ln(ltau(1,x)) <= ln(x),
     <=> ltau(1,x) >= 1/x
   which is easy to prove (below).
   The quotient ltau(1,x) / (1/x) goes to infinity (slowly).

   2 ln(2) / (x+1) = 1/x <=> 2 ln(2) x = x+1 <=> x = 1/(2 ln(2) - 1)
   <=> x = 2.58..., so for x >= 2.58... the standard lower bound
   is a better aproximation. There isn't much information here -- the rather
   poor lower bound 1/x is rendered "good" due to taking the logarithm of
   the result (a double-logarithm in effect).

   We know via the W-bounds that ltau(1,x) is asymptotically equal to
   log(x) / x, so
     g(x) ~ -log(log(x))
   indeed verifying the above numerical computations.
   E.g. -log(log(10^1000)) = -7.741..., -log(log(10^10000)) = -10.04...

(4) The binary case ltau(a,b) seems now to be handled rather well
    The next step beyond that is ltau(a,b,c).

(5) How does ltau(a,b) computed with double (instead of long double)
    compare?

(6) Compilation

    (a) The sizes of the static executable vary quite a lot:
        csltok (gcc 7.4):
> ls -l ExploreBTs*
-rwxr-xr-x 1 kullmann users 12409887 Jan 18 05:19 ExploreBTs
-rwxr-xr-x 1 kullmann users   346198 Jan 18 05:20 ExploreBTs_debug
        cswsok (gcc 7.4):
> ls -l ExploreBTs*
-rwxr-xr-x 1 csoliver users 12405103 Jan 18 15:28 ExploreBTs
-rw-r--r-- 1 csoliver users    30416 Jan 17 22:56 ExploreBTs.cpp
-rwxr-xr-x 1 csoliver users   346198 Jan 18 15:28 ExploreBTs_debug
        Very similar.
        csverify (gcc 7.3):
$ ls -l ExploreBTs*
-rwxrwxr-x 1 csoliver csoliver 2483000 Jan 18 15:30 ExploreBTs
-rw-rw-r-- 1 csoliver csoliver   30416 Jan 17 22:56 ExploreBTs.cpp
-rwxrwxr-x 1 csoliver csoliver  344856 Jan 18 15:30 ExploreBTs_debug
        The debug-version is similar, but the optimised version
        here is much smaller.
        cs2-irfan536 (gcc 7.3):
$ ls -l ExploreBTs*
-rwxrwxr-x 1 irfan536 irfan536 2375280 Jan 18 17:51 ExploreBTs
-rw-rw-r-- 1 irfan536 irfan536   33804 Jan 18 17:51 ExploreBTs.cpp
-rwxrwxr-x 1 irfan536 irfan536  344920 Jan 18 17:51 ExploreBTs_debug
        Again, that shows similarity to csverify.

*/

#include <iostream>
#include <string>
#include <algorithm>

#include <cassert>
#include <cmath>

#include <Numerics/FloatingPoint.hpp>
#include <Random/Numbers.hpp>

#include "BranchingTuples.hpp"
#include "InOut.hpp"

namespace {

  const std::string version = "0.3.14";
  const std::string date = "2.3.2019";
  const std::string program = "ExploreBTs"
#ifndef NDEBUG
  "_debug"
#endif
;
  const std::string error = "ERROR[" + program + "]: ";

  namespace FP = FloatingPoint;

  struct Result_t {
    FP::float80 t;
    FP::uint_t c;
    int place;
  };
  std::ostream& operator <<(std::ostream& out, const Result_t r) {
    return out << FP::Wrap(r.t) << " " << r.c << " " << r.place;
  }

  enum class LBlevel { ave=1, Wlb=2, Wub=3 };
  std::ostream& operator <<(std::ostream& out, const LBlevel lev) {
    switch(lev) {
    case LBlevel::ave : out << "LB::average"; break;
    case LBlevel::Wlb : out << "LB::lower_bound"; break;
    case LBlevel::Wub : out << "LB::upper_bound"; break;
    }
    return out;
  }
  inline constexpr FP::float80 lb_choice(const FP::float80 a, const FP::float80 b, const LBlevel lv) noexcept {
    switch(lv) {
    case LBlevel::ave : return FP::log(4) / (a+b);
    case LBlevel::Wlb : return BranchingTuples::ltau_Wlb(a,b);
    case LBlevel::Wub : return BranchingTuples::ltau_Wublbu(a,b);
    default : return FP::log(4) / (a+b);
    }
  }
  // Version with counting iterations:
  inline constexpr Result_t ltau(const FP::float80 a0, const FP::float80 b0, const LBlevel lb = LBlevel::ave) noexcept {
    assert(a0 > 0);
    assert(b0 > 0);
    if (a0 == b0) return {FP::log(2)/a0, 0, 1};
    const FP::float80 a = (a0 <= b0) ? a0 : b0, b = (a0 <= b0) ? b0 : a0;
    assert(a < b);
    if (FP::isinf(b)) return {0, 0, 2};
    FP::float80 x0 = lb_choice(a,b,lb);
    FP::uint_t rounds = 0;
    const FP::float80 na = -a, nb = -b;
    while (true) {
      ++rounds;
      const FP::float80 Am1 = FP::expm1(na*x0), B = FP::exp(nb*x0);
      const FP::float80 fx0 = Am1 + B;
      if (fx0 < 0) return {x0, rounds, 3};
      if (fx0 == 0) return {x0, rounds, 4};
      const FP::float80 rfpx0 = 1 / FP::fma(b,B,FP::fma(a,Am1,a));
      assert(rfpx0 > 0);
      const FP::float80 x1 = FP::fma(fx0, rfpx0, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, rounds, 5};
      x0 = x1;
    }
  }


  // ltau(a,b) with double-computation:

  struct Result_t_double {
    double t;
    FP::uint_t c;
    int place;
  };
  struct Wrap_double {
    double x;
    Wrap_double() = default;
    Wrap_double(const double x) noexcept : x(x) {}
  };
  std::ostream& operator <<(std::ostream& out, const Wrap_double x) {
    const auto prec = out.precision();
    out.precision(std::numeric_limits<double>::digits10 + 2);
    out << x.x;
    out.precision(prec);
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const Result_t_double r) {
    return out << Wrap_double(r.t) << " " << r.c << " " << r.place;
  }

  inline constexpr double log_double(const double x) noexcept {
    return std::log(x);
  }
  inline constexpr bool isinf_double(const double x) noexcept {
    return std::isinf(x);
  }
  inline constexpr double exp_double(const double x) noexcept {
    return std::exp(x);
  }
  inline constexpr double expm1_double(const double x) noexcept {
    return std::expm1(x);
  }
  inline constexpr double fma_double(const double a, const double b, const double c) noexcept {
    return std::fma(a,b,c);
  }
  inline constexpr Result_t_double ltau_double(const double a0, const double b0) noexcept {
    assert(a0 > 0);
    assert(b0 > 0);
    if (a0 == b0) return {log_double(2) / a0, 0, 1};
    const double a = (a0 <= b0) ? a0 : b0, b = (a0 <= b0) ? b0 : a0,
      na = -a, nb = -b;
    assert(a < b);
    if (isinf_double(b)) return {0, 0, 2};
    double x0 = BranchingTuples::ltau_Wublbu_d(a,b,na,nb);
    FP::uint_t rounds = 0;
    while (true) {
      ++rounds;
      const double Am1 = expm1_double(na*x0), B = exp_double(nb*x0);
      const double fx0 = Am1 + B;
      if (fx0 < 0) return {x0, rounds, 3};
      if (fx0 == 0) return {x0, rounds, 4};
      const double rfpx0 = 1 / fma_double(b,B, fma_double(a,Am1,a));
      assert(rfpx0 > 0);
      const double x1 = fma_double(fx0, rfpx0, x0);
      assert(x1 >= x0);
      if (x1 == x0) return {x0, rounds, 5};
      x0 = x1;
    }
  }


  void output_header(std::ostream& out) {
    out << "a b ltau N rp eam1 eb sum ldiff ltau2 N2 rp2 ub lbd ltau3 N3 rp3\n";
  }
  void output_single(std::ostream& out, const FP::float80 a0, const FP::float80 b0) {
    const FP::float80 a = FP::min(a0,b0), b = FP::max(a0,b0);
    const auto res = ltau(a,b);
    const FP::float80 lt = res.t,
      Am1 = FP::expm1(-a*lt), B = FP::exp(-b*lt),
      sum = Am1 + B, pred_num_its = FP::log(b) - FP::log(a);
    const auto res2 = ltau(a,b,LBlevel::Wlb);
    const auto res3 = ltau(a,b,LBlevel::Wub);
    out << FP::Wrap(a) << " " << FP::Wrap(b) << " " << res << " " << FP::Wrap(Am1) << " " << FP::Wrap(B) << " " << FP::Wrap(sum) << " " << FP::Wrap(pred_num_its) << " " << res2 << " " << FP::Wrap(BranchingTuples::ltau_Wub(a,b)) << " " << FP::Wrap(BranchingTuples::ltau_Wublb(a,b) - BranchingTuples::ltau_Wlb(a,b)) << " " << res3 << "\n";
  }

  void output_1xheader(std::ostream& out) {
    out << "la lltau N rp\n";
  }
  void output_1xsingle(std::ostream& out, const FP::float80 a) {
    const auto res = ltau(1,a);
    out << FP::Wrap(FP::log(a)) << " " << " " << FP::Wrap(-FP::log(res.t)) << " " << res.c << " " << res.place << "\n";
  }

  void show_usage() noexcept {
    std::cout << "Usage:\n";
    std::cout << "One argument x=\"H\",\"L\",N: output the header resp. the log-header resp. perform N experiments; if using +N, then with double.\n";
    std::cout << "Two arguments: a, b > 0:\n";
    std::cout << " Output: a b ltau(a,b,lb1) N ret-p exp(-a*t)-1 exp(-b*t) sum ln(b)-ln(a) ltau(a,b,lb2) N2 ret-p2 ub lbdiff ltau(a,b,lb3) N2 ret-p3.\n";
    std::cout << "Three arguments: begin, end, number of items.\n";
    std::cout << "A fourth argument \"L\": now output the log-form.\n";
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc == 1) {
    show_usage();
    return 0;
  }

  if (argc == 2) {
    const std::string arg{argv[1]};
    if (arg == "L") output_1xheader(std::cout);
    else if (arg == "H") output_header(std::cout);
    else {
      const FP::UInt_t N = std::stold(arg);
      const RandGen::seed_t seed = InOut::timestamp();
      RandGen::randgen_t g(seed);
      FP::uint_t max = 0;
      assert(not arg.empty());
      const bool use_double = arg[0] == '+';
      if (not use_double) {
        FP::float80 sum = 0;
        const LBlevel lev = LBlevel::Wub;
        for (FP::UInt_t i = 0; i < N; ++i) {
          const FP::float80 a = FP::float80(g())+1, b = FP::float80(g())+1;
          const auto res = ltau(a,b,lev), res2 = ltau(1,a,lev), res3 = ltau(1,b,lev);
          sum += res.c + res2.c + res3.c;
          const auto nmax = std::max(std::max(res.c, res2.c), res3.c);
          if (nmax > max) {
            max = nmax;
            std::cout << nmax << " " << FP::UInt_t(a) << " " << FP::UInt_t(b) << "\n";
          }
        }
        std::cout << "N=" << N << ", seed=" << seed << ", lb-method=" << lev << ", mean=" << sum/(FP::float80(N)*3) << "\n";
      }
      else {
        FP::float80 sum = 0;
        for (FP::UInt_t i = 0; i < N; ++i) {
          const double a = double(g())+1, b = double(g())+1;
          const auto res = ltau_double(a,b), res2 = ltau_double(1,a), res3 = ltau_double(1,b);
          sum += res.c + res2.c + res3.c;
          const auto nmax = std::max(std::max(res.c, res2.c), res3.c);
          if (nmax > max) {
            max = nmax;
            std::cout << nmax << " " << Wrap_double(a) << " " << Wrap_double(b) << " " << res << "\n";
          }
        }
        std::cout << "N=" << N << ", seed=" << seed << ", mean=" << sum/(FP::float80(N)*3) << "\n";
      }
    }
    return 0;
  }

  if (argc == 3) {
    const FP::float80 a0 = std::stold(argv[1]), b0 = std::stold(argv[2]);
    output_single(std::cout,a0,b0);
    return 0;
  }

  if (argc >= 4) {
    const FP::float80 begin = std::stold(argv[1]), end = std::stold(argv[2]);
    if (end < begin) return 0;
    const FP::UInt_t N = std::stoull(argv[3]);
    if (N == 0) return 0;
    const FP::float80 delta = (end - begin) / N;
    const bool logeval = (argc == 5) ? true : false;
    for (FP::UInt_t i = 0; i < N; ++i) {
      const FP::float80 x = begin + i * delta;
      if (logeval) output_1xsingle(std::cout, x);
      else output_single(std::cout, 1, x);
    }
    return 0;
  }
}
