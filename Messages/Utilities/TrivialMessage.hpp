// Oliver Kullmann, 3.12.2006 (Swansea)

/*!
  \file Messages/Utilities/TrivialMessage.hpp
  \brief Component to wrap a streamable object as message object
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
      */

      template <typename X>
      struct TrivialMessage : ::OKlib::Messages::MessagesBase {
        typedef X value_type;
        explicit TrivialMessage(const value_type& x) : x(x) {}
        std::ostream& print(std::ostream& out) const {
          return out << x;
        }
      private :
        TrivialMessage& operator =(const TrivialMessage&); // no copy assignment
        const value_type& x;
      };

      /*!
        \brief Convenience function template for class TrivialMessage
      */
      template <typename X>
      inline TrivialMessage<X> trivial_message(const X& x) {
        return TrivialMessage<X>(x);
      }
    }
  }
}

#endif
