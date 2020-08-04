#include <atl/push_down_system/algorithm.hpp>
#include <atl/automaton_utility.hpp>
#include "test.hpp"
using namespace atl;

void test_poststar() {
    nondeterministic_finite_automaton<char> nfa({'a','b','c','d'});
    auto s0 = add_initial_state(nfa);
    auto s1 = add_state(nfa);
    auto s2 = add_state(nfa);
    auto s3 = add_final_state(nfa);
    add_transition(nfa, s0, s1, 'a');
    add_transition(nfa, s0, s2, 'a');
    add_transition(nfa, s2, s3, 'b');
    add_transition(nfa, s1, s3, 'd');
    print_fa(nfa);
    cout << endl;

    push_down_system<char> pds({'a', 'b', 'c', 'd'});
    add_state(pds);
    add_state(pds);
    add_state(pds);
    add_transition(pds, 0, 1, 'a', std::vector<char>({'d'}));
    add_transition(pds, 0, 1, 'a', std::vector<char>({'c','b'}));
    print_pds(pds);

    nondeterministic_finite_automaton res;
    pds_post_star(pds, nfa, res);
    print_fa(res);
}
