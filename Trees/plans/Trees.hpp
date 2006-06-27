// Oliver Kullmann, 14.1.2006 (Swansea)

/*!
  \file Trees/plans/Trees.hpp
  \brief Plans for the module with tree datastructures

  \todo Concept design "Rooted trees with ordered children"

  Tree T:
  fully constructible, with equality
  vertex_type<Tree>::type; fully constructible with equality and
    with singular value
  root(T) of type vertex_type
  vertex_type v:
  Tree(T, v) copy constructor for the subtree of T at v
  children(T, v) yields a range, whose value-type is vertex_type
  parent(T, v) of type vertex_type (singular iff v == root(T))
  property_map(m, T, v)
  swap(T1, T2)
  swap(T, v, T2, v2) swaps the subtree of T at v with the subtree of T2 at v2
    (swaps do not invalidate vertex descriptors nor iterators, but their
    meaning change)

  in_order(T, v), pre_order(T, v), post_order(T, v) yield ranges to
  traverse the subtree of T at v.

  Refinement "mutable":

  new_child(T, iterator) returns vertex_type with parent v, inserts at
    iterator (if iterator is begin, then this is front-insertion, if iterator is
    end, then this is back-insertion)
  remove_child(T, iterator)
    (again vertex descriptors and iterators are not invalidated (if possible),
    by both operations)

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


