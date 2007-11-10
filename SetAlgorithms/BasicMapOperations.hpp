// Oliver Kullmann, 4.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file SetAlgorithms/BasicMapOperations.hpp
  \brief Basic tools for handling maps (in the std-sense as well as in
  the general sense).
*/

#ifndef BASICMAPOPERATIONS_098Uik
#define BASICMAPOPERATIONS_098Uik

#include <functional>
#include <cassert>

namespace OKlib {
  namespace SetAlgorithms {

    /*!
      \class MapValue
      \brief Adaptor to create a functor from a map in the sense of the C++ standard.

      Given an object m of type Map, via MapValue<Map>(m) we obtain a functor, that is,
      a function object which maps objects from the Map-key-type to the Map-value-type.
      The associated helper-function is map_value. Everything is handled by (const) reference.

      \todo There should also exist another version which returns the function value as a copy.

      \todo Via an error-policy there should be the possibility to throw an exception in
      case of an argument not in the domain.

    */

    template <class Map>
    struct MapValue : std::unary_function<typename Map::key_type, const typename Map::value_type::second_type&> {
      MapValue(const Map& map) : map(map) {}
    private :
      typedef typename Map::key_type key_type;
      typedef typename Map::value_type::second_type value_type;
      typedef typename Map::const_iterator const_iterator;
    public :
      const value_type& operator() (const key_type& x) const {
        const const_iterator& i(map.find(x));
        assert(i != map.end());
        return i -> second;
      }
    private :
      const Map& map;
    };

    template <class Map>
    typename MapValue<Map>::result_type map_value(const Map& map, const typename MapValue<Map>::argument_type& x) {
      return MapValue<Map>(map)(x);
    }

  }

}

#endif
