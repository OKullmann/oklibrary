// Oliver Kullmann, 8.1.2006 (Swansea)

/*!
  \file GeneralInputOutput/IOStreamSinks.hpp
  \brief Output devices according to the Boost Iostream library
*/

#ifndef IOSTREAMSINKS_iJZZw3
#define IOSTREAMSINKS_iJZZw3

#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream.hpp>

namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class BasicNullStream
      \brief BasicNullStream<Char> is an output stream discarding all output
    */

    template <typename Char>
    struct BasicNullStream : boost::iostreams::stream<boost::iostreams::basic_null_sink<Char> > {
      BasicNullStream() : boost::iostreams::stream<boost::iostreams::basic_null_sink<Char> >(boost::iostreams::basic_null_sink<Char>(), 0, 0) {}
    };

    /*!
      \typedef NullStream
      \brief Output stream discarding all output
      \todo How can we define a global constant like std::cerr and std::cout?
    */

    typedef BasicNullStream<char> NullStream;
    typedef BasicNullStream<wchar_t> WNullStream;

  }

}

#endif
