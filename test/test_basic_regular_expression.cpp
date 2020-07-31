#include <atl/regular_expression/algorithm.hpp>
#include <atl/finite_automaton/algorithm.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;
void test_basic_regular_expression() {
    basic_regular_expression<> regEx("a+|b*");
    deterministic_finite_automaton<> dfa;
    make_fa(regEx, dfa);
    //print_fa(dfa);
    std::cout << accept(dfa, "aaabb") << std::endl;
    std::cout << accept(dfa, "bb") << std::endl;
    std::cout << accept(dfa, "aaaaaaa") << std::endl;
}
