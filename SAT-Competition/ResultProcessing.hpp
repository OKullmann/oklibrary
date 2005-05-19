// Oliver Kullmann, 18.5.2005 (Swansea)

#ifndef RESULTPROCESSING_ppOgB535

#define RESULTPROCESSING_ppOgB535

#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>

#include "BasicSetOperations.hpp"

#include "SingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    class ResultNode;

    typedef std::set<const ResultNode*> SetResultNodesP;

    typedef std::map<SuperSeries, const SetResultNodesP*> MapSuperSeries;

    class ResultNode {
      ResultBasis* rb;
      MapSuperSeries::const_iterator m_sup_ser_it;
    };

    template <typename ResultIterator>
    class ResultDatabase {
      MapSuperSeries map_sup_ser;

      typedef std::vector<ResultNode> VectorResultNodes;
      VectorResultNodes result_collection;
      typedef VectorResultNodes::size_type number_results_type;
      number_results_type number_results_;

      typedef std::vector<const ResultNode*> VectorResultNodesP;
      VectorResultNodesP workbench1, workbench2;
      typedef VectorResultNodesP::const_iterator workbench_iterator;

    public :

      ResultDatabase(ResultIterator begin, const ResultIterator end) {
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        number_results_ = result_collection.size();
        workbench1.resize(number_results_); workbench2.resize(number_results_);
      }
      number_results_type number_results() const { return number_results_; }

      typedef std::vector<const SetResultNodesP*> VectorOfSetsP;
      VectorOfSetsP vector_of_sets;
      typedef std::pair<workbench_iterator, workbench_iterator> Range;

      Range intersection() {
        // ToDo: Using ::OKlib::Set::Algorithms::Intersection
//         const VectorOfSetsP::size_type size = vector_of_sets.size();
//         const workbench_iterator begin1 = workbench1.begin();
//         if (size == 0) {
//           workbench_iterator i1 = workbench1.begin();
//           for (VectorResultNodes::const_iterator j = result_collection.begin(); j != result_collection.end(); ++j) // ToDo: Using std::transform
//             *i1 = &*j;
//           return Range(begin1, workbench1.end());
//         }
//         VectorOfSetsP::const_iterator i = vector_of_sets.begin();
//         if (size == 1)
//           return Range(begin1, std::copy((*i) -> begin(), (*i) -> end(), begin1));
//         workbench_iterator end1 = std::set_intersection((*i) -> begin(), (*i) -> end(), (*(i+1)) -> begin(), (*(i+1)) -> end(), begin1);
//         if (size == 2) return Range(begin1, end1);
//         i += 2;
//         const VectorOfSetsP::const_iterator end = vector_of_sets.end();
//         const workbench_iterator begin2 = workbench2.begin();
//         workbench_iterator end2 = workbench2.end();
//         for (; i+1 < end; ++i) {
//           end2 = std::set_intersection((*i) -> begin(), (*i) -> end(), begin1, end1);
//           ++i;
//           end1 = std::set_intersection((*i) -> begin(), (*i) -> end(), begin2, end2);
//         }
//         if (i != end)
//           return Range(begin2, std::set_intersection((*i) -> begin(), (*i) -> end(), begin1, end1));
//         else
//           return Range(begin1, end1);
      }
      
    };

  }

}

#endif
