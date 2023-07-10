#include <fml/atl/fomula_automaton/fomula_automaton.hpp>
#include <iostream>
#include "test.hpp"
#include <fml/atl/automaton_utility.hpp>
using namespace atl;
using namespace ll;
namespace test {
    bool test_fomula_automaton() {
        fomula_automaton<> foa;
        int_variable a("a", 1, 10);
        int_variable b("b", int_value(1), int_value(10));
        int_variable c("c", 1, 10);
        add_control_state(foa, a, (a==1));
        add_control_state(foa, b, (b==1));
        add_input_state(foa, c);
        add_transition(foa, a, a+1, a==1);
        add_transition(foa, a, int_value(1), a>1);
        add_transition(foa, a, int_value(1), atomic_proposition("TRUE"));
        add_transition(foa, b, int_value(2), c>1);
        atomic_proposition p = a==2;
        verify_invar_nuxmv(foa, p, "source");
        unordered_map<string, vector<string> > trace_table;
        parse_trace_nuxmv(foa, "result", trace_table);
        for (auto& [var, trace] : trace_table) {
            cout << var << ": ";
            for (auto& value : trace) {
                cout << value << " -> ";
            }
            cout << endl;
        }
        //translate_nuxmv(foa, cout);
        return true;
    }
}
