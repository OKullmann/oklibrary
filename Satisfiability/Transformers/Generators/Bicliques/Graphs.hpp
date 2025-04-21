// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Basic components for handling graphs (directed and undirected; no
  parallel edges, but loops are possible)


   - scoped enum GT ("dir", "und")
   - scoped enum GrFo ("libadjlist", "dimacs", "metis")
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
     - idvec_t (vector of id_t)
     - edvec_t (vector of pairs of id_t)

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
     - edges from a to all members of idvec_t B: insert(a, B)
     - edges from a to all members of the range B: insertr(a, B)
     - from an istream: insert(istream), with every line like insert(a,B)

     - add_clique(RAN V)
     - add_biclique(RAN V1, RAN V2)
     - add_path(RAN V)

    - edge-removal (returning the number of edges removed):
     - remove_edges(edvec_t E)
     - for a range E: remove_edges(E)

    - operator == (compares all data: type, m_, M, plus comments)
    - operator <<(ostream&) (just outputs the map, one map-entry per line).


   - class AdjVecUInt: a more efficient class for algorithms on fixed graphs.
     It fixes the vertices of a graph G as the natural numbers
     0, ..., G.n(n)-1, using this order for the adjacency-lists, and it fixes
     the representation of (undirected) edges {u,v} as the pair (u',v') with
     u' <= v'. Thus there is a natural order on the edges/arcs, given by
     lexicographical order.

     Additionally there is the possibility to uses names (strings) for
     vertices, but this is not part of the "identity" of the graph (==
     only considers the type and the adjacency-matrix, and thus, besides
     being "type-safe", realised the mathematical equality of (di)graphs).

    - The out-adjacency-list A is realised as a vector of vectors of uints
      (sorted)
    - the vertices are exactly the natural numbers 0, ..., n()-1
    - const-access to A via graph()
    - type (GT) and n  (id_t) is constant from construction
    - format-members (not concisdered by ==):
     - format (GrFo); mutable
     - names (bool) can be set or unset.

    - typedefs:
     - id_t (uint64_t)
     - list_t (vector of id_t)
     - adjlist_t (vector of list_t)
     - namesvec_t (vector of string)
     - namesmap_t (map string -> id_t)
     - size_t = id_t
     - edge_t (std::pair of id_t)
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
     - format() -> GrFo
       format(GrFo) sets the format (and is constant)
     - const vertex_range : range over all vertices 0, ..., n()-1
       (TODO: In TestGraphs.cpp we have a problem using this range,
        which should be a GCC 11.4 issue.)

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
     - output(ostream, list_t) (list of vertices)
     - output(ostream) (as <<, but without parameter-line)

    - providing whole adjacency-list:
     - valid(adjlist_t) -> bool
     - set(adjlist_t B) (typically called with std::move(B))

    - graph access:
     - degree(id_t) -> id_t
     - regular(id_t) -> bool
       regular() -> bool
     - adjacent(id_t, id_t) -> bool
     - neighbours(id) -> const list_t&
     - graph() -> const adjlist_t&
     - alledges() -> vecedges_t
     - process_alledges(FUN& F) (calls F(e) for all edges, without creating
       them; F.abort() is requested before that call, if available)
     - allnonedges(bool withloops) -> vecedges_t

    - operators:
     - default copy-constructor
     - assignment assumes n and type from other is the same (these are
       the const-data-members); only move-assignment provided
     - == : ignores name-handling and format

    - output:
     - output_vertices(std::ostream, list_t) for output of a list of vertices
     - << : output in the format as given by the format-member, with
       a header specifying (at least) number of vertices and edges/arcs,
       and the body given the adjacency-information.
       The native (default) format GrFo::fulladjlist has:
      - comment-line (started with "# ") with n, m, type
      - all adjacency-lists (space-separated, first the source; either
        via indices, or, if names activated, then via names_.
     - Besides this default-format also GrFo::dimacs and GrFo::metis
       are evailable.
     - Helper-output-functions:
       - output_header(std::ostream)
       - output_body(std::ostream) for the output of the adjacency-list.
     - The above three output-functions are available also for FILE*.


   - Free-standing helper-functions:

    - struct AdjVecUIntref_put (for usage with SystemCalls::Popen)

    - make_AdjMapStr(std::istream, GT) -> AdjMapStr
    - make_AdjVecUInt(std::istream, GT, GrFo, bool) -> AdjVecUInt
      (just reading an AdjMapStr and converting to AdjVecUInt)

    - make_complete_AdjVecUInt(GT, bool, id_t) ->AdjVecUInt

    - has_loops(AdjVecUInt) -> bool
    - is_complete(AdjVecUInt) -> bool

    - add_biclique(underlying-adjacency-list, GT, two-ranges-of-vertices)

    - degree_statistics(AdjVecUInt) -> degree_statistics_t (=
      FreqStats<size_t, float80>)

    - sort_edge(edge_t) -> edge_t
    - edge_index(vecedges_t, edge_t) -> id_t
    - edge_index(vecedges_t, vecedges_t) -> list_t

    - output_matrix(AdjVecUInt, ostream)


   - Independent sets and vertex covers:

    - See also GraphTools.hpp.

    - is_independent(RAN r, AdjVecUInt) -> bool
      is_independent_sort(RAN r, AdjVecUInt) -> bool

      is_vertexcover(RAN r, AdjVecUInt) -> bool
      is_vertexcover_sort(RAN r, AdjVecUInt) -> bool

    - maximal_independent_greedy_simplest(AdjVecUInt, vec_eseeds_t) -> list_t
    - typedef stats_vertexsets_t for statistics of vertex-set-sizes
    - perform_trials(AdjVecUInt, vec_eseeds_t, size_t) ->
      tuple<list_t, stats_vertexsets_t, size_t>

    - independent2MaxSAT(AdjVecUInt) -> MaxSATClauseList
    - TODO: independent2MaxSAT(std::ostream, AdjVecUInt)
    - TODO: independent2MaxSAT(std::FILE*, AdjVecUInt)
      (these two versions do "the same" as the first, however don't create
       the clause-lists (in memory), but directly output the clauses).

TODO vertexcover2MaxSAT(AdjVecUInt) -> MaxSATClauseList

   - Bipartiteness:

    - bipart2SAT(AdjVecUInt) -> DimacsClauseList
    - bipart2SAT(std::ostream, AdjVecUInt)
    - bipart2SAT(std::FILE*, AdjVecUInt)

    - struct Vector_2cols (a representation of a 2-colouring)
    - bipart_0comp(AdjVecUInt) -> Vector_2cols


TODOS:

1. Provide derived form of AdjVecUInt with access to indexed edges.
    - This would come with alledges automatically, and would also allow
      to compute quickly the index of an edge.
    - DONE AdjVecUInt itself should get (slow) member-functions
     - edge2index(edge_t) -> id_t
     - index2edge(it_t) -> edge_t
     using summation over the sizes of the adjacency-lists.

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
#include <stack>

#include <cassert>
#include <cstdint>
#include <cstdio> // (also) for std::FILE, std::fputs
#include <cinttypes> // for PRIu64

#include <ProgramOptions/Environment.hpp>
#include <ProgramOptions/Strings.hpp>
#include <Numerics/Statistics.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

#include "Algorithms.hpp"
#include "DimacsTools.hpp"

namespace Graphs {

  // Graph-types:
  enum class GT { dir = 0, und = 1 };
  constexpr bool valid(const GT t) noexcept {
    return t==GT::und or t==GT::dir;
  }
  static_assert(valid(GT::und));
  static_assert(valid(GT::dir));

  // Graph-formats:
  enum class GrFo {
    fulladjlist = 0, // "full adjency lists"
    dimacs = 1,
    metis = 2
  };
  constexpr bool valid(const GrFo f) noexcept {
    return f==GrFo::fulladjlist or f==GrFo::dimacs or f==GrFo::metis;
  }
  static_assert(valid(GrFo::fulladjlist));
  static_assert(valid(GrFo::dimacs));
  static_assert(valid(GrFo::metis));
  constexpr char comment_symbol(const GrFo f) noexcept {
    switch (f) {
    case GrFo::fulladjlist : return '#';
    case GrFo::dimacs : return 'c';
    case GrFo::metis : return '%';
    default : return 0; }
  }
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Graphs::GT> {
    static constexpr int size = int(Graphs::GT::und)+1;
    static constexpr std::array<const char*, size> string
    {"dir", "und"};
  };
  template <>
  struct RegistrationPolicies<Graphs::GrFo> {
    static constexpr int size = int(Graphs::GrFo::metis)+1;
    static constexpr std::array<const char*, size> string
    {"fulladjlist", "dimacs", "metis"};
  };
}
namespace Graphs {
  std::ostream& operator <<(std::ostream& out, const GT t) {
    switch (t) {
    case GT::dir : return out << "directed";
    case GT::und : return out << "undirected";
    default : return out << "GT::UNKNOWN";}
  }
  std::ostream& operator <<(std::ostream& out, const GrFo f) {
    switch (f) {
    case GrFo::fulladjlist : return out << "full-adjacency-list";
    case GrFo::dimacs : return out << "Dimacs";
    case GrFo::metis : return out << "METIS";
    default : return out << "GrFo::UNKNOWN";}
  }


  // ********************************************************************

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
    // equivalent insert(a, idvec_t{b}) as defined below:
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
    typedef std::vector<id_t> idvec_t;
    std::pair<size_t, size_t> insert(const id_t& a, const idvec_t& B) {
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
    typedef std::vector<std::pair<id_t,id_t>> edvec_t;
    size_t remove_edges(const edvec_t& E) noexcept {
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
    size_t m_ = 0; // number of edges
    map_t M; // contains the (out-) neighbours of all vertices

  };


  // ********************************************************************


  // Adjacency-vector, with unsigned integers as vertex-ids, and underlying
  // names (as strings):
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

    mutable GrFo format_ = GrFo::fulladjlist;

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
    explicit AdjVecUInt(const AdjMapStr& G,
                        const GrFo f = GrFo::fulladjlist,
                        const bool with_names = true) noexcept
      : type_(G.type()), n_(G.n()), m_(G.m()), A(n_),
        format_(f), names_(with_names), namesvec(n_) {
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
      format_ = rhs.format_;
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

    GrFo format() const noexcept { return format_; }
    void format(const GrFo f) const noexcept { format_ = f; }

    void unset_names() noexcept { names_ = false; }
    void set_names() noexcept {
      assert(namesvec.empty() or namesvec.size() == n_);
      names_ = true;
      if (namesvec.empty()) namesvec.resize(n_);
    }

    bool valid(const adjlist_t& B) const noexcept {
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

    // For undirected graphs only the edges in standard form (i.e., in
    // ascending order); the order of edges as given by the adjacency lists:
    vecedges_t alledges() const noexcept {
      vecedges_t res; res.reserve(m_);
      for (id_t i = 0; i < n_; ++i)
        if (type_ == GT::dir)
          for (const id_t v : A[i]) res.emplace_back(i,v);
        else {
          const auto& L = A[i];
          const auto first = std::ranges::lower_bound(L, i), end = L.end();
          for (auto it = first; it != end; ++it) res.emplace_back(i,*it);
        }
      assert(res.size() == m_);
      return res;
    }
    // Not creating the edge-vector, but running through all edges/arcs e
    // in that order, calling F(e); before doing so, calling always
    // F.abort(), which if it returns true aborts the computation:
    template <class FUN>
    void process_alledges(FUN& F) const noexcept {
      for (id_t i = 0; i < n_; ++i)
        if (type_ == GT::dir)
          for (const id_t v : A[i]) {
            if constexpr (requires { F.abort(); }) if (F.abort()) return;
            F(edge_t{i,v});
          }
        else {
          const auto& L = A[i];
          const auto first = std::ranges::lower_bound(L, i), end = L.end();
          for (auto it = first; it != end; ++it) {
            if constexpr (requires { F.abort(); }) if (F.abort()) return;
            F(edge_t{i,*it});
          }
        }
    }

    // The number of neighbours w with w >= v:
    size_t proper_size(const id_t v) const noexcept {
      assert(v < n_);
      const auto& L = A[v];
      const auto first = std::ranges::lower_bound(L, v);
      return L.end() - first;
    }

    // The index of edge e in alledges():
    size_t edge2index(const edge_t e) const noexcept {
      const auto [v,w] = e;
      assert(v < n_);
      if (type_ == GT::dir) {
        size_t prev = 0;
        for (id_t i = 0; i < v; ++i) prev += A[i].size();
        const auto& L = A[v];
        return prev + (std::ranges::lower_bound(L, w) - L.begin());
      }
      else {
        assert(v <= w);
        size_t prev = 0;
        for (id_t i = 0; i < v; ++i) prev += proper_size(i);
        const auto& L = A[v];
        const auto first = std::ranges::lower_bound(L, v);
        return prev + (std::ranges::lower_bound(L, w) - first);
      }
    }
    edge_t index2edge(const size_t i) const noexcept {
      assert(i < m_);
      if (type_ == GT::dir) {
        id_t v = 0;
        size_t sum = A[0].size();
        while (sum <= i) sum += A[++v].size();
        assert(v < n_);
        return {v, A[v][i - sum + A[v].size()]};
      }
      else {
        id_t v = 0;
        size_t sum = proper_size(0);
        while (sum <= i) sum += proper_size(++v);
        assert(v < n_);
        return {v, A[v][i - sum + A[v].size()]};
      }
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
    bool regular(const id_t d) const noexcept {
      return std::all_of(A.begin(), A.end(),
                         [d](const auto& v)noexcept{return v.size()==d;});
    }
    bool regular() const noexcept {
      if (n_ <= 2) return true;
      else return regular(degree(0));
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

    // Output of a list of vertices (space-separated):
    void output_vertices(std::ostream& out, const list_t& L) const {
      if (names_) {
        if (L.empty()) return;
        auto it = L.begin(); const auto end = L.end();
        out << name(*it); ++it;
        for (; it != end; ++it) out << " " << name(*it);
      }
      else Environment::out_line(out, L);
    }

    void output_header(std::ostream& out) const {
      switch (format_) {
      case GrFo::fulladjlist : {
        out << "# " << n_ << " " << m_ << " " << int(type_) << "\n";
        break;
      }
      case GrFo::dimacs : {
        out << "p ";
        if (type_ == GT::dir) out << "arc"; else out << "edge";
        out << " " << n_ << " " << m_ << "\n";
        break;
      }
      case GrFo::metis : {
        out << n_ << " " << m_ << "\n";
        break;
      }}
    }
    void output_header(std::FILE* const fp) const {
      switch (format_) {
      case GrFo::fulladjlist : {
        std::fprintf(fp, "# %" PRIu64 " %" PRIu64 " %d\n", n_,m_,int(type_));
        break;
      }
      case GrFo::dimacs : {
        if (type_ == GT::dir)
          std::fprintf(fp, "p arc %" PRIu64 " %" PRIu64 "\n", n_,m_);
        else
          std::fprintf(fp, "p edge %" PRIu64 " %" PRIu64 "\n", n_,m_);
        break;
      }
      case GrFo::metis : {
        std::fprintf(fp, "%" PRIu64 " %" PRIu64 "\n", n_,m_);
        break;
      }}
    }

    // Output of (only) the adjacency-information:
    void output_body(std::ostream& out) const {
      switch (format_) {
      case GrFo::fulladjlist : {
        for (id_t v = 0; v < n_; ++v) {
          if (names_) out << namesvec[v]; else out << v;
          for (const id_t w : A[v]) {
            out << " "; if (names_) out << namesvec[w]; else out << w;
          }
          out << "\n";
        }
        break;
      }
      case GrFo::dimacs : {
        struct output {
          std::ostream& o;
          void operator()(const edge_t& e) {
            o << "e " << e.first+1 << " " << e.second+1 << "\n";
          }
        };
        output O(out); process_alledges(O);
        break;
      }
      case GrFo::metis : {
        for (id_t v = 0; v < n_; ++v) {
          const auto& a = A[v];
          if (not a.empty()) {
            out << a[0]+1;
            for (id_t j = 1; j < a.size(); ++j) out << " " << a[j]+1;
          }
          out << "\n";
        }
        break;
      }}
    }
    void output_body(std::FILE* const fp) const {
      switch (format_) {
      case GrFo::fulladjlist : {
        if (names_)
          for (id_t v = 0; v < n_; ++v) {
            std::fprintf(fp, "%s", namesvec[v].c_str());
            for (const id_t w : A[v])
              std::fprintf(fp, " %s", namesvec[w].c_str());
            std::fprintf(fp, "\n");
          }
        else
          for (id_t v = 0; v < n_; ++v) {
            std::fprintf(fp, "%" PRIu64, v);
            for (const id_t w : A[v]) std::fprintf(fp, " %" PRIu64, w);
            std::fprintf(fp, "\n");
          }
        break;
      }
      case GrFo::dimacs : {
        struct output {
          std::FILE* const o;
          void operator()(const edge_t& e) const {
            std::fprintf(o, "e %" PRIu64 " %" PRIu64 "\n", e.first+1,e.second+1);
          }
        };
        output O(fp); process_alledges(O);
        break;
      }
      case GrFo::metis : {
        for (id_t v = 0; v < n_; ++v) {
          const auto& a = A[v];
          if (not a.empty()) {
            std::fprintf(fp, "%" PRIu64, a[0]+1);
            for (id_t j = 1; j < a.size(); ++j)
              std::fprintf(fp, " %" PRIu64, a[j]+1);
          }
          std::fprintf(fp, "\n");
        }
        break;
      }}
    }

    friend std::ostream& operator <<(std::ostream& out, const AdjVecUInt& G) {
      G.output_header(out); G.output_body(out);
      return out;
    }
    friend std::FILE* operator <<(std::FILE* const fp, const AdjVecUInt& G) {
      G.output_header(fp); G.output_body(fp);
      return fp;
    }

  };


  // ********************************************************************

  // Wrapper for FILE*-output:
  struct AdjVecUIntref_put {
    const AdjVecUInt& G;
    AdjVecUIntref_put(const AdjVecUInt& G) noexcept : G(G) {}
    void operator ()(std::FILE* const fp) const { fp << G; }
  };


  AdjMapStr make_AdjMapStr(std::istream& in, const GT t) {
    AdjMapStr G(t);
    G.insert(in);
    return G;
  }
  AdjVecUInt make_AdjVecUInt(std::istream& in, const GT t,
                             const GrFo f = GrFo::fulladjlist,
                             const bool with_names = true) {
    return AdjVecUInt(make_AdjMapStr(in, t), f, with_names);
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


  inline AdjVecUInt::edge_t sort_edge(const AdjVecUInt::edge_t e) noexcept {
    return  e.first <= e.second ? e :
      AdjVecUInt::edge_t{e.second, e.first};
  }
  // Assumes E is sorted:
  inline AdjVecUInt::id_t edge_index(const AdjVecUInt::vecedges_t& E,
                                     const AdjVecUInt::edge_t e) noexcept {
    //assert(std::ranges::is_sorted(E)); too expensive in general
    return std::ranges::lower_bound(E, sort_edge(e)) - E.begin();
  }
  // Also assumes the elements of e are sorted:
  AdjVecUInt::list_t  edge_index(const AdjVecUInt::vecedges_t& E,
                                 const AdjVecUInt::vecedges_t& ev) noexcept {
    //assert(std::ranges::is_sorted(E)); too expensive in general
    assert(std::ranges::is_sorted(ev));
    assert(std::ranges::adjacent_find(ev) == ev.end());
    AdjVecUInt::list_t res; res.reserve(ev.size());
    const auto begin = E.begin(), end = E.end();
    auto it = begin;
    for (const auto& e : ev) {
      assert(e == sort_edge(e));
      it = std::lower_bound(it, end, e);
      assert(it != end);
      assert(*it == e);
      res.push_back(it - begin);
      ++it;
    }
    return res;
  }


  void output_matrix(const AdjVecUInt& G, std::ostream& out) {
    using id_t = AdjVecUInt::id_t;
    const id_t n = G.n();
    out << n << " " << n << "\n\n";
    if (n == 0) return;
    for (id_t i = 0; i < n; ++i) {
      const auto& Ns = G.neighbours(i);
      if (Ns.empty()) {
        out << "0";
        for (id_t j = 1; j < n; ++j) out << " 0";
      }
      else {
        auto it = Ns.begin();
        const auto end = Ns.end();
        if (*it == 0) { out << "1"; ++it; }
        else out << "0";
        for (id_t j = 1; j < n; ++j) {
          out << " ";
          if (j == i) out << "0";
          else if (it != end and *it == j) { out << "1"; ++it; }
          else out << "0";
        }
      }
      out << "\n";
    }
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

  template <class RAN>
  bool is_vertexcover(const RAN& r, const AdjVecUInt& G) noexcept {
    assert(G.type() == GT::und);
    assert(std::ranges::is_sorted(r));
    struct check {
      const RAN& r;
      bool is_vc = true;
      bool abort() const noexcept { return not is_vc; }
      void operator()(const AdjVecUInt::edge_t e) noexcept {
        if (not std::ranges::binary_search(r,e.first) and
            not std::ranges::binary_search(r,e.second))
          is_vc = false;
      }
    };
    check C{r}; G.process_alledges(C);
    return C.is_vc;
  }
  template <class RAN>
  bool is_vertexcover_sort(RAN r, const AdjVecUInt& G) noexcept {
    std::ranges::sort(r);
    return is_vertexcover(r,G);
  }


  // Computing a maximal independent set of G by iteratively randomly
  // choosing a vertex of lowest degree (updated after selecting a
  // choice-vertex and removal of it and its neighbours, where only the
  // effect of these neighbours need to be taken into account), in the
  // order as found:
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

  // Perform T computations of maximal_independent_greedy_simplest and
  // additionally return statistics and seed of first best set:
  typedef GenStats::BasicStats<AdjVecUInt::size_t,
                               FloatingPoint::float80> stats_vertexsets_t;
  std::tuple<AdjVecUInt::list_t, stats_vertexsets_t, AdjVecUInt::size_t>
  perform_trials(const AdjVecUInt& G,
                 RandGen::vec_eseed_t seeds,
                 const AdjVecUInt::size_t T) {
    assert(T != 0);
    if (T == 1)
      return {maximal_independent_greedy_simplest(G, seeds), {}, {}};
    else {
      AdjVecUInt::list_t res;
      stats_vertexsets_t S;
      using size_t = AdjVecUInt::size_t;
      size_t opti = 0;
      seeds.push_back(opti);
      for (size_t i = 0; i < T; ++i) {
        seeds.back() = i;
        AdjVecUInt::list_t I = maximal_independent_greedy_simplest(G, seeds);
        S += I.size();
        if (I.size() > res.size()) {
          res = std::move(I);
          opti = i;
        }
      }
      return {res,S,opti};
    }
  }

  /* Maximum independent sets of G via MaxSAT:
      - the variables are v+1 for the vertices v of G
      - for every edge {u,v} we have the (negative binary) hard clause
        {-(u+1), -(v+1)} (using process_alledges)
      - for every vertex v we have the (positive unary) soft clause
        {v+1}.
  */
  DimacsTools::MaxSATClauseList independent2MaxSAT(const AdjVecUInt& G) {
    assert(G.type() == GT::und);
    DimacsTools::MaxSATClauseList res{};
    res.dp = {G.n(), G.n() + G.m()};
    res.soft.reserve(G.n());
    for (AdjVecUInt::id_t v = 0; v < G.n(); ++v)
      res.soft.push_back(DimacsTools::Clause{DimacsTools::Var(v+1)});
    res.hard.reserve(G.m());
    struct transfer {
      DimacsTools::ClauseList& res;
      transfer(DimacsTools::MaxSATClauseList& res) noexcept : res(res.hard) {}
      void operator()(const AdjVecUInt::edge_t& e) noexcept {
        const auto& [v,w] = e;
        const DimacsTools::Lit x(v+1), y(w+1);
        res.push_back({-x, -y});
      }
    };
    transfer T(res);
    G.process_alledges(T);
    assert(res.hard.size() == G.m());
    return res;
  }


  // ********************************************************************


  DimacsTools::DimacsClauseList bipart2SAT(const AdjVecUInt& G) {
    assert(G.type() == GT::und);
    DimacsTools::DimacsClauseList res{};
    res.first.n = G.n();
    res.first.c = 2*G.m();
    res.second.reserve(res.first.c);
    struct transfer {
      DimacsTools::ClauseList& res;
      transfer(DimacsTools::DimacsClauseList& res) noexcept :
      res(res.second) {}
      void operator()(const AdjVecUInt::edge_t& e) noexcept {
        const auto& [v,w] = e;
        const DimacsTools::Lit x(v+1), y(w+1);
        res.push_back({x, y});
        res.push_back({-x, -y});
      }
    };
    transfer T(res);
    G.process_alledges(T);
    assert(res.second.size() == res.first.c);
    return res;
  }
  void bipart2SAT(std::ostream& out, const AdjVecUInt& G) {
    assert(G.type() == GT::und);
    const auto n = G.n();
    const auto c = 2*G.m();
    out << DimacsTools::dimacs_pars{n,c};
    struct transfer {
      std::ostream& out;
      transfer(std::ostream& out) noexcept : out(out) {}
      void operator()(const AdjVecUInt::edge_t& e) {
        const auto& [v,w] = e;
        const DimacsTools::Lit x(v+1), y(w+1);
        using DimacsTools::Clause;
        out << Clause({x, y}) << Clause({-x, -y});
      }
    };
    transfer T(out);
    G.process_alledges(T);
  }
  void bipart2SAT(std::FILE* const fp, const AdjVecUInt& G) {
    assert(G.type() == GT::und);
    const auto n = G.n();
    const auto c = 2*G.m();
    using DimacsTools:: operator <<;
    fp << DimacsTools::dimacs_pars{n,c};
    struct transfer {
      std::FILE* const fp;
      transfer(std::FILE* const fp) noexcept : fp(fp) {}
      void operator()(const AdjVecUInt::edge_t& e) {
        const auto& [v,w] = e;
        const DimacsTools::Lit x(v+1), y(w+1);
        using DimacsTools::Clause;
        fp << Clause({x, y}) << Clause({-x, -y});
      }
    };
    transfer T(fp);
    G.process_alledges(T);
  }

  struct Vector_2cols {
    typedef std::int_fast8_t i_t; // -1,+1 are the colours, 0 is unassigned
    typedef std::vector<i_t> vi_t;
    typedef AdjVecUInt::size_t size_t;

    explicit Vector_2cols(const size_t n) : v(n), n(n) {
      if (n != 0) { v[0] = +1; size_ = 1; }
    }
    // For testing:
    explicit Vector_2cols(vi_t v0) noexcept :
    v(std::move(v0)), n(v.size()) {
      for (const auto x : v) size_ += x != 0;
    }

    const vi_t& result() const noexcept { return v; }
    size_t size() const noexcept { return size_; }

    void init(const size_t i) noexcept {
      assert(i < n and v.size() == n);
      v[i] = -1; ++size_;
    }
    void set_curr(const i_t val) noexcept {
      assert(val == -1 or val == 1);
      curr = val;
    }
    // return -1 iff contradiction, 0 if value is new, 1 otherwise:
    i_t set(const size_t i) noexcept {
      assert(i < n and v.size() == n);
      assert(curr == -1 or curr == 1);
      i_t& vali = v[i];
      if (vali == 0) { vali = curr; ++size_; return 0; }
      else if (vali == curr) return 1;
      else return -1;
    }
    void clear() noexcept { v.clear(); size_ = 0; }

    bool is_bipartite(const AdjVecUInt& G) const noexcept {
      assert(G.n() == n);
      assert(n == v.size());
      std::vector<size_t> part1, part2;
      part1.reserve(size_/2); part2.reserve(size_/2);
      for (size_t i = 0; i < n; ++i) {
        const i_t x = v[i];
        if (x == 1) part1.push_back(i);
        else if (x == -1) part2.push_back(i);
      }
      assert(size_ == part1.size() + part2.size());
      return is_independent(part1, G) and is_independent(part2, G);
    }

    bool operator ==(const Vector_2cols&) const noexcept = default;
    friend std::ostream& operator <<(std::ostream& out,
                                     const Vector_2cols& V) {
      if (V.size() == 0) return out;
      const auto& v = V.result();
      assert(V.n == v.size());
      bool first = true;
      for (Vector_2cols::size_t i = 0; i < V.n; ++i) {
        const i_t x = v[i];
        if (x == 0) continue;
        if (first) first = false; else out << " ";
        if (x == -1) out << "-";
        out << i+1;
      }
      return out;
    }

  private :
    vi_t v;
    size_t size_ = 0; // number of assigned components
    i_t curr = 0;
  public :
    const size_t n;
  };

  // Unassigned are exactly the vertices not reachable from 0:
  Vector_2cols bipart_0comp(const AdjVecUInt& G) {
    Vector_2cols res(G.n());
    if (res.n <= 1) return res;
    using size_t = Vector_2cols::size_t;
    using i_t = Vector_2cols::i_t;
    // vertex, parent, neighbour-colour:
    std::stack<std::tuple<size_t,size_t,i_t>> S;
    for (const auto w : G.neighbours(0)) { res.init(w); S.push({w,0,+1}); }
    while (not S.empty()) {
      const auto [v,p,col] = S.top(); S.pop();
      res.set_curr(col);
      const i_t new_col = -col;
      for (const auto w : G.neighbours(v)) {
        if (w != p) {
          const auto s = res.set(w);
          if (s == -1) { res.clear(); return res; } // an odd cycle
          else if (s == 0) S.push({w,v,new_col});
          // s == 1: an even cycle
        }
      }
    }
    assert(res.is_bipartite(G));
    return res;
  }

}

#endif
