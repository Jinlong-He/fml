//
//  nondeterministic_finite_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_nondeterministic_finite_automaton_hpp 
#define atl_nondeterministic_finite_automaton_hpp

#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
#include "deterministic_finite_automaton.hpp"

namespace atl {
    template <class Symbol = char, 
              long epsilon_ = 0,
              class SymbolProperty = no_type,
              class StateProperty = no_type, 
              class AutomatonProperty = no_type>
    class nondeterministic_finite_automaton
        : public detail::nondeterministic_finite_automaton_gen<Symbol, epsilon_,
                                                               SymbolProperty,
                                                               StateProperty,
                                                               AutomatonProperty> {
        public:
            typedef Symbol symbol_type;
            typedef detail::nondeterministic_finite_automaton_gen<Symbol, epsilon_,
                                                                  SymbolProperty,
                                                                  StateProperty,
                                                                  AutomatonProperty> Base;
            typedef deterministic_finite_automaton<Symbol, epsilon_,
                                                   SymbolProperty,
                                                   StateProperty,
                                                   AutomatonProperty> DFA;

            typedef typename Base::transition_property_type transition_property_type;
            typedef typename Base::automaton_property_type automaton_property_type;
            typedef typename Base::state_property_type state_property_type;
            typedef typename Base::Symbol2StateSetMap Symbol2StateSetMap;
            typedef typename Base::TransitionMap TransitionMap;
            typedef typename Base::StateSetMap StateSetMap;
            typedef typename Base::Transition Transition;
            typedef typename Base::State State;

        public:
            nondeterministic_finite_automaton()
                : Base() {}

            nondeterministic_finite_automaton(const nondeterministic_finite_automaton& x)
                : Base(x) {}

            nondeterministic_finite_automaton& 
            operator=(const nondeterministic_finite_automaton& x) {
                if (&x != this) {
                    Base::operator=(x);
                }
                return *this;
            }

        private:
        };
}

#endif /* atl_nondeterministic_finite_automaton_hpp */
