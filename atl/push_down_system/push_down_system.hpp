//
//  push_down_system.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_push_down_system_hpp 
#define atl_push_down_system_hpp

#include "../detail/push_down_system/push_down_system.hpp"

namespace atl {
    template <class Symbol, 
              long epsilon_,
              class SymbolProperty,
              class StateProperty, 
              class AutomatonProperty>
    class push_down_system
        : public detail::push_down_system_gen<Symbol, epsilon_,
                                              SymbolProperty,
                                              StateProperty,
                                              AutomatonProperty> {
        public:
            typedef detail::push_down_system_gen<Symbol, epsilon_,
                                                 SymbolProperty,
                                                 StateProperty,
                                                 AutomatonProperty> Base;
        typedef Symbol symbol_type;
        typedef SymbolProperty symbol_property_type;
        typedef typename Base::transition_property transition_property;
        typedef typename Base::pds_label_type pds_label_type;
        typedef typename Base::transition_property_type transition_property_type;
        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;

        typedef typename Base::Transition Transition;
        typedef typename Base::State State;

        typedef unordered_set<State> StateSet;
        typedef unordered_set<Symbol> SymbolSet;

        typedef unordered_map<State, State> State2Map;

    public:
        push_down_system(const SymbolSet alphabet = SymbolSet())
            : Base(alphabet) {}

        push_down_system(const push_down_system& x)
            : Base(x) {}

        push_down_system& 
        operator=(const push_down_system& x) {
            if (&x != this) {
                Base::operator=(x);
            }
            return *this;
        }

        private:
    };
}

#endif /* atl_push_down_system_hpp */
