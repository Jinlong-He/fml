#include <iostream>
#include <string>
#include <set>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <fml/atl/automaton_utility.hpp>
#include <fml/atl/detail/algorithm.hpp>
#include <fml/atl/size_register_automaton/size_register_automaton.hpp>
#include <fml/ll/atomic_proposition.hpp>
#include <fml/ll/propositional_logic.hpp>
using namespace atl;
using namespace ll;

namespace test {

    typedef size_register_automaton::State State;
    typedef size_register_automaton::Transition Transition;
    typedef size_register_automaton::TransitionProperty TransitionProperty;
    
    bool test_sra() {
        int_variable cur("cur");
        Guard top(cur >= 0);
        Guard g2(cur >= 2);
        Guard g3(cur >= 3);
        
        Modes ii, ic, ci, cc;
        ii.push_back(RegisterMode::IDLE);
        ii.push_back(RegisterMode::IDLE);
        ic.push_back(RegisterMode::IDLE);
        ic.push_back(RegisterMode::COUNT);
        ci.push_back(RegisterMode::COUNT);
        ci.push_back(RegisterMode::IDLE);
        cc.push_back(RegisterMode::COUNT);
        cc.push_back(RegisterMode::COUNT);

        size_register_automaton sra(2);
        
        State q0 = sra.add_state(ii);
        State q1 = sra.add_state(ci);
        State q2 = sra.add_state(cc);
        State q3 = sra.add_state(ic);
        State q4 = sra.add_state(ii);

        sra.set_initial_state(q0);
        sra.set_final_state(q4);

        sra.add_transition(q0, q1, g2);
        sra.add_transition(q1, q1, top);
        sra.add_transition(q1, q2, g2);
        sra.add_transition(q1, q4, top);
        sra.add_transition(q2, q2, top);
        sra.add_transition(q2, q3, top);
        sra.add_transition(q3, q3, top);
        sra.add_transition(q3, q1, g3);

        std::cout << sra << std::endl;

        // std::cout << "complement :\n" << !sra << std::endl;

        // std::cout << "product :\n" << (sra & sra) << std::endl;

        // std::cout << "comcatenation :\n" << (sra + sra) << std::endl;

        std::cout << " Nonempty : " << (sra.is_nonempty() ? "Yes" : "No") << std::endl;

        size_register_automaton insra =
            size_register_automaton(sra_to_insra(sra));
        
        std::cout << insra << std::endl;

        print_fa(sra_to_nfa(sra));
        return true;
    }

}