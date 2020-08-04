#include <atl/regular_expression/algorithm.hpp>
#include <atl/finite_automaton/algorithm.hpp>
#include <atl/finite_automaton/operate.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;

void test_state_prop() {
    nondeterministic_finite_automaton<char, 0, char, int> nfa({'a','b','c','d'});
    auto s0 = add_initial_state(nfa, 0);
    auto s1 = add_state(nfa, 0);
    auto s2 = add_state(nfa, 1);
    auto s3 = add_final_state(nfa, 2);
    add_transition(nfa, s0, s1, 'a', 'a');
    add_transition(nfa, s0, s2, 'a', 'a');
    add_transition(nfa, s2, s3, 'b', 'b');
    add_transition(nfa, s1, s3, 'd', 'd');
    print_fa(nfa);
    cout << endl;
    deterministic_finite_automaton<char, 0, char, int> dfa;
    determinize(nfa, dfa, intersect_merge<char>(), union_merge<int>());
    print_fa(dfa);
}
