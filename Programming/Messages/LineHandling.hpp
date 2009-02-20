// Oliver Kullmann, 17.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/LineHandling.hpp
  \brief General message-class for line-start and line-end handling
*/

#ifndef LINEHANDLING_qpqPPlMn548
#define LINEHANDLING_qpqPPlMn548

#include <string>

#include <OKlib/Programming/Messages/MessagesBase.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \class MessagesLines
      \brief Basic messages class, extending MessagesBase by additionally providing virtual functions for line-start and line-end

      At the begin of a line use
      "l_start(out);" or "l_start(out) << beginning_string;"
      (for example to enable indentation), and for the end of a line use
      "l_end(out);"
      (for example to make it possible, that line-ends are replaced by spaces).
    */

    class MessagesLines : public MessagesBase {
    protected :
      virtual std::ostream& l_start(std::ostream& out) const {return out;}
      virtual void l_end(std::ostream& out) const {out << "\n";}
    };

    /*!
      \class MessagesPrePost
      \brief Extends MessagesLines by managing two strings for prefixing and postfixing lines

      Via the various "prefix" and "postfix" member functions the line-start and line-end can
      be set to some string; default is empty for line-start and "\n" for line-end. The prefix-
      and postfix-behaviour should always be free to change (and thus care must be taken when
      using it internally).

      By the cp_pp ("copy pre-post") member function both the pre- and the postfix-string are
      copied from some other message object; this is used, when a MessagesPrePost-class
      uses another MessagesPrePost-class object: before outputting this object, cp_pp should
      be applied to set the pre-post-behavour of that other object accordingly.
    */

    class MessagesPrePost : public MessagesLines {
      std::string prefix_, postfix_;
    protected :
      std::ostream& l_start(std::ostream& out) const {out << prefix_; return out;}
      void l_end(std::ostream& out) const {out << postfix_;}
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

      MessagesPrePost& cp_pp(const MessagesPrePost& m) {
        prefix_ = m.prefix();
        postfix_ = m.postfix();
        return *this;
      }
    };

  }
}

#endif
