// Oliver Kullmann, 21.2.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/Experimental/AllgKlassen200203/Variables.hpp
  \brief First approaches for models of concept Variables in the framework of the library
  \deprecated To be moved (if appropriate) to OKlib.
*/


#ifndef VARIABLESWAECHTER_ahsd3549FG

#define VARIABLESWAECHTER_ahsd3549FG

#include <string>
#include <map>
#include <vector>
#include <ostream>
#include <limits>
#include <cstddef>
#include <algorithm>
#include <cassert>
#include <iterator>

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>

#include <OKlib/General/ErrorHandling.hpp>
#include <OKlib/General/StringHandling.hpp>
#include <OKlib/General/FunctionHandling.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Auxiliary.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Traits_General.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Concepts_Variables.hpp>
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Traits_Variables.hpp>

// Only temporary, to still compile the old implementations:
#include <OKlib/OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp>


namespace Variables {

  // -----------------------------------------------------------------
  // Implementation of variables as integers
  // -----------------------------------------------------------------

  template <typename Int>
  class VariablesAsIntegers {

    BOOST_STATIC_ASSERT(std::numeric_limits<Int>::is_integer);
    BOOST_STATIC_ASSERT(std::numeric_limits<Int>::is_signed);

    Int v;
    template <typename, typename>
    friend class VariablesAsIntegers_DomainWithNameAdministration;
    template <typename>
    friend class Iterator_VariablesAsIntegers;
    explicit VariablesAsIntegers(Int i) : v(i) {
      assert(i >= 0);
    }

  public :

    VariablesAsIntegers() : v(0) {}

    friend inline bool operator ==(const VariablesAsIntegers lhs, const VariablesAsIntegers rhs) { return lhs.v == rhs.v; }
    friend inline bool operator <(const VariablesAsIntegers lhs, const VariablesAsIntegers rhs) { return lhs.v < rhs.v; }
 
    bool null() const { return v == 0; }
  };

  template <typename Int>
  inline bool operator !=(const VariablesAsIntegers<Int> lhs, const VariablesAsIntegers<Int> rhs) { return not (lhs == rhs); }
  template <typename Int>
  inline bool operator >(const VariablesAsIntegers<Int> lhs, const VariablesAsIntegers<Int> rhs) { return rhs < lhs; }
  template <typename Int>
  inline bool operator <=(const VariablesAsIntegers<Int> lhs, const VariablesAsIntegers<Int> rhs) { return lhs < rhs or lhs == rhs; }
  template <typename Int>
  inline bool operator >=(const VariablesAsIntegers<Int> lhs, const VariablesAsIntegers<Int> rhs) { return lhs > rhs or lhs == rhs; }

  // -----------------------------------------------------------------
  // Determining the domain of safe arithmetical negation
  // -----------------------------------------------------------------

  template <typename Int>
  Int signed_max() {
    const Int max = std::numeric_limits<Int>::max();
    const Int min = std::numeric_limits<Int>::min();
    return (min + max <= 0) ? max : - min;
  }
  // x of type Int can be safely negated iff
  // - signed_max() <= x <= signed_max().

  // -----------------------------------------------------------------
  // Associated iterator type
  // -----------------------------------------------------------------

  template <typename Int>
  class Iterator_VariablesAsIntegers {
    Int i;
    typedef VariablesAsIntegers<Int> Var;
    template <typename, typename>
    friend class VariablesAsIntegers_DomainWithNameAdministration;
    explicit Iterator_VariablesAsIntegers(Int i) : i(i) {}

  public :
    Iterator_VariablesAsIntegers() : i(0) {}
    Iterator_VariablesAsIntegers(Var v) : i(v.v) {}
    
    typedef Var value_type;
    typedef Var reference;
    typedef Var* pointer;
    typedef Int difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    friend inline bool operator ==(Iterator_VariablesAsIntegers lhs, Iterator_VariablesAsIntegers rhs) { return lhs.i == rhs.i; }

    Var operator *() const { return Var(i); }

    Iterator_VariablesAsIntegers& operator ++() {
      assert(i < signed_max<Int>() - 1);
      ++i; return *this;
    }
    Iterator_VariablesAsIntegers operator ++(int) {
      assert(i < signed_max<Int>() - 1);
      const Iterator_VariablesAsIntegers j = *this; ++i; return j;
    }
    Iterator_VariablesAsIntegers& operator --() {
      assert(i > 0);
      --i; return *this;
    }
    Iterator_VariablesAsIntegers operator --(int) {
      assert(i > 0);
      const Iterator_VariablesAsIntegers j = *this; --i; return j;
    }
  };

  template <typename Int>
  inline bool operator !=(Iterator_VariablesAsIntegers<Int> lhs, Iterator_VariablesAsIntegers<Int> rhs) { return not (lhs == rhs); }


}

namespace Traits_General {

  template <typename Int>
  struct MetaData<Variables::VariablesAsIntegers<Int> > {
    typedef Concepts_Variables::Variable_tag concept_tag;
  };

}


namespace Traits_General {
  namespace Basis_VariablesAsIntegers_DomainWithNameAdministration {
    template <typename Int, typename Name>
    class Basis_Traits;
  }
}

namespace Variables {

  // -----------------------------------------------------------------
  // Signed size type template
  // -----------------------------------------------------------------

  template <typename Integer>
  class Size_type_signed {
    Integer x;
  public :
    Size_type_signed() : x(0) {}

    friend inline bool operator ==(Size_type_signed lhs, Size_type_signed rhs) { return lhs.x == rhs.x; }
    friend inline bool operator <(Size_type_signed lhs, Size_type_signed rhs) { return lhs.x < rhs.x; }

    friend inline Size_type_signed operator -(Size_type_signed lhs, Size_type_signed rhs) {
      assert(lhs >= Size_type_signed() and rhs >= Size_type_signed());
      if (rhs >= lhs) return Size_type_signed();
      else
	return Size_type_signed(lhs.x - rhs.x);
    }

  private :

    template <typename, class>
    friend class VariablesAsIntegers_DomainWithNameAdministration;

    explicit Size_type_signed(Integer x) : x(x) { assert(x >= 0); }
  };

  template <typename Integer>
  inline bool operator !=(Size_type_signed<Integer> lhs, Size_type_signed<Integer> rhs) {
    return not (lhs == rhs);
  }
  template <typename Integer>
  inline bool operator <=(Size_type_signed<Integer> lhs, Size_type_signed<Integer> rhs) {
    return lhs < rhs or lhs == rhs;
  }
  template <typename Integer>
  inline bool operator >(Size_type_signed<Integer> lhs, Size_type_signed<Integer> rhs) {
    return rhs < lhs;
  }
  template <typename Integer>
  inline bool operator >=(Size_type_signed<Integer> lhs, Size_type_signed<Integer> rhs) {
    return rhs < lhs or lhs == rhs;
  }

  // -----------------------------------------------------------------
  // Domain for variables as integers
  // -----------------------------------------------------------------

  template <typename Int, typename Name>
  class VariablesAsIntegers_DomainWithNameAdministration {

    BOOST_STATIC_ASSERT(std::numeric_limits<Int>::is_integer);
    BOOST_STATIC_ASSERT(std::numeric_limits<Int>::is_signed);

    friend class ::Traits_General::Basis_VariablesAsIntegers_DomainWithNameAdministration::Basis_Traits<Int, Name>;

    Int next; // the index of the next variable

    typedef Name name_type;
    typedef VariablesAsIntegers<Int> Var;
    typedef Size_type_signed<Int> size_type;

    static Int max() { return signed_max<Int>(); }

  public :

    VariablesAsIntegers_DomainWithNameAdministration() : next(1) {}

    Var operator()(const name_type& name) {
      return Var(insert(name));
    }
    const name_type& name(Var v) const {
      return na.hash_vector[v.v] -> first;
    }

    static size_type max_size() { return size_type(max()); }
    static size_type size_type_cast(unsigned int ui) {
      if (ui <= max())
	return size_type(Int(ui));
      else
	throw Error_Variables::CapacityOverflow("VariablesAsIntegers_DomainWithNameAdministration::size_type_cast : " + StringHandling::toString_nc(ui));
    }
    void reserve(size_type r) {
      if (r.x > na.name_map.max_size() or r.x > na.hash_vector.max_size())
	throw Error_Variables::CapacityOverflow("VariablesAsIntegers_DomainWithNameAdministration::reserve : " + StringHandling::toString_nc(r.x));
      else
	na.hash_vector.reserve(r.x);
    }
    size_type capacity() const {
      const typename HashVectorType::size_type c = na.hash_vector.capacity();
      if (c >= max())
	return max_size();
      else
	return size_type(Int(c));
    }
    void increase_capacity(size_type s) {
      if (s.x <= 0)
	return;
      if (s.x > max() or na.hash_vector.capacity() > max() - s.x)
	throw Error_Variables::CapacityOverflow("VariablesAsIntegers_DomainWithNameAdministration::increase_capacity : " + StringHandling::toString_nc(s.x));
      else
	reserve(size_type(Int(na.hash_vector.capacity()) + s.x));
    }
    size_type size() const {
      assert(next >= 1); return size_type(next);
    }
    size_type n() const {
      assert(next >= 1); return size_type(next-1);
    }

  public :

    typedef Iterator_VariablesAsIntegers<Int> iterator;

    iterator begin() const {
      return iterator(0);
    }
    iterator first() const {
      return iterator(1);
    }
    iterator end() const {
      return iterator(next);
    }

    iterator find(const name_type& name) const {
      const MapIterator i = na.name_map.find(name);
      if (i == na.name_map.end())
	return end();
      else
	return iterator(i -> second);
    }

  private :
    typedef typename std::map<name_type, Int> MapType;
    // given a name, get the index
    typedef typename MapType::const_iterator MapIterator;
    typedef typename std::vector<MapIterator> HashVectorType;
    // given an index, get the iterator to <name, index>
    
    struct NameAdmin {
      MapType name_map;
      HashVectorType hash_vector;
      NameAdmin() {
	create_null_var();
      }
    private :
      void create_null_var() {
	name_map.insert(std::make_pair(name_type(), 0));
	hash_vector.push_back(name_map.begin());
      }
    };
    
    NameAdmin na;
     
    Int insert(const name_type& name) {
      typename MapType::iterator i = na.name_map.lower_bound(name);
      if (i != na.name_map.end() and i -> first == name) // old
	return i -> second;
      else { // new
	if (next == max() - 1)
	  throw Error_Variables::CapacityOverflow("VariablesAsIntegers_DomainWithNameAdministration::insert : " + StringHandling::toString_nc(next));
	{
	  i = na.name_map.insert(i, std::make_pair(name, next));
	  na.hash_vector.push_back(i);
	  // not exception safe, if the insert succeeds, but not the push_back
	}
	return next++;
      }
      
    };

  };

}

namespace Traits_General {

  namespace Basis_VariablesAsIntegers_DomainWithNameAdministration {
    template <typename Int, class Name>
    struct Basis_Traits {
      typedef Concepts_Variables::VariableDomainWithIteratorAndAllocation_tag concept_tag;
      
      typedef Traits_Variables::ThrowsTotalCapacityOverflow overflow_throw_property;
      typedef Traits_Variables::NoInvalidNames invalid_name_throw_property;
      typedef Traits_Variables::LocalDomainsIdentified comparison_property;
      typedef Traits_Variables::SafeAllocation allocation_property;
      typedef Traits_Variables::GeneralNames name_property;
      typedef Traits_Variables::OrderByCreation order_property;
      typedef Traits_Variables::NaturalSize size_property;

      typedef Variables::VariablesAsIntegers_DomainWithNameAdministration<Int, Name> VarD;
      typedef typename VarD::name_type name_type;
      typedef typename VarD::Var Var;
      typedef typename VarD::size_type size_type;
      typedef typename VarD::iterator iterator;
    private :
      ~Basis_Traits();
    };
  }
  template <typename Int, class Name>
  struct MetaData<Variables::VariablesAsIntegers_DomainWithNameAdministration<Int, Name> > : Basis_VariablesAsIntegers_DomainWithNameAdministration::Basis_Traits<Int, Name> {
  private :
    ~MetaData();
  };

  template <typename Int>
  struct MetaData<Variables::VariablesAsIntegers_DomainWithNameAdministration<Int, std::string> > : Basis_VariablesAsIntegers_DomainWithNameAdministration::Basis_Traits<Int, std::string> {
    typedef Traits_Variables::StringConvertibleName name_property;
  private :
    ~MetaData();
  };

}


// --------------------------------------------------------------------
// DEPRECATED :
// --------------------------------------------------------------------

namespace Literals {
  template <typename Index, template <typename> class InfoPolicyVar, template <typename> class InfoPolicyLit, typename Name>
  class LiteralsAsIntegers;
}


namespace Variables {

  struct Error_Variables : ErrorHandling::Error {
    Error_Variables(const std::string& what) : ErrorHandling::Error(what) {}
  };

  struct Overflow_Variables : Error_Variables {
    Overflow_Variables(const std::string& what) : Error_Variables(what) {}
  };

}

namespace Variables {

  template <class InfoPolicy, typename Name, class LiteralLink>
  class VariablesAsIndices : public InfoPolicy {

    BOOST_CLASS_REQUIRE(InfoPolicy, ConceptDefinitions, VariableIndexInfoPolicy_concept);
    BOOST_CLASS_REQUIRE(Name, boost, DefaultConstructibleConcept);
    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, OutputStreamableConcept);

    typedef typename InfoPolicy::Index Index;
    Index index;
    enum dummy { avoid_confusion };
    explicit VariablesAsIndices(Index i, dummy) : index(i) {}

  public :

    typedef typename InfoPolicy::DeliveredConcept Concept;
    typedef Name NameType;

    VariablesAsIndices() : index(0) {}
    explicit VariablesAsIndices(const NameType& name) : index(insert(name)) {}
    VariablesAsIndices(const VariablesAsIndices& v) : index(v.index) {}

    VariablesAsIndices& operator =(const VariablesAsIndices& v) {
      index = v.index;
      return *this;
    }

    friend inline bool operator ==(VariablesAsIndices lhs, VariablesAsIndices rhs) { return lhs.index == rhs.index; }
    friend inline bool operator !=(VariablesAsIndices lhs, VariablesAsIndices rhs) { return not (lhs == rhs); }
    friend inline bool operator <(VariablesAsIndices lhs, VariablesAsIndices rhs) { return lhs.index < rhs.index; }
    friend inline bool operator >(VariablesAsIndices lhs, VariablesAsIndices rhs) { return rhs < lhs; }
    friend inline bool operator <=(VariablesAsIndices lhs, VariablesAsIndices rhs) { return lhs < rhs or lhs == rhs; }
    friend inline bool operator >=(VariablesAsIndices lhs, VariablesAsIndices rhs) { return lhs > rhs or lhs == rhs; }

    bool null() const { return index == 0; }

    const NameType& name() const {
      return na.hash_vector[index] -> first;
    }

    friend inline std::ostream& operator <<(std::ostream& o, VariablesAsIndices v) {
      if (v.null())
	return o << Auxiliary::null_variable_tag;
      else
	return o << v.name();
    }

    static void clear() {
      na.clear();
      InfoPolicy::clear_info();
      LiteralLink::clear();
      N = 1;
      assert(na.name_map.size() == 1 and na.hash_vector.size() == 1 and size() == 1);
    }

    typedef Index size_type;

    static void reserve(size_type max) {
      na.hash_vector.reserve(max);
      reserve_info(max);
      LiteralLink::reserve(max);
    }

    static size_type capacity() {
      assert(na.hash_vector.capacity() >= 1 and LiteralLink::capacity() >= 1);
      return (na.hash_vector.capacity() < LiteralLink::capacity()) ? na.hash_vector.capacity() : LiteralLink::capacity();
    }

    static size_type size() {
      assert(N == na.hash_vector.size() and N == na.name_map.size());
      return N;
    }
    static size_type n() {
      assert(N == na.hash_vector.size() and N == na.name_map.size());
      return N-1;
    }

    typedef typename InfoPolicy::InfoValueType value_type;
    typedef typename InfoPolicy::InfoReferenceType reference;
    typedef typename InfoPolicy::InfoPointerType pointer;
    typedef size_type difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    reference operator *() const {
      assert(index >= 0 and index < N);
      return get_info(index);
    }
    VariablesAsIndices& operator ++() {
      assert(index != N);
      ++index;
      return *this;
    }
    VariablesAsIndices operator ++(int) {
      assert(index != N);
      Index i = index;
      ++index;
      return VariablesAsIndices(i, avoid_confusion);
    }
    VariablesAsIndices& operator --() {
      assert(index != 0);
      --index;
      return *this;
    }
    VariablesAsIndices operator --(int) {
      assert(index != 0);
      Index i = index;
      --index;
      return VariablesAsIndices(i, avoid_confusion);
    }

    pointer operator ->() const {
      assert(index < N);
      return get_info_pointer(index);
    }
    
    static VariablesAsIndices begin() {
      return VariablesAsIndices(0, avoid_confusion);
    }
    static VariablesAsIndices end() {
      return VariablesAsIndices(N, avoid_confusion);
    }

    static VariablesAsIndices find(const NameType& name) {
      const MapIterator i = na.name_map.find(name);
      if (i == na.name_map.end())
	return end();
      else
	return VariablesAsIndices(i -> second, avoid_confusion);
    }

  private :

    typedef typename std::map< NameType, Index > MapType;
    // given a name, get the index
    typedef typename MapType::const_iterator MapIterator;
    typedef typename std::vector<MapIterator> HashVectorType;
    // given an index, get the iterator to <name, index>

    struct NameAdmin {
      MapType name_map;
      HashVectorType hash_vector;
      NameAdmin() {
	create_null_var();
      }
      void clear() {
	name_map.clear();
	hash_vector.clear();
	create_null_var();
      }
    private :
      void create_null_var() {
	name_map.insert(std::make_pair(NameType(), 0));
	hash_vector.push_back(name_map.begin());
      }
    };

    static NameAdmin na;
    static Index N;

    Index insert(const NameType& name) {
      typename MapType::iterator i = na.name_map.lower_bound(name);
      if (i != na.name_map.end() and i -> first == name) // old
	return i -> second;
      else { // new
	if (N == std::numeric_limits<Index>::max())
	  throw Overflow_Variables("VariablesAsIndices::insert : " + StringHandling::toString_nc(N));
        InfoPolicy::new_info();
	{
	  i = na.name_map.insert(i, std::make_pair(name, N));
	  na.hash_vector.push_back(i);
	  LiteralLink::new_variable();
	} // EXCEPTION SAFETY ??
	return N++;
      }
    }

    template <typename, template <typename> class, template <typename> class, typename>
    friend class Literals::LiteralsAsIntegers;

  };
  template <typename Info, typename NameType, typename LiteralLink>
  typename VariablesAsIndices<Info, NameType, LiteralLink>::NameAdmin VariablesAsIndices<Info, NameType, LiteralLink>::na;

  template <typename Info, typename NameType, typename LiteralLink>
  typename Info::Index VariablesAsIndices<Info, NameType, LiteralLink>::N(1);


  // -------------------
  // Info policy classes
  // -------------------

  template <typename InfoType, typename IndexType>
  class InfoPolicyIndexVector {
  public :
    typedef ConceptDefinitions::VariableIndexInfoPolicy_tag Concept;
  protected :
    typedef ConceptDefinitions::VariableWithInfo_tag DeliveredConcept;
    typedef InfoType InfoValueType;
    typedef InfoType& InfoReferenceType;
    typedef InfoType* InfoPointerType;
    typedef IndexType Index;
  private :
    typedef typename std::vector<InfoValueType> InfoVectorType;
    static InfoVectorType info_vector;
  protected :
    ~InfoPolicyIndexVector() {}
    static void new_info() {
      info_vector.push_back(InfoValueType());
    }
    static void reserve_info(Index max) {
      info_vector.reserve(max);
    }
    static InfoReferenceType get_info(Index i) {
      return info_vector[i];
    }
    static InfoPointerType get_info_pointer(Index i) {
      return &info_vector[i];
    }
    static void clear_info() {
      info_vector.clear();
      new_info();
    }
  };
  template <typename Info, typename Index>
  typename InfoPolicyIndexVector<Info,Index>::InfoVectorType InfoPolicyIndexVector<Info,Index>::info_vector(1, Info());

  template <typename IndexType>
  class InfoPolicyEmpty {
  public :
    typedef ConceptDefinitions::VariableIndexInfoPolicy_tag Concept;
  protected :
    typedef ConceptDefinitions::VariableWithCounting_tag DeliveredConcept;
    typedef IndexType Index;
    ~InfoPolicyEmpty() {}
    static void new_info() {}
    static void reserve_info(Index max) {}
    class Dummy {};
    typedef Dummy InfoReferenceType;
    typedef Dummy InfoValueType;
    typedef Dummy InfoPointerType;
    // get_info not implemented
    static void clear_info() {}
  };

  // Policy class in case, literals don't provide info containers
  template <typename Index>
  struct EmptyLiteralLink {
    static void new_variable() {}
    static void reserve(Index max) {}
    static Index capacity(){ return std::numeric_limits<Index>::max(); }
    static void clear() {}
  };

}

namespace Variables {

  // -----------------------------------------------------------------
  // Implementation of variables as names
  // -----------------------------------------------------------------
  
  template <class Name>
  class VariablesAsNames {

    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, FullyConstructibleConcept);
    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, TotalOrderComparableConcept);
    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, OutputStreamableConcept);

    Name name_;

  public :

    typedef typename ConceptDefinitions::Variable_tag Concept;
    typedef Name NameType;

    VariablesAsNames() {}
    VariablesAsNames(const NameType& name) : name_(name) {}
    VariablesAsNames(const VariablesAsNames& v) : name_(v.name_) {}

    VariablesAsNames& operator =(const VariablesAsNames& v) {
      name_ = v.name_;
      return *this;
    }
    
    friend inline bool operator ==(VariablesAsNames lhs, VariablesAsNames rhs) { return lhs.name_ == rhs.name_; }
    friend inline bool operator !=(VariablesAsNames lhs, VariablesAsNames rhs) { return not (lhs == rhs); }
    friend inline bool operator <(VariablesAsNames lhs, VariablesAsNames rhs) { return lhs.name_ < rhs.name_; }
    friend inline bool operator >(VariablesAsNames lhs, VariablesAsNames rhs) { return rhs < lhs; }
    friend inline bool operator <=(VariablesAsNames lhs, VariablesAsNames rhs) { return lhs < rhs or lhs == rhs; }
    friend inline bool operator >=(VariablesAsNames lhs, VariablesAsNames rhs) { return lhs > rhs or lhs == rhs; }

    static void clear() {}
    bool null() const { return name_ == NameType(); }
    const NameType& name() const { return name_; }
    
    friend inline std::ostream& operator <<(std::ostream& o, VariablesAsNames v) {
      if (v.null())
	return o << Auxiliary::null_variable_tag;
      else
	return o << v.name();
    }
  };
  
}

namespace Variables {

  // -----------------------------------------------------------------
  // Implementation of variables via a global list of indexed names
  // -----------------------------------------------------------------

  template <class Name, typename Index>
  class VariablesViaReferenceCounting {

    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, FullyConstructibleConcept);
    BOOST_CLASS_REQUIRE(Name, ConceptDefinitions, OutputStreamableConcept);

    typedef typename FunctionHandling::Counter<Index> CounterType;
    static CounterType count;
    typedef typename CounterType::IndexType IndexType;

    struct Var_rep {
      const IndexType c;
      const Name n;
      explicit Var_rep(Name n) : c(count()), n(n) {
	if (c ==  std::numeric_limits<IndexType>::max())
	  throw Overflow_Variables("VariablesViaReferenceCounting::Var_rep() : " + StringHandling::toString_nc(c));
      }
    };

    typedef typename boost::shared_ptr<Var_rep> Pointer;
    Pointer vp;

  public :

    typedef typename ConceptDefinitions::VariableWithHistory_tag Concept;
    typedef Name NameType;

    VariablesViaReferenceCounting() {}
    explicit VariablesViaReferenceCounting(const NameType& name) {
      if (name != Name()) {
	const Pointer p(new Var_rep(name));
	vp = p;
      }
    }
    VariablesViaReferenceCounting(const VariablesViaReferenceCounting& v) : vp(v.vp) {}

    VariablesViaReferenceCounting& operator =(const VariablesViaReferenceCounting& var) {
      vp = var.vp;
      return *this;
    }

    friend inline bool operator ==(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return (lhs.null() and rhs.null()) or (not lhs.null() and not rhs.null() and (lhs.vp -> c == rhs.vp -> c or lhs.vp -> n == rhs.vp -> n)); }
    friend inline bool operator !=(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return not (lhs == rhs); }
    friend inline bool operator <(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return not rhs.null() and (lhs.null() or lhs.vp -> c < rhs.vp -> c); }
    friend inline bool operator >(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return rhs < lhs; }
    friend inline bool operator <=(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return lhs < rhs or lhs == rhs; }
    friend inline bool operator >=(VariablesViaReferenceCounting lhs, VariablesViaReferenceCounting rhs) { return lhs > rhs or lhs == rhs; }

    bool null() const { return vp == Pointer(); }

    const NameType& name() const {
      if (null())
	return NameType();
      else
	return vp -> n;
    }

    friend inline std::ostream& operator <<(std::ostream& o, VariablesViaReferenceCounting v) {
      if (v.null())
	return o << Auxiliary::null_variable_tag;
      else
	return o << v.name();
    }

    static void clear() {}

  };
  template <class Name, typename Index>
  FunctionHandling::Counter<Index> VariablesViaReferenceCounting<Name,Index>::count;
}

namespace Variables {

  // -----------------------------------------------------------------
  // Implementation of literals and variables as pointers
  // -----------------------------------------------------------------
  // NEEDS A COMPLETE MAKE UP

  template <class InfoPolicy, typename Name, class LiteralLink>
  class VariablesAsPointers : public InfoPolicy {
  public :

    typedef Name NameType;
    typedef std::ptrdiff_t size_type;

    VariablesAsPointers() : p(InfoPolicy::start()) {};

    explicit VariablesAsPointers(const NameType& name) : p(insert(name)) {}
 
    VariablesAsPointers(const VariablesAsPointers& v) : p(v.p) {}

    VariablesAsPointers& operator =(const VariablesAsPointers& v) {
      p = v.p;
    }

    bool operator ==(VariablesAsPointers v) const {
      return p == v.p;
    }
    bool operator !=(VariablesAsPointers v) const {
      return not operator ==(v);
    }
    bool operator <(VariablesAsPointers v) const {
      return p < v.p;
    }

    operator bool() const { return p != InfoPolicy::start(); }

    static void reserve(typename InfoPolicy::size_type max) {
      hash_vector.reserve(max);
      reserve_info(max);
      LiteralLink::reserve(max);
    }

    static typename InfoPolicy::size_type n() { return InfoPolicy::size(); }

     friend std::ostream& operator <<(std::ostream& o, VariablesAsPointers v) {
      if (v)
	return o << hash_vector[v.p - InfoPolicy::start()] -> first;
      else
	return o << "UNDEF";
    }

    typename InfoPolicy::InfoReferenceType operator *() const {
      return get_info(p);
    }

  private :

    typedef typename InfoPolicy::InfoPointerType PointerType;
    PointerType p;

    typedef typename std::map< NameType, PointerType > MapType;
    typedef typename MapType::const_iterator MapIterator;

    static MapType name_map;

    PointerType insert(const NameType& name) {
      const std::pair<MapIterator, bool> ins = name_map.insert(std::make_pair(name, 0));
      if (ins.second) { // new
	const PointerType p = InfoPolicy::new_info();
	hash_vector.push_back(ins.first);
	LiteralLink::new_variable(p);
	return p;
      }
      else // old
	return ins.first -> second;
    }

    typedef typename std::vector<MapIterator> HashVectorType;
    static HashVectorType hash_vector;
  };
  template <typename Info, typename NameType, typename LiteralLink>
  typename VariablesAsPointers<Info, NameType, LiteralLink>::MapType VariablesAsPointers<Info, NameType, LiteralLink>::name_map;

  template <typename Info, typename NameType, typename LiteralLink>
  typename VariablesAsPointers<Info, NameType, LiteralLink>::HashVectorType VariablesAsPointers<Info, NameType, LiteralLink>::hash_vector(1, MapIterator());

  // -------------------
  // Info policy classes
  // -------------------
  
  template <typename Info>
  class InfoPolicyPointerVector {
  public :
    typedef Info InfoValueType;
    typedef Info& InfoReferenceType;
    typedef Info* InfoPointerType;
  private :
    typedef typename std::vector<Info> InfoVectorType;
    static InfoVectorType info_vector;
  public :
    typedef typename InfoVectorType::size_type size_type;
  protected :
    ~InfoPolicyPointerVector() {}
     InfoPointerType new_info() const {
      info_vector.push_back(Info());
      return info_vector.end() - 1;
    }
    void reserve_info(size_type max) const {
      info_vector.reserve(max + 1);
    }
    Info& get_info(Info* p) const {
      return *p;
    }

    Info* start() const {
      return &info_vector[0];
    }
  };
  template <typename Info>
  typename InfoPolicyPointerVector<Info>::InfoVectorType InfoPolicyPointerVector<Info>::info_vector(1, Info());

}

namespace Variables {

  // -----------------------------------------------------------------
  // Typedefs for testing
  // -----------------------------------------------------------------

  struct Occurrences {
    int occ;
    Occurrences() : occ(0) {}
    Occurrences(int o) : occ(o) {}
  };
  
  template <typename Index>
  class InfoPolicyIndexVectorOccurrences : public InfoPolicyIndexVector<Occurrences,Index> {};

  typedef VariablesAsIndices<InfoPolicyIndexVectorOccurrences<int>, std::string, EmptyLiteralLink<int> > VarIntOccString;
  typedef VariablesAsIndices<InfoPolicyIndexVector<Occurrences,signed char>, int, EmptyLiteralLink<signed char> > VarCharOccInt;

  typedef VariablesAsIndices<InfoPolicyEmpty<int>, std::string, EmptyLiteralLink<int> > VarIntEmptyString;

  typedef VariablesViaReferenceCounting<std::string, unsigned int> VarRefIntString;
  typedef VariablesViaReferenceCounting<int, signed char> VarRefCharInt;
  
}

#endif
