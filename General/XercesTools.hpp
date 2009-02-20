// Oliver Kullmann, 30.12.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// use with

// (g++) -I/h/21/GemeinsameBasis/C++/Xerces_include -I/h/21/GemeinsameBasis/C++/Loki

// to create the .o-file, where (10.1.2003) we have

// kullmann@csltok:~/csoliver/C++> ls -l Xerces_include
// lrwxrwxrwx    1 kullmann users          26 Jan 10 19:49 Xerces_include -> xerces-c-src2_1_0/include/

// while for the creation of the executable you need to add

// /h/21/GemeinsameBasis/C++/Xerces_so

// to the list of .o-files, where (10.1.2003) we have

// kullmann@csltok:~/csoliver/C++> ls -l Xerces_so
// lrwxrwxrwx    1 kullmann users          41 Jan 10 20:30 Xerces_so -> xerces-c-src2_1_0/lib/libxerces-c.so.21.0



#ifndef XERCESTOOLSWAECHTER

#define XERCESTOOLSWAECHTER

#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include <ostream>
#include <istream>
#include <iterator>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>

#include <Singleton.h>

#include <OKlib/General/FunctionHandling.hpp>
#include <OKlib/General/ErrorHandling.hpp>

namespace XercesTools {

  struct Error : ErrorHandling::Error {
    Error(const std::string& what) : ErrorHandling::Error(what) {}
    void add(const std::string& s) { what_ += s; }
  };
  struct missing_required_attribute : Error {
    missing_required_attribute(const std::string& what) : Error(what) {}
  };
  struct checked_conversion_failed : Error {
    checked_conversion_failed(const std::string& what) : Error(what) {}
  };

}

namespace XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  // Tools for xml-string handling

  inline const std::string x2s (const XMLCh* const X) {
    const char * const Z = xercesc::XMLString::transcode(X);
    std::string E(Z);
    delete [] Z;
    return E;
  }

  inline void write_xml(std::ostream& target, const XMLCh* const s) {
    struct transformation {
      const char * const p;
      transformation(const XMLCh* const s) : p(xercesc::XMLString::transcode(s)) {}
      ~transformation() { delete [] p; }
    };
    transformation transformation(s);
    target << transformation.p;
   }

  inline std::ostream& operator << (std::ostream& S, const XMLCh* const X) {
    write_xml(S, X);
    return S;
  }

}

// --------------------------------------------------------------------------

// Reading content and attributes of elements into variables
// Motivated by [Davis Nash: Combining STL with SAX and XPath for
//   Effective XML Parsing; CUJ January 2003, pages 6-12].

// --------------------------------------------------------------------------


namespace  XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  // Wrapper for objects presenting xml-elements and attributes ------------

  class Wrapper {
  public :
    virtual void put(const std::string& s) const = 0;
    virtual ~Wrapper() {}
  };

  template <typename T>
  class WrapperData : public Wrapper {
  public :
    WrapperData(T& item) : ref_(item) {}
    virtual void put(const std::string& s) const {
      std::istringstream stream(s);
      stream >> ref_;
    }
  private :
    T& ref_;
  };

  // Special element types --------------------------------------------------
  // (used by the application)

  class full_string {
    std::ostringstream o;
  public :
    const std::string str() const { return o.str(); }
    operator const std::string() const { return o.str(); }
    operator const char*() const { return o.str().c_str(); }
    friend std::istream& operator >> (std::istream&, full_string&);
  };
  std::istream& operator >> (std::istream& i, full_string& fs) {
    return i >> fs.o.rdbuf();
    // default: i with skipws (skipping leading whitespace)
  }
  class full_string_co { // can be copied
    std::string s;
  public :
    full_string_co() {}
    full_string_co(const std::string s) : s(s) {}
    operator const std::string() const { return s; }
    friend std::istream& operator >> (std::istream& i, full_string_co& fs) {
      std::ostringstream o;
      i >> o.rdbuf();
      fs.s = o.str();
      return i;
    }
  };

  template <typename T>
  class checked {
    T t;
  public :
    checked() {}
    checked(const T& t) : t(t) {}
    operator const T() const { return t; }
    template <typename>
    friend std::istream& operator >> (std::istream&, checked<T>&);
  };
  template <typename T>
  std::istream& operator >> (std::istream& i, checked<T>& t) {
    i >> t.t;
    // default: i with skipws (skipping leading whitespace)
    if (not i) {
      if (i.bad())
	throw checked_conversion_failed(std::string("conversion failed: badbid is set; target type: ") + typeid(T).name());
      else if (i.eof())
	throw checked_conversion_failed(std::string("conversion failed: eofbid is set; target type: ") + typeid(T).name());
    else
      throw checked_conversion_failed(std::string("conversion failed: failbid is set; target type: ") + typeid(T).name());
    }
    else if (i.peek() != std::istream::traits_type::eof()) {
      std::ostringstream remain;
      i >> remain.rdbuf();
      throw checked_conversion_failed("remaining characters: " + remain.str() + "; target type: " + typeid(T).name());
    }
  }

  template <typename T, template <typename> class Container >
  struct ElementStack {
    typedef Container<T> ContainerType;
    ContainerType C;
    friend std::istream& operator >> <> (std::istream&, ElementStack&);
  };
  template <typename T, template <typename> class Container >
  std::istream& operator >> (std::istream& is, ElementStack<T,Container>& st) {
    st.C.push_back(T());
    typename ElementStack<T,Container>::ContainerType::iterator i = st.C.end(); --i;
    is >> *i;
    return is;
  }


  // Containers for elements and attributes --------------------------------

  class ElementMap {
  protected :
    ElementMap() {}
    ~ElementMap() {
      std::for_each(element_map.begin(), element_map.end(), FunctionHandling::DeleteObjectSecond());
    }

    template <typename T>
    void add(const std::string& name, T& ref) {
      element_map.insert(std::make_pair(name, new WrapperData<T>(ref)));
    }
    
    typedef std::map<std::string, Wrapper*> element_map_t;
    element_map_t element_map;
  };

  enum Attribute_types { required, implied };
  class AttributeMap {
  protected :
    AttributeMap() {}
    ~AttributeMap() {
      for (attribute_map_t::const_iterator i = attribute_map.begin(); i != attribute_map.end(); ++i) {
	const attribute_vector_2& p = i -> second;
	std::for_each(p.first.begin(), p.first.end(), FunctionHandling::DeleteObjectSecond());
	std::for_each(p.second.begin(), p.second.end(), FunctionHandling::DeleteObjectSecond());
      }
    }

    template <typename T>
    void add(const std::string& element_name, const std::string& attribute_name, Attribute_types t, T& ref) {
      attribute_map_t::iterator i = attribute_map.lower_bound(element_name);
      if (i == attribute_map.end() or i -> first != element_name)
	i = attribute_map.insert(i, std::make_pair(element_name, attribute_vector_2()));
      switch (t) {
      case required : i -> second.first.push_back(std::make_pair(attribute_name, new WrapperData<T>(ref))); break;
      case implied : i -> second.second.push_back(std::make_pair(attribute_name, new WrapperData<T>(ref))); break;
      }
    }

    typedef std::vector<std::pair<std::string, Wrapper*> > attribute_vector;
    typedef std::pair<attribute_vector, attribute_vector> attribute_vector_2;
    typedef std::map<std::string, attribute_vector_2> attribute_map_t;
    attribute_map_t attribute_map;
  };


  // Policy class for element and attribute handling -----------------------

  class SimpleElementAttributeHandling : private ElementMap, private AttributeMap {
  public :
    /* not with g++ Version 3.2.1  (error):
    using ElementMap::add;
    using AttributeMap::add;
    */
    // Workaround:
    template <typename T>
    void add(const std::string& name, T& ref) {
       ElementMap::add(name, ref);
     }
    template <typename T>
    void add(const std::string& element_name, const std::string& attribute_name, Attribute_types t, T& ref) {
      AttributeMap::add(element_name, attribute_name, t, ref);
    }
    
  protected :
    SimpleElementAttributeHandling() : text_activated(false) {}
    ~SimpleElementAttributeHandling() {}

    void startElement_(const XMLCh* const name_, xercesc::AttributeList& atts) {
      const std::string& name = x2s(name_);
      if (not text_activated) {
	if (element_map.find(name) != element_map.end())
	  text_activated = true;
      }
      else
	current_text.str("");
      const attribute_map_t::const_iterator i = attribute_map.find(name);
      if (i != attribute_map.end()) {
	for (attribute_vector::const_iterator j = i -> second.first.begin(); j != i -> second.first.end(); ++j) {
	  // filling of required attributes
	  const std::string& att_name(j -> first);
	  const XMLCh* ap = atts.getValue(att_name.c_str());
	  if (ap == 0)
	    throw missing_required_attribute(att_name);
	  try {
	    j -> second -> put(x2s(ap));
	  }
	  catch (Error& e) {
	    e.add("\nElement = " + name + "; Attribute = " + att_name);
	    throw e;
	  }
	}
	for (attribute_vector::const_iterator j = i -> second.second.begin(); j != i -> second.second.end(); ++j) {
	  // filling of implied attributes
	  const std::string& att_name(j -> first);
	  if (const XMLCh* ap = atts.getValue(att_name.c_str())) {
	    try {
	      j -> second -> put(x2s(ap));
	    }
	    catch (Error& e) {
	      e.add("\nElement = " + name + "; Attribute = " + att_name);
	      throw e;
	    }
	  }
	}
      }
    }
    void endElement_(const XMLCh* const name_) {
      if (not text_activated) return;
      const std::string& name = x2s(name_);
      const element_map_t::const_iterator i = element_map.find(name);
      if (i != element_map.end()) {
	try {
	  i -> second -> put(current_text.str());
	}
	catch (Error& e) {
	  e.add("\nElement = " + name);
	  throw e;
	}
      }
      current_text.str("");
      text_activated = false;
    }

    void characters_(const XMLCh* const text, const unsigned int length) {
      if (text_activated)
	write_xml(current_text, text);
      // is it safe to ignore length?!
    }

  private :
    bool text_activated;
    std::ostringstream current_text;
  };



  // The handler class for extraction of xml-elements and attributes --------

  template <class ElementAttributeHandler>
  class Unique_content_extraction_handler : public xercesc::HandlerBase, public ElementAttributeHandler {
  public :
    Unique_content_extraction_handler() {}

    void startElement(const XMLCh* const name, xercesc::AttributeList& atts) {
      ElementAttributeHandler::startElement_(name, atts);
     }
    void endElement(const XMLCh* const name) {
      ElementAttributeHandler::endElement_(name);
    }
    void characters(const XMLCh* const text, const unsigned int length) {
      ElementAttributeHandler::characters_(text, length);
    }
  };

  
  typedef Unique_content_extraction_handler<SimpleElementAttributeHandling> SUCEH;

}

// --------------------------------------------------------------------------

// END OF IMPLEMENTATION
// Reading content and attributes of elements into variables

// EXAMPLE for USAGE

/*

  try {
    SUCEH E;
    full_string Person; E.add("Person", Person);
    checked<int> Age; E.add("Person", "Age", required, Age);
    string PName; E.add("Person", "Name", implied, PName);
    // ATTENTION: after skipping leading whitespaces, "string" reads
    // only until the first next whitespace is found!
    string a; E.add("a", a);
    full_string File; E.add("File", File);
    string Filename; E.add("File", "Name", required, Filename);

    SimpleParserWrapper P(&E); // see below
    P.parse(filename);

    cout << "Person = " << Person << "|\n";
    cout << "Number characters = " << Person.str().size() << "\n";
    cout << "Age = " << Age << "\n";
    cout << "Name = " << PName << "\n";
    cout << "a = " << a << "\n";
    cout << "File = " << File << "\n";
    cout << "Filename = " << Filename << "\n";
   }

  catch (const ErrorHandling::Error& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const XMLException& e) {
    cerr << XMLError2string(e) << "\n";
    return 1;
  }
  catch (const SAXException& e) {
    cerr << SAXError2string(e) << "\n";
    return 1;
  }

  InitTermSingleton::Instance().terminate();
 

*/

// Now an input

/*

<a> q <Person Age = " 6" Name = " Martina " > jj j </Person> zzz <File Name = "lll" > uio </File> ooo </a>

*/

// leads to the output

/*
Person = jj j |
Number characters = 5
Age = 6
Name = Martina
a =
File = uio
Filename = lll

*/

// --------------------------------------------------------------------------



namespace  XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  // Simple error handler

  class SimpleErrorHandler : public xercesc::HandlerBase {
    void error(const xercesc::SAXParseException& f) { throw f; }
    // for non-fatal errors; in case of fatal errors a SAXException
    // is thrown
    void warning(const xercesc::SAXParseException& f) { throw f; }
  };

}

namespace  XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  // Representation of exceptions from the Xerces library

  namespace ErrorMessages {

    const int AnzahlSprachen = 2;
    const char * const Messages [][AnzahlSprachen] = {
      {"Fehler abgeleitet von XMLException:", // 0
       "Error derived from XMLException:"},
      {"Typ der Ausnahme = ", // 1
       "type of exception = "},
      {"Meldung = ", // 2
       "message = "},
      {"Dateiname = ", // 3
       "source file = "},
      {"Zeilennummer = ", // 4
       "line number = "},
      {"Fehler abgeleitet von SAXException:", // 5
       "Error derived from SAXException:"},
      {"Fehler abgeleitet von SAXParseException:", // 6
       "Error derived from SAXParseException:"},
      {"Spaltennummer = ", // 7
       "column number = "},
    };

  }

  inline const std::string XMLError2string(const xercesc::XMLException& e, MessageHandling::Sprachen L = MessageHandling::English) {
    MessageHandling::Messages<ErrorMessages::AnzahlSprachen> M(ErrorMessages::Messages, L);
    std::ostringstream s;
    s << M(0) << " " << M(1) << e.getType() << "; " << M(2) << e.getMessage() << "; " << M(3) << e.getSrcFile() << "; " << M(4) << e.getSrcLine();
    return s.str();
  }

  inline const std::string SAXError2string(const xercesc::SAXException& e, MessageHandling::Sprachen L = MessageHandling::English) {
    MessageHandling::Messages<ErrorMessages::AnzahlSprachen> M(ErrorMessages::Messages, L);
    std::ostringstream s;
    try {
      
      const xercesc::SAXParseException& e2 = dynamic_cast<const xercesc::SAXParseException&>(e);
      s << M(6) << " " << M(2) << e2.getMessage() << "; " << M(3) << e2.getSystemId() << "; " << M(4) << e2.getLineNumber() << "; " << M(7) << e2.getColumnNumber();
    return s.str();
    }
    catch(...) { // not derived from SAXParseException
      s << M(5) << " " << M(2) << e.getMessage();
      return s.str();
    }
  }
}

namespace  XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

  // Initialisation

  class Inititialisation_and_Termination {

    bool active_;
    // if false, then either XMLPlatformUtils::Initialize() has never been
    // called, or after its last call XMLPlatformUtils::Terminate() has
    // been called
    Inititialisation_and_Termination() : active_(false) {}

    Inititialisation_and_Termination(const Inititialisation_and_Termination&);
    Inititialisation_and_Termination& operator = (const Inititialisation_and_Termination&);
    Inititialisation_and_Termination* operator &();

    ~Inititialisation_and_Termination() {}

    friend class Loki::CreateStatic<Inititialisation_and_Termination>;

  public :
    
    bool active() const { return active_; }
    void initialise() {
      if (not active_) {
        xercesc::XMLPlatformUtils::Initialize();
	active_ = true;
      }
    }
    void terminate() {
      if (active_) {
        xercesc::XMLPlatformUtils::Terminate();
	active_ = false;
      }
    }
  };

  typedef Loki::SingletonHolder<Inititialisation_and_Termination, Loki::CreateStatic> InitTermSingleton;

  class InitBasis {
  protected:
    InitBasis() { InitTermSingleton::Instance().initialise(); }
  };

}

namespace  XercesTools { 
  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------

      
  // A parser wrapper

  class SimpleParserWrapper : private InitBasis {
  public :
    SimpleParserWrapper(xercesc::DocumentHandler * const DH) {
      parser.setValidationScheme(xercesc::SAXParser::Val_Auto);
      parser.setErrorHandler(&errhandler);
      parser.setDocumentHandler(DH);
    }
    void parse(const std::string& filename) {
      parser.parse(filename.c_str());
    }
    // throws:
    // XMLException (from Xerces)
    // XercesTools::Error

  private :
    SimpleErrorHandler errhandler;
    xercesc::SAXParser parser;
  };

}

#endif
