//
//  cast.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/12.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_finite_automaton_cast_hpp 
#define atl_finite_automaton_cast_hpp 

#include <queue>
#include "../detail/finite_automaton/deterministic_finite_automaton.hpp"
#include "../detail/finite_automaton/nondeterministic_finite_automaton.hpp"
#include "closure.hpp"
#include "merge.hpp"
#include "copy.hpp"
using std::queue;
using std::cout;
using std::endl;

namespace atl {
    struct determinize_impl {
        template <typename NFA, 
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void
        add_epsilon_transition(const NFA& a_in, 
                               NFA& a_out, 
                               typename NFA::State source,
                               typename NFA::State target,
                               typename NFA::symbol_property_type const& p,
                               SymbolPropertyMerge symbol_property_merge,
                               Direction direction = forward) {
            if (direction == forward) {
                typename NFA::OutTransitionIter o_it, o_end;
                for (tie(o_it, o_end) = out_transitions(a_in, target); o_it != o_end; o_it++) {
                    const auto& t = atl::get_property(a_in, *o_it);
                    auto new_target = atl::target(a_in, *o_it);
                    if (t.symbol == epsilon(a_in)) {
                        add_epsilon_transition(a_in, a_out, source, new_target,
                                               symbol_property_merge(p, t.symbol_property),
                                               symbol_property_merge, forward);
                    } else {
                        add_transition(a_out, source, new_target,
                                       symbol_property_merge(p, t.symbol_property));
                    }
                }
            } else {
                typename NFA::InTransitionIter i_it, i_end;
                for (tie(i_it, i_end) = in_transitions(a_in, source); i_it != i_end; i_it++) {
                    const auto& t = atl::get_property(a_in, *i_it);
                    auto new_target = atl::target(a_in, *i_it);
                    if (t.symbol == epsilon(a_in)) {
                        add_epsilon_transition(a_in, a_out, source, new_target,
                                               symbol_property_merge(t.symbol_property, p),
                                               symbol_property_merge, backward);
                    } else {
                        add_transition(a_out, source, new_target,
                                       symbol_property_merge(t.symbol_property, p));
                    }
                }
            }
        }

        template <typename NFA, 
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void
        remove_epsolon_transition(const NFA& a_in, 
                                  NFA& a_out, 
                                  StateMerge state_merge,
                                  SymbolPropertyMerge symbol_property_merge) {
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
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                typename NFA::StateSet set;
                typename NFA::StateSetMap set_map;
                for (auto state_in : state_set(a_in)) {
                    set.clear();
                    set.insert(state_in);
                    epsilon_closure(state_in, set, a_in);
                    State state_out = -1;
                    auto iter = set_map.find(set);
                    if (iter == set_map.end()) {
                        if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                            state_out = add_state(a_out);
                        } else {
                            std::vector<StateProperty> props;
                            for (auto s : set)
                                props.push_back(atl::get_property(a_in, s));
                            state_out = add_state(a_out, state_merge(props));
                        }
                        if (has_final_state(a_in, set)) set_final_state(a_out, state_out);
                        set_map[set] = state_out;
                    } else {
                        state_out = iter -> second;
                    }
                    state2_map[state_in] = state_out;
                }
                set_initial_state(a_out, state2_map[initial_state(a_in)]);

                typename NFA::TransitionIter it, end;
                for (tie(it, end) = transitions(a_in); it != end; it++) {
                    State source = state2_map[atl::source(a_in, *it)];
                    State target = state2_map[atl::target(a_in, *it)];
                    add_transition(a_out, source, target, atl::get_property(a_in, *it));
                }
            } else {
                for (auto state_in : state_set(a_in)) {
                    State state_out = -1;
                    if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                        state_out = add_state(a_out);
                    } else {
                        state_out = add_state(a_out, atl::get_property(a_in, state_in));
                    }
                    if (is_final_state(a_in, state_in)) set_final_state(a_out, state_out);
                    state2_map[state_in] = state_out;
                }
                set_initial_state(a_out, state2_map[initial_state(a_in)]);
                typename NFA::TransitionIter it, end;
                for (tie(it, end) = transitions(a_in); it != end; it++) {
                    State source = state2_map[atl::source(a_in, *it)];
                    State target = state2_map[atl::target(a_in, *it)];
                    const auto& t = atl::get_property(a_in, *it);
                    if (t.symbol == epsilon(a_in)) {
                        add_epsilon_transition(a_in, a_out, source, target, 
                                               symbol_property_merge, forward);
                        add_epsilon_transition(a_in, a_out, source, target, 
                                               symbol_property_merge, backward);
                    } else {
                        add_transition(a_out, source, target, t);
                    }
                }
            }
        }

        template <typename NFA>
        static void 
        get_determinize_map(const NFA& nfa,
                            typename NFA::StateSet const& states,
                            typename NFA::Symbol2StateSetMap& map) {
            typedef typename NFA::symbol_property_type SymbolProperty;
            typename NFA::StateSet new_states;
            for (const auto& state : states) {
                if (transition_map(nfa).count(state) == 0) continue;
                for (auto& map_pair : transition_map(nfa).at(state)) {
                    const auto& symbol = map_pair.first;
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        if (symbol == nfa.epsilon()) continue;
                        new_states.clear();
                        get_targets_in_map(nfa, state, symbol, new_states);
                        if (new_states.size() > 0) {
                            epsilon_closure(nfa, new_states, new_states);
                            map[symbol].insert(new_states.begin(), new_states.end());
                        }
                    } else {
                        for (auto& map_pair1 : map_pair.second) {
                            const auto& symbol_property = map_pair1.first;
                            new_states.clear();
                            get_targets_in_map(nfa, state, symbol, symbol_property, new_states);
                            if (new_states.size() > 0) {
                                map[symbol][symbol_property].insert(new_states.begin(), 
                                                                    new_states.end());
                            }
                        }
                    }
                }
            }
        }

        template <typename NFA, 
                  typename DFA,
                  typename StateMerge>
        static void 
        add_determinize_transition(const NFA& nfa,
                                   DFA& dfa,
                                   typename NFA::State source,
                                   typename NFA::StateSet const& state_set,
                                   typename NFA::StateSetMap& set_map,
                                   typename DFA::transition_property_type const& t,
                                   StateMerge state_merge) {
            typedef typename NFA::State State;
            typedef typename NFA::state_property_type StateProperty;
            State target = -1;
            auto set_iter = set_map.find(state_set);
            if (set_iter == set_map.end()) {
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    target = add_state(dfa);
                } else {
                    std::vector<StateProperty> props;
                    for (const auto& s : state_set)
                        props.push_back(atl::get_property(nfa, s));
                    target = add_state(dfa, merge(props.begin(), props.end()));
                }
                set_map[state_set] = target;
                do_determinize(nfa, dfa, target, state_set, set_map, state_merge);
            } else {
                target = set_iter -> second;
            }
            add_transition(dfa, source, target, t);
        }

        template <typename NFA, 
                  typename DFA,
                  typename Merge>
        static void 
        do_determinize(const NFA& nfa,
                       DFA& dfa,
                       typename DFA::State source,
                       typename NFA::StateSet const& state_set,
                       typename NFA::StateSetMap& set_map,
                       Merge merge) {
            if (has_final_state(nfa, state_set)) set_final_state(dfa, source);
            typedef typename NFA::State State;
            typedef typename NFA::state_property_type StateProperty;
            typedef typename NFA::symbol_property_type SymbolProperty;
            typedef typename NFA::transition_property_type TransitionProperty;

            typename NFA::Symbol2StateSetMap map;
            get_determinize_map(nfa, state_set, map);
            for (auto& map_pair : map) {
                const auto& symbol = map_pair.first;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    add_determinize_transition(nfa, dfa, source, map_pair.second, set_map, 
                                               symbol, merge);
                } else {
                    for (auto& map_pair1 : map_pair.second) {
                        const auto& symbol_property = map_pair1.first;
                        add_determinize_transition(nfa, dfa, source, map_pair1.second, set_map, 
                                                   TransitionProperty(symbol, symbol_property), 
                                                   merge);
                    }
                }
            }
        }

        template <typename NFA, 
                  typename SymbolPropertyMerge,
                  typename StateMerge>
        static void apply(const NFA& a_in,
                          typename NFA::DFA& a_out,
                          SymbolPropertyMerge symbol_property_merge,
                          StateMerge state_merge) {
            typedef typename NFA::state_property_type StateProperty;
            typedef typename NFA::symbol_property_type SymbolProperty;
            typedef typename NFA::automaton_property_type AutomatonProperty;

            atl::clear(a_out);
            atl::set_alphabet(a_out, atl::alphabet(a_in));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }

            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                typename NFA::StateSet state_set;
                epsilon_closure(a_in, state_set);
                typename NFA::State initial_state = -1;
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    initial_state = add_initial_state(a_out);
                } else {
                    std::vector<StateProperty> props;
                    for (const auto& s : state_set) 
                        props.push_back(atl::get_property(a_in, s));
                    initial_state = add_initial_state(a_out, state_merge(props.begin(), 
                                                                         props.end()));
                }
                typename NFA::StateSetMap state_set_map({{state_set, initial_state}});
                do_determinize(a_in, a_out, a_out.initial_state(), state_set, state_set_map,
                               symbol_property_merge);
            } else {
                NFA nfa;
                typename NFA::StateSet state_set({initial_state(a_in)});
                remove_epsolon_transition(a_in, nfa, state_merge, symbol_property_merge);
                typename NFA::State initial_state = -1;
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    initial_state = add_initial_state(a_out);
                } else {
                    initial_state = add_initial_state(a_out, 
                                        atl::get_property(nfa, initial_state(nfa)));
                }
                typename NFA::StateSetMap state_set_map({{state_set, initial_state}});
                do_determinize(a_in, nfa, initial_state(a_out), state_set, state_set_map,
                               symbol_property_merge);
            }
            set_forward_reachable_flag(a_out, 1);
        }
    };

    template <typename NFA,
              typename Merge1,
              typename Merge2>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::DFA& a_out,
                Merge1 merge1,
                Merge2 merge2) {
        typedef typename NFA::symbol_property_type SymbolProperty;
        if (is_undeterministic(a_in)) {
            determinize_impl::apply(a_in, a_out, merge1, merge2);
        } else {
            copy_fa(a_in, a_out);
        }
    }

    template <typename NFA,
              typename Merge>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::DFA& a_out,
                Merge merge) {
        typedef typename NFA::symbol_property_type SymbolProperty;
        if (is_undeterministic(a_in)) {
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                determinize_impl::apply(a_in, a_out, merge, merge);
            } else {
                determinize_impl::apply(a_in, a_out, merge,
                                        intersect_merge<typename NFA::symbol_property_type>());
            }
        } else {
            copy_fa(a_in, a_out);
        }
    }

    template <typename NFA>
    inline void
    determinize(const NFA& a_in, 
                typename NFA::DFA& a_out) {
        if (is_undeterministic(a_in)) {
            determinize_impl::apply(a_in, a_out, 
                             intersect_merge<typename NFA::state_property_type>(),
                             intersect_merge<typename NFA::symbol_property_type>());
        } else {
            copy_fa(a_in, a_out);
        }
    }

    struct minimize_impl {
        template <typename DFA>
        static void 
        remove_dead_states(const DFA& a_in,
                          DFA& a_out) {
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            typename DFA::StateSet reachable_closure;
            atl::reachable_closure(a_in, reachable_closure);
            copy_fa(a_in, a_out, reachable_closure);
        }

        template <typename DFA>
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
            ID count1 = transition_map_.count(s1), count2 = transition_map_.count(s2);
            if (count1 == 0 && count2 == 0) return true;
            if ((count1 & count2) == 0) return false;
            auto &map1 = transition_map_.at(s1);
            auto &map2 = transition_map_.at(s2);
            if (map1.size() != map2.size()) return false;
            for (auto& map_pair : map1) {
                if (map2.count(map_pair.first) == 0) return false;

                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    if (state2_map.at(map_pair.second) != 
                        state2_map.at(map2.at(map_pair.first))) return false;
                } else {
                    auto& map1_ = map_pair.second;
                    auto& map2_ = map2.at(map_pair.first);
                    if (map1_.size() != map2_.size()) return false;
                    for (auto& map_pair1 : map1_) {
                        if (map2_.count(map_pair1.first) == 0) return false;
                        if (state2_map.at(map_pair1.second) != 
                            state2_map.at(map2_.at(map_pair1.first))) return false;
                    }
                }
            }
            return true;
        }

        template <typename DFA>
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
                        StateSet delStateSet;
                        for (const auto& new_state : set) {
                            if (is_equal(a_in, state, new_state, state2_map)) {
                                new_eqs.insert(new_state);
                                delStateSet.insert(new_state);
                            }
                        }
                        eqs.push(new_eqs);
                        for (auto delState : delStateSet) {
                            set.erase(delState);
                        }
                        State new_map_state = -1;
                        if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                            new_map_state = add_state(a_out);
                        } else {
                            new_map_state = add_state(a_out, atl::get_property(a_in, 
                                                                  *(new_eqs.begin())));
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

        template <typename DFA>
        static void 
        add_minimize_transition(const DFA& a_in, 
                                DFA& a_out,
                                typename DFA::State source,
                                typename DFA::State state,
                                typename DFA::State2Map const& old_state2_map,
                                typename DFA::State2Map& new_state2_map,
                                typename DFA::transition_property_type const& t) {
            typedef typename DFA::State State;
            typedef typename DFA::state_property_type StateProperty;
            State old_target = old_state2_map.at(state), target = -1;
            auto map_iter = new_state2_map.find(old_target);
            if (map_iter == new_state2_map.end()) {
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    target = add_state(a_out);
                } else {
                    target = add_state(a_out, atl::get_property(a_in, state));
                }
                new_state2_map[old_target] = target;
                do_minimize(a_in, a_out, state, target, old_state2_map, new_state2_map);
            } else {
                target = map_iter -> second;
            }
            add_transition(a_out, source, target, t);
        }

        template <typename DFA>
        static void 
        do_minimize(const DFA& a_in,
                    DFA& a_out,
                    typename DFA::State state,
                    typename DFA::State source,
                    typename DFA::State2Map const& old_state2_map,
                    typename DFA::State2Map& new_state2_map) {
            typedef typename DFA::State State;
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::transition_property_type TransitionProperty;

            if (is_final_state(a_in, state)) set_final_state(a_out, source);
            const auto& transition_map_ = transition_map(a_in);
            if (transition_map_.count(state) == 0) return;
            const auto& map = transition_map_.at(state);
            for (const auto& map_pair : map) {
                const auto& symbol = map_pair.first;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    auto target = map_pair.second;
                    add_minimize_transition(a_in, a_out, source, target,
                                            old_state2_map, new_state2_map, symbol);
                } else {
                    for (const auto& map_pair1 : map_pair.second) {
                        const auto& symbol_property = map_pair1.first;
                        auto target = map_pair1.second;
                        add_minimize_transition(a_in, a_out, source, target,
                                                old_state2_map, new_state2_map,
                                                TransitionProperty(symbol, symbol_property));
                    }
                }
            }
        }

        template <typename DFA>
        static void 
        apply(const DFA& a_in,
              DFA& a_out) {
            typedef typename DFA::State State;
            typedef typename DFA::StateSet StateSet;
            typedef typename DFA::State2Map State2Map;
            typedef typename DFA::TransitionMap TransitionMap;
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;

            StateSet unfinal_states, final_states;
            atl::clear(a_out);
            State2Map state2_map;
            State unfinal_state = add_state(a_out),
                  final_state = add_state(a_out);
            DFA dfa;
            remove_dead_states(a_in, dfa);

            for (auto state : state_set(dfa)) {
                if (is_final_state(dfa, state)) {
                    final_states.insert(state);
                    state2_map[state] = final_state;
                } else {
                    unfinal_states.insert(state);
                    state2_map[state] = unfinal_state;
                }
            }

            std::queue<StateSet> eqs({final_states});
            if (unfinal_states.size() > 0) eqs.push(unfinal_states);
            get_equivalences(dfa, a_out, eqs, state2_map);

            atl::clear(a_out);
            atl::set_alphabet(a_out, alphabet(dfa));
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(dfa));
            }
            
            State initial_state_ = -1;
            if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                initial_state_ = add_initial_state(a_out);
            } else {
                initial_state_ = add_initial_state(a_out, atl::get_property(dfa, 
                                                              initial_state(dfa)));
            }
            State2Map new_state2_map({{state2_map[initial_state(dfa)], initial_state_}});
            do_minimize(dfa, a_out, initial_state(dfa), initial_state_, 
                        state2_map, new_state2_map);
            set_minimal_flag(a_out, 1);
        }
    };

    template <typename FA,
              typename Merge1,
              typename Merge2>
    inline void
    minimize(const FA& a_in,
             typename FA::DFA& a_out,
             Merge1 merge1,
             Merge2 merge2) {
        if constexpr (std::is_same<FA, typename FA::DFA>::value) {
            if (!is_minimal(a_in)) {
                minimize_impl::apply(a_in, a_out);
            } else {
                copy_fa(a_in, a_out);
            }
        } else {
            typename FA::DFA dfa;
            determinize(a_in, dfa, merge1, merge2);
            minimize_impl::apply(dfa, a_out);
        }
    }

    template <typename FA,
              typename Merge>
    inline void
    minimize(const FA& a_in,
             typename FA::DFA& a_out,
             Merge merge) {
        if constexpr (std::is_same<FA, typename FA::DFA>::value) {
            if (!is_minimal(a_in)) {
                minimize_impl::apply(a_in, a_out);
            } else {
                copy_fa(a_in, a_out);
            }
        } else {
            typename FA::DFA dfa;
            determinize(a_in, dfa, merge);
            minimize_impl::apply(dfa, a_out);
        }
    }

    template <typename FA>
    inline void
    minimize(const FA& a_in,
             typename FA::DFA& a_out) {
        if constexpr (std::is_same<FA, typename FA::DFA>::value) {
            if (!is_minimal(a_in)) {
                minimize_impl::apply(a_in, a_out);
            } else {
                copy_fa(a_in, a_out);
            }
        } else {
            typename FA::DFA dfa;
            determinize(a_in, dfa);
            minimize_impl::apply(dfa, a_out);
        }
    }
}

#endif /* atl_finite_automaton_cast_hpp */
