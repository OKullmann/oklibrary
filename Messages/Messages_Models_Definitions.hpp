// Oliver Kullmann, 2005 (Swansea)

/*!
  \file Messages_Models_Definitions.hpp
  \brief Messages base class and related constructions
  \todo The systems seems not to work! I suspect TB did some (wrong) changes.
  \todo Add documentation
*/

#ifndef MESSAGESMODELSDEFINITIONS_j6gg9bbff3

#define MESSAGESMODELSDEFINITIONS_j6gg9bbff3

#include <string>
#include <ios>
#include <map>
#include <cassert>
#include <iterator>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/placeholders.hpp>

#include "IOTools.hpp"

namespace OKlib {
  
  namespace Messages {

    /*!
      \class W
      \brief W<Name> is a class just holding Name (accessible via partial
      specialisation), where Name is a C string.
    */
    template <const char* Name>
    struct W {};

    // ###########################################################

    /*!
      \class Language_base
      \brief Polymorphic base class for language objects. A language_base pointer inserted into
      a std::ostream sets the language of that stream.

      Contains the (private) static map "catalogue", mapping language-names to language_base pointers.
      A language-name is a C string. Public static operations are
       - static const Language_base* language(language_names) yields access to the (private) map
       - static iterator begin(), static end() enable iteration over the map of language objects.
      Public non-static operations are
       - int index_language() const yields the index of the language-subsystem which identifies it with std::ostreams
         (the index for the pword in ostreams using for an object p of type Language_base* the assignment
         os.pword(p -> index_language()) = const_cast<void*>(static_cast<const void*>(p)););
         transfers the call to the pure virtual function index_language_().
       - language_names name() const returns the language-name of the language object; transfers the call to the pure virtual
         function name_().
      \todo Why does index_language() depend on the language *object*?
    */
      
    class Language_base {
    public :
      typedef const char* language_names;
    private :
      typedef std::map<language_names, const Language_base*, IOTools::string_compare> Map;
      
    public :

      class iterator;
      static const Language_base* language(language_names const name) {
        const Map::const_iterator i = catalogue.find(name);
        return (i == catalogue.end()) ? (Language_base*)0 : i -> second;
      }

      language_names name() const { return name_(); }

      static iterator begin() { return iterator(catalogue.begin()); }
      static iterator end() { return iterator(catalogue.end()); }

      int index_language() const { return index_language_(); }

    protected :

      static Map catalogue;
      virtual ~Language_base() {}

    private :

      virtual language_names name_() const = 0;
      virtual int index_language_() const = 0;
      
    public:

      /*!
        \class iterator
        \brief Iterator facade to enable iterations through all available Language_base pointers
        \todo Likely this should be implemented using the boost transform-iterator adaptor.
      */

      class iterator : std::iterator<std::bidirectional_iterator_tag, const Language_base*, Map::difference_type> {
        typedef Language_base::Map::const_iterator internal_iterator;
	
        internal_iterator it;
        explicit iterator(const internal_iterator i) { it = i; }
        friend iterator Language_base::begin();
        friend iterator Language_base::end();
	
      public :
	
        iterator() {}
	 
        const Language_base* operator * () const { return it -> second; }
        
        iterator& operator ++() { ++it; return *this; }
        iterator operator ++(int) { return iterator(it++); }
      
        iterator& operator --() { --it; return *this; }
        iterator operator --(int) { return iterator(it--); }
	 
        friend bool operator == (const iterator lhs, const iterator rhs) {
          return lhs.it == rhs.it;
        }
        friend bool operator != (const iterator lhs, const iterator rhs) {
          return not (lhs == rhs);
        }
      };
    };

    // #########################
    // Static data members
    
    Language_base::Map Language_base::catalogue;

    // #########################
    // Associated global functions
    
    template <typename charT, class Traits>
    inline std::basic_ostream<charT, Traits>& operator <<(std::basic_ostream<charT, Traits>& os, const Language_base* p) {
      os.pword(p -> index_language()) = const_cast<void*>(static_cast<const void*>(p));
      return os;
    }
    
    // ###########################################################

    /*!
      \class MessageService
      \brief A class templete hosting basic types for message services, and providing the nested classes
      Messages_base (from which all message classes are to be derived) and Languages<Name> (representing
      the available languages).

      MessageService manages the (private) static data members int index_language and const Messages_base* transfer.
      MessageService<default_language, SequenceLanguages, charT, Traits, String> offers access to the
      default language, the character type, the character type and the string type, and defines
      the nested class Languages, the derived nested class template Language,  and the nested class Messages_base.
      Languages is derived from Language_base, and provides the language index (referring to the static
      data member). Language<Name> then completes the definition.
      \todo What is the purpose of transfer?
    */

    template <const char* default_language, class SequenceLanguages = boost::mpl::vector0<>, typename charT = char, class Traits = std::char_traits<charT>, typename String = const charT*>
    class MessageService {
      MessageService(); // not to be implemented (no constructor defined)
    public :

      typedef charT char_type;
      typedef Traits traits_type;
      typedef String string_type;

      /*!
        \class Languages
        \brief Derived from Language_base. Implements index_language_() (using the static data member
        of MessageService). Adds public member function string_type translated() const, which calls
        the pure virtual function translated_().
        \todo What is the purpose of translated() ?
      */

      class Languages : public Language_base {
        virtual string_type translated_() const = 0;
        int index_language_() const { return MessageService::index_language; }
      public :
        string_type translated() const { return translated_(); }
      };

      /*!
        \class Language
        \brief Language<const char* Name> is derived from Languages, and provides objects who upon creation (by the default
        constructor) insert Name into the basic language container (only once). Contain the static (private) member
        const Language* p, representing the language instance.

        Implements name_() (returning Name) and translated_().
      */
      
      template <const char* Name>
      class Language; // defined below

      typedef Language<default_language> Default;

    private :

      // ---------------------------------------------------
      // private nested class MessageBase (in MessageService)
      // ---------------------------------------------------

      struct MessageBase {
      protected :
        virtual ~MessageBase() {}
        virtual string_type default_translate() const = 0;
      };
      
      // ---------------------------------------------------
      // private nested class P (in MessageService): Metafunction class
      // P::apply<W<Name> >::type has virtual member function
      // string_type translate(const Language<Name>*) const.
      // ---------------------------------------------------

      struct P {
        template <typename T>
        struct apply;
        template <const char* Name>
        struct apply<W<Name> > {
          struct type : virtual MessageBase {
            virtual string_type translate(const Language<Name>*) const {
              return this -> default_translate();
            }
          };
        };
      };

      // nested private class intermediate_base:
      // derived from a P<language_1>, P<language_2>, ...
      // where language in SequenceLanguage
      typedef typename boost::mpl::template transform<SequenceLanguages, P>::type language_vector;
      typedef typename boost::mpl::inherit_linearly<language_vector, boost::mpl::inherit2<boost::mpl::placeholders::_1, boost::mpl::placeholders::_2> >::type intermediate_base;
      
    public :
    
      // ---------------------------------------------------
      // public nested class Messages_base (in MessageService)
      // derived from P<W<default_language> > and intermediate_base
      // ---------------------------------------------------
      
      class Messages_base : public P::template apply<W<default_language> >::type, public intermediate_base {
      public :
        string_type operator() (const Languages* const p) const {
          transfer = identity();
          assert(p);
          return p -> translated();
        }

        // TO DO: make translate and print private
        virtual string_type translate(const Default*) const = 0;

        // TO DO: improve organisation regarding "print"
        template <typename charT_s, class Traits_s>
        std::ios_base::iostate print(std::basic_ostream<charT_s, Traits_s>& os) const {
          const void* p = os.pword(index_language);
          const Languages* const lp = (p) ? static_cast<const Languages*>(p) : static_cast<const Languages*>(Default::p);
          assert(lp);
          if (os.good() and (os << operator ()(lp)).good())
            return std::ios_base::goodbit;
          else
            return std::ios_base::failbit;
        }
        
      private :
        virtual string_type default_translate() const {
          return translate(Default::p);
        };
        virtual const Messages_base* identity() const = 0;

        template <typename charT_s, class Traits_s>
        friend typename std::basic_ostream<charT_s, Traits_s>& operator <<(typename std::basic_ostream<charT_s, Traits_s>& os, const Messages_base& m) {
          if (not os.good()) return os;
          return IOTools::generic_inserter(os, m);
        }
    
      };
    
    private :
    
      static const Messages_base* transfer;

    public :

      template <const char* Name>
      class Language : public Languages {
        const char* name_() const { return Name; }
      private :
        Language() {
          assert(not p); // to prevent creation of more than one object
          Languages::catalogue.insert(std::make_pair(Name, this));
        }
      public :
        ~Language() {}
        static const Languages* ptr() {
          return static_cast<const Languages*>(p);
        }
      private :
        virtual string_type translated_() const {
          assert(transfer);
          return (transfer) -> P::template apply<W<Name> >::type::translate(this);
        }
        friend class Messages_base;
        static const Language* p; // points to a newly created object
      };

    private :

      static const int index_language;
    };

     // #########################
    // Static data members

    template <const char* default_language, class SequenceLanguages, typename charT, class Traits, typename String>
    const int MessageService<default_language, SequenceLanguages, charT, Traits, String>::index_language(std::ios_base::xalloc());
    
    template <const char* default_language, class SequenceLanguages, typename charT, class Traits, typename String>
    const typename MessageService<default_language, SequenceLanguages, charT, Traits, String>::Messages_base* MessageService<default_language, SequenceLanguages, charT, Traits, String>::transfer;
    
    template <const char* default_language, class SequenceLanguages, typename charT, class Traits, typename String>
    template <const char* Name>
    const typename MessageService<default_language, SequenceLanguages, charT, Traits, String>::template Language<Name>* MessageService<default_language, SequenceLanguages, charT, Traits, String>::Language<Name>::p = new typename MessageService<default_language, SequenceLanguages, charT, Traits, String>::template Language<Name>;
    
  }
}

#endif
