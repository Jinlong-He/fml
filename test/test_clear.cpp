#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    bool test_clear1() {
        nondeterministic_finite_automaton<> result, expect;
        add_initial_state(result);
        add_state(result);
        add_final_state(result);
        add_transition(result, 0, 1, 'a');
        add_transition(result, 0, 1, 'b');
        add_transition(result, 1, 1, 'a');
        add_transition(result, 1, 2, 'a');
        add_transition(result, 2, 1, 'b');

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 1, 2, 'a');

        clear_in_transitions(result, 1);
        return (transition_map(result) == transition_map(expect));
    }

    bool test_clear2() {
        nondeterministic_finite_automaton<> result, expect;
        add_initial_state(result);
        add_state(result);
        add_final_state(result);
        add_transition(result, 0, 1, 'a');
        add_transition(result, 0, 1, 'b');
        add_transition(result, 1, 1, 'a');
        add_transition(result, 1, 2, 'a');
        add_transition(result, 2, 1, 'b');

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);

        clear_state(result, 1);
        return (transition_map(result) == transition_map(expect));
    }

    bool test_clear3() {
        deterministic_finite_automaton<> result, expect;
        add_initial_state(result);
        add_state(result);
        add_final_state(result);
        add_transition(result, 0, 1, 'a');
        add_transition(result, 0, 1, 'b');
        add_transition(result, 1, 1, 'a');
        add_transition(result, 1, 2, 'b');
        add_transition(result, 2, 1, 'a');
        add_transition(result, 2, 0, 'b');

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 1, 2, 'b');
        add_transition(expect, 2, 0, 'b');

        clear_in_transitions(result, 1);
        return (transition_map(result) == transition_map(expect));
    }

    bool test_clear4() {
        deterministic_finite_automaton<char, 0, char> result, expect;
        add_initial_state(result);
        add_state(result);
        add_final_state(result);
        add_transition(result, 0, 1, 'a', '1');
        add_transition(result, 0, 1, 'b', '1');
        add_transition(result, 1, 1, 'a', '1');
        add_transition(result, 1, 2, 'b', '1');
        add_transition(result, 2, 1, 'a', '1');
        add_transition(result, 2, 0, 'b', '1');

        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 1, 2, 'b', '1');
        add_transition(expect, 2, 0, 'b', '1');

        clear_in_transitions(result, 1);
        return (transition_map(result) == transition_map(expect));
    }
}
