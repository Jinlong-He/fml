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
#include "../detail/push_down_system/push_down_system.hpp"
using std::cout;
using std::endl;

namespace atl {
    struct pre_star_impl {

        template <PDS_PARAMS,
                  NFA_PARAMS,
                  typename Merge>
        void
        do_pre_star_0(NFA& nfa_out,
                      
                ) {
        }

        template <PDS_PARAMS,
                  NFA_PARAMS,
                  typename Merge>
        void
        apply(const PDS& pds,
              const NFA& nfa_in,
              NFA& nfa_out,
              typename PDS::State2Map const& state2_map) {
            //remove_epsilon.
            copy_fa(nfa_in, nfa_out);
            typedef typename PDS::State State;
            for (auto& map_pair : transition_map(pds)) {
                State source = map_pair.first;
                for (auto& map_pair1 : map_pair) {
                    auto symbol = map_pair1.first;
                    for (auto& map_pair2 : map_pair1) {
                        State target = map_pair2.first;
                        for (auto& stack : map_pair2.second) {
                            if (stack.size() == 0) {
                            } else if (stack.size() == 1) {
                            } else {
                            }
                        }
                    }
                }
            }
        }
        
    };
}

#endif /* atl_finite_automaton_operate_hpp */

