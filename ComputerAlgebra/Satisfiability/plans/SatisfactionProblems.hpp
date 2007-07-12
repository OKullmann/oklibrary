// Oliver Kullmann, 29.6.2007 (Swansea)

/*!
  \file ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp
  \brief Plans for satisfaction problems (in general)
  
  
  \todo Variables
  <ul>
   <li> The set of all variables in some context is Va. </li>
  </ul>

  
  \todo Value set
  <ul>
   <li> The set of all possible values for the variable in Va (in some context) is Val. </li>
  </ul>


  \todo Total assignment
  <ul>
   <li> one total assignment: f: Va -> Val </li>
   <li> the set TASS of all total assignments </li>
  </ul>
   

  \todo Domain association and allowed total assignments
  <ul>
   <li> A domain association is a special kind of condition, given by a set of signed literals,
   which either specify the set of allowed or forbidden values for some variables. </li>
   <li> So a "domain association" is nothing than a partial assignment, and perhaps we
   should drop this notion at all --- there is a set of variables and its associated value
   set (the set of possible values for any variable), and if one wishes to introduce restricted
   variable domains, then a partial multiassigment is to be considered ?! </li>
   <li> So a "problem" typically is given as a pair (P, phi), where P is a problem instance and
   phi is a partial multiassignment. One can speak of phi as the "domain association". </li>
  </ul>
  

  \todo The notion of "condition"
  <ul>
   <li> A condition is a map from TASS to {0,1}. </li>
   <li> A solution is a total assignment evaluating to 1; perhaps better,
   a "satisfying assignment", since as interesting (in general) are the "falsifying
   assignments". </li>
   <li> So for a most general condition, all what can be done (at first) is to run through
   all total assignment; this is the "oracle model". </li>
   <li> The fundamental task (for the theory of generalised satisfiability(!))
   for a condition P (like "problem"):
   <center> Represent P^{-1}(0) and P^{-1}(1). </center>
   "Representation" of sets of total assignments can mean different things:
    <ol>
     <li> Measure the set, either with its natural probability in the product probability
     space TASS, or just classifying it as empty or non-empty. </li>
     <li> Give a power-clause-set representation, i.e., a signed CNF-representation of P^{-1}(0) resp.
     a signed DNF-representation of P^{-1}(1), where we have several refinements:
      <ol>
       <li> A prime power-clause-set, that is, a prime signed CNF resp. a prime signed DNF: no clause can be
       replaced by a smaller one (where for CNF-clauses C,D it is C smaller than D iff C semantically
       implies D). </li>
       <li> A hitting power-clause-set, that is, a hitting signed CNF resp. a hitting signed DNF:
       each pair of clauses clash, i.e., the sets of represented total assignments
       are disjoint. </li>
       <li> A special case of a hitting clause-set is a full clause-set, where each
       pair of clauses contains (exactly) the same variables. A full DNF representation
       is just how "constraint satisfaction problems" are commonly understood.
      </ol>
      The power-clause-set can be given either explicitly listed, or "online", one clause after another.
     </li>
     <li> Give a BDD representation; this represents directly P^{-1}(0) and P^{-1}(1)
     together. </li>
    </ol>
   In each case, also partial information is important:
    <ol>
     <li> Regading measurement, we can have
      <ol>
       <li> upper bounds </li>
       <li> lower bounds </li>
       <li> approximations </li>
      </ol>
     </li>
     <li> For power-clause-set representations, we can have sub-clause-sets. </li>
     <li> BDD representations are harder to make partial, since it represents satisfying
     and falsifying assignments at the same time. </li>
    </ol>
   </li>
   <li> Given a partial multi-assignment phi, the most fundamental operation is the application
   phi*P, which is the condition which is false for total assignments f not covered by phi,
   and P(f) otherwise.
    <ol>
     <li> So phi*P in general is just the pair (P,phi) explained above as "problem".
     The fact, that a variable v has been eliminated, since phi assigned
     a specific value to v (in general phi might only restrict v to a subset of its domain),
     can be recorded explicitely; forgetting this explicit recording, we arrive at the
     usual notion of the application of a partial assignment to a generalised clause-set. </li>
     <li> For clause-sets we can forget the variables which already have been assigned, since
     we can define a composition of partial assignments where the first assignments always
     renders a later assignment void; however in the present of multi-assignments this is
     not possible. </li>
     <li> This notion of phi*P is satisfiability centered, we also need phi*'P, which renders
     every total assignment not covered by phi true. </li>
    </ol>
   </li>
   <li> Now the above fundamental task is to be considered for phi * P (and phi *' P). </li>
   <li> A set of variables V is a "backdoor" for P regarding one of the above fundamental tasks,
   if for every assignment of values to the variables in V the task can be "efficiently" solved.
   The knowledge about some such backdoors is important structural information, either given with
   the problem instance itself (a priori), or computed later (a posteriori). Given some set
   of variables, an interesting task is to find a smallest backdoor (from the ones we know)
   which includes this set.
    <ol>
     <li> Consider as an example the condition given by AES (see Cryptanalysis/plans/Rijndael.hpp):
     This is a condition on n+n+k boolean variables, where n is block length and k the key length.
     Given n+?+k or ?+n+k variables, the remaining variables are uniquely determined and can
     be efficiently computed by the encryption resp. decryption algorithm. </li>
     <li> So we have two backdoors for computing a full DNF representation. </li>
    </ol>
   </li>
  </ul>


  \todo Partial assignments
  <ul>
   <li> A finite number of variables gets a value. </li>
   <li> Inclusion relation </li>
   <li> Composition of partial assignments </li>
   <li> Multi-assignments </li>
  </ul>


  \todo %Evaluation of partial assignments


*/

