#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test intersect for nondeterministic_finite_automaton<char> & 
    //                   nondeterministic_finite_automaton<char>
    bool test_intersect1() {
        nondeterministic_finite_automaton<> lhs({'a', 'b'});
        nondeterministic_finite_automaton<> rhs({'a', 'b'});
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_state(lhs);
        set_final_state(lhs, 0);
        add_transition(lhs, 0, 0, char('a'));
        add_transition(lhs, 0, 1, char('a'));
        add_transition(lhs, 1, 2, char('b'));
        add_transition(lhs, 2, 2, char('b'));
        add_transition(lhs, 0, 3, char('a'));

        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        set_final_state(rhs, 0);
        add_transition(rhs, 0, 1, char('a'));
        add_transition(rhs, 0, 2, char('a'));
        add_transition(rhs, 1, 2, char('b'));
        deterministic_finite_automaton<> result, expect({'a', 'b'});
        result = (lhs & rhs);
        add_initial_state(expect);
        add_final_state(expect);
        add_final_state(expect);
        set_final_state(expect, 0);
        add_transition(expect, 0, 1, char('a'));
        add_transition(expect, 1, 2, char('b'));
        return (result == expect && accept(expect, "ab") && !accept(expect, "aaa"));
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_intersect2() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_state(lhs);
        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(lhs, 1, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 2, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 0, 3, Symbol("a"), Symbol("1"));

        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("2"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        deterministic_finite_automaton<Symbol, 0, Symbol> result1, result2, expect1, expect2;
        result1 = (lhs & rhs);
        intersect_fa(lhs_dfa, rhs_dfa, result2, union_merge<Symbol>());
        add_initial_state(expect1);
        add_state(expect1);
        add_final_state(expect1);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1&2"));
        add_transition(expect1, 1, 2, Symbol("b"), Symbol("1&2"));

        add_initial_state(expect2);
        add_state(expect2);
        add_final_state(expect2);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1|2"));
        add_transition(expect2, 1, 2, Symbol("b"), Symbol("1|2"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a")});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_intersect3() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool> rhs({Symbol("a"), Symbol("b")});
        add_initial_state(lhs, false);
        add_state(lhs, false);
        add_final_state(lhs, false);
        add_state(lhs, false);

        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(lhs, 1, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 2, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 0, 3, Symbol("a"), Symbol("1"));

        add_initial_state(rhs, true);
        add_state(rhs, true);
        add_final_state(rhs, true);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("2"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));

        deterministic_finite_automaton<Symbol, 0, Symbol, bool> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> rhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> result1, result2, result3, result4,
                                                                expect1, expect2, expect3, expect4;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        intersect_fa(lhs_dfa, rhs_dfa, result1);
        intersect_fa(lhs_dfa, rhs_dfa, result2, union_merge<Symbol>(), union_merge<bool>());
        intersect_fa(lhs_dfa, rhs_dfa, result3, intersect_merge<Symbol>(), union_merge<bool>());
        intersect_fa(lhs_dfa, rhs_dfa, result4, union_merge<Symbol>());
        add_initial_state(expect1, false);
        add_state(expect1, false);
        add_final_state(expect1, false);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1&2"));
        add_transition(expect1, 1, 2, Symbol("b"), Symbol("1&2"));

        add_initial_state(expect2, true);
        add_state(expect2, true);
        add_final_state(expect2, true);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1|2"));
        add_transition(expect2, 1, 2, Symbol("b"), Symbol("1|2"));

        add_initial_state(expect3, true);
        add_state(expect3, true);
        add_final_state(expect3, true);
        add_transition(expect3, 0, 1, Symbol("a"), Symbol("1&2"));
        add_transition(expect3, 1, 2, Symbol("b"), Symbol("1&2"));

        add_initial_state(expect4, false);
        add_state(expect4, false);
        add_final_state(expect4, false);
        add_transition(expect4, 0, 1, Symbol("a"), Symbol("1|2"));
        add_transition(expect4, 1, 2, Symbol("b"), Symbol("1|2"));

        std::vector<Symbol> word1({Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({Symbol("a"), Symbol("b"), Symbol("b")});
        return (result2 == expect2) && (result2 == expect2) && 
               (result3 == expect3) && (result4 == expect4) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    bool test_intersect4() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> rhs({Symbol("a"), Symbol("b")});
        set_property(lhs, Symbol("lhs"));
        set_property(rhs, Symbol("rhs"));
        add_initial_state(lhs, false);
        add_state(lhs, false);
        add_final_state(lhs, false);
        add_state(lhs, false);

        add_transition(lhs, 0, 0, Symbol("a"), Symbol("0"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(lhs, 1, 2, Symbol("b"), Symbol("2"));
        add_transition(lhs, 2, 2, Symbol("b"), Symbol("2"));
        add_transition(lhs, 0, 3, Symbol("a"), Symbol("3"));

        add_initial_state(rhs, true);
        add_state(rhs, true);
        add_final_state(rhs, true);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));

        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> rhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> result1;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> result2;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        intersect_fa(lhs_dfa, rhs_dfa, result1);
        intersect_fa(lhs_dfa, rhs_dfa, result2, union_merge<Symbol>(), union_merge<bool>(), union_merge<Symbol>());

        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> 
            expect1({Symbol("a"), Symbol("b")}),
            expect2({Symbol("a"), Symbol("b")});
        set_property(expect1, Symbol("lhs&rhs"));
        set_property(expect2, Symbol("lhs|rhs"));
        add_initial_state(expect1, false);
        add_state(expect1, false);
        add_final_state(expect1, false);
        add_transition(expect1, 0, 1, Symbol("a"), Symbol("1&1"));
        add_transition(expect1, 1, 2, Symbol("b"), Symbol("2&2"));
        add_initial_state(expect2, true);
        add_state(expect2, true);
        add_final_state(expect2, true);
        add_transition(expect2, 0, 1, Symbol("a"), Symbol("1|1"));
        add_transition(expect2, 1, 2, Symbol("b"), Symbol("2|2"));
        std::vector<Symbol> word1({Symbol("a"), Symbol("b")});
        std::vector<Symbol> word2({});
        return (result1 == expect1) && (result2 == expect2) &&
               accept(expect1, word1) && !accept(expect2, word2);
    }
}
