#include <atl/regular_expression/basic_regular_expression.hpp>
#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;
void test_basic_regular_expression() {
    basic_regular_expression<> regEx("a+|b*");
    deterministic_finite_automaton<> dfa;
    make_fa(regEx, dfa);
    print_fa(dfa);
}
