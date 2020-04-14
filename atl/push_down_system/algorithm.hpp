//
//  algorithm.hpp
//  atl 
//
//  Created by Jinlong He on 2019/12/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_operate_hpp 
#define atl_finite_automaton_operate_hpp 

#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
#include "../finite_automaton/copy.hpp"
#include "../finite_automaton/closure.hpp"
#include "../detail/push_down_system/push_down_system.hpp"
using std::cout;
using std::endl;

namespace atl {

    struct pds_post_star_impl {

        template <PDS_PARAMS,
                  NFA_PARAMS>
        static void
        apply(const PDS& pds,
              const NFA& nfa_in,
              NFA& nfa_out,
              typename PDS::State2Map& state2_map) {
            typedef typename PDS::State State;
            typedef typename PDS::StateSet StateSet;
            typedef typename PDS::symbol_type Symbol;
            copy_fa(nfa_in, nfa_out);
            unordered_map<pair<State, Symbol>, State> mid_state_map;
            for (auto state : state_set(pds)) {
                if (state2_map.count(state) == 0) {
                    state2_map[state] = add_state(nfa_out);
                }
                for (auto& symbol : alphabet(pds)) {
                    mid_state_map[pair<State, Symbol>(state, symbol)] = add_state(nfa_out);
                }
            }

            ID count = 1;
            while (count > 0) {
                count = 0;
                for (const auto& map_pair : transition_map(pds)) {
                    State state = state2_map.at(map_pair.first);
                    for (const auto& map_pair1 : map_pair.second) {
                        auto& symbol = map_pair1.first;
                        StateSet targets;
                        get_targets(nfa_out, state, symbol, targets);
                        for (const auto& map_pair2 : map_pair1.second) {
                            State source = state2_map.at(map_pair2.first);
                            for (const auto& stack : map_pair2.second) {
                                for (const auto target : targets) {
                                    if (stack.size() == 0) {
                                        if (add_transition(nfa_out, source, target, 
                                                           epsilon(nfa_out)).second)
                                            count++;
                                    } else if (stack.size() == 1) {
                                        if (add_transition(nfa_out, source, target, 
                                                           stack[0]).second)
                                            count++;
                                    } else {
                                        State mid = mid_state_map.at(pair<State, Symbol>
                                                                      (source, stack[0]));
                                        if (add_transition(nfa_out, source, mid, 
                                                           stack[0]).second)
                                            count++;
                                        if (add_transition(nfa_out, mid, target, 
                                                           stack[1]).second)
                                            count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };

    template <PDS_PARAMS,
              NFA_PARAMS>
    inline void
    pds_post_star(const PDS& pds,
                  const NFA& nfa_in,
                  NFA& nfa_out,
                  typename PDS::State2Map& state2_map) {
        pds_post_star_impl::apply(pds, nfa_in, nfa_out, state2_map);
    }

    template <PDS_PARAMS,
              NFA_PARAMS>
    inline void
    pds_post_star(const PDS& pds,
                  const NFA& nfa_in,
                  NFA& nfa_out) {
        typename PDS::State2Map state2_map;
        for (auto state : state_set(nfa_in)) {
            state2_map[state] = state;
        }
        pds_post_star(pds, nfa_in, nfa_out, state2_map);
    }

    //struct pre_star_impl {

    //    template <PDS_PARAMS,
    //              NFA_PARAMS,
    //              typename Merge>
    //    void
    //    apply(const PDS& pds,
    //          const NFA& nfa_in,
    //          NFA& nfa_out,
    //          typename PDS::State2Map const& state2_map) {
    //        //remove_epsilon.
    //        copy_fa(nfa_in, nfa_out);
    //        typedef typename PDS::State State;
    //        for (auto& map_pair : transition_map(pds)) {
    //            State source = map_pair.first;
    //            for (auto& map_pair1 : map_pair) {
    //                auto symbol = map_pair1.first;
    //                for (auto& map_pair2 : map_pair1) {
    //                    State target = map_pair2.first;
    //                    for (auto& stack : map_pair2.second) {
    //                        if (stack.size() == 0) {
    //                        } else if (stack.size() == 1) {
    //                        } else {
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //};
}

#endif /* atl_finite_automaton_operate_hpp */

