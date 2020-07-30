#include <atl/regular_expression/algorithm.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;
void test_basic_regular_expression(const char* text) {
    basic_regular_expression<> regEx(text);
    deterministic_finite_automaton<> dfa;
    make_fa(regEx, dfa);
    print_fa(dfa);
}
