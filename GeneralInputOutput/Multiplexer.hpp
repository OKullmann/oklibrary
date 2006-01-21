// Oliver Kullmann, 19.1.2006 (Swansea)

/*!
  \file Multiplexer.hpp
  \brief Components for gathering and distributing input and output from and to different streams
  \todo Test all components.
  \todo Complete the documentation.
*/

#ifndef MULTIPLEXER_8UyT5R4

#define MULTIPLEXER_8UyT5R4

#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <ostream>

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/tee.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

#include "IOStreamSinks.hpp"

namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class OStreamDescriptor
      \brief Concrete type for a pair consisting of a label and a string, which describe some output-stream.
      \todo Throw exceptions instead of asserting.
    */
    
    struct OStreamDescriptor {

      enum label_type { nullstream = 0, stdcerr = 1, stdcout = 2, stdofstreamappend = 3, stdofstreamoverwrite = 4, stdostringstream = 5  };

      typedef std::string string_type;

      label_type label;
      string_type name;

      OStreamDescriptor(const label_type label, const string_type& name) : label(label), name(name) {
        assert(not name.empty() or label <= stdcout);
        assert(name.empty() or label > stdcout);
      }
      OStreamDescriptor(const string_type& description) {
        if (description == "NULL")
          label = nullstream;
        else if (description == "cerr")
          label = stdcerr;
        else if (description == "cout")
          label = stdcout;
        else {
          typedef string_type::size_type size_type;
          const size_type& size(description.size());
          assert(size > 0);
          const size_type& eq_sign(description.find('='));
          assert(eq_sign != string_type::npos);
          assert(eq_sign < size - 1);
          const string_type& lhs(description.substr(0, eq_sign));
          if (lhs == "ofstreamappend")
            label = stdofstreamappend;
          else if (lhs == "ofstreamoverwrite")
            label = stdofstreamoverwrite;
          else
            label = stdostringstream;
          name = string_type(description.substr(eq_sign+1, size));
        }
      }
    };

    bool operator <(const OStreamDescriptor& lhs, const OStreamDescriptor& rhs) {
      return lhs. label < rhs.label or (lhs.label == rhs.label and lhs.name < rhs.name);
    }

    // ############################################

    /*!
      \class OStreamMultiplexer
      \brief Class for creating ostreams which comprise described ostreams.
    */

    class OStreamMultiplexer {
      OStreamMultiplexer(const OStreamMultiplexer&); // no copying
      OStreamMultiplexer& operator =(const OStreamMultiplexer&); // no assignment
    public :
      typedef ::OKlib::GeneralInputOutput::OStreamDescriptor ostream_descriptor_type;
      typedef ostream_descriptor_type::label_type label_type;
      typedef ostream_descriptor_type::string_type string_type;

      typedef boost::ptr_map<string_type, std::stringstream> stringstream_map_type;
      typedef stringstream_map_type::const_iterator stringstream_map_iterator;

      stringstream_map_type stringstream_map;

      typedef boost::ptr_map<string_type, std::ofstream> ofstream_map_type;
      typedef ofstream_map_type::const_iterator ofstream_map_iterator;

      ofstream_map_type ofstream_map;

    private :
      typedef boost::iostreams::tee_filter<std::ostream> tee_filter_type;
      typedef boost::ptr_list<tee_filter_type> filter_list_type;

      filter_list_type filter_list;

    public :
      typedef boost::ptr_vector<boost::iostreams::filtering_ostream> fostream_vector_type;
      fostream_vector_type fostream_vector;

    private :
      ::OKlib::GeneralInputOutput::NullStream null_stream;

    public :
      template <class MultiPassInputRange>
      OStreamMultiplexer(const MultiPassInputRange& range) {
        typedef MultiPassInputRange range1_type;
        typedef typename boost::range_const_iterator<range1_type>::type range1_iterator;
        typedef typename boost::range_value<range1_type>::type range2_type;
        typedef typename boost::range_const_iterator<range2_type>::type range2_iterator;

        const range1_iterator& end(boost::end(range));

        for (range1_iterator i(boost::begin(range)); i != end; ++i) {
          const range2_iterator& end(boost::end(*i));
          for (range2_iterator j(boost::begin(*i)); j != end; ++j) {
            const label_type& label(j -> label);
            string_type name(j -> name);
            switch (label) {
            case ostream_descriptor_type::stdofstreamappend : {
              const ofstream_map_iterator& element(ofstream_map.find(name));
              if (element == ofstream_map.end())
                ofstream_map.insert(name, new std::ofstream(name.c_str(), std::ios::out | std::ios::app));
              break;
            }
            case ostream_descriptor_type::stdofstreamoverwrite : {
              const ofstream_map_iterator& element(ofstream_map.find(name));
              if (element == ofstream_map.end())
                ofstream_map.insert(name, new std::ofstream(name.c_str()));
              break;
            }
            case ostream_descriptor_type::stdostringstream : {
              const stringstream_map_iterator& element(stringstream_map.find(name));
              if (element == stringstream_map.end())
                stringstream_map.insert(name, new std::stringstream);
              break;
            }
            default :
              break;
            }
          }
        }

        for (range1_iterator i(boost::begin(range)); i != end; ++i) {
          fostream_vector.push_back(new boost::iostreams::filtering_ostream);
          if (boost::empty(*i)) {
            fostream_vector.back().push(null_stream);
            continue;
          }
          const range2_iterator& end(boost::end(*i));
          for (range2_iterator j(boost::begin(*i)); j != end; ++j) {
            const label_type& label(j -> label);
            const string_type& name(j -> name);
            if (boost::next(j) == end) {
              switch (label) {
              case ostream_descriptor_type::nullstream :
                fostream_vector.back().push(null_stream); break;
              case ostream_descriptor_type::stdcerr :
                fostream_vector.back().push(std::cerr); break;
              case ostream_descriptor_type::stdcout :
                fostream_vector.back().push(std::cout); break;
              case ostream_descriptor_type::stdostringstream :
                fostream_vector.back().push(stringstream_map[name]); break;
              default :
                fostream_vector.back().push(ofstream_map[name]); break;
              }
              }
              else {
                switch (label) {
                case ostream_descriptor_type::nullstream :
                  break;
                case ostream_descriptor_type::stdcerr :
                  filter_list.push_back(new tee_filter_type(std::cerr)); break;
                case ostream_descriptor_type::stdcout :
                  filter_list.push_back(new tee_filter_type(std::cout)); break;
                case ostream_descriptor_type::stdostringstream :
                  filter_list.push_back(new tee_filter_type(stringstream_map[name])); break;
                default :
                  filter_list.push_back(new tee_filter_type(ofstream_map[name])); break;
                }
                fostream_vector.back().push(filter_list.back());
              }
          }
        }
      }
    };

  }

}

#endif
