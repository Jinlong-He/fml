//
//  algorithm.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_algorithm_hpp 
#define atl_detail_finite_automaton_algorithm_hpp 

#include <queue>
#include <vector>
#include <fml/atl/detail/finite_automaton/closure.hpp>
#include <fml/atl/detail/finite_automaton/merge.hpp>
#include <fml/atl/detail/finite_automaton/copy.hpp>
#include <fml/atl/detail/finite_automaton/deterministic_finite_automaton.hpp>
#include <fml/atl/detail/finite_automaton/nondeterministic_finite_automaton.hpp>
using std::queue;

namespace atl::detail {
    struct accept_impl {
        template <DFA_PARAMS>
        static bool 
        apply(const DFA& dfa,
              const std::vector<DFA_SYMBOL>& word) {
            typedef typename DFA::StateSet StateSet;
            StateSet work({initial_state(dfa)}), new_work;
            for (const auto& symbol : word) {
                for (auto state : work) {
                    get_targets_in_map(dfa, state, symbol, new_work);
                }
                work.clear();
                if (new_work.size() > 0) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                } else {
                    return false;
                }
            }
            if (has_final_state(dfa, work)) return true;
            return false;
        }

        template <NFA_PARAMS>
        static bool 
        apply(const NFA& nfa,
              const std::vector<NFA_SYMBOL>& word) {
            typedef typename NFA::StateSet StateSet;
            StateSet work({initial_state(nfa)}), new_work;
            for (const auto& symbol : word) {
                get_targets(nfa, work, symbol, new_work);
                work.clear();
                if (new_work.size() > 0) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                } else {
                    return false;
                }
            }
            if (has_final_state(nfa, work)) return true;
            return false;
        }
    };
}

namespace atl {
    template <DFA_PARAMS>
    inline bool
    accept(const DFA& dfa,
           const std::vector<DFA_SYMBOL>& word) {
        return detail::accept_impl::apply(dfa, word);
    }

    template <DFA_PARAMS>
    inline bool
    accept(const DFA& dfa,
           const std::string& word) {
        return detail::accept_impl::apply(dfa, std::vector<char>(word.begin(), word.end()));
    }

    template <NFA_PARAMS>
    inline bool
    accept(const NFA& nfa,
           const std::vector<NFA_SYMBOL>& word) {
        return detail::accept_impl::apply(nfa, word);
    }

    template <NFA_PARAMS>
    inline bool
    accept(const NFA& nfa,
           const std::string& word) {
        return detail::accept_impl::apply(nfa, std::vector<char>(word.begin(), word.end()));
    }
}

#endif /* atl_detail_finite_automaton_algorithm_hpp */
