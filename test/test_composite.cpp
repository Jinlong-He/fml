#include <atl/automaton_utility.hpp>
#include <atl/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp>
#include <atl/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    bool test_composite1() {
        nondeterministic_letter2letter_transducer<> lhs, rhs;
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, 'a', 'b');
        add_transition(lhs, 0, 1, 'a', 'b');
        add_transition(lhs, 1, 2, 'b', 'c');

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, 'b', 'c');
        add_transition(rhs, 0, 0, 'c', 'b');

        deterministic_letter2letter_transducer<> result, expect;
        result = (lhs * rhs);
        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a', 'c');
        add_transition(expect, 1, 1, 'a', 'c');
        add_transition(expect, 1, 2, 'b', 'b');
        std::vector<string> result_words1, expect_words1({"ccccb"}),
                            result_words2, expect_words2({"bbbb", "bbbb"}),
                            result_words3, expect_words3({"cbbc"});
        translate(expect, "aaaab", result_words1);
        translate(lhs, "aaaa", result_words2);
        translate(rhs, "bccb", result_words3);
        return (result == expect) && 
               (result_words1 == expect_words1) && 
               (result_words2 == expect_words2) &&
               (result_words3 == expect_words3);
    }

    bool test_composite2() {
        nondeterministic_letter2letter_transducer<> lhs, rhs;
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_transition(lhs, 0, 0, 'a', 'b');
        add_transition(lhs, 0, 1, 'a', 'c');
        add_transition(lhs, 1, 2, 'b', 'c');

        add_initial_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 0, 'b', 'c');
        add_transition(rhs, 0, 0, 'c', 'b');

        deterministic_letter2letter_transducer<> result, expect, test;
        result = (lhs * rhs);
        add_initial_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 0, 'a', 'c');
        add_transition(expect, 0, 1, 'a', 'b');
        add_transition(expect, 1, 2, 'b', 'b');
        std::vector<string> result_words1, expect_words1({"cccbb"}),
                            result_words2, expect_words2({"bbbc", "bbbb"}),
                            result_words3, expect_words3({"cbbc"});
        translate(expect, "aaaab", result_words1);
        translate(lhs, "aaaa", result_words2);
        translate(rhs, "bccb", result_words3);
        return (result == expect) && 
               (result_words1 == expect_words1) && 
               (result_words2 == expect_words2) &&
               (result_words3 == expect_words3);
    }
}
