// Oliver Kullmann, 14.1.2006 (Swansea)

/*!
  \file Trees/plans/general.hpp
  \brief Plans for the module with tree datastructures


  \todo Update namespaces.


  \todo Concept design "Rooted trees with ordered children"
  <ul>
   <li> Tree T:
    <ol>
     <li> fully constructible, with equality </li>
     <li> <code>vertex_type<Tree>::type;</code>
      fully constructible with equality and
      with singular value </li>
     <li> <code>root(T)</code> of type vertex_type </li>
     <li> <code>vertex_type v;</code> </li>
     <li> <code>Tree(T, v)</code> copy constructor for the subtree of T at v </li>
     <li> <code>children(T, v)</code> yields a range, whose value-type is vertex_type </li>
     <li> <code>parent(T, v)</code> of type vertex_type (singular iff v == root(T)) </li>
     <li> <code>property_map(m, T, v)</code> </li>
     <li> <code>swap(T1, T2)</code> </li>
     <li> <code>swap(T, v, T2, v2)</code> swaps the subtree of T at v with the
      subtree of T2 at v2 (swaps do not invalidate vertex descriptors nor iterators,
      but their meaning change). </li>
    </ol>
   </li>
   <li> <code>in_order(T, v), pre_order(T, v), post_order(T, v)</code> yield ranges to
   traverse the subtree of T at v. </li>
   <li> Refinement "mutable":
    <ol>
     <li> <code>new_child(T, iterator)</code> returns vertex_type with parent v, inserts at
      iterator (if iterator is begin, then this is front-insertion, if iterator is
      end, then this is back-insertion) </li>
     <li> <code>remove_child(T, iterator)</code> (again vertex descriptors and
      iterators are not invalidated (if possible), by both operations). </li>
    </ol>
   </li>
  </ul>


  \todo In a module GraphTrees rooted trees as Boost-graphs
  with one distinguished vertex establishing, while Parent-graph-trees
  additionally are equipped for every vertex with a property yielding the
  parent vertex (which in case of the root is the root itself).

*/

/*!
  \namespace OKlib::Trees
  \brief Module for trees (algorithms and data structures; all kinds of trees)
*/

namespace OKlib {
  namespace Trees {
  }
}


