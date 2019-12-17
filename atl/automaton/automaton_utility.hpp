//
//  automaton_utility.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_automaton_utility_hpp 
#define atl_automaton_utility_hpp

#include <iostream>
#include "detail/automaton.hpp"
using std::cout;
using std::endl;
namespace atl {
    template <typename Automaton>
    inline void
    print_automaton(const Automaton& a) {
        typename Automaton::StateIter s_it, s_end;
        for (tie(s_it, s_end) = states(a); s_it != s_end; s_it++) {
            if constexpr (!std::is_same<typename Automaton::state_property_type, 
                                        boost::no_property>::value) {
                cout << (*s_it) << 
                        "{" << atl::get_property(a, *s_it) << "}" << endl;
            }
            typename Automaton::OutTransitionIter t_it, t_end;
            for (tie(t_it, t_end) = out_transitions(a, *s_it); t_it != t_end; t_it++) {
                cout << (*t_it) <<
                     "{" << atl::get_property(a, *t_it) << "}" << endl;
            }
        }
    }

    template <typename Automaton>
    inline void
    print_fa(const Automaton& a) {
        cout << "init: " << a.initial_state() << endl;
        cout << "final: ";
        for (auto s : a.final_state_set()) {
            cout << s << " ";
        }
        cout << endl;
        print_automaton(a);
    }
}

#endif /* atl_automaton_utility_hpp */
