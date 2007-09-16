// Oliver Kullmann, 10.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file GeneralInputOutput/IOStreamFilters.hpp
  \brief Filters according to the Boost Iostream library.
*/

#ifndef IOSTREAMFILTERS_99ooKL
#define IOSTREAMFILTERS_99ooKL

#include <algorithm>
#include <memory>
#include <string>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/char_traits.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/pipeline.hpp>
#include <boost/iostreams/filter/line.hpp>

namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class BasicCounter
      \brief Extension of the filter boost::iostreams::basic_counter with a member function
      for the number of characters read from the current line.

      Filter for iostreams counting the number of lines read, the total number of characters read and the
      number of characters read from the current line.
    */

    template<typename Ch>
    class BasicCounter  {
    public:
      typedef Ch char_type;
      struct category
        : boost::iostreams::dual_use,
          boost::iostreams::filter_tag,
          boost::iostreams::multichar_tag,
          boost::iostreams::optimally_buffered_tag { };

      explicit BasicCounter(int first_line = 0, int first_char = 0, int current_char = 0)
        : lines_(first_line), chars_(first_char), current_chars_(current_char) { }

      int lines() const { return lines_; }
      int characters() const { return chars_; }
      int current_characters() const { return current_chars_; }
      std::streamsize optimal_buffer_size() const { return 0; }
      
      template<typename Source>
      std::streamsize read(Source& src, char_type* const s, const std::streamsize n) {
        const std::streamsize result = boost::iostreams::read(src, s, n);
        if (result == -1)
          return -1;
        chars_ += result;
        int newlines = 0;
        const char_type* last_newline = 0;
        const char_type* end = s + result;
        for (const char_type* p = s; p != end; ++p)
          if (*p ==  boost::iostreams::char_traits<Ch>::newline()) {
            ++newlines;
            last_newline = p;
          }
        lines_ += newlines;
        if (newlines == 0)
          current_chars_ += result;
        else
          current_chars_ = end - ++last_newline;
        return result;
      }
      
      template<typename Sink>
      std::streamsize write(Sink& snk, const char_type* s, std::streamsize n) {
        std::streamsize result = boost::iostreams::write(snk, s, n);
        lines_ += std::count(s, s + result, boost::iostreams::char_traits<Ch>::newline());
        chars_ += result;
        return result;
      }
    private:
      int lines_;
      int chars_;
      int current_chars_;
    };
    BOOST_IOSTREAMS_PIPABLE(BasicCounter, 1)

    typedef BasicCounter<char> Counter;
    typedef BasicCounter<wchar_t> WCounter;

    // ########################################################

    /*!
      \class BasicIndentLines
      \brief Filter for iostreams indenting each line

      Remark: In order to make sure that the full output reached the underlying sink,
      flushing of the filter(-stream) is necessary.
    */

    template<typename Ch, typename Alloc = std::allocator<Ch> >
    class BasicIndentLines : public ::boost::iostreams::basic_line_filter<Ch, Alloc> {
      typedef ::boost::iostreams::basic_line_filter<Ch, Alloc> base_type;
    public :
      typedef Ch char_type;
      typedef typename base_type::string_type string_type;
      typedef unsigned long int size_type;
    private :
      size_type indent;
      char_type ch;
    public :
      explicit BasicIndentLines(const unsigned long int indentation = 0, const char_type ch = ' ') : indent(indentation), ch(ch) {}
      size_type indentation() const { return indent; }
      size_type& indentation() { return indent; }
      char_type character() const { return ch; }
      char_type& character() { return ch; }
    private :
      string_type do_filter(const string_type& line) {
        return string_type(indent, ch) + line;
      }
    };

    typedef BasicIndentLines<char> IndentLines;
    typedef BasicIndentLines<wchar_t> WIndentLines;
    
  }

}

#endif
