#include <atl/push_down_system/push_down_system.hpp>
#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    bool test_poststar() {
        nondeterministic_finite_automaton<char> nfa({'a','b','c','d'});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_transition(nfa, 0, 3, 'a');
        add_transition(nfa, 3, 4, 'a');

        push_down_system<char> pds({'a', 'b', 'c', 'd'});
        add_state(pds);
        add_state(pds);
        add_state(pds);
        add_transition(pds, 0, 1, 'a', std::vector<char>({'b', 'a'}));
        add_transition(pds, 1, 2, 'b', std::vector<char>({'c', 'a'}));
        add_transition(pds, 2, 0, 'c', std::vector<char>({'b'}));
        add_transition(pds, 0, 0, 'b', std::vector<char>());

        nondeterministic_finite_automaton result, expect;
        pds_post_star(pds, nfa, result);
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_state(expect);
        add_state(expect);
        add_transition(expect, 0, 3, 'a');
        add_transition(expect, 0, 6, 'b');
        add_epsilon_transition(expect, 0, 6);
        add_transition(expect, 1, 5, 'b');
        add_transition(expect, 2, 6, 'c');
        add_transition(expect, 3, 4, 'a');
        add_transition(expect, 5, 3, 'a');
        add_transition(expect, 5, 5, 'a');
        add_transition(expect, 6, 5, 'a');
        return (result == expect);
    }
}
