#include <atl/regular_expression/algorithm.hpp>
#include <atl/finite_automaton/algorithm.hpp>
#include <atl/finite_automaton/operate.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;
void test_tempalte() {
    deterministic_finite_automaton<char, 0, char> dfa1({'a','b','c','d'});
    deterministic_finite_automaton<char, 0, char> dfa2({'a','b','c','d'});
    auto s10 = add_initial_state(dfa1);
    auto s11 = add_state(dfa1);
    auto s12 = add_state(dfa1);
    auto s13 = add_final_state(dfa1);
    add_transition(dfa1, s10, s11, 'a', 'b');
    add_transition(dfa1, s11, s11, 'b', 'b');
    add_transition(dfa1, s11, s13, 'd', 'b');
    add_transition(dfa1, s12, s13, 'd', 'b');

    auto s20 = add_initial_state(dfa2);
    auto s23 = add_final_state(dfa2);
    add_transition(dfa2, s20, s23, 'a', 'b');
    add_transition(dfa2, s23, s23, 'd', 'b');
    print_fa(dfa1);
    print_fa(dfa2);
    print_fa((dfa1 & dfa2));
    print_fa((dfa1 | dfa2));
    print_fa((dfa1 + dfa2));
    //print_fa(!dfa1);
}
