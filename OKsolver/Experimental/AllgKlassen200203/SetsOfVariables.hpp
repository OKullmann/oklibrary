// Oliver Kullmann, 13.8.2003 (Swansea)

#ifndef SETSOFVARIABLESWAECHTER

#define SETSOFVARIABLESWAECHTER

namespace SetsOfVariables {

  // -----------------------------------------------------------------
  // Implementation of a singleton variables set in the obvious way
  // -----------------------------------------------------------------

  template <class Variables>
  class SingletonVarSet {
    
    BOOST_CLASS_REQUIRE(Variables, ConceptDefinitions, Variable_concept);

    Variables v;

  public :

    typedef ConceptDefinitions::SingletonSetOfVariables_tag Concept;
    typedef Variables Var;

    SingletonVarSet() {}
    explicit SingletonVarSet(Var v) : v(v) {}
    SingletonVarSet(const SingletonVarSet& vs) : v(vs.v) {}
    SingletonVarSet& operator =(SingletonVarSet vs) { v = vs.v; }
    operator Var() const { return v; }
    bool empty() const { return v.null(); }
    void clear() { v = Var(); }
    typedef int size_type;
    size_type size() const { return (v.null()) ? 0 : 1; }
    class iterator {
    private :
      Var i;
      iterator(Var v) : i(v) {}
      friend class SingletonVarSet;
    public :
      iterator() {}
      iterator(const iterator& it) : i(it.i) {}
      iterator& operator =(iterator ii) {
	i = ii.i;
	return *this;
      }
      friend inline bool operator ==(iterator lhs, iterator rhs) {
	return lhs.i == rhs.i;
      }
      friend inline bool operator !=(iterator lhs, iterator rhs) {
	return not (lhs == rhs);
      }
      typedef Var value_type;
      typedef Var reference;
      typedef Var* pointer;
      typedef size_type difference_type;
      typedef std::forward_iterator_tag iterator_category;

      reference operator *() const {
	return i;
      }
      iterator& operator ++() {
	assert(not i.null());
	i = Var();
	return *this;
      }
      iterator operator ++(int) {
	assert(not i.null());
	iterator it(*this);
	i = Var();
	return it;
      }
    };
    typedef iterator const_iterator;

    iterator begin() const {
      return iterator(v);
    }
    iterator end() const {
      return iterator();
    }
  };

}

#endif
