//
//  closure.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_closure_hpp 
#define atl_detail_finite_automaton_closure_hpp 

#include <fml/util/util.hpp>
#include <fml/atl/detail/automaton.hpp>
#include <fml/atl/detail/finite_automaton/nondeterministic_finite_automaton.hpp>

namespace atl::detail {
    enum Direction {forward, backward};
    struct reachable_closure_impl {
        template <FA_PARAMS>
        static void apply(const FA& fa,
                          typename FA::StateSet const& states_in,
                          typename FA::StateSet& states_out,
                          Direction direction = forward) {
            typedef typename FA::State State;
            typedef typename FA::StateSet StateSet;
            typedef typename FA::InTransitionIter InTransitionIter;
            typedef typename FA::OutTransitionIter OutTransitionIter;
            states_out.insert(states_in.begin(), states_in.end());
            StateSet work(states_in), new_work;
            while (work.size() > 0) {
                for (auto state : work) {
                    if (direction == forward) {
                        OutTransitionIter first, last;
                        tie(first, last) = out_transitions(fa, state);
                        for (; first != last; first++) {
                            State new_state = target(fa, *first);
                            if (states_out.insert(new_state).second) {
                                new_work.insert(new_state);
                            }
                        }
                    } else {
                        InTransitionIter first, last;
                        tie(first, last) = in_transitions(fa, state);
                        for (; first != last; first++) {
                            State new_state = source(fa, *first);
                            if (states_out.insert(new_state).second) {
                                new_work.insert(new_state);
                            }
                        }
                    }
                }
                work.clear();
                if (new_work.size() > 0) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                }
            }
        }
    };
};

namespace atl {
    template <FA_PARAMS>
    inline void
    reachable_closure(const FA& a,
                      typename FA::StateSet const& states_in,
                      typename FA::StateSet& states_out,
                      detail::Direction direction = detail::forward) {
        detail::reachable_closure_impl::apply(a, states_in, states_out, direction);
    }

    template <FA_PARAMS>
    inline void
    reachable_closure(const FA& fa,
                      typename FA::StateSet& reachable_closure) {
            typename FA::StateSet forward_states({initial_state(fa)}),
                                  backward_states(final_state_set(fa));
            atl::reachable_closure(fa, forward_states, forward_states, detail::forward);
            atl::reachable_closure(fa, backward_states, backward_states, detail::backward);
            util::set_intersection(forward_states, backward_states, reachable_closure);
    }

    template <FA_PARAMS>
    inline bool
    is_empty(const FA& a) {
        if (initial_state(a) == -1) return true;
        if (is_forward_reachable(a)) {
            if (a.final_state_set().size() == 0) return true;
            return false;
        } else {
            typename FA::StateSet states;
            reachable_closure(a, states);
            if (!has_final_state(a, states)) return true;
            return false;
        }
    }
}

namespace atl::detail {
    struct epsilon_closure_impl {
        template <NFA_PARAMS>
        static void apply(const NFA& nfa,
                          typename NFA::StateSet const& states_in,
                          typename NFA::StateSet& states_out) {
            typedef typename NFA::StateSet StateSet;
            states_out = states_in;
            if (!has_epsilon_transition(nfa)) return;
            StateSet work(states_in), new_work, targets;
            while (work.size()) {
                targets.clear();
                get_targets_in_map(nfa, work, nfa.epsilon(), targets);
                for (auto new_state : targets) {
                    if (states_out.insert(new_state).second) {
                        new_work.insert(new_state);
                    }
                }
                work.clear();
                if (new_work.size()) {
                    work.insert(new_work.begin(), new_work.end());
                    new_work.clear();
                }
            }
        }
    };
}

namespace atl {
    template <NFA_PARAMS>
    inline void
    epsilon_closure(const NFA& fa,
                    typename NFA::StateSet const& states_in,
                    typename NFA::StateSet& states_out) {
        detail::epsilon_closure_impl::apply(fa, states_in, states_out);
    }

    template <NFA_PARAMS>
    inline void
    epsilon_closure(const NFA& fa,
                    typename NFA::StateSet& closure) {
        closure.insert(fa.initial_state());
        epsilon_closure(fa, closure, closure);
    }

    template <NFA_PARAMS>
    inline void
    get_targets(const NFA& nfa, 
                typename NFA::State s, 
                const NFA_SYMBOL& c, 
                typename NFA::StateSet& targets) {
        typename NFA::StateSet closure({s});
        epsilon_closure(nfa, closure, closure);
        get_targets_in_map(nfa, closure, c, targets);
        epsilon_closure(nfa, targets, targets);
    }

    template <NFA_PARAMS>
    inline void
    get_targets(const NFA& nfa, 
                typename NFA::StateSet const& states, 
                const NFA_SYMBOL& c, 
                typename NFA::StateSet& targets) {
        typename NFA::StateSet closure(states);
        epsilon_closure(nfa, closure, closure);
        get_targets_in_map(nfa, closure, c, targets);
        epsilon_closure(nfa, targets, targets);
    }
}

#endif /* atl_detail_finite_automaton_closure_hpp */
