// Oliver Kullmann, 18.12.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef XMLHANDLINGWAECHTER

#define XMLHANDLINGWAECHTER

#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

#include <OKlib/General/ErrorHandling.hpp>

namespace XMLHandling {

  // ----------------------------------------------------------
  // Writing of XML elements
  // ----------------------------------------------------------

  // Policy classes for the presentation

  struct XML_Presentation {
    static const std::string begin_token_empty() { return "<";}
    static const std::string end_token_empty() { return "/>";}
    static const std::string begin_token_left() { return "<";}
    static const std::string begin_token_right() { return ">";}
    static const std::string end_token_left() { return "</";}
    static const std::string end_token_right() { return ">";}
    static const std::string name_representation(const std::string& name) {
      return name; }
    static const std::string attribute_separator() { return " ";}
    static const std::string attribute_representation(const std::pair<std::string, std::string>& p) {
      return p.first + " = \"" + p.second + "\"";
    }
  protected :
    XML_Presentation() {}
    ~XML_Presentation() {}
  };

  struct XML_Presentation_sepvar {
    static const std::string begin_token_empty() { return "<";}
    static const std::string end_token_empty() { return "/>";}
    static const std::string begin_token_left() { return "<";}
    static const std::string begin_token_right() { return ">";}
    static const std::string end_token_left() { return "</";}
    static const std::string end_token_right() { return ">";}
    static const std::string name_representation(const std::string& name) {
      return name; }
    const std::string attribute_separator() const { return sep; }
    static const std::string attribute_representation(const std::pair<std::string, std::string>& p) {
      return p.first + " = \"" + p.second + "\"";
    }
    void set_attsep(const std::string& separator) { sep = separator; }
  protected :
    XML_Presentation_sepvar() : sep(" ") {}
    ~XML_Presentation_sepvar() {}
  private :
    std::string sep;
  };

  

  // Base class, so that empty and non-empty Elements as well as elements with different presentations can be put into a container at the same time

  class Basis {
  public :
    virtual const std::string str() const = 0;
  protected :
    Basis(const std::string element_name) :
      name_(element_name) {}
    virtual ~Basis() {}
    const std::string name() const { return name_;};
    void set_(const std::string& attribute, const std::string& value) {
      attribute_value_vector.push_back(std::make_pair(attribute, value));
    }
  private :
    const std::string name_;
    template <class> friend class Element_basis;
    std::vector< std::pair<std::string, std::string> > attribute_value_vector;
  };
  template < class Presentation >
  class Element_basis : public Presentation, public Basis {
  protected :
    Element_basis(const std::string element_name) :
      Basis(element_name) {}
    void add_attribute_value_list(std::ostream& o) const {
      if (not attribute_value_vector.empty()) {
	o <<  Presentation::attribute_separator();
	std::transform(attribute_value_vector.begin(), attribute_value_vector.end(), std::ostream_iterator<std::string>(o,  Presentation::attribute_separator().c_str()), Presentation::attribute_representation);
      }
    }
  };
  std::ostream& operator << (std::ostream& o, const Basis* const e) {
    return o << e -> str();
  }


  // The main class: Element

  enum Modifier {is_empty, is_not_empty};
  template < class Presentation, Modifier m > class Element;

  // Assignment is not possible.
  // Via the operator << it is possible to include sub-elements.
  // The sub-element-separator is always added after the starting tag, and after each sub-element.

  // ATTENTION: The copy constructor as well as operator << only take references (in form of addresses), and its the user's responsibility to ensure sufficient life time of the elements being referenced.

  template < class Presentation >
  class Element<Presentation, is_not_empty> : public Element_basis<Presentation> {

  public :

    Element(const std::string& name, const std::string& sub_element_separator = "\n") :
      Element_basis<Presentation>(name), e_sep(sub_element_separator) {}

    const std::string str() const {
      std::ostringstream result;
      result << Presentation::begin_token_left() << Presentation::name_representation(Presentation::name());
      Presentation::add_attribute_value_list(result);
      result << Presentation::begin_token_right() << e_sep;
      std::copy(sub_elements.begin(), sub_elements.end(), std::ostream_iterator<const Basis*>(result, e_sep.c_str()));
      result << Presentation::end_token_left() << Presentation::name_representation(Presentation::name()) << Presentation::end_token_right();
      return result.str();
    }

    Element& set(const std::string& attribute, const std::string& value) {
      Presentation::set_(attribute, value);
      return *this;
    }

    Element& operator << (const Basis& e) {
      sub_elements.push_back(&e);
      return *this;
    }

  private :
    
    const std::string e_sep;
    std::vector<const Basis*> sub_elements;
    Element& operator = (const Element&);
  };


  template < class Presentation >
  class Element<Presentation, is_empty> : public Element_basis<Presentation> {
  public :

    Element(const std::string& name) :
      Element_basis<Presentation>(name) {}

    const std::string str() const {
      std::ostringstream result; // Initialisierung mit einer Zeichenkette und dem Anfuegungsmodus funktioniert nicht in g++ Version 3.0.4!
      result << Presentation::begin_token_empty() << Presentation::name_representation(Presentation::name());
      Presentation::add_attribute_value_list(result);
      result << Presentation::end_token_empty();
      return result.str();
    }

    Element& set(const std::string& attribute, const std::string& value) {
      Presentation::set_(attribute, value);
      return *this;
    }

  private :
    Element& operator = (const Element&);
  };


  template < class Presentation >
  inline std::ostream& operator <<(std::ostream& o, const Element_basis<Presentation>& e) {
    return o << e.str();
  }

  typedef Element<XML_Presentation, is_not_empty> xml_e;
  typedef  Element<XML_Presentation, is_empty> xml_ee;
  typedef Element<XML_Presentation_sepvar, is_not_empty> xml_e_sep;
  typedef  Element<XML_Presentation_sepvar, is_empty> xml_ee_sep;

  /* EXAMPLES --------------------------------------------------------------

xml_e e1("abc", " ");
xml_ee e3("xyz");
cout << e1 << "\n" << e3 << "\n";

produces

<abc> </abc>
<xyz/>

then

cout << (xml_e("123").set("Feld1", "w1").set("Feld2", "w2") << xml_ee("456") << e1.set("Feld3", "JJJ").set("Feld4", "ZZZ") << xml_ee("999").set("%%%", "***")) << "\n";

produces

<123 Feld1 = "w1" Feld2 = "w2" >
<456/>
<abc Feld3 = "JJJ" Feld4 = "ZZZ" > </abc>
<999 %%% = "***" />
</123>

and now

cout << e1 << "\n";

produces

<abc Feld3 = "JJJ" Feld4 = "ZZZ" > </abc>

while

xml_e aux("&&&"); e1 << aux;
cout << e1 << "\n";

produces

<abc Feld3 = "JJJ" Feld4 = "ZZZ" > <&&&>
</&&&> </abc>

and finally

xml_e eneu(e1);
cout << eneu << "\n";

produces again

<abc Feld3 = "JJJ" Feld4 = "ZZZ" > <&&&>
</&&&> </abc>

  */
}


namespace XMLHandling {

  // ----------------------------------------------------------
  // XML declaration
  // ----------------------------------------------------------

  enum standalone_types { not_standalone = 0, standalone = 1 };

  class xml_declaration10 {
    const std::string& encoding;
    const standalone_types s;
  public :
    xml_declaration10(const standalone_types s = not_standalone, const std::string& encoding = "") : encoding(encoding), s(s) {}
    friend inline std::ostream& operator <<(std::ostream& o, const xml_declaration10& d) {
      o << "<?xml version=\"1.0\" ";
      if (not d.encoding.empty())
	o << "encoding=\"" << d.encoding << "\" ";
      o << "standalone=\"";
      if (d.s)
	o << "yes";
      else
	o << "no";
      return o << "\" ?>\n";
    }
  };

  // ----------------------------------------------------------
  // DTD declaration
  // ----------------------------------------------------------

  // Policy classes for the DTD declaration

  class dtd_access_policy_base {
  protected :
    dtd_access_policy_base() {}
    ~dtd_access_policy_base() {}
    standalone_types st() const { return not_standalone; }
    void transfer_specification(std::ostream& o) const {}
  };

  class external_dtd : protected dtd_access_policy_base {
    const std::string& ext;
  protected :
    typedef std::string parameters;
    external_dtd(const parameters& ext) : ext(ext) {}
    ~external_dtd() {}
    void transfer_specification(std::ostream& o) const {
      o << " SYSTEM \"" << ext << "\"";
    }
  };

  class internal_dtd : protected dtd_access_policy_base {
    const std::string& internal;
  protected :
    typedef std::string parameters;
    internal_dtd(const parameters& internal) : internal(internal) {}
    ~internal_dtd() {};
    standalone_types st() const { return standalone; }
    void transfer_specification(std::ostream& o) const {
      o << " [\n" << internal << "]";
    }
  };


  class encoding_policy_base {
  protected :
    encoding_policy_base() {}
    ~encoding_policy_base() {}
    std::string enc() const { return ""; }
   };

  class no_encoding : protected encoding_policy_base {
  protected :
    typedef void parameters;
    no_encoding() {}
    ~no_encoding() {}
  };

  class with_encoding : protected encoding_policy_base {
    const std::string& enc_;
  protected :
    typedef std::string parameters;
    with_encoding(const parameters& enc) : enc_(enc) {}
    ~with_encoding() {}
    const std::string& enc() const { return enc_; }
  };
    

  template < typename dtd_access_policy, typename encoding_policy >
  class dtd_declaration10 : public dtd_access_policy, public encoding_policy {
    const std::string& element;
    friend std::ostream& operator << <>(std::ostream&, const dtd_declaration10&);
  public :
    dtd_declaration10(const std::string& element, const typename dtd_access_policy::parameters& ap, const typename encoding_policy::parameters& ep) :
      dtd_access_policy(ap),
      encoding_policy(ep),
      element(element) {}
   };

  template < typename dtd_access_policy, typename encoding_policy >
  inline std::ostream& operator <<(std::ostream& o, const dtd_declaration10<dtd_access_policy, encoding_policy>& d) {
    o << xml_declaration10(d.st(), d.enc());
    o << "<!DOCTYPE " << d.element;
    d.transfer_specification(o);
    return o << ">\n";
  }

  template < typename dtd_access_policy >
  class dtd_declaration10<dtd_access_policy, no_encoding> : public dtd_access_policy, public no_encoding {
    const std::string& element;
    friend std::ostream& operator << <>(std::ostream&, const dtd_declaration10&);
  public :
    dtd_declaration10(const std::string& element, const typename dtd_access_policy::parameters& ap) :
      dtd_access_policy(ap),
      element(element) {}
};

  typedef dtd_declaration10<external_dtd, no_encoding> ext_dtd_decl;
  typedef dtd_declaration10<internal_dtd, no_encoding> int_dtd_decl;
}

#endif
