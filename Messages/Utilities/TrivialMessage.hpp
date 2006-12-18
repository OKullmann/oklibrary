// Oliver Kullmann, 3.12.2006 (Swansea)

/*!
  \file Messages/Utilities/TrivialMessage.hpp
  \brief Components to wrap a streamable object as a message object
*/

#ifndef TRIVIALMESSAGE_jdjNNaml74re34
#define TRIVIALMESSAGE_jdjNNaml74re34

#include <Transitional/Messages/MessagesBase.hpp>

namespace OKlib {
  namespace Messages {
    namespace Utilities {

      /*!
        \class TrivialMessage
        \brief TrivialMessage<X>(x) prints x

        Attention: Takes parameter x by (const) reference.
      */
      template <typename X>
      struct TrivialMessage : ::OKlib::Messages::MessagesBase {
        typedef X value_type;
        explicit TrivialMessage(const value_type& x) : x(x) {}
        std::ostream& print(std::ostream& out) const { return out << x; }
      private :
        TrivialMessage& operator =(const TrivialMessage&); // no copy assignment
        const value_type& x;
      };
      /*!
        \class TrivialMessageC
        \brief TrivialMessageC<X>(x) prints x

        Different from TrivialMessage, here x is passed by value.
      */
      template <typename X>
      struct TrivialMessageC : ::OKlib::Messages::MessagesBase {
        typedef X value_type;
        explicit TrivialMessageC(const value_type x) : x(x) {}
        std::ostream& print(std::ostream& out) const { return out << x; }
      private :
        const value_type x;
      };

      //! Convenience function template for class TrivialMessage
      template <typename X>
      inline TrivialMessage<X> trivial_message(const X& x) {
        return TrivialMessage<X>(x);
      }
      //! Overload for C-strings (to avoid potential code bloat)
      inline TrivialMessageC<const char*> trivial_message(const char*  x) {
        return TrivialMessageC<const char*>(x);
      }
    }
  }
}

#endif
