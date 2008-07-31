// Oliver Kullmann, 26.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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


  \todo Function application
  <ul>
   <li> What is the difference between "apply" and "xreduce" ?
    <ul>
     <li> When the arity of the function being applied isn't known, for 
     instance when the function is a "noun" or isn't defined at this point,
     "apply" continues to treat the function as an n-ary function where n is
     the size of the list argument, whereas "xreduce" treats the function as a
     binary function.
     \verbatim
(%i11) apply(nounify(union), [a,b,c,d]);
Evaluation took 0.00 seconds (0.00 elapsed) using 416 bytes.
(%o11)                                   union(a, b, c, d)
(%i12) xreduce(nounify(union), [a,b,c,d]);
Evaluation took 0.00 seconds (0.00 elapsed) using 696 bytes.
(%o12)                            union(union(union(a, b), c), d)
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo List operations
  <ul>
   <li> Adding an element:
    <ol>
     <li> A frequent operation is to add an element x to a list L *in-place*.
     </li>
     <li> Apparently the most efficient operations seems to be
     <code>L : cons(x,L)</code> resp. <code>L : endcons(x,L)</code>,
     where both operations seem to take place not in-place, but involve
     copying and re-assignment, making it very inefficient?? </li>
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
     <li> The function name for list-formation is "[", and for set-creation it
     is "{". </li>
     <li> "outermap" seems obscure, and does strange things with lists of lists.
     </li>
    </ol>
   </li>
   <li> DONE (document that create_list is to be used, and not makelist;
   and tell the Maxima mailing list that "makelist" should be "deprecated")
   List creation:
    <ol>
     <li> Better avoid makelist, since it is less powerfull then
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
  </ul>


  \todo Document important programming techniques
  <ul>
   <li> Bugs of Maxima and their corrections:
    <ol>
     <li> ext_integer_partitions </li>
     <li> corr_cartesian_product </li>
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
    </ol>
   </li>
   <li> Lists vs. arrays:
    <ol>
     <li> Arrays allow much faster index access than lists. </li>
     <li> However, running through a list L sequentially via "for x in L do"
     is fastest (so, given an array a, where one wants to run through the elements
     sequentially, it's fastest to do so by "for x in listarray(a)" do. </li>
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
   <li> Randomisation:
    <ol>
     <li> Given a seed 0 <= n < 2^32, the state of all randomised functions
     is set by set_random_state(make_random_state(n)). </li>
     <li> Randomised functions are "random", "random_permutation". </li>
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
     it seems better to define it as such a local arrau-function. </li>
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
       "part(A,1)" and the value by "part(A,2)". </li>
      </ol>
     </li>
     <li> See ComputerAlgebra/DataStructures/Lisp/HashMaps.mac for interfaces.
      <ol>
       <li> The NEW interface (which removes the problem associated with
       having lists as keys in the hash map) consists of the functions
        <ul>
         <li> sm2hm for creating hash-maps (from set-maps) </li>
         <li> set_hm for seting a single value, and compose_hm_sm for
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
     float (for example "newton"), and thus can't be used (succesfully)
     in Comp. </li>
     <li> It seems that an expression involving some bfloat-argument
     is always evaluated in bfloat-arithmetic. </li>
    </ol>
   </li>
   <li> Call by reference (or "call by name"):
    <ol>
     <li> Pass referency to x by "f('x). </li>
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


  \todo Partial compilation
  <ul>
   <li> "compfile" writes Lisp-code into files? </li>
   <li> It seems that we should use "compile(all)" from time to time, and also
   with "oklib".
    <ol>
     <li> There are warnings about undefined global variables, but since they
     involve even for example "inf" it seems we can ignore these warnings? </li>
     <li> And there are error messages? </li>
     <li> And once we used compile(all), we get messages 
     "Illegal `remvalue' attempt" ?? </li>
     <li> "define_variable" can be used to get rid off some warnings, but since
     the return types are so restricted, it only applies for a few cases. </li>
     <li> Furthermore there is "translate(all)" ??? What's the difference? </li>
     <li> The output looks rather similar, though there are no error messages. </li>
     <li> For example "all_def34n : all_unsinghitting(3,4,'all_def34);"
     stops working after "compile(all)" or "translate(all)", so perhaps
     we can forget all that here? </li>
     <li> DONE (apparently with Maxima 5.15.0 it works again)
     Now it stopped working at all, and so we need to investigate the
     failures. </li>
    </ol>
   </li>
  </ul>


  \todo Document simplification of expressions
  <ul>
   <li> Apparently "is(a = b)" only applies very basic simplification
   rules, for example "is(4+5=9)" is true, while for "equivalence" one
   needs to use "is(equal(a,b))":
   \verbatim
is(equal(log(4)/log(2),2));
true
is(log(4)/log(2) = 2);
false
   \endverbatim
   </li>
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

*/

