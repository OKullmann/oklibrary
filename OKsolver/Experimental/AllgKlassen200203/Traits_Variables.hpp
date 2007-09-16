// Oliver Kullmann, 18.8.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TRAITSVARIABLESWAECHTER_jakjdkfjt52190

#define TRAITSVARIABLESWAECHTER_jakjdkfjt52190

#include <exception>
#include <stdexcept>
#include <string>


namespace Error_Variables {

  struct Basis_Variables_Error {};
 
  struct CapacityOverflow : std::runtime_error, virtual Basis_Variables_Error {
    explicit CapacityOverflow(const std::string& what) : runtime_error(what) {}
  };

  struct InvalidName : std::runtime_error, virtual Basis_Variables_Error {
    explicit InvalidName(const std::string& what) : runtime_error(what) {}
  };

}


namespace Traits_Variables {

  struct ThrowsTotalCapacityOverflow {};
  struct DoesntThrowTotalCapacityOverflow {};

  struct ThrowsInvalidName {};
  struct NoInvalidNames {};

  struct GeneralNames {};
  // for GeneralNames the name type should neither be integral nor string
  struct SpecialNames : virtual GeneralNames {};
  struct NaturalNumberConvertibleName : virtual SpecialNames {};
  struct StringConvertibleName : virtual SpecialNames, virtual NoInvalidNames {};

  struct ComparisonDifferentDomainsUndefinedBehaviour {};
  struct ComparisonDifferentDomainsImplementationDefined : virtual ComparisonDifferentDomainsUndefinedBehaviour {};
  // this is the common base type of the two following types (comparison of variables from different domains results in implementation defined behaviour):
  struct LocalDomainsIdentified : virtual ComparisonDifferentDomainsImplementationDefined {};
  // only for LocalDomainsIdentified comparison of variables created from different domains has the expected behaviour
  struct OneGlobalDomain : virtual ComparisonDifferentDomainsUndefinedBehaviour {};
  // here variables created by different domains, but with the same name are equal.

  struct AllocationBehaviour {};
  struct AllocationCausesInvalidation : virtual AllocationBehaviour {};
  struct SafeAllocation : virtual AllocationBehaviour {};

  struct OrderImplementationDefined {};
  struct OrderNameInherited : virtual OrderImplementationDefined {};
  struct OrderByCreation : virtual OrderImplementationDefined {};

  struct SizeBehaviour {};
  struct NaturalSize : virtual SizeBehaviour {};
  struct SizeNameRelated : virtual SizeBehaviour {};
  struct SizeIndexRelated : virtual SizeNameRelated {};
  

  // Consider a variable domain type VarD and objects D, D1, D2 of type VarD:

  // If MetaData<VarD>::comparison_property is just ComparisonDifferentDomainsUndefinedBehaviour, then comparing variables (via == or <) from D1 and D2 results in undefined behaviour; if it is ComparisonDifferentDomainsImplementationDefined then comparing variables from different domains results in implementation defined behaviour. If comparison_property is one of the two derived types, then the behaviour is defined as follows://
  // If comparison_property is LocalDomainsIdentified, then in case of name1 != NameType() we always have
  // D1(name1) != D2(name2),
  // while if comparison_property is OneGlobalDomain, then we have
  // D1(name1) == D2(name2) iff name1 == name2.
  // W.r.t. comparing via < comparing variables from different domains results in implementation-defined behaviour, except of the case where MetaData<VarD>::order_property is OrderNameInherited.

  // If MetaData<VarD>::allocation_property is AllocationCausesInvalidation, then a call of D(name) with name != NameType() may cause reallocation, and thus invalidates all existing variables (and iterators), if there has not been a sufficient call to D.reserve() before:
  // Calling D.reserve(s) for an empty domain guarantees, that s-1 calls of D(name) can be done without reallocation.
  // If capacity() is available, then "reallocation" can be seen as equivalent to capacity() increasing its value.

  // If MetaData<VarD>::name_property is NaturalNumberConvertibleName, then VarD(int x) for x >= 0 is possible.
  // If MetaData<VarD>::name_property is StringConvertibleName, then VarD(std::string) is possible, and MetaData<VarD>::name_throw_property is NoInvalidNames.

  // If MetaData<VarD>::order_property is OrderNameInherited, then for variables v, w we have v < w iff D.name(v) < D.name(w) (provided v, w have been created by D).
  // If MetaData<VarD>::order_property is OrderByCreation, then for variables v, w we have v < w iff v has been created before w.

  // If VarD has size():
  // If MetaData<VarD>::size_property is NaturalSize, then creation of a new variable increases the size of the domain exactly by one; if it is SizeNameRelated, it may be larger (but never less), depending on the name (regarded as index); if it is SizeIndexRelated, then name_property must be NaturalNumberConvertibleName, and after creating a variable v with index i we have size() >= i+1.

  
}

#endif
