#include <atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/finite_automaton/deterministic_finite_automaton.hpp>
#include "test.hpp"
using namespace atl;

//test minimize for nondeterministic_finite_automaton<char>
void test_minimize1() {
    nondeterministic_finite_automaton<> nfa({'a', 'b'});
    add_initial_state(nfa);
    add_state(nfa);
    add_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);

    add_transition(nfa, 0, 1, char('a'));
    add_transition(nfa, 0, 2, char('b'));
    add_transition(nfa, 1, 2, char('b'));
    add_transition(nfa, 1, 3, char('a'));
    add_transition(nfa, 2, 1, char('a'));
    add_transition(nfa, 2, 4, char('b'));
    add_transition(nfa, 3, 2, char('a'));
    add_transition(nfa, 3, 5, char('b'));
    add_transition(nfa, 4, 4, char('b'));
    add_transition(nfa, 4, 6, char('a'));
    add_transition(nfa, 5, 6, char('a'));
    add_transition(nfa, 6, 5, char('b'));
    add_transition(nfa, 6, 3, char('a'));
    print_fa(nfa);
    cout << "****************************" << endl;
    deterministic_finite_automaton<> dfa;
    minimize(nfa, dfa);
    print_fa(dfa);
    cout << "**************************** end minimize1 ****************************" << endl;
}

class Symbol {
public:
    Symbol() : id_("") {}
    Symbol(const string& id) : id_(id) {}
    Symbol(long id) : id_("") {}
    const string& id() const {
        return id_;
    }
    bool operator== (const Symbol& x) const {
        return id_ == x.id_;
    }

    bool operator!= (const Symbol& x) const {
        return id_ != x.id_;
    }

    bool operator< (const Symbol& x) const {
        return id_ < x.id_;
    }

    Symbol operator& (const Symbol& x) const {
        return Symbol(id_ + x.id_);
    }

    friend std::ostream& operator<< (std::ostream& os, const Symbol& x) {
        os << x.id_;
        return os;
    }
private:
    string id_;
};

std::size_t hash_value(const Symbol& c) {
    return boost::hash<string>()(c.id());
}

//test minimize for nondeterministic_finite_automaton<Symbol>
void test_minimize2() {
    nondeterministic_finite_automaton<Symbol> nfa({Symbol("a"), Symbol("b")});
    add_initial_state(nfa);
    add_state(nfa);
    add_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);

    add_transition(nfa, 0, 1, Symbol("a"));
    add_transition(nfa, 0, 2, Symbol("b"));
    add_transition(nfa, 1, 2, Symbol("b"));
    add_transition(nfa, 1, 3, Symbol("a"));
    add_transition(nfa, 2, 1, Symbol("a"));
    add_transition(nfa, 2, 4, Symbol("b"));
    add_transition(nfa, 3, 3, Symbol("a"));
    add_transition(nfa, 3, 5, Symbol("b"));
    add_transition(nfa, 4, 4, Symbol("b"));
    add_transition(nfa, 4, 6, Symbol("a"));
    add_transition(nfa, 5, 6, Symbol("a"));
    add_transition(nfa, 6, 5, Symbol("b"));
    add_transition(nfa, 6, 3, Symbol("a"));
    print_fa(nfa);
    cout << "****************************" << endl;
    deterministic_finite_automaton<Symbol> dfa;
    minimize(nfa, dfa);
    print_fa(dfa);
    cout << "**************************** end minimize2 ****************************" << endl;
}

//test minimize for nondeterministic_finite_automaton<char, 0, Symbol>
void test_minimize3() {
    nondeterministic_finite_automaton<Symbol, 0, Symbol> nfa({Symbol("a"), Symbol("b")});
    add_initial_state(nfa);
    add_state(nfa);
    add_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);
    add_final_state(nfa);

    add_transition(nfa, 0, 1, Symbol("a"), Symbol("a"));
    add_transition(nfa, 0, 2, Symbol("b"), Symbol("b"));
    add_transition(nfa, 1, 2, Symbol("b"), Symbol("b"));
    add_transition(nfa, 1, 3, Symbol("a"), Symbol("a"));
    add_transition(nfa, 2, 1, Symbol("a"), Symbol("a"));
    add_transition(nfa, 2, 4, Symbol("b"), Symbol("b"));
    add_transition(nfa, 3, 3, Symbol("a"), Symbol("b"));
    add_transition(nfa, 3, 5, Symbol("b"), Symbol("b"));
    add_transition(nfa, 4, 4, Symbol("b"), Symbol("b"));
    add_transition(nfa, 4, 6, Symbol("a"), Symbol("a"));
    add_transition(nfa, 5, 6, Symbol("a"), Symbol("a"));
    add_transition(nfa, 6, 5, Symbol("b"), Symbol("b"));
    add_transition(nfa, 6, 3, Symbol("a"), Symbol("a"));
    print_fa(nfa);
    cout << "****************************" << endl;
    deterministic_finite_automaton<Symbol, 0, Symbol> dfa;
    minimize(nfa, dfa);
    print_fa(dfa);
    cout << "**************************** end minimize3**************************" << endl;
}

//test minimize for nondeterministic_finite_automaton<char, 0, Symbol, bool>
void test_minimize4() {
    nondeterministic_finite_automaton<Symbol, 0, Symbol, bool> nfa({'a', 'b'});
    add_initial_state(nfa, false);
    add_state(nfa, false);
    add_state(nfa, false);
    add_final_state(nfa, true);
    add_final_state(nfa, true);
    add_final_state(nfa, true);
    add_final_state(nfa, false);

    add_transition(nfa, 0, 1, Symbol("a"), Symbol("a"));
    add_transition(nfa, 0, 2, Symbol("b"), Symbol("b"));
    add_transition(nfa, 1, 2, Symbol("b"), Symbol("b"));
    add_transition(nfa, 1, 3, Symbol("a"), Symbol("a"));
    add_transition(nfa, 2, 1, Symbol("a"), Symbol("a"));
    add_transition(nfa, 2, 4, Symbol("b"), Symbol("b"));
    add_transition(nfa, 3, 3, Symbol("a"), Symbol("b"));
    add_transition(nfa, 3, 5, Symbol("b"), Symbol("b"));
    add_transition(nfa, 4, 4, Symbol("b"), Symbol("b"));
    add_transition(nfa, 4, 6, Symbol("a"), Symbol("a"));
    add_transition(nfa, 5, 6, Symbol("a"), Symbol("a"));
    add_transition(nfa, 6, 5, Symbol("b"), Symbol("b"));
    add_transition(nfa, 6, 3, Symbol("a"), Symbol("a"));
    print_fa(nfa);
    cout << "****************************" << endl;
    deterministic_finite_automaton<Symbol, 0, Symbol, bool> dfa;
    minimize(nfa, dfa);
    print_fa(dfa);
    cout << "**************************** end minimize4 ****************************" << endl;
}
