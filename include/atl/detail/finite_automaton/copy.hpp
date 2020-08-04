//
//  copy.hpp
//  atl 
//
//  Created by Jinlong He on 2019/11/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_finite_automaton_copy_hpp 
#define atl_detail_finite_automaton_copy_hpp 

#include "finite_automaton.hpp"

namespace atl::detail {
    struct copy_fa_impl {
        template <FA_PARAMS>
        static void 
        copy_states(const FA& a_in,
                    FA& a_out,
                    typename FA::State2Map& state2_map,
                    typename FA::StateSet const& states) {
            typedef typename FA::state_property_type StateProperty;
            typedef typename FA::State State;

            State state_copy = -1;
            auto& states_copy = states.size() > 0 ? states : state_set(a_in);
            for (auto state : states_copy) {
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    state_copy = add_state(a_out);
                } else {
                    state_copy = add_state(a_out, atl::get_property(a_in, state));
                }
                state2_map[state] = state_copy;
            }
            set_initial_state(a_out, state2_map.at(initial_state(a_in)));
            for (auto state : final_state_set(a_in)) {
                if (state2_map.count(state) > 0) {
                    set_final_state(a_out, state2_map.at(state));
                }
            }
        }

        template <FA_PARAMS>
        static void 
        copy_transitions(const FA& a_in,
                         FA& a_out,
                         typename FA::State2Map const& state2_map) {
            typename FA::OutTransitionIter t_it, t_end;
            for (const auto map_pair : state2_map) {
                auto state = map_pair.first;
                for (tie(t_it, t_end) = out_transitions(a_in, state); t_it != t_end; t_it++) {
                    auto source = state2_map.at(state);
                    auto target = state2_map.at(atl::target(a_in, *t_it));
                    const auto& t = atl::get_property(a_in, *t_it);
                    add_transition(a_out, source, target, t);
                }
            }
        }

        template <FA_PARAMS>
        static void 
        apply(const FA& a_in,
              FA& a_out,
              typename FA::State2Map& state2_map,
              typename FA::StateSet const& states) {
            typedef typename FA::automaton_property_type AutomatonProperty;
            atl::clear(a_out);
            set_alphabet(a_out, alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }
            copy_states(a_in, a_out, state2_map, states);
            copy_transitions(a_in, a_out, state2_map);
        }
    };

    template <FA_PARAMS>
    inline void
    copy_fa(const FA& a_in,
            FA& a_out,
            typename FA::State2Map& state2_map,
            typename FA::StateSet const& states = typename FA::StateSet()) {
        copy_fa_impl::apply(a_in, a_out, state2_map, states);
    }

    template <FA_PARAMS>
    inline void
    copy_fa(const FA& a_in,
            FA& a_out,
            typename FA::StateSet const& states = typename FA::StateSet()) {
        typename FA::State2Map state2_map;
        copy_fa_impl::apply(a_in, a_out, state2_map, states);
    }
}

#endif /* atl_detail_finite_automaton_copy_hpp */
