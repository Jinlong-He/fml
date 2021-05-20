//
//  algorithm.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_algorithm_hpp 
#define atl_detail_algorithm_hpp

#include <atl/detail/automaton.hpp>
#include <atl/detail/finite_automaton/merge.hpp>

using std::unordered_map;
namespace atl::detail {
    #define AUTOMATON_PARAMS1 typename TP1, typename SP1, typename AP1
    #define AUTOMATON1 detail::automaton_gen<TP1, SP1, AP1>
    #define AUTOMATON_PARAMS2 typename TP2, typename SP2, typename AP2
    #define AUTOMATON2 detail::automaton_gen<TP2, SP2, AP2>

    struct product_impl {
        template <AUTOMATON_PARAMS1,
                  AUTOMATON_PARAMS2,
                  AUTOMATON_PARAMS,
                  class Equal,
                  class TransitionMerge,
                  class StateMerge,
                  class AutomatonMerge>
        static void
        apply(const AUTOMATON1& a1,
              const AUTOMATON2& a2,
              AUTOMATON& a_out,
              Equal equal,
              TransitionMerge transition_merge,
              StateMerge state_merge,
              AutomatonMerge automaton_merge) {
            typedef typename AUTOMATON::State State;
            typedef typename AUTOMATON::StateIter StateIter;
            typedef typename AUTOMATON::OutTransitionIter OutTransitionIter;
            StateIter s_it1, s_end1, s_it2, s_end2;
            OutTransitionIter t_it1, t_end1, t_it2, t_end2;
            unordered_map<pair<State, State>, State> state2_map;
            atl::set_property(a_out, automaton_merge(atl::get_property(a1),
                                                     atl::get_property(a2)));
            for (tie(s_it1, s_end1) = states(a1); s_it1 != s_end1; s_it1++) {
                tie(t_it1, t_end1) = out_transitions(a1, *s_it1);
                if (t_it1 == t_end1) continue;
                for ( ; t_it1 != t_end1; t_it1++) {
                    auto& p1 = atl::get_property(a1, *t_it1);
                    for (tie(s_it2, s_end2) = states(a2); s_it2 != s_end2; s_it2++) {
                        tie(t_it2, t_end2) = out_transitions(a2, *s_it2);
                        if (t_it2 == t_end2) continue;
                        for ( ; t_it2 != t_end2; t_it2++) {
                            auto& p2 = atl::get_property(a2, *t_it2);
                            if (equal(p1, p2)) {
                                auto state_pair = pair(*s_it1, *s_it2);
                                auto pair_iter = state2_map.find(state_pair);
                                State source = -1, target = -1;
                                if (pair_iter != state2_map.end()) {
                                    source = pair_iter -> second;
                                } else {
                                    source = add_state(a_out, 
                                      state_merge(atl::get_property(a1, *s_it1),
                                                  atl::get_property(a2, *s_it2)));
                                    state2_map[state_pair] = source;
                                }
                                auto v_it1 = atl::target(a1, *t_it1);
                                auto v_it2 = atl::target(a2, *t_it2);
                                state_pair = pair(v_it1, v_it2);
                                pair_iter = state2_map.find(state_pair);
                                if (pair_iter != state2_map.end()) {
                                    target = pair_iter -> second;
                                } else {
                                    target = add_state(a_out, 
                                      state_merge(atl::get_property(a1, v_it1),
                                                  atl::get_property(a2, v_it2)));
                                    state2_map[state_pair] = target;
                                }
                                add_transition(a_out, source, target,
                                        transition_merge(p1, p2));
                            }
                        }
                    }
                }
            }
        }
    };

    template <AUTOMATON_PARAMS1,
              AUTOMATON_PARAMS2,
              AUTOMATON_PARAMS>
    inline void
    product(const AUTOMATON1& a1,
            const AUTOMATON2& a2,
            AUTOMATON& a_out) {
        detail::product_impl::apply(a1, a1, a_out, 
                                    equal<TP1, TP2>(),
                                    merge<TP1, TP2, TP>(), 
                                    merge<SP1, SP2, SP>(), 
                                    merge<AP1, AP2, AP>());
    }
}

#endif /* atl_detail_algorithm_hpp */

