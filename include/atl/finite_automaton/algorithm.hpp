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
        static bool 
        apply(const DFA& dfa,
              const std::basic_string<typename DFA::symbol_type>& word) {
            typedef typename DFA::StateSet StateSet;
            StateSet work({initial_state(dfa)}), newWork;
            for (const auto& symbol : word) {
                for (auto state : work) {
                    get_targets_in_map(dfa, state, symbol, newWork);
                }
                work.clear();
                if (newWork.size() > 0) {
                    work.insert(newWork.begin(), newWork.end());
                    newWork.clear();
                } else {
                    return false;
                }
            }
            return true;
        }
    };
    template <DFA_PARAMS>
    inline bool
    accept(const DFA& dfa,
           const std::basic_string<typename DFA::symbol_type>& word) {
        return accept_impl::apply(dfa, word);
    }
}

#endif /* atl_finite_automaton_algorithm_hpp */
