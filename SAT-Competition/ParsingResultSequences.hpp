// Oliver Kullmann, 3.6.2005 (Swansea)

#ifndef PARSINGRESULTSEQUENCES_uJnBv45

#define PARSINGRESULTSEQUENCES_uJnBv45

namespace OKlib {

  namespace SATCompetition {

    template <class ParserResult, class OutputIterator>
    class ParserResultSequence : public ::OKlib::Parser::ParserBase<typename ParserResult::char_type, typename ParserResult::ParseIterator> {
      typedef typename ParserResult::ParseIterator ParseIterator;
      OutputIterator& begin;
      typedef typename ParserResult::result_type Result;
      Result r;
      ParserResult p;
      struct action {
        OutputIterator& begin;
        Result& r;
        action(OutputIterator& begin, Result& r) : begin(begin), r(r) {}
        void operator() (ParseIterator, ParseIterator) const {
          *begin++ = r;
        }
      };
    public :
      ParserResultSequence(OutputIterator& begin) : begin(begin), p(r) {
        this -> parser_ = +((p.parser() >> boost::spirit::eol_p)[action(begin, r)]);
      }
    };

    template <class ParserResult, class OutputIterator>
    // ToDo: Conceptualisation
    // ParserResult is a parser for results
    // OutputIterator::value_type is Result
    // ToDo: It seems better to have the two operators in different specialisations.
    // ToDo: Making functor classes (with corresponding type traits) out of it.
    struct Copy_results {

      typedef typename ParserResult::ParseIterator ParseIterator;
      typedef boost::spirit::parse_info<ParseIterator> parse_info_it;
      typedef typename ParserResult::char_type char_type;
      typedef boost::spirit::parse_info<const char_type*> parse_info_c;

      parse_info_it operator() (const ParseIterator begin_in, const ParseIterator end_in, OutputIterator begin_out) {
        ParserResultSequence<ParserResult, OutputIterator> p(begin_out);
        return boost::spirit::parse(begin_in, end_in, p.parser());
      }
      template <typename PIterator>
      parse_info_c operator() (const PIterator begin_in, OutputIterator begin_out) {
        ParserResultSequence<ParserResult, OutputIterator> p(begin_out);
        return boost::spirit::parse(begin_in, p.parser());
      }
      
    };

    template <class ParserResult, class OutputIterator>
    inline typename Copy_results<ParserResult, OutputIterator>::parse_info_it copy_results(const typename ParserResult::ParseIterator begin_in, const typename ParserResult::ParseIterator end_in, const OutputIterator begin_out) {
      return Copy_results<ParserResult, OutputIterator>()(begin_in, end_in, begin_out);
    }
    template <class ParserResult, typename PIterator, class OutputIterator>
    inline typename Copy_results<ParserResult, OutputIterator>::parse_info_c copy_results(const PIterator begin_in, const OutputIterator begin_out) {
      return Copy_results<ParserResult, OutputIterator>()(begin_in, begin_out);
    }

    // ----------------------------------------------

    template <template <typename Result, typename CharT, typename ParseIterator> class ParserResult, class OutputIterator, class Result_ = Result>
    struct Copy_results_from_file {

      typedef char char_type; // ToDo: to generalise
      typedef boost::spirit::file_iterator<char_type> file_iterator;
      typedef boost::spirit::position_iterator<file_iterator> ParseIterator;
      typedef boost::spirit::parse_info<ParseIterator> parse_info_f;
      typedef ParserResult<Result_, char_type, ParseIterator> Parser;

      parse_info_f operator() (const boost::filesystem::path& filename, OutputIterator begin_out) {
        const std::string native_filename(filename.native_file_string());
        file_iterator file_begin(native_filename.c_str());
        if (not file_begin)
          throw ParserError("Error when opening " + native_filename);
        const file_iterator file_end(file_begin.make_end());
        const ParseIterator parse_begin(file_begin, file_end, native_filename);
        const ParseIterator parse_end;
        return copy_results<Parser>(parse_begin, parse_end, begin_out);
      }

    };

  }

}

#endif
