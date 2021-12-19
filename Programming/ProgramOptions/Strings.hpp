// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* String helper functions

   String helper functions:
    - STR(x) is a macro, putting quotation marks around x
    - qu(string) adds quotes around a string
    - replace(string, char, char), remove(string, char)
    - basename(string) extracts the part of the string before "."
    - auto_prg(filename) ("automatic" program-name from file-name)
    - split(string, char), split(istream, char),
      split(istream, char, char& final_character)
      split2(string, char1, char2)
    - transform_spaces(string, char) replaces whitespace-characters,
      contracting adjacent ones and eliminating leading and trailing ones.
    - remove_spaces, remove_trailing_spaces, remove_leading_spaces,
      remove_leadingtrailing_spaces
    - get_content(std:filesystem::path).

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
    std::stringstream ss(s.data());
    tokens_t res;
    std::string item;
    while (std::getline(ss, item, sep)) res.push_back(item);
    return res;
  }
  inline std::vector<tokens_t> split2(const std::string_view s, const char sep1, const char sep2) {
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

  // Remove all whitespace:
  inline void remove_spaces(std::string& s) noexcept {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(std::remove_if(s.begin(), s.end(), sp), s.end());
  }
  // Transforms whitespace into char alt, contracting adjacent whitespace,
  // and eliminating leading and trailing whitespace:
  inline std::string transform_spaces(std::string s, const char alt = ' ') {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), sp));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), sp).base(), s.end());
    s.erase(std::unique(s.begin(), s.end(), [&](const char c1, const char c2){return sp(c1) and sp(c2);}), s.end());
    std::replace_if(s.begin(), s.end(), sp, alt);
    return s;
  }
  inline std::string remove_trailing_spaces(std::string s) {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(std::find_if_not(s.rbegin(), s.rend(), sp).base(), s.end());
    return s;
  }
  inline std::string remove_leading_spaces(std::string s) {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), sp));
    return s;
  }
  inline std::string remove_leadingtrailing_spaces(std::string s) {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), sp));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), sp).base(), s.end());
    return s;
  }

  std::string get_content(const std::filesystem::path& p) {
    std::ifstream content(p);
    if (not content)
      throw std::runtime_error("ERROR[Environment::get_content]: "
        "Can't open file\n  " + p.string());
    std::stringstream s; s << content.rdbuf();
    if (s.bad() or content.bad())
      throw std::runtime_error("ERROR[Environment::get_content]: "
        "Reading-error with file\n  " + p.string());
    return s.str();
  }

}

#endif
