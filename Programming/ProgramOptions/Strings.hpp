// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* String helper functions

   String helper functions:

    - STR(x) is a macro, putting quotation marks around x
    - qu(string) adds quotes around a string

    - basename(string) extracts the part of the string before "."
    - auto_prg(filename) ("automatic" program-name from file-name)

    - replace(string, char, char), remove(string, char)
    - cutoff(string, char)

    - typedef size_t
    - typedef tokens_t = vector<string>

    - split(string, char), split(istream, char),
      split(istream, char, char& final_character),
      split_cutoff(istream, char, char cutoff-character)
        all -> tokens_t
    - split2(string, char1, char2) -> vector<tokens_t>
    - for char2= ' ' :
      split2_cutoff(istream, char, cutoff-character) -> vector<tokens_t>

    - isspace(char)
    - onlyspaces(string), starts_with_space(string), ends_with_space(string)
    - remove_spaces (modifying or not),
      remove_trailing_spaces, remove_leading_spaces,
      remove_leadingtrailing_spaces
    - transform_spaces(string, char), transform_spaces(string&, char)
      replaces whitespace-characters, contracting adjacent ones and
      eliminating leading and trailing ones

    - get_content(std::istream), get_content(std:filesystem::path)
    - get_lines(std::istream), get_lines(std:filesystem::path)

    - class CDstr ("component digit string") for the "digit-components" of a
      string, which either are all-digit or none-digit
    - class DecompStr contains the decomposition of a string into these
      components
    - comparator-class AlphaNum for alphanumerical comparison of strings.

    - tyepdefs index_vec_t, index_map_t, indstr_t
      for indexing strings
    - valid(indstr_t) checks whether an indexing is correct
    - indexing_strings(Iterator, Iterator, bool ignore_duplicates)
      -> indstr_t.

    - out_line(ostream&, RAN R, sep, width)
    - out_lines(ostream& RAN R, sep1, sep2, width)

    - printsize(ostream&, X x): number of characters printed with x
    - print2dformat(ostream&, VEC2d, seps): formated printing of ragged matrix


TODOS:

1. Switch to namespace "Strings".

*/

#ifndef STRINGS_fi927Z4OV3
#define STRINGS_fi927Z4OV3

#include <string>
#include <string_view>
#include <algorithm>
#include <sstream>
#include <ostream>
#include <istream>
#include <vector>
#include <locale>
#include <filesystem>
#include <fstream>
#include <exception>
#include <compare>
#include <cctype>
#include <map>
#include <utility>

namespace Environment {

// Turning the value of a macro into a string:
#define SVBR333ZxeL(x) #x
#define STR(x) SVBR333ZxeL(x)

  inline std::string qu(std::string s) {
    return "\"" + s + "\"";
  }


  // Replace character x by y in string s (returning a copy):
  inline std::string replace(std::string s, const char x, const char y) {
    std::replace(s.begin(), s.end(), x, y);
    return s;
  }
  // Remove character x in s:
  inline std::string remove(std::string s, const char x) {
    s.erase(std::remove(s.begin(), s.end(), x), s.end());
    return s;
  }
  // Remove all content after first character c (including c; possibly there
  // is no c):
  inline void cutoff(std::string& s, const char c) noexcept {
    s.resize(std::min(s.size(), s.find(c)));
  }


  // The initial part of the string before the first '.':
  inline std::string basename(const std::string_view name) {
    return std::string(name.substr(0, name.find('.')));
  }

  // The "automatic" program name, derived from the filename:
  inline std::string auto_prg(const std::string_view filename) {
#ifdef NDEBUG
    return basename(filename);
#else
    return basename(filename) + "_debug";
#endif
  }

  // Split string s into a vector of tokens, using separator sep (ignoring
  // a final character sep, but otherwise possibly producing empty tokens):
  typedef std::vector<std::string> tokens_t;
  inline tokens_t split(const std::string_view s, const char sep) {
    std::istringstream ss(s.data());
    tokens_t res;
    std::string item;
    while (std::getline(ss, item, sep)) res.push_back(item);
    return res;
  }
  inline std::vector<tokens_t> split2(const std::string_view s,
                                      const char sep1, const char sep2) {
    const tokens_t res0 = split(s, sep1);
    std::vector<tokens_t> res; res.reserve(res0.size());
    for (const std::string& str : res0)
      res.push_back(split(str, sep2));
    return res;
  }
  inline tokens_t split(std::istream& s, const char sep) {
    tokens_t res;
    std::string item;
    while (std::getline(s, item, sep)) res.push_back(item);
    return res;
  }
  // Now also determining the final character extracted (0 iff none):
  inline tokens_t split(std::istream& s, const char sep, char& final) {
    final = 0;
    tokens_t res;
    if (s.eof()) return res;
    std::string item;
    while (std::getline(s, item, sep)) {
      res.push_back(item);
      s.unget(); s.get(final);
    }
    return res;
  }

  inline bool isspace(const char c) noexcept {
    const std::locale loc;
    return std::isspace(c,loc);
  }
  inline bool onlyspaces(const std::string& s) noexcept {
    return std::all_of(s.begin(), s.end(), isspace);
  }
  inline bool starts_with_space(const std::string& s) noexcept {
    if (s.empty()) return false;
    else return isspace(s[0]);
  }
  inline bool ends_with_space(const std::string& s) noexcept {
    if (s.empty()) return false;
    else return isspace(s.back());
  }


  // Splitting on sep, cutting off after c, and removing empty items:
  inline tokens_t split_cutoff(std::istream& s,
                               const char sep, const char c) {
    assert(not s.bad());
    tokens_t res;
    std::string item;
    while (std::getline(s, item, sep)) {
      cutoff(item, c);
      if (not onlyspaces(item)) res.push_back(item);
    }
    return res;
  }


  // Remove all whitespace:
  inline void remove_spaces(std::string& s) noexcept {
    s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());
  }
  inline std::string remove_spaces(const std::string& s) noexcept {
    std::string res = s;
    remove_spaces(res);
    return res;
  }
  // Transforms whitespace into char alt, contracting adjacent whitespace,
  // and eliminating leading and trailing whitespace:
  inline void transform_spaces_mod(std::string& s, const char alt = ' ') {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    s.erase(std::unique(s.begin(), s.end(), [&](const char c1, const char c2){return isspace(c1) and isspace(c2);}), s.end());
    std::replace_if(s.begin(), s.end(), isspace, alt);
  }
  inline std::string transform_spaces(std::string s, const char alt = ' ') {
    transform_spaces_mod(s, alt);
    return s;
  }


  // Secondary split on spaces, eliminating all whitespace otherwise:
  inline std::vector<tokens_t> split2_cutoff(std::istream& s,
                                      const char sep1,
                                      const char c) {
    assert(not s.bad());
    std::vector<tokens_t> res;
    std::string line;
    while (std::getline(s, line, sep1)) {
      cutoff(line, c);
      transform_spaces_mod(line);
      if (not line.empty()) res.push_back(split(line, ' '));
    }
    return res;
  }


  inline std::string remove_trailing_spaces(std::string s) {
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    return s;
  }
  inline std::string remove_leading_spaces(std::string s) {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    return s;
  }
  inline std::string remove_leadingtrailing_spaces(std::string s) {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    return s;
  }


  std::string get_content(const std::istream& in) {
    assert(in);
    std::ostringstream s; s << in.rdbuf();
    assert(not s.bad());
    if (in.bad())
      throw std::runtime_error("ERROR[Environment::get_content(in)]: "
        "Reading-error");
    return s.str();
  }
  tokens_t get_lines(const std::istream& in) {
    return split(get_content(in), '\n');
  }
  std::string get_content(const std::filesystem::path& p) {
    std::ifstream content(p);
    if (not content)
      throw std::runtime_error("ERROR[Environment::get_content(p)]: "
        "Can't open file\n  " + p.string());
    std::ostringstream s; s << content.rdbuf();
    if (s.bad() or content.bad())
      throw std::runtime_error("ERROR[Environment::get_content(p)]: "
        "Reading-error with file\n  " + p.string());
    return s.str();
  }
  tokens_t get_lines(const std::filesystem::path& p) {
    return split(get_content(p), '\n');
  }


  /*
    "Components with digits"

    The contained string is either all digits or all non-digits, with
    alphanumerical comparison (with all-digits < all-non-digits).
  */
  class CDstr {
    std::string s;
  public :
    const std::string& operator()() const noexcept { return s; }
    // Non-empty, either all digits or no digits, in the former case
    // no leading zero except for zero itself:
    static bool valid(const std::string& s) noexcept {
      if (s.empty()) return false;
      const char f = s.front();
      if (not std::isdigit(f))
        return not std::any_of(s.begin()+1, s.end(),
                               [](const char c){return std::isdigit(c);});
      else if (f == '0') return s.size() == 1;
      else return std::all_of(s.begin()+1, s.end(),
                              [](const char c){return std::isdigit(c);});
    }

    typedef std::string::size_type size_t;
    explicit CDstr(const std::string& in, size_t& pos)
      : s(extract(in, pos)) { assert(valid(s)); }
    static std::string extract(const std::string& in, size_t& pos0) {
      const size_t pos = pos0;
      const size_t size = in.size();
      assert(pos < size);
      const char f = in[pos];
      ++pos0;
      if (std::isdigit(f)) {
        if (f != '0') {
          while (std::isdigit(in[pos0])) ++pos0;
          return in.substr(pos, pos0-pos);
        }
        else {
          while (in[pos0] == '0') ++pos0;
          if (pos0 == size) return "0";
          const size_t pos1 = pos0;
          while (std::isdigit(in[pos0])) ++pos0;
          if (pos0 == pos1) return "0";
          else return in.substr(pos1, pos0-pos1);
        }
      }
      else {
        while (pos0 < size and not std::isdigit(in[pos0])) ++pos0;
        return in.substr(pos, pos0-pos);
      }
    }

    bool operator ==(const CDstr&) const noexcept = default;
    // Numeric strictly less than non-numerical, otherwise alphabetical:
    std::strong_ordering operator <=>(const CDstr& rhs) const noexcept {
      assert(valid(s) and valid(rhs.s));
      const bool d = std::isdigit(s.front()), dr = std::isdigit(rhs.s.front());
      if (d) {
        if (not dr) return std::strong_ordering::less;
        else {
          const auto res = s.size() <=> rhs.s.size();
          if (res < 0) return std::strong_ordering::less;
          else if (res > 0) return std::strong_ordering::greater;
          else return s <=> rhs.s;
        }
      }
      else {
        if (dr) return std::strong_ordering::greater;
        else return s <=> rhs.s;
      }
    }
  };

  struct DecompStr {
    typedef std::vector<CDstr> v_t;
  private :
    v_t D;
    static bool valid(const v_t& D) noexcept {
      return std::all_of(D.begin(), D.end(),
                         [](const CDstr& s){return CDstr::valid(s());});
    }

  public :

    DecompStr() noexcept = default;
    explicit DecompStr(const std::string& s) : D(decomp(s)) {}
    static v_t decomp(const std::string& s) {
      v_t res;
      const CDstr::size_t size = s.size();
      for (CDstr::size_t pos = 0; pos < size; ) res.emplace_back(s, pos);
      return res;
    }

    const v_t& operator()() const noexcept { return D; }

    bool operator ==(const DecompStr&) const noexcept = default;
    std::strong_ordering operator <=>(const DecompStr& rhs) const noexcept {
      return D <=> rhs.D;
    }
  };

  struct AlphaNum {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      return DecompStr(lhs) < DecompStr(rhs);
    }
  };


  // Indexing strings
  typedef std::vector<std::string> index_vec_t;
  typedef std::map<std::string, size_t> index_map_t;
  typedef std::pair<index_vec_t, index_map_t> indstr_t;
  bool valid(const indstr_t& is) noexcept {
    const size_t N = is.first.size();
    if (N != is.second.size()) return false;
    for (const auto& [s,i] : is.second)
      if (i >= N or is.first[i] != s) return false;
    return true;
  }

  template <class IT>
  indstr_t indexing_strings(IT begin, const IT end,
                            const bool ignore_duplicates = true) {
    indstr_t res;
    if (begin == end) return res;
    {const auto& val = *begin;
     res.first.push_back(val); res.second.insert({val,0}); ++begin;
    }
    for (size_t i = 1; begin != end; ++begin) {
      const auto& val = *begin;
      const auto f = res.second.find(val);
      if (f == res.second.end()) {
        res.first.push_back(val); res.second.insert({val,i}); ++i;
      }
      else {
        if (not ignore_duplicates)
          throw std::runtime_error("ERROR[Environment::indexing_strings]: "
            " Duplication \"" + val + "\" at position " + std::to_string(i));
      }
    }
    assert(valid(res));
    return res;
  }
  template <class RAN>
  indstr_t indexing_strings(const RAN& r,
                            const bool ignore_duplicates = true) {
    return indexing_strings(r.cbegin(), r.cend(), ignore_duplicates);
  }


  // Output the range R, separated by sep, and with given width
  // for w != 0:
  template <class RAN>
  void out_line(std::ostream& out, const RAN& R,
                const std::string& sep = " ",
                const std::streamsize w = 0) {
    if (R.empty()) return;
    auto it = R.begin(); const auto end = R.end();
    if (w == 0) {
      out << *it; ++it;
      for (; it != end; ++it) out << sep << *it;
    }
    else {
      out.width(w);
      out << *it; ++it;
      for (; it != end; ++it) {
        out << sep; out.width(w); out << *it;
      }
    }
  }
  // Output a nested range:
  template <class RAN>
  void out_lines(std::ostream& out, const RAN& R,
                 const std::string& sep1 = "\n",
                 const std::string& sep2 = " ",
                 const std::streamsize w = 0) {
    const auto end = R.end();
    for (auto it = R.begin(); it != end; ++it) {
      out_line(out, *it, sep2, w);
      out << sep1;
    }
  }


  template <typename X>
  std::string::size_type printsize(std::ostream& out, const X& x) {
    std::ostringstream ss; ss.flags(out.flags());
    ss << x;
    return ss.str().size();
  }

  // Print a ragged 2d-matrix M, using per column the maximum width needed
  // for that column, plus seps many spaces for separation:
  template <class VEC2d>
  void print2dformat(std::ostream& out, const VEC2d& M,
                     std::string::size_type seps = 1) {
    typedef std::string::size_type size_t;
    if (M.empty()) return;
    const size_t cols = (std::ranges::max_element(M, {},
                           [](const auto& R){return R.size();})) -> size();
    std::vector<size_t> max_size(cols);
    for (const auto& R : M)
      for (size_t i = 0; i < R.size(); ++i)
        max_size[i] = std::max(max_size[i], printsize(out, R[i]));
    for (const auto& R : M) {
      const size_t s = R.size();
      if (s == 0) {
        out << "\n"; continue;
      }
      out.width(max_size[0]); out << R[0];
      for (size_t i = 1; i < s; ++i) {
        out << std::string(seps, ' ');
        out.width(max_size[i]); out << R[i];
      }
      out << "\n";
    }
  }

}

#endif
