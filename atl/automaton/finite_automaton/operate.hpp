//
//  operate.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/12/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_operate_hpp 
#define atl_finite_automaton_operate_hpp 

#include <queue>
#include "../detail/automaton.hpp"
#include "deterministic_finite_automaton.hpp"
#include "nondeterministic_finite_automaton.hpp"
#include "closure.hpp"
#include "merge.hpp"
using std::queue;
using std::cout;
using std::endl;

namespace atl {
    struct intersect_impl {
        template <typename DFA,
                  typename Merge>
        static void 
        get_intersect_map(const DFA& a_lhs,
                          const DFA& a_rhs,
                          typename DFA::Symbol2StatePairMap& map,
                          typename DFA::Symbol2StateMap const& map_lhs,
                          typename DFA::Symbol2StateMap const& map_rhs,
                          Merge merge,
                          bool reverse = false) {
            typedef typename DFA::StatePair StatePair;
            typedef typename DFA::symbol_property_type SymbolProperty;
            for (const auto& map_pair_lhs : map_lhs) {
                auto iter_rhs = map_rhs.find(map_pair_lhs.first);
                if (iter_rhs != map_rhs.end()) {
                    const auto& map_pair_rhs = *iter_rhs;
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        if (!reverse) {
                            map[map_pair_lhs.first] = StatePair(map_pair_lhs.second, 
                                                                map_pair_rhs.second);
                        } else {
                            map[map_pair_lhs.first] = StatePair(map_pair_rhs.second, 
                                                                map_pair_lhs.second);
                        }
                    } else {
                        for (const auto& map_pair_lhs1 : map_pair_lhs.second) {
                            for (const auto& map_pair_rhs1 : map_pair_rhs.second) {
                                if (!reverse) {
                                    map[map_pair_lhs.first][merge(map_pair_lhs1.first,
                                                                  map_pair_rhs1.first)] =
                                    StatePair(map_pair_lhs1.second, 
                                              map_pair_rhs1.second);
                                } else {
                                    map[map_pair_lhs.first][merge(map_pair_rhs1.first,
                                                                  map_pair_lhs1.first)] =
                                    StatePair(map_pair_rhs1.second, 
                                              map_pair_lhs1.second);
                                }
                            }
                        }
                    }
                }
            }
        }

        template <typename DFA,
                  typename Merge>
        static void 
        get_intersect_map(const DFA& a_lhs,
                          const DFA& a_rhs,
                          typename DFA::State state_lhs,
                          typename DFA::State state_rhs,
                          typename DFA::Symbol2StatePairMap& map,
                          Merge merge) {
            typedef typename DFA::StatePair StatePair;
            const auto& transition_map_lhs = a_lhs.transition_map();
            const auto& transition_map_rhs = a_rhs.transition_map();
            auto iter_lhs = transition_map_lhs.find(state_lhs);
            auto iter_rhs = transition_map_rhs.find(state_rhs);
            if (iter_lhs != transition_map_lhs.end() && iter_rhs != transition_map_rhs.end()) {
                const auto& map_lhs = iter_lhs -> second;
                const auto& map_rhs = iter_rhs -> second;
                if (map_lhs.size() <= map_rhs.size()) {
                    get_intersect_map(a_lhs, a_rhs, map, map_lhs, map_rhs, merge);
                } else {
                    get_intersect_map(a_lhs, a_rhs, map, map_rhs, map_lhs, merge, 1);
                }
            }
        }

        template <typename DFA,
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void
        add_intersect_transition(const DFA& a_lhs,
                                 const DFA& a_rhs,
                                 DFA& a_out,
                                 typename DFA::State source,
                                 typename DFA::StatePair const& state_pair,
                                 typename DFA::StatePairMap& pair_map,
                                 typename DFA::transition_property_type const& t,
                                 StateMerge state_merge,
                                 SymbolPropertyMerge symbol_property_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::state_property_type StateProperty;
            auto pair_iter = pair_map.find(state_pair);
            State target = -1;
            if (pair_iter != pair_map.end()) {
                target = pair_iter -> second;
            } else {
                State new_state_lhs = state_pair.first;
                State new_state_rhs = state_pair.second;
                if constexpr (std::is_same<StateProperty, 
                                           boost::no_property>::value) {
                    target = add_state(a_out);
                } else {
                    target = add_state(a_out, 
                                   state_merge(atl::get_property(a_lhs, new_state_lhs), 
                                               atl::get_property(a_rhs, new_state_rhs)));
                }
                pair_map[state_pair] = target;
                do_intersect(a_lhs, a_rhs, a_out, 
                             new_state_lhs, new_state_rhs, target,
                             pair_map, state_merge, symbol_property_merge);
            }
            add_transition(a_out, source, target, t);
        }

        template <typename DFA,
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void 
        do_intersect(const DFA& a_lhs,
                     const DFA& a_rhs,
                     DFA& a_out,
                     typename DFA::State state_lhs,
                     typename DFA::State state_rhs,
                     typename DFA::State state_out,
                     typename DFA::StatePairMap& pair_map,
                     StateMerge state_merge,
                     SymbolPropertyMerge symbol_property_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::transition_property_type TransitionProperty;
            if (is_final_state(a_lhs, state_lhs) && is_final_state(a_rhs, state_rhs)) 
                set_final_state(a_out, state_out);
            const auto& map_lhs = a_lhs.transition_map();
            const auto& map_rhs = a_rhs.transition_map();
            typename DFA::Symbol2StatePairMap map;
            get_intersect_map(a_lhs, a_rhs, state_lhs, state_rhs, map, symbol_property_merge);
            for (auto& map_pair : map) {
                auto& symbol = map_pair.first;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    auto& state_pair = map_pair.second;
                    add_intersect_transition(a_lhs, a_rhs, a_out,
                                             state_out, state_pair, pair_map, symbol,
                                             state_merge, symbol_property_merge);
                } else {
                    for (auto& map_pair1 : map_pair.second) {
                        auto& symbol_property = map_pair1.first;
                        auto& state_pair = map_pair1.second;
                        add_intersect_transition(a_lhs, a_rhs, a_out,
                                                 state_out, state_pair, pair_map,
                                                 TransitionProperty(symbol, symbol_property),
                                                 state_merge, symbol_property_merge);
                    }
                }
            }
        }

        template <typename DFA,
                  typename StateMerge,
                  typename SymbolPropertyMerge,
                  typename FAMerge>
        static void 
        apply(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              StateMerge state_merge,
              SymbolPropertyMerge symbol_property_merge,
              FAMerge fa_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::StatePair StatePair;
            typedef typename DFA::StatePairMap& StatePairMap;
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            if constexpr (!std::is_same<AutomatonProperty, no_type>::value) {
                atl::set_property(a_out, fa_merge(atl::get_property(a_lhs), 
                                                  atl::get_property(a_rhs)));
            }
            
            State initial_state_lhs = initial_state(a_lhs),
                  initial_state_rhs = initial_state(a_rhs),
                  initial_state_out = -1;
            if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                  initial_state_out = add_initial_state(a_out);
            } else {
                  initial_state_out = add_initial_state(a_out, 
                                      state_merge(atl::get_property(a_lhs, initial_state_lhs),
                                                  atl::get_property(a_rhs, initial_state_rhs)));
            }

            StatePairMap pair_map({{StatePair(initial_state_lhs, initial_state_lhs),
                                    initial_state_out}}); 
            do_intersect(a_lhs, a_rhs, a_out, 
                         initial_state_lhs, initial_state_rhs, 
                         a_out.initial_state(), pair_map, state_merge, symbol_property_merge);
        }
    };

    template <typename DFA,
              typename StateMerge,
              typename SymbolPropertyMerge,
              typename FAMerge>
    inline void
    intersect_fa(const DFA& a_lhs,
                 const DFA& a_rhs,
                 DFA& a_out,
                 StateMerge state_merge = intersect_merge<typename DFA::state_property_type>(),
                 SymbolPropertyMerge symbol_property_merge = 
                    intersect_merge<typename DFA::symbol_property_merge>(),
                 FAMerge fa_merge = intersect_merge<typename DFA::automaton_property_type>()) {
        intersect_impl::apply(a_lhs, a_rhs, a_out, state_merge, symbol_property_merge, fa_merge);
    }

    template <typename DFA,
              typename StateMerge,
              typename SymbolPropertyMerge,
              typename FAMerge>
    inline DFA 
    get_intersect_fa(const DFA& a_lhs,
                     const DFA& a_rhs,
                     StateMerge state_merge,
                     SymbolPropertyMerge symbol_property_merge,
                     FAMerge fa_merge) {
        DFA a_out;
        intersect_impl::apply(a_lhs, a_rhs, a_out, state_merge, symbol_property_merge, fa_merge);
        return a_out;
    }

    template <typename DFA>
    inline DFA 
    get_intersect_fa(const DFA& a_lhs,
                     const DFA& a_rhs) {
        DFA a_out;
        intersect_impl::apply(a_lhs, a_rhs, a_out, 
                              intersect_merge<typename DFA::state_property_type>(),
                              intersect_merge<typename DFA::symbol_property_type>(),
                              intersect_merge<typename DFA::automaton_property_type>());
        return minimize(a_out);
    }
}

#endif /* atl_finite_automaton_operate_hpp */

