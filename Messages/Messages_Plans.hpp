// Oliver Kullmann, 29.1.2006 (Swansea)

/*!
  \file Messages_Plans.hpp
  \brief Plans for the messages module
  \todo First the old design has to be understood and reflected.
  \todo Three levels of detailedness:
   - basic : short (but nevertheless complete)
   - full : says everything explicitely
   - extensive : for example also with examples.
  \todo Instead of having a language-pointer in the pword,
  there should be a pointer to an object of a dedicated polymorphic
  hierarchy, which is responsible to insert a message object into
  the stream.
  \todo Most parts of the language mechanism should leave the class MessageBase
  and go to its own place; languages do not need to know the pword-index.
  A parameter to MessageBase should be a class LanguageProvider, which
  amongst others has a typevector L[n] (as SequenceLanguages now), where n
  is the number of the language. In general the languages in L[n] can stand in any derivation
  relation, only that L[0] is the root is required.
  \todo Yet languages are identified by C-strings; likely it would simplify things
  if the primary language identification mechanism (at compile time) would
  just be the index. Perhaps we can use the index from the ISBN numbers?
  \todo Each concrete message class implements a set of member functions
  (like "translate" now) which depend on two parameters (instead of just one),
  the language and the level; simple overloading enables to leave out some
  implementations, but in general it is expected that most implementations
  are there (overloading means that the compiler takes care that only "conservative"
  replacements for missing translate-implementations are possible).
  \todo For languages we allow in LanguageProvider an arbitrary type list;
  whether we can do this also for the levels --- or perhaps this is too
  complicated?
*/

/*!
  \namespace OKlib::Messages
  \brief Module for objects representing units of meaning
*/

namespace OKlib {
  namespace Messages {
  }
}

