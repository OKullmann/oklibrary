// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Basic components for handling graphs

   - scoped enum GT ("dir" and "und")

   - class AdjMapStr: a simple string-based class for creating graphs
    - the (out-)adjacency-list is realised as a map M from (arbitrary) strings
      to sets of strings (using alphanumerical order of strings)
    - the vertices are exactly the arguments of M
    - const-access to M via graph()

    - typedefs:
     - id_t (std::string)
     - idv_t (vector of id_t)
     - edv_t (vector of pairs of id_t)

    - one constructor from GT (empty graph)
    - edge-insertion (returning the number of vertices and edges inserted --
      new vertices are always automatically inserted):
     - a single edge from a to b: insert(a, b)
     - edges from a to all members of idv_t B: insert(a, B)
     - edges from a to all members of the range B: insertr(a, B)
    - edge-removal (returning the number of edges removed):
     - remove_edges(edv_t E)
     - for a range E: remove_edges(E)


   - class AdjVecUInt: a more efficient class for algorithms on fixed graphs

   - make_AdjVecUInt(std::istream, GT) -> AdjVecUInt
   - has_loops(AdjVecUInt)
   - add_biclique(underlying-adjacency-list, GT, two-ranges-of-vertices)


TODOS:

1. Complete documentation

2. Provide derived form of AdjVecUInt with access to indexed edges

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
    AdjVecUInt(const GT t, const size_t n) noexcept
      : type_(t), n_(n), A(n), names_(false) {}
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

    GT type() const noexcept { return type_; }
    bool with_names() const noexcept { return names_; }
    size_t n() const noexcept { return n_; }
    size_t m() const noexcept { return m_; }
    size_t loops() const noexcept {
      size_t count = 0;
      for (id_t i = 0; i < n_; ++i) count += adjacent(i,i);
      return count;
    }

    void unset_names() noexcept { names_ = false; }
    void set_names() noexcept { names_ = true; }

    bool valid(const adjlist_t& B) noexcept {
      if (B.size() != n_) return false;
      for (size_t i = 0; i < n_; ++i) {
        const auto& L = B[i];
        if (not std::is_sorted(L.begin(), L.end())) return false;
        if (std::any_of(L.begin(), L.end(),
                        [this](const id_t v){return v >= n_;})) return false;
      }
      if (type_ == GT::und)
        for (size_t i = 0; i < n_; ++i)
          for (const id_t v : B[i]) {
            const auto& L = B[v];
            if (not binary_search(L.begin(), L.end(), i)) return false;
          }
      return true;
    }
    void set(adjlist_t B) noexcept {
      assert(valid(B));
      A = std::move(B);
      assert(B.empty());
      m_ = 0;
      for (const auto& v : A) m_ += v.size();
      if (type_ == GT::und) {
        const size_t l = loops();
        assert(l <= m_); m_ -= l;
        assert(m_ % 2 == 0); m_ /= 2;
        m_ += l;
      }
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
      assert(names_);
      return namesvec[x];
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

  private :

    GT type_;
    size_t n_ = 0, m_ = 0;
    adjlist_t A;
    // invariants for A: A.size() = n_, all A[i] are sorted

    bool names_ = true;
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

}

#endif
