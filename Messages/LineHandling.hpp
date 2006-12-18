// Oliver Kullmann, 17.12.2006 (Swansea)

/*!
  \file Message/LineHandling.hpp
  \brief General message-class for line-start and line-end handling
*/

#ifndef LINEHANDLING_qpqPPlMn548
#define LINEHANDLING_qpqPPlMn548

#include <string>

#include <Transitional/Messages/MessagesBase.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \class MessagesLines
      \brief Basic messages class, extending MessagesBase by additionally providing virtual functions for line-start and line-end
    */

    class MessagesLines : public MessagesBase {
    protected :
      virtual std::ostream& l_start(std::ostream& out) const {return out;}
      virtual std::ostream& l_end(std::ostream& out) const {out << "\n"; return out;}
    };

    /*!
      \class MessagesPrePost
      \brief Extends MessagesLines by managing two strings for prefixing and postfixing lines
    */

    class MessagesPrePost : public MessagesLines {
      std::string prefix_, postfix_;
    protected :
      std::ostream& l_start(std::ostream& out) const {out << prefix_; return out;}
      std::ostream& l_end(std::ostream& out) const {out << postfix_; return out;}
    public :
      MessagesPrePost() :
        prefix_(),
        postfix_("\n")
      {}

      const std::string& prefix() const {return prefix_;}
      std::string& prefix() {return prefix_;}
      const std::string& postfix() const {return postfix_;}
      std::string& postfix() {return postfix_;}

      typedef std::string::size_type size_type;
      MessagesPrePost& set_prefix(const size_type n) {
        prefix_.resize(n, ' '); return *this;
      }
      MessagesPrePost& set_prefix(const std::string s) {
        prefix_ = s; return *this;
      }
      MessagesPrePost& set_postfix(const std::string s) {
        postfix_ = s; return *this;
      }
    };

  }
}

#endif
