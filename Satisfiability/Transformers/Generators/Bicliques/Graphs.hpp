// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Basic  omponents for handling graphs

*/

#ifndef GRAPHS_yNSfT2tQs4
#define GRAPHS_yNSfT2tQs4

#include <set>
#include <map>
#include <string>

namespace Graphs {

  // Graph-types:
  enum class GT { und = 0, dir = 1 };
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

    typedef map_t::size_type size_t;

    explicit AdjMapStr(const GT t) noexcept : type_(t) {}

    GT type() const noexcept { return type_; }
    // Number of vertices:
    size_t n() const noexcept { return M.size(); }
    // number of edges/arcs:
    size_t m() const noexcept { return m_; }

    // Returns true iff the addition of edge {a,b} resp. arc (a,b) took place:
    bool add(const id_t& a, const id_t& b) {
      assert(a != b);
      const auto fa = M.find(a);
      if (type_ == GT::dir) {
        if (fa == M.end()) {
          M.try_emplace(a, set_t{b});
          ++m_;
          M.try_emplace(b, set_t{});
          return true;
        }
        else {
          const auto fb = fa->second.find(b);
          if (fb == fa->second.end()) {
            fa->second.insert(b);
            ++m_;
            M.try_emplace(b, set_t{});
            return true;
          }
          else return false;
        }
      }
      else {
        assert(type_ == GT::und);
        if (fa == M.end()) {
          M.try_emplace(a, set_t{b});
          ++m_;
          const auto ins = M.try_emplace(b, set_t{a});
          if (not ins.second) ins.first->second.insert(a);
          return true;
        }
        else {
          if (fa->second.find(b) == fa->second.end()) {
            fa->second.insert(b);
            ++m_;
            const auto ins = M.try_emplace(b, set_t{a});
            if (not ins.second) ins.first->second.insert(a);
            return true;
          }
          else return false;
        }
      }
    }

    const set_t& neighbours(const id_t& x) const noexcept {
      const auto fx = M.find(x);
      assert(fx != M.end());
      return fx->second;
    }
    const map_t graph() const noexcept { return M; }

    bool operator ==(const AdjMapStr& rhs) const noexcept = default;

  private :

    const GT type_;
    size_t m_ = 0;
    map_t M; // contains the (out-) neighbours of all vertices

  };

}

#endif
