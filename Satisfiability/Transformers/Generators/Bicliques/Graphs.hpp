// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Basic components for handling graphs (directed and undirected; no
  parallel edges, but loops are possible)


   - scoped enum GT ("dir" and "und")
   - valid(GT)


   - class AdjMapStr: a simple string-based class for creating graphs

    - the (out-)adjacency-list is realised as a map M from (arbitrary) strings
      to sets of strings (using alphanumerical order of strings)
    - the vertices are exactly the arguments of M
    - const-access to M via graph()

    - typedefs:
     - id_t (std::string)
     - set_t (set of id_t with alphanumerical sorting)
     - map_t (map from id_t to set_t, with alphanumerical sorting)
     - iterator (map-const-iterator)
     - ins_ret_t (pair of iterator, bool)
     - size_t (size_t of map_t)
     - idv_t (vector of id_t)
     - edv_t (vector of pairs of id_t)

    - public data:
     - comments (vector of string)

    - one constructor from GT (empty graph)

    - statistics:
     - type() -> GT
     - n() -> size_t (number of vertices)
     - m() -> size_t (number of edges)

    - vertex-insertion:
     - insert(id_t) -> ins_ret_t (inserts the empty adjacency-set if the
       vertex is not already present, otherwise no-op)

    - edge-insertion (returning the number of vertices and edges inserted --
      new vertices are always automatically inserted):
     - a single edge from a to b: insert(a, b)
     - edges from a to all members of idv_t B: insert(a, B)
     - edges from a to all members of the range B: insertr(a, B)
     - from an istream: insert(istream), with every line like insert(a,B)

    - edge-removal (returning the number of edges removed):
     - remove_edges(edv_t E)
     - for a range E: remove_edges(E)


   - class AdjVecUInt: a more efficient class for algorithms on fixed graphs

    - the out-adjacency-list A is realised as a vector of vectors of uints
    - the vertices are exactly the natural numbers 0, ..., n()-1
    - const-access to A via graph()
    - type and n is constant from construction

    - typedefs:
     - id_t (uint64_t)
     - list_t (vector of id_t)
     - adjlist_t (vector of list_t)
     - namesvec_t (vector os string)
     - namesmap_t (map string -> id_t)
     - size_t = id_t
     - edge_t (pair of id_t)
     - vecedges_t (vector of edge_t)

    - constructors:
     - AdjVecUInt(GT) : empty graph (explicit)
     - AdjVecUInt(GT, size_t n) : n isolated vertices
     - AdjVecUInt(GT, adjlist_t)
     - AdjVecUInt(AdjMapStr) : indexing in the given order, with storing
       the names (explicit)

    - statistics:
     - type() -> GT
     - n() -> size_t (number of vertices)
     - m() -> size_t (number of edges)
     - loops() -> size_t
     - vertex_range : range over vertices

    - name-handling:
     - with_names() -> bool : whether names are active
     - unset_names() : deactivate names
     - set_names() : activate names; if the names-vector was empty before,
       resizes it to n()
     - name(id_t) -> string
     - set_name(id, string) : sets the name of id resp. changes it
     - allnames() -> const namesvec_t&
     - index(string) -> id_t
     - allindices() -> const namesmap_t&
     - output(ostream, list_t)

    - providing whole adjacency-list:
     - valid(adjlist_t) -> bool
     - set(adjlist_t)

    - graph access:
     - adjacent(id_t, id_t) -> bool
     - neighbours(id) -> const list_t&
     - graph() -> const adjlist_t&
     - alledges() -> vecedges_t
     - all nonedges(bool withloops) -> vecedges_t

    - operators:
     - assignment assumes n and type from other is the same;
       only move-assignment
     - == : ignores name-handling
     - << :
      - comment-line (started with "# ") with n, m, type
      - all adjacency-lists (either via indices, or, whith names activated,
        via names).


   - Free-standing helper-functions:

    - make_AdjMapStr(std::istream, GT) -> AdjMapStr
    - make_AdjVecUInt(std::istream, GT) -> AdjVecUInt
      (just reading an AdjMapStr and converting to AdjVecUInt)

    - make_complete_AdjVecUInt(GT, bool, id_t) ->AdjVecUInt

    - has_loops(AdjVecUInt) -> bool
    - is_complete(AdjVecUInt) -> bool

    - add_biclique(underlying-adjacency-list, GT, two-ranges-of-vertices)

    - degree_statistics(AdjVecUInt) -> degree_statistics_t (=
      FreqStats<size_t, float80>)

    - independent sets:

     - is_independent(RAN r, AdjVecUInt) -> bool
       is_independent_sort(RAN r, AdjVecUInt) -> bool
     - maximal_independent_greedy_simplest(AdjVecUInt, vec_eseeds_t) -> list_t


TODOS:

1. Provide derived form of AdjVecUInt with access to indexed edges

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
#include <iterator>

#include <cassert>
#include <cstdint>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/Statistics.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

#include "Algorithms.hpp"

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
    typedef std::set<id_t, Environment::AlphaNum> set_t;
    typedef std::map<id_t, set_t, Environment::AlphaNum> map_t;

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

    std::vector<std::string> comments; // lines without additional eol's

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

    // Inserting a range R of neighbours, returning the number of
    // inserted vertices and edges/arcs:
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

    // Removes a range of edges (returns the number of removed edges):
    template <class RAN>
    size_t remove_edgesr(const RAN& E) noexcept {
      size_t count = 0;
      for (const auto& [v,w] : E) {
        const auto f = M.find(v);
        if (f == M.end()) continue;
        const auto r = f->second.erase(w);
        count += r;
        if (r == 1 and type_ == GT::und) M[w].erase(v);
      }
      assert(count <= m_);
      m_ -= count;
      return count;
    }
    typedef std::vector<std::pair<id_t,id_t>> edv_t;
    size_t remove_edges(const edv_t& E) noexcept {
      return remove_edgesr(E);
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
        Environment::cutoff(line, '#');
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
    const map_t& graph() const noexcept { return M; }

    bool operator ==(const AdjMapStr& rhs) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const AdjMapStr& G) {
      out << "# " << G.n() << " " << G.m() << " " << int(G.type()) << "\n";
      for (const std::string& c : G.comments)
        out << "# " << c << "\n";
      for (const auto& p : G.graph()) {
        out << p.first;
        for (const auto& v : p.second) out << " " << v;
        out << "\n";
      }
      return out;
    }


    // For a range V of vertices:
    template <class RAN>
    std::pair<size_t, size_t> add_clique(const RAN& V) {
      std::pair<size_t, size_t> count{};
      if (V.empty()) return count;
      const auto begin = V.begin(), end = V.end();
      {const auto next = std::next(begin);
       if (next == end) {
         count.first += insert(*begin).second;
         return count;
       }
      }
      for (auto i = begin; i != end; ++i)
        for (auto j = std::next(i); j != end; ++j) {
          const auto res = insert(*i, *j);
          count.first += res.first; count.second += res.second;
        }
      return count;
    }
    // For ranges V1, V2 of vertices:
    template <class RAN>
    std::pair<size_t, size_t> add_biclique(const RAN& V1, const RAN& V2) {
      std::pair<size_t, size_t> count{};
      if (not V1.empty())
        for (const auto& v : V1) {
          const auto res = insertr(v, V2);
          count.first += res.first; count.second += res.second;
        }
      else
        for (const auto& v : V2) count.first += insert(v).second;
      return count;
    }
    // For a range V of vertices
    template <class RAN>
    std::pair<size_t, size_t> add_path(const RAN& V) {
      std::pair<size_t, size_t> count{};
      if (V.empty()) return count;
      auto i = V.begin();
      auto j = i; ++j;
      if (j == V.end()) {
        count.first += insert(*i).second;
        return count;
      }
      do {
        const auto res = insert(*i,*j);
        count.first += res.first; count.second += res.second;
        i = j;
      } while (++j != V.end());
      return count;
    }

  private :

    const GT type_;
    size_t m_ = 0;
    map_t M; // contains the (out-) neighbours of all vertices

  };


  // ********************************************************************


  // Adjacency-vector, with unsigned integers as vertex-ids, and underlying
  // names:
  struct AdjVecUInt {
    typedef std::uint64_t id_t;
    typedef std::vector<id_t> list_t;
    typedef std::vector<list_t> adjlist_t;

    typedef std::vector<std::string> namesvec_t;
    typedef std::map<std::string, id_t> namesmap_t;

    typedef id_t size_t;

  private :

    const GT type_;
    const size_t n_ = 0;
    size_t m_ = 0;
    adjlist_t A;
    // invariants for A: A.size() = n_, all A[i] are sorted

    bool names_ = true;
    namesvec_t namesvec;
    namesmap_t namesmap;

  public :

    explicit AdjVecUInt(const GT t) noexcept : type_(t) {}
    AdjVecUInt(const GT t, const size_t n) noexcept
      : type_(t), n_(n), A(n), names_(false) {}
    AdjVecUInt(const GT t, adjlist_t A0) : type_(t), n_(A0.size()),
      A(std::move(A0)), names_(false) {
      assert(valid(A)); m_ = num_edges(); }
    explicit AdjVecUInt(const AdjMapStr& G) noexcept
      : type_(G.type()), n_(G.n()), m_(G.m()), A(n_), namesvec(n_) {
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

    AdjVecUInt(const AdjVecUInt& other) = default;

    AdjVecUInt& operator =(AdjVecUInt&& rhs) noexcept {
      assert(rhs.type_ == type_);
      assert(rhs.n_ == n_);
      m_ = rhs.m_;
      A = std::move(rhs.A);
      names_ = rhs.names_;
      namesvec = std::move(rhs.namesvec);
      namesmap = std::move(rhs.namesmap);
      return *this;
    }

    GT type() const noexcept { return type_; }
    bool with_names() const noexcept { return names_; }
    size_t n() const noexcept { return n_; }

    typedef std::ranges::iota_view<id_t, id_t> iota_view;
    const iota_view vertex_range{0, n_};

    size_t m() const noexcept { return m_; }
    size_t loops() const noexcept {
      size_t count = 0;
      for (id_t i = 0; i < n_; ++i) count += adjacent(i,i);
      return count;
    }

    void unset_names() noexcept { names_ = false; }
    void set_names() noexcept {
      assert(namesvec.empty() or namesvec.size() == n_);
      names_ = true;
      if (namesvec.empty()) namesvec.resize(n_);
    }

    bool valid(const adjlist_t& B) noexcept {
      if (B.size() != n_) return false;
      for (size_t i = 0; i < n_; ++i) {
        const auto& L = B[i];
        if (not std::is_sorted(L.begin(), L.end())) return false;
        if (std::adjacent_find(L.begin(), L.end()) != L.end())
          return false;
        if (std::any_of(L.begin(), L.end(),
                        [this](const id_t v){return v >= n_;}))
          return false;
      }
      if (type_ == GT::und)
        for (size_t i = 0; i < n_; ++i)
          for (const id_t v : B[i]) {
            const auto& L = B[v];
            if (not binary_search(L.begin(), L.end(), i)) return false;
          }
      return true;
    }
  private :
    size_t num_edges() const noexcept {
      size_t res = 0;
      for (const auto& v : A) res += v.size();
      if (type_ == GT::und) {
        const size_t num_loops = loops();
        assert(num_loops <= res); res -= num_loops;
        assert(res % 2 == 0); res /= 2;
        res += num_loops;
      }
      return res;
    }
  public :
    void set(adjlist_t B) noexcept {
      assert(valid(B));
      A = std::move(B);
      assert(B.empty());
      m_ = num_edges();
    }

    const list_t& neighbours(const id_t x) const noexcept {
      assert(x < n_);
      return A[x];
    }
    const adjlist_t& graph() const noexcept {
      return A;
    }

    const std::string& name(const id_t x) const noexcept {
      assert(x < n_);
      return namesvec[x];
    }
    void set_name(const id_t x, std::string s) noexcept {
      assert(not s.empty());
      assert(x < n_);
      auto& sold = namesvec[x];
      if (not sold.empty()) {
        const auto f = namesmap.find(sold);
        assert(f != namesmap.end() and f->second == x);
        namesmap.erase(f);
      }
      sold = std::move(s);
      namesmap[sold] = x;
    }
    const namesvec_t& allnames() const noexcept {
      return namesvec;
    }
    // Returns n_ for invalid names:
    id_t index(const std::string& s) const noexcept {
      assert(names_);
      const auto f = namesmap.find(s);
      if (f == namesmap.end()) return n_;
      else return f->second;
    }
    const namesmap_t& allindices() const noexcept {
      return namesmap;
    }

    typedef std::pair<id_t,id_t> edge_t; // sorted for undirected edges
    typedef std::vector<edge_t> vecedges_t;

    // For undirected graphs only the edges in standard form:
    vecedges_t alledges() const noexcept {
      vecedges_t res; res.reserve(m_);
      for (id_t i = 0; i < n_; ++i)
        for (const id_t v : A[i])
          if (type_ == GT::dir or v >= i) res.emplace_back(i,v);
      assert(res.size() == m_);
      return res;
    }
    // The complement-edges:
    vecedges_t allnonedges(const bool withloops = false) const noexcept {
      if (type_ == GT::dir) {
        if (withloops) {
          const id_t size = n_ * n_ - m_;
          if (size == 0) return {};
          vecedges_t res; res.reserve(size);
          for (id_t v = 0; v < n_; ++v) {
            const auto& row = A[v];
            if (row.empty())
              for (id_t i = 0; i < n_; ++i)
                res.push_back({v,i});
            else {
              id_t w = 0;
              for (id_t i = 0; i < row.size(); ++i,++w)
                while (w < row[i]) res.push_back({v,w++});
              while (w < n_) res.push_back({v,w++});
            }
          }
          assert(res.size() == size);
          return res;
        }
        else {
          const id_t size = n_ * (n_ - 1) - (m_ - loops());
          if (size == 0) return {};
          vecedges_t res; res.reserve(size);
          for (id_t v = 0; v < n_; ++v) {
            const auto& row = A[v];
            if (row.empty()) {
              for (id_t i = 0; i < n_; ++i)
                if (i != v) res.push_back({v,i});
            }
            else {
              id_t w = 0;
              for (id_t i = 0; i < row.size(); ++i,++w)
                while (w < row[i])
                  if (w != v) res.push_back({v,w++});
                  else ++w;
              while (w < n_)
                if (w != v) res.push_back({v,w++});
                else ++w;
            }
          }
          assert(res.size() == size);
          return res;
        }
      }
      else {
        assert(type_ == GT::und);
        if (withloops) {
          const id_t size = (n_ * (n_+1))/2 - m_;
          if (size == 0) return {};
          vecedges_t res; res.reserve(size);
          for (id_t v = 0; v < n_; ++v) {
            const auto& row = A[v];
            if (row.empty())
              for (id_t i = v; i < n_; ++i) res.push_back({v,i});
            else {
              id_t i = 0; while (i < row.size() and row[i] < v) ++i;
              id_t w = v;
              for (; i < row.size(); ++i,++w)
                while (w < row[i]) res.push_back({v,w++});
              while (w < n_) res.push_back({v,w++});
            }
          }
          assert(res.size() == size);
          return res;
        }
        else {
          const id_t size = (n_ * (n_-1))/2 - (m_ - loops());
          if (size == 0) return {};
          vecedges_t res; res.reserve(size);
          for (id_t v = 0; v < n_; ++v) {
            const auto& row = A[v];
            if (row.empty())
              for (id_t i = v+1; i < n_; ++i) res.push_back({v,i});
            else {
              id_t i = 0; while (i < row.size() and row[i] <= v) ++i;
              id_t w = v+1;
              for (; i < row.size(); ++i,++w)
                while (w < row[i]) res.push_back({v,w++});
              while (w < n_) res.push_back({v,w++});
            }
          }
          assert(res.size() == size);
          return res;
        }
      }
    }

    id_t degree(const id_t v) const noexcept {
      assert(v < n_);
      return A[v].size();
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
          return std::ranges::binary_search(A[w], v);
      }
    }

    bool operator ==(const AdjVecUInt& rhs) const noexcept {
      return type_ == rhs.type_ and n_ == rhs.n_ and m_ == rhs.m_
        and A == rhs.A;
    }

    void output(std::ostream& out, const list_t& L) const {
      if (names_) {
        if (L.empty()) return;
        auto it = L.begin(); const auto end = L.end();
        out << name(*it); ++it;
        for (; it != end; ++it) out << " " << name(*it);
      }
      else Environment::out_line(out, L);
    }

    friend std::ostream& operator <<(std::ostream& out, const AdjVecUInt& G) {
      out << "# " << G.n_ << " " << G.m_ << " " << int(G.type_) << "\n";
      for (id_t v = 0; v < G.n_; ++v) {
        if (G.names_) out << G.namesvec[v]; else out << v;
        for (const id_t w : G.A[v]) {
          out << " ";
          if (G.names_) out << G.namesvec[w]; else out << w;
        }
        out << "\n";
      }
      return out;
    }

  };


  // ********************************************************************


  AdjMapStr make_AdjMapStr(std::istream& in, const GT t) {
    AdjMapStr G(t);
    G.insert(in);
    return G;
  }
  AdjVecUInt make_AdjVecUInt(std::istream& in, const GT t) {
    return AdjVecUInt(make_AdjMapStr(in, t));
  }
  AdjVecUInt make_complete_AdjVecUInt(const GT t, const bool with_loops,
                                      const AdjVecUInt::id_t n) {
    AdjVecUInt res(t, n);
    if (n == 0) return res;
    AdjVecUInt::adjlist_t A(n);
    using id_t = AdjVecUInt::id_t;
    for (id_t i = 0; i < n; ++i)
      for (id_t j = 0; j < n; ++j)
        if (with_loops or j != i) A[i].push_back(j);
    res.set(A);
    return res;
  }


  bool has_loops(const AdjVecUInt& G) noexcept {
    using id_t = AdjVecUInt::id_t;
    for (id_t i = 0; i < G.n(); ++i)
      if (G.adjacent(i,i)) return true;
    return false;
  }
  // If at least one loop is present, then all loops must be present
  // for completeness, otherwise none:
  bool is_complete(const AdjVecUInt& G) noexcept {
    using id_t = AdjVecUInt::id_t;
    const id_t n = G.n();
    if (n == 0) return true;
    const id_t m = G.m();
    if (G.type() == GT::dir) {
      if (has_loops(G)) return m == n*n;
      else return m == n*(n-1);
    }
    else {
      if (has_loops(G)) return m == (n*(n+1))/2;
      else return m == (n*(n-1))/2;
    }
  }


  // Without sorting or removal of duplicates:
  template <class RAN>
  void add_biclique(AdjVecUInt::adjlist_t& A, const GT t,
                    const RAN& V1, const RAN& V2) {
    using id_t = AdjVecUInt::id_t;
    for (const id_t v1 : V1) {
      assert(v1 < A.size()); auto& neigh = A[v1];
      for (const id_t v2 : V2) { assert(v2<A.size()); neigh.push_back(v2); }
    }
    if (t == GT::und) {
      for (const id_t v2 : V2) {
        assert(v2 < A.size()); auto& neigh = A[v2];
        for (const id_t v1 : V1) { assert(v1<A.size()); neigh.push_back(v1); }
      }
    }
  }


  typedef GenStats::FreqStats<size_t, FloatingPoint::float80>
          degree_statistics_t;
  degree_statistics_t degree_statistics(const AdjVecUInt& G) {
    return {G.graph(), [](auto){return true;},
        [](const auto& L){return L.size();}};
  }


  // ********************************************************************


  // See Algorithms::is_independent for the implicit version (with an
  // adjacency-predicate; there loops are ignored, here taken into account):
  template <class RAN>
  bool is_independent(const RAN& r, const AdjVecUInt& G) noexcept {
    assert(G.type() == GT::und);
    assert(std::ranges::is_sorted(r));
    using id_t = AdjVecUInt::id_t;
    for (const id_t v : r) {
      assert(v < G.n());
      if (not Algorithms::empty_intersection(r, G.neighbours(v))) return false;
    }
    return true;
  }
  template <class RAN>
  bool is_independent_sort(RAN r, const AdjVecUInt& G) noexcept {
    std::ranges::sort(r);
    return is_independent(r,G);
  }

  // Computing a maximal independent set of G by iteratively randomly
  // choosing a vertex of lowest degree (updated after selecting a
  // choice-vertex and removal of it and its neighbours, where only the
  // effect of these neighbours need to be taken into account):
  AdjVecUInt::list_t
  maximal_independent_greedy_simplest(const AdjVecUInt& G,
                                      const RandGen::vec_eseed_t& seeds) {
    assert(G.type() == GT::und);
    AdjVecUInt::list_t res;

    using id_t = AdjVecUInt::id_t;
    struct P {
      id_t v, d;
      bool operator <(const P& o) const noexcept { return d < o.d; }
    };
    typedef std::multiset<P> set_t;
    typedef set_t::const_iterator iterator;
    typedef std::vector<iterator> vec_t;

    vec_t avail; avail.reserve(G.n());
    set_t sorted;
    const iterator end = sorted.cend();
    for (const id_t v : G.vertex_range) {
      const iterator it = sorted.emplace(v,G.degree(v));
      avail.push_back(it);
    }

    RandGen::RandGen_t g(seeds);
    while (not sorted.empty()) {
      const iterator begin = sorted.cbegin();
      const id_t degree = begin->d;
      vec_t choices; choices.push_back(begin);
      for (auto it = std::next(begin); it != end
             and it->d == degree; ++it)
        choices.push_back(it);
      const id_t n = choices.size();
      const iterator choice = n==1 ? choices[0] :
        choices[RandGen::UniformRange(g, n)()];
      const id_t element = choice->v;
      assert(avail[element] == choice);
      res.push_back(element);
      sorted.erase(choice);
      avail[element] = end;
      for (const id_t v : G.neighbours(element)) {
        iterator& it = avail[v];
        if (it != end) {
          sorted.erase(it);
          it = end;
          for (const id_t w : G.neighbours(v)) {
            iterator& itw = avail[w];
            if (itw != end) {
              const id_t d = itw-> d;
              assert(d >= 1);
              sorted.erase(itw);
              itw = sorted.emplace(w, d-1);
            }
          }
        }
      }
    }
    assert(is_independent_sort(res, G));
    return res;
  }

}

#endif
