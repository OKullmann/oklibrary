// Oliver Kullmann, 12.8.2004 (Swansea)

#ifndef TESTSRAMSEY_hz551010

#define TESTSRAMSEY_hz551010

#include <cassert>

namespace Tests_Ramsey {

  template <class Enum>
  void test_enumeration_hyperedges() {
    std::cout << "test_enumeration_hyperedges<Enum> with Enum = " <<typeid(Enum).name() << std::endl;
    {
      std::cout << "q = 0, r = 0" << std::endl;
      const int max_trivial_test = 10;
      for (int i = 0; i != max_trivial_test; ++i) {
	Enum enm(0,0,i);
	assert(enm.number_vertices_hypergraph == 1);
	assert(enm.size_hyperedges == 1);
	assert(enm.number_hyperedges == 1);
	assert(enm.current_hyperedge[0].empty());
      }
    }
    {
      std::cout << "q = 1, r = 0" << std::endl;
      const int max_trivial_test = 10;
      for (int i = 1; i != max_trivial_test; ++i) {
	Enum enm(1,0,i);
	assert(enm.number_vertices_hypergraph == 1);
	assert(enm.size_hyperedges == 1);
	assert(enm.number_hyperedges == i);
	for (typename Enum::size_type j = 0; j != enm.number_hyperedges; ++j, enm.next()) {
	  assert(enm.current_hyperedge.size() == 1);
	  assert(enm.current_hyperedge[0].empty());
	}
      }
    }
    {
      std::cout << "q = 1, r = 1" << std::endl;
      const int max_trivial_test = 10;
      for (int i = 1; i != max_trivial_test; ++i) {
	Enum enm(1,1,i);
	assert(enm.number_vertices_hypergraph == i);
	assert(enm.size_hyperedges == 1);
	assert(enm.number_hyperedges == i);
	for (typename Enum::size_type j = 0; j != enm.number_hyperedges; ++j, enm.next()) {
	  assert(enm.current_hyperedge.size() == 1);
	  assert(enm.current_hyperedge[0][0] == j);
	}
      }
    }
  }
}

#endif
