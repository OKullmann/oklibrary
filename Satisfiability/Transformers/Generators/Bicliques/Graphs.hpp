// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Basic components for handling graphs

   - scoped enum GT ("dir" and "und")

   - class AdjMapStr: a simple string-based class for creating graphs
   - class AdjVecUint: a more efficient class for algorithms on fixed graphs.

*/

#ifndef GRAPHS_yNSfT2tQs4
#define GRAPHS_yNSfT2tQs4

#include <set>
#include <map>
#include <string>
#include <utility>
#include <ostream>
#include <istream>
#include <ranges>
#include <algorithm>

#include <cassert>
#include <cstdint>

#include <ProgramOptions/Strings.hpp>

namespace Graphs {

  // Graph-types:
  enum class GT { dir = 0, und = 1 };
  constexpr bool valid(const GT t) noexcept {
    return t==GT::und or t==GT::dir;
  }
  static_assert(valid(GT::und));
  static_assert(valid(GT::dir));


  // Adjacency-map, with strings as vertex-ids:
  struct AdjMapStr {

    typedef std::string id_t;
    typedef std::set<id_t> set_t;
    typedef std::map<id_t, set_t> map_t;

    typedef map_t::const_iterator iterator;
    typedef std::pair<iterator, bool> ins_ret_t;

    typedef map_t::size_type size_t;

    explicit AdjMapStr(const GT t) noexcept : type_(t) {}

    GT type() const noexcept { return type_; }
    // Number of vertices:
    size_t n() const noexcept { return M.size(); }
    // number of edges/arcs:
    size_t m() const noexcept { return m_; }

    ins_ret_t insert(const id_t& v) {
      return M.try_emplace(v, set_t{});
    }
  private :
    // possibly change the set of neighbours:
    typedef map_t::iterator nc_iterator;
    // "nc": non-constant version:
    std::pair<nc_iterator, bool> insert_nc(const id_t& v) {
      return M.try_emplace(v, set_t{});
    }
  public :

    // Returns the number of inserted vertices and edges/arcs ({a,b} resp.
    // (a,b)); possibly more efficient for a single insertion than the
    // equivalent insert(a, idv_t{b}) as defined below:
    std::pair<size_t, size_t> insert(const id_t& a, const id_t& b) {
      const auto fa = M.find(a);
      if (type_ == GT::dir) {
        if (fa == M.end()) {
          [[maybe_unused]] const auto tea = M.try_emplace(a, set_t{b});
          assert(tea.second);
          ++m_;
          if (a == b) return {1, 1};
          const auto teb = M.try_emplace(b, set_t{});
          return {1 + teb.second, 1};
        }
        else {
          const auto fb = fa->second.find(b);
          if (fb == fa->second.end()) {
            fa->second.insert(b);
            ++m_;
            if (a == b) return {0, 1};
            const auto teb = M.try_emplace(b, set_t{});
            return {teb.second, 1};
          }
          else return {0,0};
        }
      }
      else {
        assert(type_ == GT::und);
        if (fa == M.end()) {
          [[maybe_unused]] const auto tea = M.try_emplace(a, set_t{b});
          assert(tea.second);
          ++m_;
          const auto teb = M.try_emplace(b, set_t{a});
          if (not teb.second) {
            teb.first->second.insert(a);
            return {1,1};
          }
          else return {2,1};
        }
        else {
          if (fa->second.find(b) == fa->second.end()) {
            [[maybe_unused]] const auto ia = fa->second.insert(b);
            assert(ia.second);
            ++m_;
            const auto teb = M.try_emplace(b, set_t{a});
            if (not teb.second) {
              teb.first->second.insert(a);
              return {0,1};
            }
            else return {1,1};
          }
          else return {0,0};
        }
      }
    }

    // Returns the number of inserted vertices and edges/arcs:
    template <class RAN>
    std::pair<size_t, size_t> insertr(const id_t& a, const RAN& B) {
      const size_t N = B.size();
      size_t numv=0, nume=0;
      const auto fa = insert_nc(a);
      numv += fa.second;
      if (N == 0) { return {numv, nume}; }
      const nc_iterator ia = fa.first;
      for (const auto& v : B) {
        const auto iv = insert_nc(v);
        if (iv.second) {
          ++numv; ++nume;
          ia->second.insert(v);
          if (type_ == GT::und) iv.first->second.insert(a);
        }
        else {
          const auto iva = ia->second.insert(v);
          if (iva.second) {
            ++nume;
            if (type_ == GT::und and v != a) iv.first->second.insert(a);
          }
        }
      }
      m_ += nume;
      return {numv, nume};
    }
    typedef std::vector<id_t> idv_t;
    std::pair<size_t, size_t> insert(const id_t& a, const idv_t& B) {
      return insertr(a, B);
    }

    /* Reading vertices and edges from in:
        - line-based
        - ignoring everything from some "#" on
        - ignoring space-symbols (other than for separation)
        - ignoring empty lines
        - format "a b_1 ... b_k", k >= 0, with a, b_i strings, and the b_i
          (out-)neighbours of a
       As with the insert-functions in general, ignoring duplications, and
       returning the number of new vertices and edges/arcs.
    */
    std::pair<size_t, size_t> insert(std::istream& in) {
      size_t numv=0, nume=0;
      std::string line;
      while (std::getline(in, line, '\n')) {
        line.resize(std::min(line.size(), line.find('#')));
        Environment::transform_spaces_mod(line);
        if (Environment::onlyspaces(line)) continue;
        const auto tokens = Environment::split(line, ' ');
        const auto N = tokens.size();
        assert(N >= 1);
        const auto res = insertr(tokens[0],
          std::ranges::views::counted(tokens.begin()+1, N-1));
        numv += res.first; nume += res.second;
      }
      return {numv, nume};
    }

    const set_t& neighbours(const id_t& x) const noexcept {
      const auto fx = M.find(x);
      assert(fx != M.end());
      return fx->second;
    }
    const map_t graph() const noexcept { return M; }

    bool operator ==(const AdjMapStr& rhs) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const AdjMapStr& G) {
      out << "# " << G.n() << " " << G.m() << " " << int(G.type()) << "\n";
      for (const auto& p : G.graph()) {
        out << p.first;
        for (const auto& v : p.second) out << " " << v;
        out << "\n";
      }
      return out;
    }


    template <class RAN>
    std::pair<size_t, size_t>  add_clique(const RAN& V) {
      std::pair<size_t, size_t>  count{};
      for (auto i = V.begin(); i != V.end(); ++i)
        for (auto j = std::next(i); j != V.end(); ++j) {
          const auto res = insert(*i, *j);
          count.first += res.first; count.second += res.second;
        }
      return count;
    }

  private :

    const GT type_;
    size_t m_ = 0;
    map_t M; // contains the (out-) neighbours of all vertices

  };


  // Adjacency-vector, with unsigned integers as vertex-ids, and underlying
  // names:
  struct AdjVecUInt {
    typedef std::uint64_t id_t;
    typedef std::vector<id_t> list_t;
    typedef std::vector<list_t> adjlist_t;

    typedef std::vector<std::string> namesvec_t;
    typedef std::map<std::string, id_t> namesmap_t;

    typedef id_t size_t;

    explicit AdjVecUInt(const GT t) noexcept : type_(t) {}
    explicit AdjVecUInt(const AdjMapStr& G) noexcept :
    type_(G.type()), n_(G.n()), m_(G.m()), A(n_), namesvec(n_) {
      typedef namesmap_t::const_iterator iterator;
      iterator hint = namesmap.begin();
      for (id_t i = 0; const auto& p : G.graph()) {
        namesvec[i] = p.first;
        hint = namesmap.emplace_hint(hint, p.first, i);
        ++i;
      }
      assert(namesmap.size() == n_);
      for (id_t i = 0; const auto& p : G.graph()) {
        A[i].reserve(p.second.size());
        for (const auto& v : p.second)
          A[i].push_back(namesmap[v]);
        assert(A[i].size() == p.second.size());
        assert(std::ranges::is_sorted(A[i]));
        ++i;
      }
      assert(namesmap.size() == n_);
    }

    GT type() const noexcept { return type_; }
    size_t n() const noexcept { return n_; }
    size_t m() const noexcept { return m_; }

    const list_t& neighbours(const id_t& x) const noexcept {
      assert(x < n_);
      return A[x];
    }
    const adjlist_t& graph() const noexcept {
      return A;
    }
    const std::string& name(const id_t& x) const noexcept {
      assert(x < n_);
      return namesvec[x];
    }
    const namesvec_t& allnames() const noexcept {
      return namesvec;
    }
    // Returns n_ for invalid names:
    id_t index(const std::string& s) const noexcept {
      const auto f = namesmap.find(s);
      if (f == namesmap.end()) return n_;
      else return f->second;
    }
    const namesmap_t& allindices() const noexcept {
      return namesmap;
    }

    typedef std::pair<id_t,id_t> edge_t; // sorted for undirected edges
    typedef std::vector<edge_t> vecedges_t;

    vecedges_t alledges() const noexcept {
      vecedges_t res; res.reserve(m_);
      for (id_t i = 0; i < n_; ++i)
        for (const id_t v : A[i])
          if (type_ == GT::dir or v >= i) res.emplace_back(i,v);
      assert(res.size() == m_);
      return res;
    }

    bool adjacent(const id_t v, const id_t w) const noexcept {
      assert(v < n_ and w < n_);
      if (type_ == GT::dir)
        return std::ranges::binary_search(A[v], w);
      else {
        assert(type_ == GT::und);
        if (A[v].size() <= A[w].size())
          return std::ranges::binary_search(A[v], w);
        else
          return std::ranges::binary_search(A[v], w);
      }
    }

  private :

    GT type_;
    size_t n_ = 0, m_ = 0;
    adjlist_t A;
    // invariants for A: A.size() = n_, all A[i] are sorted

    namesvec_t namesvec;
    namesmap_t namesmap;
  };

  AdjVecUInt make_AdjVecUInt(std::istream& in, const GT t) {
    AdjMapStr G(t);
    G.insert(in);
    return AdjVecUInt(G);
  }


  bool has_loops(const AdjVecUInt& G) noexcept {
    using id_t = AdjVecUInt::id_t;
    for (id_t i = 0; i < G.n(); ++i)
      if (G.adjacent(i,i)) return true;
    return false;
  }

}

#endif
