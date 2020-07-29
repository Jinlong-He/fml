//
//  copy.hpp
//  atl 
//
//  Created by Jinlong He on 2019/11/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_copy_hpp 
#define atl_finite_automaton_copy_hpp 

#include "../detail/finite_automaton/finite_automaton.hpp"

namespace atl {
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
            //auto& states_copy = states.size() > 0 ? states : state_set(a_in);
            if (states.size() == 0) {
                typename FA::StateIter it, end;
                for (tie(it, end) = atl::states(a_in); it != end; it++) {
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        state_copy = add_state(a_out);
                    } else {
                        state_copy = add_state(a_out, atl::get_property(a_in, *it));
                    }
                    state2_map[*it] = state_copy;
                }
            } else {
                for (auto state : states) {
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        state_copy = add_state(a_out);
                    } else {
                        state_copy = add_state(a_out, atl::get_property(a_in, state));
                    }
                    state2_map[state] = state_copy;
                }
            }
            set_initial_state(a_out, state2_map.at(initial_state(a_in)));
            for (auto state : final_state_set(a_in)) {
                set_final_state(a_out, state2_map.at(state));
            }
        }

        template <FA_PARAMS>
        static void 
        copy_transitions(const FA& a_in,
                         FA& a_out,
                         typename FA::State2Map& state2_map) {
            typename FA::OutTransitionIter t_it, t_end;
            for (auto state : state_set(a_in)) {
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

#endif /* atl_finite_automaton_copy_hpp */
