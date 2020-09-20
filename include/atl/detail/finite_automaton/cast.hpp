//
//  cast.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_cast_hpp 
#define atl_detail_finite_automaton_cast_hpp 

#include <queue>
#include <atl/detail/finite_automaton/closure.hpp>
#include <atl/detail/finite_automaton/merge.hpp>
#include <atl/detail/finite_automaton/copy.hpp>
#include <atl/detail/finite_automaton/deterministic_finite_automaton.hpp>
#include <atl/detail/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/automaton_utility.hpp>

using std::queue;

namespace atl::detail {
    struct determinize_impl {
        template <NFA_PARAMS, 
                  typename SymbolPropertyMerge>
        static void
        add_epsilon_transition(const NFA& a_in, 
                               NFA& a_out, 
                               typename NFA::State source,
                               typename NFA::State target,
                               typename NFA::symbol_property_type const& p,
                               typename NFA::State2Map const& state2_map,
                               SymbolPropertyMerge symbol_property_merge,
                               Direction direction = forward) {
            typedef typename NFA::symbol_property_type SymbolProperty;
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
            } else {
                if (direction == forward) {
                    auto new_source = state2_map.at(source);
                    typename NFA::OutTransitionIter o_it, o_end;
                    for (tie(o_it, o_end) = out_transitions(a_in, target); o_it != o_end; o_it++) {
                        const auto& t = atl::get_property(a_in, *o_it);
                        auto new_target = state2_map.at(atl::target(a_in, *o_it));
                        if (t.default_property == epsilon(a_in)) {
                            add_epsilon_transition(a_in, a_out, source, atl::target(a_in, *o_it),
                                                   symbol_property_merge(p, t.extended_property),
                                                   state2_map, symbol_property_merge, forward);
                        } else {
                            add_transition(a_out, new_source, new_target, t.default_property,
                                           symbol_property_merge(p, t.extended_property));
                        }
                    }
                } else {
                    auto new_target = state2_map.at(target);
                    typename NFA::InTransitionIter i_it, i_end;
                    for (tie(i_it, i_end) = in_transitions(a_in, source); i_it != i_end; i_it++) {
                        const auto& t = atl::get_property(a_in, *i_it);
                        auto new_source = state2_map.at(atl::source(a_in, *i_it));
                        if (t.default_property == epsilon(a_in)) {
                            add_epsilon_transition(a_in, a_out, atl::source(a_in, *i_it), target,
                                                   symbol_property_merge(t.extended_property, p),
                                                   state2_map, symbol_property_merge, backward);
                        } else {
                            add_transition(a_out, new_source, new_target, t.default_property,
                                           symbol_property_merge(t.extended_property, p));
                        }
                    }
                }
            }
        }

        template <NFA_PARAMS, 
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge>
        static void
        remove_epsolon_transition(const NFA& a_in, 
                                  NFA& a_out, 
                                  SymbolPropertyMerge symbol_property_merge,
                                  StatePropertyMerge state_property_merge) {
            typedef typename NFA::State State;
            typedef typename NFA::state_property_type StateProperty;
            typedef typename NFA::symbol_property_type SymbolProperty;
            typedef typename NFA::automaton_property_type AutomatonProperty;
            atl::clear(a_out);
            set_alphabet(a_out, alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }
            typename NFA::State2Map state2_map;
            typename NFA::StateSet closure;
            for (auto state_in : state_set(a_in)) {
                closure.clear();
                closure.insert(state_in);
                epsilon_closure(a_in, closure, closure);
                State state_out = -1;
                auto iter = state2_map.find(state_in);
                if (iter == state2_map.end()) {
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        state_out = add_state(a_out);
                    } else {
                        std::vector<StateProperty> props;
                        for (auto s : closure) props.push_back(atl::get_property(a_in, s));
                        state_out = add_state(a_out, state_property_merge(props.begin(), props.end()));
                    }
                    if (has_final_state(a_in, closure)) set_final_state(a_out, state_out);
                    state2_map[state_in] = state_out;
                } else {
                    state_out = iter -> second;
                }
            }
            set_initial_state(a_out, state2_map[initial_state(a_in)]);
            typename NFA::TransitionIter it, end;
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                for (tie(it, end) = transitions(a_in); it != end; it++) {
                    State source = state2_map[atl::source(a_in, *it)];
                    State target = state2_map[atl::target(a_in, *it)];
                    add_transition(a_out, source, target, atl::get_property(a_in, *it));
                }
            } else {
                for (tie(it, end) = transitions(a_in); it != end; it++) {
                    State source = atl::source(a_in, *it);
                    State target = atl::target(a_in, *it);
                    State new_source = state2_map.at(source);
                    State new_target = state2_map.at(target);
                    const auto& t = atl::get_property(a_in, *it);
                    if (t.default_property == epsilon(a_in)) {
                        add_epsilon_transition(a_in, a_out, source, target, t.extended_property,
                                               state2_map, symbol_property_merge, forward);
                        add_epsilon_transition(a_in, a_out, source, target, t.extended_property,
                                               state2_map, symbol_property_merge, backward);
                    } else {
                        add_transition(a_out, new_source, new_target, t);
                    }
                }
            }
        }

        template <NFA_PARAMS>
        static void 
        get_determinize_map(const NFA& nfa,
                            typename NFA::StateSet const& states,
                            typename NFA::Symbol2StateSetMap& map) {
            typedef typename NFA::symbol_property_type SymbolProperty;
            typename NFA::StateSet new_states;
            for (auto state : states) {
                if (transition_map(nfa).count(state) == 0) continue;
                for (const auto& [symbol, prop_map] : transition_map(nfa).at(state)) {
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        if (symbol == epsilon(nfa)) continue;
                        new_states.clear();
                        get_targets_in_map(nfa, state, symbol, new_states);
                        if (new_states.size() > 0) {
                            if (has_epsilon_transition(nfa)) {
                                epsilon_closure(nfa, new_states, new_states);
                            }
                            map[symbol].insert(new_states.begin(), new_states.end());
                        }
                    } else {
                        for (auto& map_pair : prop_map) {
                            const auto& symbol_property = map_pair.first;
                            new_states.clear();
                            get_targets_in_map(nfa, state, symbol, symbol_property, new_states);
                            if (new_states.size() > 0) {
                                map[symbol][symbol_property].insert(new_states.begin(), new_states.end());
                            }
                        }
                    }
                }
            }
        }

        template <NFA_PARAMS, 
                  typename StatePropertyMerge>
        static void 
        add_determinize_transition(const NFA& nfa,
                                   typename NFA::dfa_type& dfa,
                                   typename NFA::State source,
                                   typename NFA::StateSet const& state_set,
                                   typename NFA::StateSetMap& set_map,
                                   typename NFA::transition_property_type const& t,
                                   StatePropertyMerge state_property_merge) {
            typedef typename NFA::State State;
            typedef typename NFA::state_property_type StateProperty;
            State target = -1;
            auto set_iter = set_map.find(state_set);
            if (set_iter == set_map.end()) {
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    target = add_state(dfa);
                } else {
                    std::vector<StateProperty> props;
                    for (auto s : state_set)
                        props.push_back(atl::get_property(nfa, s));
                    target = add_state(dfa, state_property_merge(props.begin(), props.end()));
                }
                set_map[state_set] = target;
                do_determinize(nfa, dfa, target, state_set, set_map, state_property_merge);
            } else {
                target = set_iter -> second;
            }
            add_transition(dfa, source, target, t);
        }

        template <NFA_PARAMS, 
                  typename StatePropertyMerge>
        static void 
        do_determinize(const NFA& nfa,
                       typename NFA::dfa_type& dfa,
                       typename NFA::State source,
                       typename NFA::StateSet const& state_set,
                       typename NFA::StateSetMap& set_map,
                       StatePropertyMerge state_property_merge) {
            if (has_final_state(nfa, state_set)) set_final_state(dfa, source);
            typedef typename NFA::symbol_property_type SymbolProperty;
            typedef typename NFA::transition_property_type TransitionProperty;

            typename NFA::Symbol2StateSetMap map;
            get_determinize_map(nfa, state_set, map);
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                for (auto& [symbol, target] : map) {
                    add_determinize_transition(nfa, dfa, source, target, set_map, symbol, state_property_merge);
                }
            } else {
                for (auto& [symbol, prop_map] : map) {
                    for (auto& [symbol_property, target] : prop_map) {
                        add_determinize_transition(nfa, dfa, source, target, set_map, 
                                                   TransitionProperty(symbol, symbol_property), 
                                                   state_property_merge);
                    }
                }
            }
        }

        template <NFA_PARAMS, 
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge>
        static void apply(const NFA& a_in,
                          typename NFA::dfa_type& a_out,
                          SymbolPropertyMerge symbol_property_merge,
                          StatePropertyMerge state_property_merge) {
            typedef typename NFA::state_property_type StateProperty;
            typedef typename NFA::symbol_property_type SymbolProperty;
            typedef typename NFA::automaton_property_type AutomatonProperty;
            atl::clear(a_out);
            if (is_empty(a_in)) return;
            atl::set_alphabet(a_out, atl::alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }

            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                typename NFA::StateSet state_set({initial_state(a_in)});
                if (has_epsilon_transition(a_in)) {
                    epsilon_closure(a_in, state_set);
                }
                typename NFA::State initial_state = -1;
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    initial_state = add_initial_state(a_out);
                } else {
                    std::vector<StateProperty> props;
                    for (const auto& s : state_set) 
                        props.push_back(atl::get_property(a_in, s));
                    initial_state = add_initial_state(a_out, state_property_merge(props.begin(), props.end()));
                }
                typename NFA::StateSetMap state_set_map({{state_set, initial_state}});
                do_determinize(a_in, a_out, a_out.initial_state(), state_set, state_set_map, state_property_merge);
            } else {
                if (has_epsilon_transition(a_in)) {
                    NFA nfa;
                    remove_epsolon_transition(a_in, nfa, symbol_property_merge, state_property_merge);
                    typename NFA::StateSet state_set({initial_state(nfa)});
                    typename NFA::State initial_state = -1;
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        initial_state = add_initial_state(a_out);
                    } else {
                        initial_state = add_initial_state(a_out, atl::get_property(nfa, nfa.initial_state()));
                    }
                    typename NFA::StateSetMap state_set_map({{state_set, initial_state}});
                    do_determinize(nfa, a_out, a_out.initial_state(), state_set, state_set_map, state_property_merge);
                } else {
                    typename NFA::StateSet state_set({initial_state(a_in)});
                    typename NFA::State initial_state = -1;
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        initial_state = add_initial_state(a_out);
                    } else {
                        initial_state = add_initial_state(a_out, atl::get_property(a_in, a_in.initial_state()));
                    }
                    typename NFA::StateSetMap state_set_map({{state_set, initial_state}});
                    do_determinize(a_in, a_out, a_out.initial_state(), state_set, state_set_map, state_property_merge);
                }
            }
            set_forward_reachable_flag(a_out, 1);
        }
    };
}

namespace atl {
    template <NFA_PARAMS>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::dfa_type& a_out) {
        if (is_undeterministic(a_in)) {
            detail::determinize_impl::apply(a_in, a_out, 
                                            intersect_merge<typename NFA::symbol_property_type>(), 
                                            union_merge<typename NFA::state_property_type>());
        } else {
            copy_fa(a_in, a_out);
        }
    }

    template <NFA_PARAMS,
              typename SymbolPropertyMerge>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::dfa_type& a_out,
                SymbolPropertyMerge symbol_property_merge) {
        if (is_undeterministic(a_in)) {
            detail::determinize_impl::apply(a_in, a_out, symbol_property_merge, 
                                            union_merge<typename NFA::state_property_type>());
        } else {
            copy_fa(a_in, a_out);
        }
    }

    template <NFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::dfa_type& a_out,
                SymbolPropertyMerge symbol_property_merge,
                StatePropertyMerge state_property_merge) {
        if (is_undeterministic(a_in)) {
            detail::determinize_impl::apply(a_in, a_out, symbol_property_merge, state_property_merge);
        } else {
            copy_fa(a_in, a_out);
        }
    }
};

namespace atl::detail {
    struct normalize_impl {
        template <DFA_PARAMS>
        static void
        add_nomalize_transition(const DFA& a_in,
                                DFA& a_out,
                                typename DFA::State source,
                                typename DFA::State target,
                                typename DFA::transition_property_type const& t,
                                typename DFA::State2Map& state2_map,
                                std::vector<typename DFA::State>& work) {
            typedef typename DFA::state_property_type StateProperty;
            auto new_target = -1;
            if (state2_map.count(target) == 0) {
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    new_target = add_state(a_out);
                } else {
                    new_target = add_state(a_out, atl::get_property(a_in, target));
                }
                if (is_final_state(a_in, target)) set_final_state(a_out, new_target);
                state2_map[target] = new_target;
                work.push_back(target);
            } else {
                new_target = state2_map.at(target);
            }
            add_transition(a_out, source, new_target, t);
        }

        template <DFA_PARAMS>
        static void
        add_nomalize_transition(const DFA& a_in,
                                DFA& a_out,
                                typename DFA::State source,
                                typename DFA::State2Map& state2_map) {
            typedef typename DFA::State State;
            auto new_source = state2_map.at(source);
            std::vector<State> work;
            typename DFA::OutTransitionIter o_it, o_end;
            for (tie(o_it, o_end) = out_transitions(a_in, source); o_it != o_end; o_it++) {
                auto target = atl::target(a_in, *o_it);
                add_nomalize_transition(a_in, a_out, new_source, target, atl::get_property(a_in, *o_it), 
                                        state2_map, work);
            }
            for (auto state : work) {
                add_nomalize_transition(a_in, a_out, state, state2_map);
            }
        }

        template <DFA_PARAMS>
        static void
        apply(const DFA& a_in,
              DFA& a_out) {
            typedef typename DFA::automaton_property_type AutomatonProperty;
            typedef typename DFA::state_property_type StateProperty;
            clear(a_out);
            if (is_empty(a_in)) return;
            atl::set_alphabet(a_out, atl::alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }
            typename DFA::State initial_state_ = -1;
            if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                initial_state_ = add_initial_state(a_out);
            } else {
                initial_state_ = add_initial_state(a_out, atl::get_property(a_in, initial_state(a_in)));
            }
            typename DFA::State2Map state2_map({{initial_state(a_in), initial_state_}});
            if (is_final_state(a_in, initial_state(a_in))) set_final_state(a_out, initial_state(a_out));
            add_nomalize_transition(a_in, a_out, initial_state(a_in), state2_map);
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    normalize(const DFA& a_in,
              DFA& a_out) {
        detail::normalize_impl::apply(a_in, a_out);
    }
};

namespace atl::detail {
    struct minimize_impl {
        template <DFA_PARAMS>
        static void 
        remove_dead_states(const DFA& a_in,
                           DFA& a_out) {
            typename DFA::StateSet reachable_closure;
            atl::reachable_closure(a_in, reachable_closure);
            copy_fa(a_in, a_out, reachable_closure);
        }

        template <DFA_PARAMS>
        static bool 
        is_equal(const DFA& dfa,
                 typename DFA::State s1,
                 typename DFA::State s2,
                 typename DFA::State2Map const& state2_map) {
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::symbol_property_type SymbolProperty;

            const auto& transition_map_ = transition_map(dfa);
            if constexpr (!std::is_same<StateProperty, boost::no_property>::value) {
                if (atl::get_property(dfa, s1) != atl::get_property(dfa, s2)) return false;
            }
            ID count1 = transition_map_.count(s1),
               count2 = transition_map_.count(s2);
            if (count1 != count2) return false;
            if (count1 == 0) return true;
            const auto &map1 = transition_map_.at(s1);
            const auto &map2 = transition_map_.at(s2);
            if (map1.size() != map2.size()) return false;
            auto iter1 = map1.begin(), end1 = map1.end(), iter2 = map2.begin();
            while (iter1 != end1) {
                if (iter1 -> first != iter2 -> first) return false;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    if (state2_map.at(iter1 -> second) != state2_map.at(iter2 -> second)) return false;
                } else {
                    auto& map1_ = iter1 -> second;
                    auto& map2_ = iter2 -> second;
                    if (map1_.size() != map2_.size()) return false;
                    auto iter1_ = map1_.begin(), end1_ = map1_.end(), iter2_ = map2_.begin();
                    while (iter1_ != end1_) {
                        if (iter1_ -> first != iter2_ -> first) return false;
                        if (state2_map.at(iter1_ -> second) != state2_map.at(iter2_ -> second)) return false;
                        iter1_++;
                        iter2_++;
                    }
                }
                iter1++;
                iter2++;
            }
            return true;
        }

        template <DFA_PARAMS>
        static void 
        get_equivalences(const DFA& a_in, 
                         DFA& a_out,
                         queue<typename DFA::StateSet>& eqs, 
                         typename DFA::State2Map& state2_map) {
            typedef typename DFA::State State;
            typedef typename DFA::StateSet StateSet;
            typedef typename DFA::state_property_type StateProperty;

            ID cur_size = eqs.size(), last_size = 0;
            while (cur_size != last_size) {
                for (ID i = 0; i < cur_size; ++i) {
                    StateSet& set = eqs.front();
                    if (set.size() == 0) continue;
                    while (set.size() > 0) {
                        State state = *(set.begin());
                        StateSet new_eqs;
                        new_eqs.insert(state);
                        set.erase(state);
                        StateSet del_states;
                        for (auto new_state : set) {
                            if (is_equal(a_in, state, new_state, state2_map)) {
                                new_eqs.insert(new_state);
                                del_states.insert(new_state);
                            }
                        }
                        eqs.push(new_eqs);
                        for (auto del_state : del_states) {
                            set.erase(del_state);
                        }
                        State new_map_state = -1;
                        if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                            new_map_state = add_state(a_out);
                        } else {
                            new_map_state = add_state(a_out, atl::get_property(a_in, *(new_eqs.begin())));
                        }
                        for (auto new_state : new_eqs) {
                            state2_map[new_state] = new_map_state;
                        }
                    }
                    eqs.pop();
                }
                last_size = cur_size;
                cur_size = eqs.size();
            }
        }

        template <DFA_PARAMS>
        static void 
        apply(const DFA& a_in,
              DFA& a_out) {
            typedef typename DFA::State State;
            typedef typename DFA::StateSet StateSet;
            typedef typename DFA::State2Map State2Map;
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            typedef typename DFA::transition_property_type TransitionProperty;
            clear(a_out);
            if (is_empty(a_in)) return;
            StateSet unfinal_states, final_states;
            State2Map state2_map;
            DFA dfa1, dfa2;
            atl::set_alphabet(dfa2, atl::alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(dfa2, atl::get_property(a_in));
            }
            remove_dead_states(a_in, dfa1);
            State unfinal_state = add_state(dfa2), final_state = add_state(dfa2);
            for (auto state : state_set(dfa1)) {
                if (is_final_state(dfa1, state)) {
                    final_states.insert(state);
                    state2_map[state] = final_state;
                } else {
                    unfinal_states.insert(state);
                    state2_map[state] = unfinal_state;
                }
            }
            std::queue<StateSet> eqs({final_states});
            if (unfinal_states.size() > 0) eqs.push(unfinal_states);
            get_equivalences(dfa1, dfa2, eqs, state2_map);
            
            set_initial_state(dfa2, state2_map.at(initial_state(dfa1)));
            const auto& transition_map_ = transition_map(dfa1);
            for (auto& [state, source] : state2_map) {
                if (is_final_state(dfa1, state)) set_final_state(dfa2, source);
                if (transition_map_.count(state) > 0) {
                    const auto& map = transition_map_.at(state);
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        for (const auto& [symbol, target] : map) {
                            add_transition(dfa2, source, state2_map.at(target), symbol);
                        }
                    } else {
                        for (const auto& [symbol, prop_map] : map) {
                            for (const auto& [prop, target] : prop_map) {
                                add_transition(dfa2, source, state2_map.at(target), 
                                               TransitionProperty(symbol, prop));
                            }
                        }
                    }
                }
            }
            normalize(dfa2, a_out);
            set_minimal_flag(a_out, 1);
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    minimize(const DFA& a_in,
             DFA& a_out) {
        if (!is_minimal(a_in)) {
            detail::minimize_impl::apply(a_in, a_out);
        } else {
            copy_fa(a_in, a_out);
        }
    }

    template <NFA_PARAMS>
    inline void
    minimize(const NFA& a_in,
             typename NFA::dfa_type& a_out) {
        typename NFA::dfa_type dfa;
        determinize(a_in, dfa, intersect_merge<typename NFA::symbol_property_type>(), 
                    union_merge<typename NFA::state_property_type>());
        detail::minimize_impl::apply(dfa, a_out);
    }

    template <NFA_PARAMS,
              typename SymbolPropertyMerge>
    inline void
    minimize(const NFA& a_in,
             typename NFA::dfa_type& a_out,
             SymbolPropertyMerge symbol_property_merge) {
        typename NFA::dfa_type dfa;
        determinize(a_in, dfa, symbol_property_merge, 
                    union_merge<typename NFA::state_property_type>());
        detail::minimize_impl::apply(dfa, a_out);
    }

    template <NFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge>
    inline void
    minimize(const NFA& a_in,
             typename NFA::dfa_type& a_out,
             SymbolPropertyMerge symbol_property_merge,
             StatePropertyMerge state_property_merge) {
        typename NFA::dfa_type dfa;
        determinize(a_in, dfa, symbol_property_merge, state_property_merge);
        detail::minimize_impl::apply(dfa, a_out);
    }
}

#endif /* atl_detail_finite_automaton_cast_hpp */
