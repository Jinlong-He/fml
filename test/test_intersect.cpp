#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test intersect for nondeterministic_finite_automaton<char> & 
    //                   nondeterministic_finite_automaton<char>
    void test_intersect1() {
        nondeterministic_finite_automaton<> lhs({'a', 'b'});
        nondeterministic_finite_automaton<> rhs({'a', 'b'});
        add_initial_state(lhs);
        add_state(lhs);
        add_final_state(lhs);
        add_state(lhs);

        add_transition(lhs, 0, 0, char('a'));
        add_transition(lhs, 0, 1, char('a'));
        add_transition(lhs, 1, 2, char('b'));
        add_transition(lhs, 2, 2, char('b'));
        add_transition(lhs, 0, 3, char('a'));
        print_fa(lhs);

        cout << "****************************" << endl;
        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        add_transition(rhs, 0, 1, char('a'));
        add_transition(rhs, 0, 2, char('a'));
        add_transition(rhs, 1, 2, char('b'));
        print_fa(rhs);
        cout << "****************************" << endl;
        print_fa((lhs & rhs));
        cout << "**************************** end intersect1 ****************************" << endl;
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    void test_intersect2() {
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
        print_fa(lhs);

        cout << "****************************" << endl;
        add_initial_state(rhs);
        add_state(rhs);
        add_final_state(rhs);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("2"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));
        print_fa(rhs);
        cout << "****************************" << endl;
        deterministic_finite_automaton<Symbol, 0, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> rhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol> res1;
        deterministic_finite_automaton<Symbol, 0, Symbol> res2;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        intersect_fa(lhs_dfa, rhs_dfa, res1);
        intersect_fa(lhs_dfa, rhs_dfa, res2, union_merge<Symbol>());
        print_fa(res1);
        cout << "****************************" << endl;
        print_fa(res2);
        cout << "**************************** end intersect2 ****************************" << endl;
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    void test_intersect3() {
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
        print_fa(lhs);

        cout << "****************************" << endl;
        add_initial_state(rhs, true);
        add_state(rhs, true);
        add_final_state(rhs, true);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("2"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));
        print_fa(rhs);
        cout << "****************************" << endl;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> rhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> res1;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool> res2;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        intersect_fa(lhs_dfa, rhs_dfa, res1);
        intersect_fa(lhs_dfa, rhs_dfa, res2, union_merge<Symbol>(), union_merge<bool>());
        print_fa(res1);
        cout << "****************************" << endl;
        print_fa(res2);
        cout << "**************************** end intersect3 ****************************" << endl;
    }

    //test intersect for nondeterministic_finite_automaton<Symbol> &
    //                   nondeterministic_finite_automaton<Symbol>
    void test_intersect4() {
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> lhs({Symbol("a"), Symbol("b")});
        nondeterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> rhs({Symbol("a"), Symbol("b")});
        set_property(lhs, Symbol("lhs"));
        set_property(rhs, Symbol("rhs"));
        add_initial_state(lhs, false);
        add_state(lhs, false);
        add_final_state(lhs, false);
        add_state(lhs, false);

        add_transition(lhs, 0, 0, Symbol("a"), Symbol("1"));
        add_transition(lhs, 0, 1, Symbol("a"), Symbol("1"));
        add_transition(lhs, 1, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 2, 2, Symbol("b"), Symbol("1"));
        add_transition(lhs, 0, 3, Symbol("a"), Symbol("1"));
        print_fa(lhs);

        cout << "****************************" << endl;
        add_initial_state(rhs, true);
        add_state(rhs, true);
        add_final_state(rhs, true);
        add_transition(rhs, 0, 1, Symbol("a"), Symbol("2"));
        add_transition(rhs, 0, 2, Symbol("a"), Symbol("2"));
        add_transition(rhs, 1, 2, Symbol("b"), Symbol("2"));
        print_fa(rhs);
        cout << "****************************" << endl;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> lhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> rhs_dfa;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> res1;
        deterministic_finite_automaton<Symbol, 0, Symbol, bool, Symbol> res2;
        minimize(lhs, lhs_dfa);
        minimize(rhs, rhs_dfa);
        intersect_fa(lhs_dfa, rhs_dfa, res1);
        intersect_fa(lhs_dfa, rhs_dfa, res2, union_merge<Symbol>(), union_merge<bool>(), union_merge<Symbol>());
        print_fa(res1);
        cout << "****************************" << endl;
        print_fa(res2);
        cout << "**************************** end intersect4 ****************************" << endl;
    }
}
