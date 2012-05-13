// Oliver Kullmann, 26.6.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/MaximaTechniques.hpp
  \brief Plans on discussions and documentation for (built-in) Maxima-functionality


  \todo Local variables
  <ul>
   <li> How to make sure, that variables are local? </li>
   <li> Is "local" needed? For what is it good for? </li>
   <li> It seems not necessary to protect the parameter, or? </li>
   <li> Can everything done with "block" ? </li>
   <li> How to obtain information about symbols (so that for example clashes
   can be avoided) ? </li>
   <li> A serious problem is that function-parameters are overwritten by
   existing functions with the same name.
    <ol>
     <li> See 'Function "min_scanning"' in
     ComputerAlgebra/Numerical/Lisp/plans/general.hpp. </li>
     <li> So we reserve names "f", possibly followed by digits: Nobody
     shall define functions with these names. </li>
     <li> For function definitions using function parameters, we use then
     these special names, and also kill the ones used before. </li>
     <li> Actually, using
     \verbatim
function(f) := block(local(f), ...)
     \endverbatim
     solves most problems, except that if a global function "f" is defined,
     and we call "function(f)", then inside the block this function is
     not evaluated. </li>
     <li> However, seems much better than the current practice "kill(f)". </li>
    </ol>
   </li>
   <li> Regarding time efficiency:
    <ol>
     <li> At C++ level, definitely every variable is declared as local as
     possible. </li>
     <li> However here, with dynamic scoping, creation of blocks is expensive.
     </li>
     <li> Of course, in general we do disregard efficiency consideration w.r.t.
     implementation issues at Maxima level. </li>
     <li> Though in case we have a simple loop, which uses some auxiliary
     local variable, and where the variable can be easily declared just outside
     of the loop, then we can place the auxiliary variable in the block just
     outside of the loop. </li>
    </ol>
   </li>
  </ul>


  \todo Declaring variables
  <ul>
   <li> Declaring global variables:
    <ol>
     <li> The modes ("types") of "define_variable" are
     \verbatim
number,fixnum,rational,boolean,float,list,any
     \endverbatim
     (for example "inf" is of type "any"). </li>
     <li> Unclear what fixnum means: It can store for example 100000!.
     It appears just to be an integer. Perhaps this is special for
     CLisp? </li>
     <li> And apparently "float" = "any" ?? </li>
     <li> It seems we should declare all of our global variables
     in this way. </li>
     <li> It can be used also inside functions, after the block-
     declaration (or elsewhere). </li>
     <li> However, inside functions mode_declare is more appropriate,
     since it leaves out the default-value. </li>
    </ol>
   </li>
   <li> See "Variables" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Generators.hpp. </li>
  </ul>


  \todo Elementary arithmetic
  <ul>
   <li> For sums or products over lists, perhaps using a function, apply
   sum_l(L), sum_s(S), gsum_l(f,L), gsum_s(f,S), and similar for "prod"
   (see ComputerAlgebra/DataStructures/Lisp/Lists.mac). This is much faster
   than "sum", but cannot handle symbolic calculations. </li>
   <li> The natural logarithm (to base e) is log(x), to base 10 it's log10(x),
   while the binary logarithm is ld(x) (defined in
   ComputerAlgebra/NumberTheory/Lisp/Auxiliary.mac). </li>
  </ul>


  \todo List operations
  <ul>
   <li> cons and endcons:
    <ol>
     <li> cons is considerably faster than endcons. </li>
     <li> So, if needed, it is much faster building up a list using
     cons and reverse at the end than by using endcons. </li>
    </ol>
   </li>
   <li> Adding an element:
    <ol>
     <li> A frequent operation is to add an element x to a list L *in-place*.
     </li>
     <li> Apparently the most efficient operations seems to be
     <code>L : cons(x,L)</code> resp. <code>L : endcons(x,L)</code>,
     where both operations seem to take place not in-place, but involve
     copying and re-assignment, making it very inefficient. </li>
     <li> Though cons is rather harmless. </li>
     <li> In general we want to avoid any efficiency-considerations, however
     having in-place modifications of lists would also increase ease of
     use. </li>
    </ol>
   </li>
   <li> map and relatives:
    <ol>
     <li> Names of ordinary operations are "+", "*", etc., for example
     \verbatim
map("+",[1,2],[3,4]) = [4,6]
      \endverbatim
     </li>
     <li> The function name for list-formation is "[", while for set-creation
     it is "set" (one could also use "{", but this could invoke some
     Maxima-bugs, and "set" seems also more readable). </li>
     <li> "outermap" seems obscure, and does strange things with lists of lists.
     </li>
    </ol>
   </li>
   <li> What is the difference between "apply" and "xreduce" ?
    <ul>
     <li> When the arity of the function being applied isn't known, for 
     instance when the function is a "noun" or isn't defined at this point,
     "apply" continues to treat the function as an n-ary function where n is
     the size of the list argument, whereas "xreduce" treats the function as a
     binary function.
     \verbatim
apply(nounify(union), [a,b,c,d]);
  union(a, b, c, d)
xreduce(nounify(union), [a,b,c,d]);
  union(union(union(a, b), c), d)
     \endverbatim
     </li>
     <li> For "union" or "+" for example "apply" is much faster than "xreduce",
     and "xreduce" also cannot handle empty lists. </li>
    </ul>
   </li>
   <li> DONE (document that create_list is to be used, and not makelist;
   and tell the Maxima mailing list that "makelist" should be "deprecated")
   List creation:
    <ol>
     <li> Better avoid makelist, since it is less powerful then
     create_list, and also does not handle well the case of empty lists. </li>
     <li> Better, we should use makelist only if we are sure that the
     second list bound is not smaller than the first, and if we do not need
     the power of create_list? </li>
     <li> However, create_list is also much faster! So we do not use makelist
     anymore! </li>
    </ol>
   </li>
  </ul>


  \todo Set operations
  <ul>
   <li> See "Set creation" in 
   ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp. </li>
   <li> The Maxima function "makeset" creates only trouble, since it cannot
   handle "subscripts", and thus we do not use it. </li>
   <li> Regarding the underlying ordering:
    <ol>
     <li> The underlying ordering of sets is given by "orderlessp" (and the
     transposed, "ordergreatp"). </li>
     <li> It seems that "<" (resp. ">") realises only a subset of these
     relations, and thus shouldn't be used in this context. </li>
     <li> Is it guaranteed that the underlying order of at least "simple
     sets" is lexicographical ordering? "Simple sets" are sets of integers,
     sets of sets of integers, sets of lists of integers and so on. </li>
    </ol>
   </li>
  </ul>


  \todo Creating function-terms via lambda
  <ul>
   <li> How to simplify the function term?
    <ol>
     <li> Consider the following example:
     \verbatim
snbl2cdn(x) := [
 {var_snbl(x)}, 
  buildq([v:var_snbl(x), e:val_snbl(x), s:sgn_snbl(x)], lambda([ta], 
   if s=+1 then is(ta(v)=e) else is(ta(v)#e)))
]$

snbl2cdn([1,3,+1]);
 [{1},lambda([ta],if 1 = +1 then is(ta(1) = 3) else is(ta(1) # 3))]
     \endverbatim
     (from
     ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Conditions.mac)
     </li>
     <li> But actually we want the result
     \verbatim
[{1},lambda([ta],is(ta(1) = 3))]
     \endverbatim
     </li>
     <li> Using "optimize" makes things actually worse:
     \verbatim
snbl2cdn(x) := [
 {var_snbl(x)}, 
  optimize(buildq([v:var_snbl(x), e:val_snbl(x), s:sgn_snbl(x)], lambda([ta], 
   if s=+1 then is(ta(v)=e) else is(ta(v)#e))))
]$

snbl2cdn([1,3,+1]);
optimize: encountered a special form; answer may be wrong.
 [{1},block([%1],%1:ta(1),lambda([ta],if 1 = +1 then is(%1 = 3) else is(%1 # 3)))]
     \endverbatim
     </li>
     <li> "ev" should be helpful here, but either it does nothing or it leads
     to errors:
     \verbatim
snbl2cdn(x) := [
 {var_snbl(x)}, 
  ev(buildq([v:var_snbl(x), e:val_snbl(x), s:sgn_snbl(x)], lambda([ta],
  if s=+1 then is(ta(v)=e) else is(ta(v)#e))), simp,eval,eval)
]$
snbl2cdn([1,3,+1]);
 [{1},lambda([ta],if 1 = +1 then is(ta(1) = 3) else is(ta(1) # 3))]
snbl2cdn(x) := [
 {var_snbl(x)}, 
  ev(buildq([v:var_snbl(x), e:val_snbl(x), s:sgn_snbl(x)], lambda([ta],
  if s=+1 then is(ta(v)=e) else is(ta(v)#e))), simp,eval,eval,pred)
]$
snbl2cdn([1,3,+1]);
Maxima encountered a Lisp error:
 Not a valid property list ($TA)
     \endverbatim
     </li>
     <li> And if attempting to use eval with "pred" to the obtained
     expression, then Maxima runs into an infinite loop! </li>
     <li> A solution is achieved by
     \verbatim
snbl2cdn(x) := block([v:var_snbl(x), e:val_snbl(x), s:sgn_snbl(x), t, ta],
 t : if s=+1 then buildq([v,e],is(ta(v)=e)) 
  elseif s=-1 then buildq([v,e],is(ta(v)#e)) else
  buildq([s,v,e],if s=+1 then is(ta(v)=e) else is(ta(v)#e)),
 return([{v}, buildq([t], lambda([ta],t))]))$
     \endverbatim
     Is this complexity needed? </li>
    </ol>
   </li>
  </ul>


  \todo Bugs of Maxima and their corrections
  <ul>
   <li> Problems with the empty Maxima-digraph:
   \verbatim
is_sconnected(empty_digraph(1));
  true
is_sconnected(empty_digraph(2));
  false
is_sconnected(empty_digraph(0));
  false
   \endverbatim
   where the last result should be true. We have corrected this in
   sconnected_dg_p(G), but notify the Maxima mailing-list. </li>
   <li> set_partitions(n,k) produces sets which can not be used further.
   The temporary fix is to use apply "resimplify(expr):=expand(expr,1,1)$"
   to the result. </li>
   <li> Regarding the combination of memoisation and recursion, see
   "Weak recursion for memoised functions" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> apply
    <ol>
     <li> See "Apply-functionality" in
     ComputerAlgebra/DataStructures/Lisp/plans/Lists.hpp. </li>
    </ol>
   </li>
   <li> ext_integer_partitions </li>
   <li> DONE (corrected in Maxima itself)
   corr_cartesian_product </li>
   <li> "symmetricp(M)" from ctensor does not work as announced:
    <ol>
     <li> We have "symmetric_m_p" instead (in
     ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac). </li>
     <li> Tell the Maxima mailing list. </li>
    </ol>
   </li>
   <li> genmatrix(f,m,n) cannot handle the cases n=0: Use
   genmatrix_m(f,m,n) resp. genmatrix_sm(f,n) instead (in
   ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac). </li>
   <li> DONE (scalarmatrixp set to false now, which disables this unwanted
   behaviour)
   The "matrix multiplication .", applied to two 1x1 matrices, does not
   return a matrix but a number:
    <ol>
     <li> So before applying matrix multiplication A . B, one needs to check
     that matrix_size(X)[1] * matrix_size(X)[2] is at least 2 for X = A,B.
     </li>
     <li> This behaviour is unfortunate, since for example charpoly
     does not convert a single number into a square matrix. </li>
     <li> The reason is the overloading with scalar product. </li>
     <li> So use prod_m (in
     ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac) instead! </li>
     <li> Ask on the Maxima mailing list whether this behaviour can be
     turned off, or whether there is a named operator for matrix
     multiplication (so that overloading can be controlled); or perhaps
     functions like charpoly do accept single numbers as input. </li>
    </ol>
   </li>
   <li> charpoly:
    <ol>
     <li> Since Maxima uses the form of the characteristic polynomial, where
     the leading coefficient is (-1)^n (n the order), we replace it by
     the more standard form, where the leading coefficient is always 1:
     Use charpoly_m(M) instead! (Provided in
     ComputerAlgebra/CombinatorialMatrices/Lisp/Basics.mac.) </li>
     <li> This function also uses (internally) ratmx:true; see the following
     discussion. </li>
     <li> At least for matrices containing numbers, ratmx:true has to be
     set, otherwise the computations become infeasible even for relatively
     small matrices. </li>
     <li> However, with ratmx:true, floating-point numbers are replaced by
     rather crude approximations with rational fractions, and, rather
     annoyingly, for each such approximation output is produced. </li>
     <li> On the other hand, when computing the characteristic polynomial
     for a random 10x10 matrix with entries from [0,1), the computation
     of the characteristic polynomial happens rather quickly, but for
     example printing it seems impossible: The "computation" for the
     printing-action apparently quickly finishes, but without result,
     and actually an apparently endless computation is performed to produce
     the output. </li>
     <li> So also for floating-point matrices setting ratmx:true seems
     necessary. </li>
     <li> However, one should not set ratmx:true by default, since then
     suddenly strange dis-equalities between matrix-entries happen
     (matrices appear to be equal, have the same values, but the internal
     representations are different). </li>
     <li> Discuss this on the Maxima mailing list! </li>
     <li> Another related bug is that the computation seems to complete,
     "Evaluation took 0.0000 seconds (0.0001 elapsed) using 80 bytes.",
     but actually the system is computing, and doesn't take further
     input. </li>
     <li> There is ncharpoly in Maxima, which computes the characteristic
     polynomial with the right sign (though this is not documented!), and
     is faster for larger matrices; but it gets the empty matrix wrong, and
     is slower for smaller matrices --- charpoly_m handles all this. </li>
    </ol>
   </li>
   </li>
   <li> unique([2,1]) = [1,2]:
    <ol>
     <li> Use stable_unique instead (in
     ComputerAlgebra/DataStructures/Lisp/Lists.mac). </li>
     <li> Notify the Maxima mailing-list about the incomplete
     documentation. </li>
    </ol>
   </li>
   <li> "every" and "some" always run through the whole list:
    <ol>
     <li> So except of cases where this is what is needed, every_s and 
     some_s ("s" for "short circuit") shall be used (provided in
     DataStructures/Lisp/Lists.mac). </li>
     <li> Notify the Maxima mailing-list about the incomplete
     documentation! </li>
    </ol>
   </li>
   <li> "0^0" yields an error:
    <ol>
     <li> On the contrary x^0 evaluates to 1 (for an unknown x). </li>
     <li> The option-variable dotident is the result of x^^0; what is
     the difference between "x^0" and "x^^0" ? </li>
     <li> And 0^x evaluates to 0 for an unknown x, which is obviously
     incorrect! </li>
     <li> Via "pow(b,e)" in ComputerAlgebra/NumberTheory/Lisp/Auxiliary.mac
     we try to correct the most blatant false behaviour. </li>
     <li> Notify the Maxima mailing-list! </li>
    </ol>
   </li>
   <li> random_tree(n) :
    <ol>
     <li> The function random() is not used (so that one cannot control
     the random generator). </li>
     <li> It is not a random choice amongst all n^(n-2) trees. </li>
     <li> Use randomtree_pr1_og (in
     ComputerAlgebra/Graphs/Lisp/Trees/Generators.mac) instead (the random
     process is specified, and random() is used). </li>
     <li> Tell the Maxima mailing list to improve the documentation of
     random_tree(n). </li>
    </ol>
   </li>
  </ul>


  \todo Issues with internal representations
  <ul>
   <li> Maxima functions for polynomials like gcdex produce their output
   (in most cases!) in so-called CRE-form. </li>
   <li> Whether some part of the term contains CRE-elements can be seen by 
   setting "display2d:true" (the default), where then "/R/" shows up.
   Unfortunately, just using disp2d doesn't work. </li>
   <li> We provide the test-function tcre_p(p), which is true iff p contains
   some element in CRE-form. </li>
  </ul>


  \todo Document important programming techniques
  <ul>
   <li> Lists vs. arrays:
    <ol>
     <li> Arrays allow much faster index access than lists. </li>
     <li> However, running through a list L sequentially via "for x in L do"
     is fastest (so, given an array a, where one wants to run through the
     elements sequentially, it's fastest to do so by "for x in listarray(a)" 
     do. </li>
     <li> So, when running through a list L where one also needs access to the
     index i, it's fastest to do 
     \verbatim
block([i:1], for x in L do (loop actions; i : i + 1))
     \endverbatim
     </li>
       <li> This is also true when changing the elements, e.g.
     \verbatim
for i : 1 thru 10000 do (B[i] : B[i] + 1);
     \endverbatim
     is slower than
     \verbatim
block([i:1],for x in B do (B[i] : x+1, i:i+1));
     \endverbatim
     </li>
     <li> But fastest is
     \verbatim
B : map(lambda([x],x+1),B)$
     \endverbatim
     Thus, when changing whole lists, this most elegant solution is also 
     fastest. </li>
     <li> A problem with arrays is, that their indices are 0-based, different
     from all the other conventions at the Maxima/Lisp level (accordingly
     the graphs-module uses 0-based indices for its own graphs). </li>
    </ol>
   </li>
   <li> "first(L), rest(L), cons(x,L)" belong together, and are faster than
   "last(L), rest(L,-1), endcons(x,L)" (lists are stored single-linked). </li>
   <li> Loops:
    <ol>
    <li> In "for x in X do" list/set X is evaluated only once, before
    entering the loop. </li>
    <li> In "for i : a thru b do" term b is evaluated with every iteration,
    while a is evaluated only once. </li>
    <li> Conditions under "while" and "unless" are evaluated with every
    iteration. </li>
    </ol>
   </li>
   <li> Shallow copy of list arguments for functions:
    <ol>
     <li> Give examples for this behaviour. </li>
     <li> This effects especially clause-sets (which as sets are
     copied) and formal clause-sets (which as lists are only shallowly
     copied)! </li>
    </ol>
   </li>
   <li> How to handle local variables (see above).
    <ol>
     <li> A source of errors regarding the block-expression is that
     returns inside it just leave *this* block --- so one has to
     be careful by introducing blocks (since they change the semantics)! </li>
    </ol>
   </li>
   <li> Local functions
    <ol>
     <li> Local functions can be defined, e.g.,
     \verbatim
block([f], local(f), f(x) := x)
     \endverbatim
     and also array-functions (supporting memoisation) are possible. </li>
     <li> However it seems that these local functions are not values, and thus
     cannot be carried around. </li>
     <li> So if for example just an auxiliary hash-function is needed, then
     it seems better to define it as such a local array-function. </li>
    </ol>
   </li>
   <li> All different types of loops (see the existing code).
   Especially nested loops ("create_list"). </li>
   <li> How to create lambda-terms (see the existing code). </li>
   <li> Function-application (also "map" etc.; see above). </li>
   <li> Hash tables
    <ol>
     <li> Package "graphs" establishes the following DEPRECATED interface to
     lisp-hash-tables. </li>
     <li> First use "load(graphs);" </li>
     <li> Via "h : hash_table()" a new hash table is created. </li>
     <li> Via "set_hash("abc", h, 78)" we set h("abc") = 78. </li>
     <li> Via "get_hash("abc", h)" we get (h("abc") if defined, and
     false otherwise. </li>
     <li> And via "hash_table_data(h)" we get the list of all assignments
     in h.
      <ol>
       <li> For an assignment A ("x -> y") we obtain the argument by
       "first(A)" and the value by "second(A)". </li>
      </ol>
     </li>
     <li> See ComputerAlgebra/DataStructures/Lisp/HashMaps.mac for interfaces.
      <ol>
       <li> The NEW interface (which removes the problem associated with
       having lists as keys in the hash map) consists of the functions
        <ul>
         <li> sm2hm for creating hash-maps (from set-maps) </li>
         <li> set_hm for setting a single value, and compose_hm_sm for
         setting the values of a whole set-map </li>
         <li> ev_hm for evaluating the hash-map at a single argument, and hm2sm
         for creating a whole set-map out of it. </li>
        </ul>
       </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Dynamic binding: Important to understand this, since nowadays
   static binding is most common. </li>
   <li> Arbitrary precision:
    <ol>
     <li> For a local computation Comp with d decimal digits use
     \verbatim
block([fpprec : fpprec], fpprec : d, Comp)
     \endverbatim
     </li>
     <li> Numbers used in Comp must be converted to bfloat, since otherwise
     exact computations are used, while number-literals like
     "1.0" are by default interpreted as float. </li>
     <li> However numerical procedures often enforce usage of type
     float (for example "newton"), and thus can't be used (successfully)
     in Comp. </li>
     <li> It seems that an expression involving some bfloat-argument
     is always evaluated in bfloat-arithmetic. </li>
    </ol>
   </li>
   <li> Call by reference (or "call by name"):
    <ol>
     <li> Pass reference to x by "f('x). </li>
     <li> Inside f, for assignments to x use "x :: value". </li>
     <li> And on the right-hand-side use "ev(x)". </li>
     <li> To avoid name-clashes, call the formal parameter e.g. "_x". </li>
    </ol>
   </li>
   <li> Plotting of functions:
    <ol>
     <li> Options are additional parameters to the plot-function, of
     the form "[option,value]". </li>
     <li> Plotting to a terminal window of x -> x*exp(x), with explicit
     specification of the y-label:
     \verbatim
plot2d(exp(x)*x,[x,-7,1],[ylabel,"x * exp(x)"],[gnuplot_term, x11]);
     \endverbatim
     With newer gnuplot-versions the specification of the terminal
     type shouldn't be needed. </li>
     <li> But also with gnuplot version 4.2.3 we get the error messages
     about "unknown or ambiguous terminal type" ?? </li>
     <li> However it works after "set_plot_option([gnuplot_term, x11]);". </li>
     <li> Creating a ps-file via:
\verbatim
plot2d(exp(x)*x,[x,-7,1],[ylabel,"x * exp(x)"],
  [gnuplot_term,postscript],[gnuplot_out_file,"xexpx.ps"]);
\endverbatim
     With newer gnuplot-versions the terminal-type specification "ps"
     should also work. </li>
     <li> But again this is not true. </li>
     <li> If the output-file is not specified, then a file "maxplot.ps"
     is created in the home-directory of the user. </li>
    </ol>
   </li>
  </ul>


  \todo Randomisation
  <ul>
   <li> Given a seed 0 <= n < 2^32, the state of all randomised functions
   is set by set_random_state(make_random_state(n)). </li>
   <li> This is achieved by the OKlibrary-function "set_random"
   (in ComputerAlgebra/DataStructures/Lisp/Lists.mac). </li>
   <li> Randomised functions are "random", "random_permutation". </li>
   <li> A copy of the current random state is obtained by
   make_random_state(false). </li>
   <li> So for example to compute a random_permutation from seed n, without
   disturbing the global random state, use
   \verbatim
block([s : make_random_state(false)],
  set_random(n),
  ... use random_permutation ...
  set_random_state(s)
)
   \endverbatim
   </li>
   <li> Making runs using the Lisp-random-function reproducible (for example
   ifactors using randomisation driven by this function):
   \verbatim
:lisp (defvar s (make-random-state))
(%i27) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 91.8810 seconds (92.0770 elapsed)
(%i28) :lisp (setq *random-state* (make-random-state s))
#<random-state 0000000015815000>
(%i28) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 90.3660 seconds (90.5580 elapsed)
(%i29) :lisp (setq *random-state* (make-random-state s))
#<random-state 0000000016e87000>
(%i29) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 90.9720 seconds (91.1640 elapsed)
(%i30) :lisp (defvar s2 (make-random-state))
S2
(%i30) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 46.4100 seconds (46.5370 elapsed)
(%i31) :lisp (setq *random-state* (make-random-state s2))
#<random-state 0000000016f92000>
(%i31) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 46.8380 seconds (46.9590 elapsed)
(%i32) :lisp (setq *random-state* (make-random-state s2))
#<random-state 0000000016dcf000>
(%i32) ifactors(1000000000000000000000000000000000000000000000000000000000007-1)$
Evaluation took 46.5850 seconds (46.6750 elapsed)
   \endverbatim
   </li>
   <li> Via "(make-random-state)" a copy of the current state is created. </li>
   <li> This copy is stored in s (note: Lisp is *reference-based*). </li>
   <li> And via "(make-random-state s)" a copy of this state is created (only
   in this way the state s can be re-used, i.e., is not overwritten). </li>
   <li> This is the outcome of a discussion on the Maxima mailing list
   22/23.10.2011. </li>
  </ul>


  \todo Partial compilation
  <ul>
   <li> "compfile" writes Lisp-code into files? </li>
   <li> It seems that we should use "compile(all)" from time to time, and also
   with "oklib".
    <ol>
     <li> There are warnings about undefined global variables, but since they
     involve even for example "inf" it seems we can ignore these warnings?
     </li>
     <li> And there are error messages? </li>
     <li> And once we used compile(all), we get messages 
     "Illegal `remvalue' attempt" ?? </li>
     <li> "define_variable" can be used to get rid off some warnings, but since
     the return types are so restricted, it only applies for a few cases. </li>
     <li> Furthermore there is "translate(all)" ??? What's the difference? 
     </li>
     <li> The output looks rather similar, though there are no error messages.
     </li>
     <li> For example "all_def34n : all_unsinghitting(3,4,'all_def34);"
     stops working after "compile(all)" or "translate(all)", so perhaps
     we can forget all that here? </li>
     <li> With Maxima 5.18.1 and Ecl 9.4.1 compile(all) runs quite a while,
     and seems to succeed (after having first used oklib_load_all()), but
     then we get for example
     \verbatim
gt34_200 : greentao_nbfcsud([3,4],200)$
Maxima encountered a Lisp error:
 The function $ARITHPROG_PRIMES_OHG is undefined.
arithprog_primes_ohg(3,10);
Evaluation took 0.0210 seconds (0.0220 elapsed)
(%o54) [[2,3,5,7,11,13,17,19,23,29],
        [{3,5,7},{3,7,11},{5,11,17},{3,11,19},{7,13,19},{3,13,23},{11,17,23},{5,17,29},{17,23,29}]]
     \endverbatim
     </li>
     <li> And apparently all kill-commands are ignored. </li>
     <li> And the gains seem to be tiny in most cases, e.g.
     \verbatim
(%i1) oklib_load_all();
(%i2) G : arithprog_primes_ohg(3,300)$
Evaluation took 17.0910 seconds (34.4490 elapsed)
(%i3) length(G[2]);
Evaluation took 0.0000 seconds (0.0000 elapsed)
(%o3) 4377
     \endverbatim
     versus
     \verbatim
(%i1) oklib_load_all();
(%i2) compile(arithprog_primes_ohg);
(%i3) G : arithprog_primes_ohg(3,300)$
Evaluation took 16.9090 seconds (33.8590 elapsed)
(%i4) length(G[2]);
Evaluation took 0.0000 seconds (0.0010 elapsed)
(%o4) 4377
     \endverbatim
     Perhaps stronger gains are realised when more functions are compiled,
     but then we get the above errors. </li>
     <li> For example, arithprog_primes_finish can't be compiled since it uses
     the variable primes_rev_init_seg, while compiling first arithprog_primes
     and then arithprog_primes_ohg seems to result just in a tiny speed-up. 
     </li>
     <li> DONE (apparently with Maxima 5.15.0 it works again)
     Now it stopped working at all, and so we need to investigate the
     failures. </li>
    </ol>
   </li>
   <li> How to store the result of compile for further sessions?
    <ol>
     <li> It seems that "compile(all)" has only an effect for the current
     session. </li>
     <li> And since it takes a long time to complete, this is not a viable
     action in most cases. </li>
     <li> It should be possible to store the results? </li>
     <li> Of course then we have the problem of managing changes? </li>
     <li> Perhaps we should use compile_file? </li>
     <li> Apparently translate and translate_file is a part of compile resp.
     compile_all ? </li>
     <li> However, "? translate" lists many bugs, while "? compile" doesn't?
     </li>
    </ol>
   </li>
  </ul>


  \todo Document simplification of expressions
  <ul>
   <li> DONE (use simplify_t)
   What is needed is "simplify(t)" for arbitrary terms t. </li>
   <li> General functions for simplifying terms:
    <ol>
     <li> radcan </li>
     <li> ratsimp </li>
     <li> factor </li>
     <li> expand </li>
     <li> ratexpand </li>
     <li> trigreduce </li>
     <li> trigsimp </li>
    </ol>
   </li>
   <li> DONE (use simplify_t)
   Simplifying equalities:
    <ol>
     <li> "is(a = b)" only applies very basic simplification
     rules, for example "is(4+5=9)" is true, while for "equivalence" one
     needs to use "is(equal(a,b))":
     \verbatim
is(equal(log(4)/log(2),2));
true
is(log(4)/log(2) = 2);
false
     \endverbatim
     </li>
     <li> Another example:
     \verbatim
is( (y1-y2)^2+(x1-x2)^2 = (y2-y1)^2+(x2-x1)^2 );
  false
is(equal((y1-y2)^2+(x1-x2)^2, (y2-y1)^2+(x2-x1)^2));
  true
     <li> DONE (implemented in simplify_t)
     According to Stavros Macrakis (Maxima mailing-list, 28.11.2011) we
     have that is(equal(a,b)) is equivalent to is(ratsimp(a-b)=0). </li>
    </ol>
   </li>
   <li> DONE (use simplify_t)
   To get for example "sum(i,i,1,n)" simplified, append ", simpsum". </li>
   <li> See 'What is "equalp" ?' in ComputerAlgebra/plans/Maxima.hpp. </li>
  </ul>


  \todo Timing
  <ul>
   <li> Is there a timing command ? </li>
   <li> "time (%o1, %o2, %o3, ...)" returns the time it took to
   compute the respective outputs. </li>
   <li> With "showtime: true " Maxima outputs the timing result
   after each computation. </li>
  </ul>


  \todo Printing
  <ul>
   <li> For any kind of investigations, printf should be used, and perhaps
   better documentation should be made available. </li>
   <li> Perhaps also the string-functions in
   ComputerAlgebra/DataStructures/Lisp/Strings.mac are of relevance here. </li>
  </ul>

*/

