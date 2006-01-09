// Oliver Kullmann, 10.10.2005 (Swansea)

/*!
  \file IOStreamFilters.hpp
  \brief Filters according to the Boost Iostream library.
*/

#ifndef IOSTREAMFILTERS_99ooKL

#define IOSTREAMFILTERS_99ooKL

#include <algorithm>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/char_traits.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/pipeline.hpp>

namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class BasicCounter
      \brief Extension of the filter boost::iostreams::basic_counter with a member function
      for the number of characters read of the current line.

      Filter for iostreams counting the number of lines read, the total number of characters read and the
      number of characters read of the current line.
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
    
  }

}

#endif
