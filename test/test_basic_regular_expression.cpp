#include <fml/atl/regular_expression/basic_regular_expression.hpp>
#include <fml/atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <fml/atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;
namespace test {
    // test basic regular expression for char.
    bool test_basic_regular_expression1() {
        basic_regular_expression<> regEx("a");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression2() {
        basic_regular_expression<> regEx("(a*b+)|(a+)");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        add_transition(expect, 0, 2, 'b');
        add_transition(expect, 1, 1, 'a');
        add_transition(expect, 1, 2, 'b');
        add_transition(expect, 2, 2, 'b');
        return (result == expect);
    }

    // test basic regular expression for Symbol.
    bool test_basic_regular_expression3() {
        std::vector<Symbol> word({Symbol("a"), Symbol("|"), Symbol("b")});
        basic_regular_expression<Symbol> regEx(word);
        deterministic_finite_automaton<Symbol> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, Symbol("a"));
        add_transition(expect, 0, 1, Symbol("b"));
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression4() {
        basic_regular_expression<> regEx("a(b|c)?");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        add_transition(expect, 1, 2, 'b');
        add_transition(expect, 1, 2, 'c');
        return (result == expect);
    }

    // test basic regular expression for char.
    bool test_basic_regular_expression5() {
        basic_regular_expression<> regEx("a\\|c");
        deterministic_finite_automaton<> result, expect;
        make_fa(regEx, result);
        add_initial_state(expect);
        add_state(expect);
        add_state(expect);
        add_final_state(expect);
        add_transition(expect, 0, 1, 'a');
        add_transition(expect, 1, 2, '|');
        add_transition(expect, 2, 3, 'c');
        return (result == expect);
    }
}
