#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

namespace test {
    //test determinize for nondeterministic_finite_automaton<char>
    void test_determinize1() {
        nondeterministic_finite_automaton<> nfa({'a', 'b'});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_state(nfa);
        add_final_state(nfa);

        add_transition(nfa, 0, 1, char(0));
        add_transition(nfa, 0, 3, 'a');
        add_transition(nfa, 0, 4, 'a');
        add_transition(nfa, 1, 2, 'b');
        add_transition(nfa, 2, 7, char(0));
        add_transition(nfa, 3, 6, char(0));
        add_transition(nfa, 4, 5, char(0));
        add_transition(nfa, 5, 1, char(0));
        add_transition(nfa, 5, 8, 'b');
        add_transition(nfa, 6, 8, 'b');
        add_transition(nfa, 7, 8, 'a');
        print_fa(nfa);
        cout << "****************************" << endl;
        deterministic_finite_automaton<> dfa;
        determinize(nfa, dfa);
        print_fa(dfa);
        cout << "**************************** end determinize1 ****************************" << endl;
    }

    //test determinize for nondeterministic_finite_automaton<Symbol>
    void test_determinize2() {
        nondeterministic_finite_automaton<Symbol> nfa({Symbol("a"), Symbol("b")});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_state(nfa);
        add_final_state(nfa);

        add_transition(nfa, 0, 1, Symbol(0));
        add_transition(nfa, 0, 3, Symbol("a"));
        add_transition(nfa, 0, 4, Symbol("a"));
        add_transition(nfa, 1, 2, Symbol("b"));
        add_transition(nfa, 2, 7, Symbol(0));
        add_transition(nfa, 3, 6, Symbol(0));
        add_transition(nfa, 4, 5, Symbol(0));
        add_transition(nfa, 5, 1, Symbol(0));
        add_transition(nfa, 5, 8, Symbol("b"));
        add_transition(nfa, 6, 8, Symbol("b"));
        add_transition(nfa, 7, 8, Symbol("a"));
        print_fa(nfa);
        cout << "****************************" << endl;
        deterministic_finite_automaton<Symbol> dfa;
        determinize(nfa, dfa);
        print_fa(dfa);
        cout << "**************************** end determinize2 ****************************" << endl;
    }

    //test determinize for nondeterministic_finite_automaton<char, 0, Symbol>
    void test_determinize3() {
        nondeterministic_finite_automaton<char, 0, Symbol> nfa({'a', 'b'});
        add_initial_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_state(nfa);
        add_final_state(nfa);
        add_final_state(nfa);
        add_state(nfa);
        add_final_state(nfa);

        add_transition(nfa, 0, 1, char(0), Symbol("0"));
        add_transition(nfa, 0, 3, 'a', Symbol("a"));
        add_transition(nfa, 0, 4, 'a', Symbol("a"));
        add_transition(nfa, 1, 2, 'b', Symbol("b"));
        add_transition(nfa, 2, 7, char(0), Symbol("0"));
        add_transition(nfa, 3, 6, char(0), Symbol("0"));
        add_transition(nfa, 4, 5, char(0), Symbol("0"));
        add_transition(nfa, 5, 1, char(0), Symbol("0"));
        add_transition(nfa, 5, 8, 'b', Symbol("b"));
        add_transition(nfa, 6, 8, 'b', Symbol("b"));
        add_transition(nfa, 7, 8, 'a', Symbol("a"));
        print_fa(nfa);
        cout << "****************************" << endl;
        deterministic_finite_automaton<char, 0, Symbol> dfa;
        determinize(nfa, dfa);
        print_fa(dfa);
        cout << "**************************** end determinize3**************************" << endl;
    }

    //test determinize for nondeterministic_finite_automaton<char, 0, Symbol, bool>
    void test_determinize4() {
        nondeterministic_finite_automaton<char, 0, Symbol, bool> nfa({'a', 'b'});
        add_initial_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_state(nfa, false);
        add_final_state(nfa, true);
        add_final_state(nfa, true);
        add_state(nfa, false);
        add_final_state(nfa, true);

        add_transition(nfa, 0, 1, char(0), Symbol("0"));
        add_transition(nfa, 0, 3, 'a', Symbol("a"));
        add_transition(nfa, 0, 4, 'a', Symbol("a"));
        add_transition(nfa, 1, 2, 'b', Symbol("b"));
        add_transition(nfa, 2, 7, char(0), Symbol("0"));
        add_transition(nfa, 3, 6, char(0), Symbol("0"));
        add_transition(nfa, 4, 5, char(0), Symbol("0"));
        add_transition(nfa, 5, 1, char(0), Symbol("0"));
        add_transition(nfa, 5, 8, 'b', Symbol("b"));
        add_transition(nfa, 6, 8, 'b', Symbol("b"));
        add_transition(nfa, 7, 8, 'a', Symbol("a"));
        print_fa(nfa);
        cout << "****************************" << endl;
        deterministic_finite_automaton<char, 0, Symbol, bool> dfa;
        determinize(nfa, dfa);
        print_fa(dfa);
        cout << "**************************** end determinize4 ****************************" << endl;
    }
}
