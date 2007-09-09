// Oliver Kullmann, 8.9.2007 (Swansea)

#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>

#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>

#include <Transitional/Programming/Sequences/declarations.hpp>
#include <Transitional/Programming/Sequences/Comparisons.hpp>
#include <Transitional/Programming/Sequences/tests/Comparisons.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_common_part_std(new ::OKlib::Prg::Seq::tests::CommonPart< ::OKlib::Prg::Seq::Common_part, ::OKlib::Prg::Seq::Implementation_common_part_std>);

  ::OKlib::TestSystem::RunTest test_common_part_self(new ::OKlib::Prg::Seq::tests::CommonPart< ::OKlib::Prg::Seq::Common_part, ::OKlib::Prg::Seq::Implementation_common_part_self>);

}

