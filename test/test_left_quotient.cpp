#include <atl/automaton_utility.hpp>
#include <atl/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp>
#include <atl/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    bool test_left_quotient1() {
        deterministic_letter2letter_transducer<> dl2lt;
        add_initial_state(dl2lt);
        add_state(dl2lt);
        add_final_state(dl2lt);
        add_transition(dl2lt, 0, 0, 'a', 'b');
        add_transition(dl2lt, 0, 1, 'a', 'a');
        add_transition(dl2lt, 1, 2, 'b', 'c');

        deterministic_letter2letter_transducer<> result, expect;
        left_quotient(dl2lt, "aaa", result);
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a', 'a');
        add_transition(expect, 0, 2, 'a', 'b');
        add_transition(expect, 0, 3, 'b', 'c');
        add_transition(expect, 1, 3, 'b', 'c');
        add_transition(expect, 2, 1, 'a', 'a');
        add_transition(expect, 2, 2, 'a', 'b');
        return (result == expect);
    }

    bool test_left_quotient2() {
        nondeterministic_letter2letter_transducer<> nl2lt;
        add_initial_state(nl2lt);
        add_state(nl2lt);
        add_final_state(nl2lt);
        add_transition(nl2lt, 0, 0, 'a', 'b');
        add_transition(nl2lt, 0, 1, 'a', 'b');
        add_transition(nl2lt, 1, 2, 'b', 'c');

        deterministic_letter2letter_transducer<> result, expect;
        left_quotient(nl2lt, "aaa", result);
        add_initial_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 0, 'a', 'b');
        add_transition(expect, 0, 1, 'b', 'c');
        return (result == expect);
    }

    bool test_left_quotient3() {
        return true;
    }

    bool test_left_quotient4() {
        return true;
    }
}
