//
//  algorithm.hpp
//  atl 
//
//  Created by Jinlong He on 2019/11/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_algorithm_hpp 
#define atl_finite_automaton_algorithm_hpp 

#include <queue>
#include "closure.hpp"
#include "merge.hpp"
#include "copy.hpp"
#include "../detail/finite_automaton/deterministic_finite_automaton.hpp"
#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
using std::queue;

namespace atl {
    struct accept_impl {
        template <DFA_PARAMS>
        static void
        apply(const DFA& a,
              const std::basic_string<DFA_SYMBOL>& word) {
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::State State;
            const auto& transition_map_ = transition_map(a);
            State state = initial_state(a);
            ID count = transition_map_.count(state);
            for (const auto& symbol : word) {
                if (count > 0) {
                    const auto& map = transition_map_.at(state);
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        if (map.count(symbol) > 0) {
                            state = map.at(symbol);
                        }
                    } else {
                        //todo
                    }
                }
            }
        }
    };
}

#endif /* atl_finite_automaton_algorithm_hpp */
