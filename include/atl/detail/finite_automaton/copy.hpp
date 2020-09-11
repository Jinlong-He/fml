//
//  copy.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_copy_hpp 
#define atl_detail_finite_automaton_copy_hpp 

#include <atl/detail/finite_automaton/finite_automaton.hpp>

namespace atl::detail {
    struct copy_fa_impl {
        template <FA_PARAMS>
        static void 
        copy_states(const FA& a_in,
                    FA& a_out,
                    typename FA::State2Map& state2_map,
                    typename FA::StateSet const& state_set) {
            typedef typename FA::state_property_type StateProperty;
            typedef typename FA::State State;
            typename FA::StateIter s_it, s_end;
            State state_copy = -1;
            if (state_set.size() > 0) {
                for (auto state : state_set) {
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        state_copy = add_state(a_out);
                    } else {
                        state_copy = add_state(a_out, atl::get_property(a_in, state));
                    }
                    state2_map[state] = state_copy;
                }
            } else {
                for (tie(s_it, s_end) = states(a_in); s_it != s_end; s_it++) {
                    auto state = *s_it;
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
            for (const auto& [state, source] : state2_map) {
                for (tie(t_it, t_end) = out_transitions(a_in, state); t_it != t_end; t_it++) {
                    auto target = atl::target(a_in, *t_it);
                    if (state2_map.count(target) > 0) {
                        auto new_target = state2_map.at(target);
                        const auto& t = atl::get_property(a_in, *t_it);
                        add_transition(a_out, source, new_target, t);
                    }
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
            if (is_empty(a_in)) return;
            set_alphabet(a_out, alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }
            copy_states(a_in, a_out, state2_map, states);
            copy_transitions(a_in, a_out, state2_map);
        }
    };
};

namespace atl {
    template <FA_PARAMS>
    inline void
    copy_fa(const FA& a_in,
            FA& a_out,
            typename FA::State2Map& state2_map,
            typename FA::StateSet const& states = typename FA::StateSet()) {
        detail::copy_fa_impl::apply(a_in, a_out, state2_map, states);
    }

    template <FA_PARAMS>
    inline void
    copy_fa(const FA& a_in,
            FA& a_out,
            typename FA::StateSet const& states = typename FA::StateSet()) {
        typename FA::State2Map state2_map;
        detail::copy_fa_impl::apply(a_in, a_out, state2_map, states);
    }
}

#endif /* atl_detail_finite_automaton_copy_hpp */
