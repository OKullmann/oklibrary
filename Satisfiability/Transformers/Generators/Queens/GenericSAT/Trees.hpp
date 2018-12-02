// Oliver Kullmann, 13.10.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Content:

*/

#ifndef TREES_F3LTgAj0Hc
#define TREES_F3LTgAj0Hc

#include <limits>
#include <type_traits>
#include <vector>
#include <ostream>
#include <string>

#include <cstdint>
#include <cassert>

#include "InOut.hpp"
#include "Colour.hpp"

namespace Trees {

  // Nodes and edges

  // The indices for nodes and edges:
  typedef std::uint_fast64_t index_t;
  typedef index_t node_t;
  typedef index_t edge_t;
  constexpr index_t max_index = std::numeric_limits<index_t>::max() - 1;
  inline constexpr bool validindex(const index_t i) noexcept {
    return i <= max_index;
  }

  inline constexpr bool null(const index_t i) noexcept { return i == 0; }
  inline constexpr bool validnode(const node_t v) noexcept { return v >= 1; }
  inline constexpr bool validedge(const edge_t e) noexcept { return e >= 2; }

  typedef std::vector<node_t> node_vt;

  // Since the node-index itself is given, only the children are needed:
  struct TreeNode { node_t l, r; };
  static_assert(std::is_pod_v<TreeNode>);

  inline constexpr bool operator ==(const TreeNode lhs, const TreeNode rhs) noexcept {
    return lhs.l == rhs.l and lhs.r == rhs.r;
  }
  inline constexpr bool operator !=(const TreeNode lhs, const TreeNode rhs) noexcept {
    return lhs.l != rhs.l or lhs.r != rhs.r;
  }

  inline constexpr bool valid(const TreeNode v) noexcept {
    return validindex(v.l) and validindex(v.r);
  }
  inline constexpr bool leaf(const TreeNode v) noexcept {
    return v.l == 0 and v.r == 0;
  }

  typedef std::vector<TreeNode> Tree;

  // The types of nodes: unsatisfiable/satisfiable leaf/inner-node:
  enum class NodeType { undef=0, sl=1, ul=2, si=3, ui=4 };

  inline constexpr bool leaf(const NodeType t) noexcept {
    return t == NodeType::ul or t == NodeType::sl;
  }
  inline constexpr bool innernode(const NodeType t) noexcept {
    return t == NodeType::ui or t == NodeType::si;
  }
  inline constexpr bool satisfiable(const NodeType t) noexcept {
    return t == NodeType::sl or t ==NodeType::si;
  }
  inline constexpr bool unsatisfiable(const NodeType t) noexcept {
    return t == NodeType::ul or t ==NodeType::ui;
  }

  std::ostream& operator <<(std::ostream& out, const NodeType t) {
    return out << InOut::code(t);
  }

  typedef std::vector<NodeType> NodeType_v;


  // Trees created along DFS

  struct NoOpTree {
    void add(node_t, NodeType) {}
    void add(node_t, TreeNode, NodeType) noexcept {}
    node_t root_info() noexcept { return {}; }
    TreeNode after_left_info(node_t) const noexcept { return {}; }
  };

  class BasicTree {
    Tree T;
    NodeType_v nt;
    Colour::Colour_v col;
    Colour::Colour_v coledg;
    node_vt parent;
    node_t next = 0;
  public :
    BasicTree() noexcept : T(1), nt(1), col(1), coledg(1), parent(1) {
      assert(T.size() == 1);
      assert(T[0] == TreeNode());
      assert(nt.size() == 1);
      assert(nt[0] == NodeType::undef);
      assert(col.size() == 1);
      assert(coledg.size() == 1);
      assert(parent.size() == 1);
    }

    index_t numver() const noexcept {
      assert(T.size() >= 1);
      return T.size() - 1;
    }

    node_t index() const noexcept { return next; }
    node_t next_index() noexcept {
      assert(next < max_index);
      return ++next;
    }

    node_t root_info() noexcept { return next_index(); }
    TreeNode after_left_info(const node_t root_index) const noexcept {
      return {root_index+1, index()+1};
    }

    void add(const node_t i, const NodeType t) {
      assert(validnode(i));
      assert(leaf(t));
      assert(T.size() == nt.size());
      assert(col.size() == nt.size());
      assert(coledg.size() == col.size());
      if (i >= nt.size()) {
        T.resize(i+1); nt.resize(i+1); col.resize(i+1); coledg.resize(i+1);
      }
      nt[i] = t;
      col[i] = (satisfiable(t)) ? sat : unsat;
      coledg[i] = col[i];
    }
    void add(const node_t i, const TreeNode v, const NodeType t) noexcept {
      assert(validnode(i));
      assert(valid(v));
      assert(not leaf(v));
      assert(not leaf(t));
      assert(T.size() == nt.size());
      assert(col.size() == nt.size());
      assert(coledg.size() == col.size());
      assert(i < T.size());
      T[i] = v; nt[i] = t;
      col[i] = (satisfiable(t)) ? sat : unsat;
      coledg[i] = col[i];
      const auto max = std::max(v.l, v.r);
      if (max >= parent.size()) parent.resize(max+1);
      parent[v.l] = i; parent[v.r] = i;
    }
    const Tree& tree() const noexcept { return T; }
    const NodeType_v& nodetypes() const noexcept { return nt; }
    const Colour::Colour_v& colours() const noexcept { return col; }
    const Colour::Colour_v& ecolours() const noexcept { return coledg; }
    const node_vt& parents() const noexcept { return parent; }

    static constexpr Colour::Colour sat{255,255,0,255}; // yello
    static constexpr Colour::Colour unsat{0,0,0,255}; // black
  };

  // Outputting the tree-structure information, without closing bracket;
  // due to Tulip-bug (version 5.2), the node-indices need to start with zero,
  // as well as the edge-indices, so we subtrackt 1 resp. 2 (these changes
  // are emphasised by bracketing); and also the output of edges must be
  // sorted by edge-index:
  void output_tree(std::ostream& out, const Tree& T, const node_vt& parent, const std::string& author, const std::string& comment) {
    out << "(tlp \"2.3\"\n";
    out << "(date \"" << InOut::timestamp() << "\")\n";
    out << "(author \"" << author << "\")\n";
    out << "(comments \"" << comment << "\")\n";
    assert(T.size() >= 1);
    const auto numv = T.size()-1;
    out << "(nb_nodes " << numv << ")\n";
    out << "(nodes 0.." << (numv-1) << ")\n";
    for (edge_t e = 2; e < T.size(); ++e) {
      const node_t target = e, source = parent[target];
      out << "(edge " << (e-2) << " " << (source-1) << " " << (target-1) << ")\n";
    }
  }

  template <class PropVec>
  void output_nodeprop(std::ostream& out, const std::string& proptype, const std::string propname, const PropVec& V) {
    typedef typename PropVec::value_type prop_t;
    out << "(property 0 " << proptype << " \"" << propname << "\"\n";
    out << "(default \"" << prop_t() << "\" \"" << prop_t() << "\")\n";
    for (node_t i = 1; i < V.size(); ++i)
      out << "(node " << (i-1) << " \"" << V[i] << "\")\n";
    out << ")\n";
  }
  template <class PropVec>
  void output_prop(std::ostream& out, const std::string& proptype, const std::string propname, const PropVec& Vn, const PropVec& Ve) {
    typedef typename PropVec::value_type prop_t;
    out << "(property 0 " << proptype << " \"" << propname << "\"\n";
    out << "(default \"" << prop_t() << "\" \"" << prop_t() << "\")\n";
    for (node_t i = 1; i < Vn.size(); ++i)
      out << "(node " << (i-1) << " \"" << Vn[i] << "\")\n";
    for (edge_t i = 2; i < Ve.size(); ++i)
      out << "(edge " << (i-2) << " \"" << Ve[i] << "\")\n";
    out << ")\n";
  }

  void output(std::ostream& out, const BasicTree& T, const std::string& author, const std::string comment) {
    output_tree(out, T.tree(), T.parents(), author, comment);
    output_nodeprop(out, "int", "type", T.nodetypes());
    output_prop(out, "color", "viewColor", T.colours(), T.ecolours());
    out << ")\n";
  }

}

#endif
