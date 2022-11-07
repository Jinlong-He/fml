//
//  operate.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_finite_automaton_operate_hpp 
#define atl_detail_finite_automaton_operate_hpp 

#include "atl/detail/automaton.hpp"
#include <atl/detail/finite_automaton/closure.hpp>
#include <atl/detail/finite_automaton/merge.hpp>
#include <atl/detail/finite_automaton/copy.hpp>
#include <atl/detail/finite_automaton/cast.hpp>
#include <atl/detail/finite_automaton/deterministic_finite_automaton.hpp>
#include <atl/detail/finite_automaton/nondeterministic_finite_automaton.hpp>

namespace atl::detail {
    struct intersect_impl {
        template <DFA_PARAMS,
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
            for (const auto& [symbol, map_lhs1] : map_lhs) {
                auto iter_rhs = map_rhs.find(symbol);
                if (iter_rhs != map_rhs.end()) {
                    const auto& map_rhs1 = iter_rhs -> second;
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        if (!reverse) {
                            map[symbol] = StatePair(map_lhs1, map_rhs1);
                        } else {
                            map[symbol] = StatePair(map_rhs1, map_lhs1);
                        }
                    } else {
                        for (const auto& [p_lhs, state_lhs] : map_lhs1) {
                            for (const auto& [p_rhs, state_rhs] : map_rhs1) {
                                if (!reverse) {
                                    map[symbol][merge(p_lhs, p_rhs)] = StatePair(state_lhs, state_rhs);
                                } else {
                                    map[symbol][merge(p_rhs, p_lhs)] = StatePair(state_rhs, state_lhs);
                                }
                            }
                        }
                    }
                }
            }
        }

        template <DFA_PARAMS,
                  typename Merge>
        static void 
        get_intersect_map(const DFA& a_lhs,
                          const DFA& a_rhs,
                          typename DFA::State state_lhs,
                          typename DFA::State state_rhs,
                          typename DFA::Symbol2StatePairMap& map,
                          Merge merge) {
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

        template <DFA_PARAMS,
                  typename StatePropertyMerge,
                  typename SymbolPropertyMerge>
        static void
        add_intersect_transition(const DFA& a_lhs,
                                 const DFA& a_rhs,
                                 DFA& a_out,
                                 typename DFA::State source,
                                 typename DFA::StatePair const& state_pair,
                                 typename DFA::StatePairMap& pair_map,
                                 typename DFA::transition_property_type const& t,
                                 StatePropertyMerge state_property_merge,
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
                if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                    target = add_state(a_out);
                } else {
                    target = add_state(a_out, state_property_merge(atl::get_property(a_lhs, new_state_lhs), 
                                                                   atl::get_property(a_rhs, new_state_rhs)));
                }
                pair_map[state_pair] = target;
                do_intersect(a_lhs, a_rhs, a_out, 
                             new_state_lhs, new_state_rhs, target,
                             pair_map, state_property_merge, symbol_property_merge);
            }
            add_transition(a_out, source, target, t);
        }

        template <DFA_PARAMS,
                  typename StatePropertyMerge,
                  typename SymbolPropertyMerge>
        static void 
        do_intersect(const DFA& a_lhs,
                     const DFA& a_rhs,
                     DFA& a_out,
                     typename DFA::State state_lhs,
                     typename DFA::State state_rhs,
                     typename DFA::State state_out,
                     typename DFA::StatePairMap& pair_map,
                     StatePropertyMerge state_property_merge,
                     SymbolPropertyMerge symbol_property_merge) {
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::transition_property_type TransitionProperty;
            if (is_final_state(a_lhs, state_lhs) && is_final_state(a_rhs, state_rhs)) 
                set_final_state(a_out, state_out);
            typename DFA::Symbol2StatePairMap map;
            get_intersect_map(a_lhs, a_rhs, state_lhs, state_rhs, map, symbol_property_merge);
            if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                for (auto& [symbol, state_pair] : map) {
                    add_intersect_transition(a_lhs, a_rhs, a_out,
                                             state_out, state_pair, pair_map, symbol,
                                             state_property_merge, symbol_property_merge);
                }
            } else {
                for (auto& [symbol, prop_map] : map) {
                    for (auto& [property, state_pair] : prop_map) {
                        add_intersect_transition(a_lhs, a_rhs, a_out,
                                                 state_out, state_pair, pair_map,
                                                 TransitionProperty(symbol, property),
                                                 state_property_merge, symbol_property_merge);
                    }
                }
            }
        }

        template <DFA_PARAMS,
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge,
                  typename AutomatonPropertyMerge>
        static void 
        apply(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge,
              AutomatonPropertyMerge automaton_property_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::StatePair StatePair;
            typedef typename DFA::StatePairMap StatePairMap;
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            clear(a_out);
            if (is_empty(a_lhs) || is_empty(a_rhs)) return;
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, automaton_property_merge(atl::get_property(a_lhs), 
                                                                  atl::get_property(a_rhs)));
            }
            typename DFA::SymbolSet alphabet_;
            util::set_intersection(alphabet(a_lhs), alphabet(a_rhs), alphabet_);
            set_alphabet(a_out, alphabet_);
            
            State initial_state_lhs = initial_state(a_lhs),
                  initial_state_rhs = initial_state(a_rhs),
                  initial_state_out = -1;
            if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                  initial_state_out = add_initial_state(a_out);
            } else {
                  initial_state_out = add_initial_state(a_out, 
                                      state_property_merge(atl::get_property(a_lhs, initial_state_lhs),
                                                           atl::get_property(a_rhs, initial_state_rhs)));
            }

            StatePairMap pair_map({{StatePair(initial_state_lhs, initial_state_rhs),
                                    initial_state_out}}); 
            do_intersect(a_lhs, a_rhs, a_out, 
                         initial_state_lhs, initial_state_rhs, 
                         initial_state(a_out), pair_map, state_property_merge, symbol_property_merge);
            if (final_state_set(a_out).size() == 0) clear(a_out);
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    intersect_fa(const DFA& a_lhs,
                 const DFA& a_rhs,
                 DFA& a_out) {
        DFA dfa;
        detail::intersect_impl::apply(a_lhs, a_rhs, dfa, 
                                      intersect_merge<DFA_SYMBOL_PROP>(),
                                      intersect_merge<DFA_STATE_PROP>(),
                                      intersect_merge<DFA_AUT_PROP>());
        minimize(dfa, a_out);
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge>
    inline void
    intersect_fa(const DFA& a_lhs,
                 const DFA& a_rhs,
                 DFA& a_out,
                 SymbolPropertyMerge symbol_property_merge) {
        DFA dfa;
        detail::intersect_impl::apply(a_lhs, a_rhs, dfa, 
                                      symbol_property_merge, 
                                      intersect_merge<DFA_STATE_PROP>(),
                                      intersect_merge<DFA_AUT_PROP>());
        minimize(dfa, a_out);
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge>
    inline void
    intersect_fa(const DFA& a_lhs,
                 const DFA& a_rhs,
                 DFA& a_out,
                 SymbolPropertyMerge symbol_property_merge,
                 StatePropertyMerge state_property_merge) {
        DFA dfa;
        detail::intersect_impl::apply(a_lhs, a_rhs, dfa, 
                                      symbol_property_merge, 
                                      state_property_merge, 
                                      intersect_merge<DFA_AUT_PROP>());
        minimize(dfa, a_out);
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge,
              typename AutomatonPropertyMerge>
    inline void
    intersect_fa(const DFA& a_lhs,
                 const DFA& a_rhs,
                 DFA& a_out,
                 SymbolPropertyMerge symbol_property_merge,
                 StatePropertyMerge state_property_merge,
                 AutomatonPropertyMerge automaton_property_merge) { 
        DFA dfa;
        detail::intersect_impl::apply(a_lhs, a_rhs, dfa, 
                                      symbol_property_merge, 
                                      state_property_merge, 
                                      automaton_property_merge);
        minimize(dfa, a_out);
    }
};

namespace atl::detail {
    struct union_impl {
        template <DFA_PARAMS,
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge,
                  typename AutomatonPropertyMerge>
        static void 
        apply(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge,
              AutomatonPropertyMerge automaton_property_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::StateSet StateSet;
            typedef typename DFA::State2Map State2Map;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            clear(a_out);
            if (is_empty(a_lhs)) {
                minimize(a_rhs, a_out);
                return;
            }
            if (is_empty(a_rhs)) {
                minimize(a_lhs, a_out);
                return;
            }
            typename DFA::nfa_type nfa;
            typename DFA::SymbolSet alphabet_;
            util::set_union(alphabet(a_lhs), alphabet(a_rhs), alphabet_);
            set_alphabet(nfa, alphabet_);
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(nfa, automaton_property_merge(atl::get_property(a_lhs), 
                                                                atl::get_property(a_rhs)));
            }
            State2Map state2_map_lhs, state2_map_rhs;
            State state_lhs, state_rhs, state;
            copy_fa_impl::copy_states(a_lhs, nfa, state2_map_lhs, StateSet());
            copy_fa_impl::copy_transitions(a_lhs, nfa, state2_map_lhs);
            state_lhs = initial_state(nfa);
            copy_fa_impl::copy_states(a_rhs, nfa, state2_map_rhs, StateSet());
            copy_fa_impl::copy_transitions(a_rhs, nfa, state2_map_rhs);
            state_rhs = initial_state(nfa);
            state = add_initial_state(nfa);
            add_epsilon_transition(nfa, state, state_lhs);
            add_epsilon_transition(nfa, state, state_rhs);
            minimize(nfa, a_out, symbol_property_merge, state_property_merge);
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    union_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out) {
        detail::union_impl::apply(a_lhs, a_rhs, a_out, 
                                  union_merge<DFA_SYMBOL_PROP>(),
                                  union_merge<DFA_STATE_PROP>(),
                                  union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS, 
              typename SymbolPropertyMerge>
    inline void
    union_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge) {
        detail::union_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge,
                                  union_merge<DFA_STATE_PROP>(),
                                  union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS, 
              typename SymbolPropertyMerge,
              typename StatePropertyMerge>
    inline void
    union_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge,
             StatePropertyMerge state_property_merge) {
        detail::union_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge, state_property_merge,
                                  union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS, 
              typename SymbolPropertyMerge,
              typename StatePropertyMerge,
              typename AutomatonPropertyMerge>
    inline void
    union_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyMerge symbol_property_merge,
             StatePropertyMerge state_property_merge,
             AutomatonPropertyMerge automaton_property_merge) {
        detail::union_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge, state_property_merge,
                                  automaton_property_merge);
    }
};

namespace atl::detail {
    struct complement_impl {
        template <DFA_PARAMS>
        static void
        apply(const DFA& a_in,
              DFA& a_out) {
            typedef typename DFA::State State;
            typedef typename DFA::State2Map State2Map;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, atl::get_property(a_in));
            }
            set_alphabet(a_out, alphabet(a_in));
            State2Map state2_map;
            copy_fa(a_in, a_out, state2_map);
            State trap_state = add_state(a_out);
            auto& alphabet_ = alphabet(a_out);
            for (auto state : state_set(a_out)) {
                if (is_final_state(a_out, state)) {
                    remove_final_state(a_out, state);
                } else {
                    set_final_state(a_out, state);
                }
                if (transition_map(a_out).count(state) == 0) {
                    for (auto& c : alphabet_) {
                        add_transition(a_out, state, trap_state, c);
                    }
                } else {
                    auto& map = transition_map(a_out).at(state);
                    if (map.size() == alphabet_.size()) continue;
                    for (auto& c : alphabet_) {
                        if (map.count(c) == 0) {
                            add_transition(a_out, state, trap_state, c);
                        }
                    }
                }
            }
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    complement_fa(const DFA& a_in,
                  DFA& a_out) {
        detail::complement_impl::apply(a_in, a_out);
    }
};

namespace atl::detail{
    struct concat_impl {
        template <DFA_PARAMS,
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge,
                  typename AutomatonPropertyMerge>
        static void 
        apply(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge,
              AutomatonPropertyMerge automaton_property_merge) {
            typedef typename DFA::State State;
            typedef typename DFA::StateSet StateSet;
            typedef typename DFA::State2Map State2Map;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            clear(a_out);
            if (is_empty(a_lhs)) {
                minimize(a_rhs, a_out);
                return;
            }
            if (is_empty(a_rhs)) {
                minimize(a_lhs, a_out);
                return;
            }
            typename DFA::nfa_type nfa;
            typename DFA::SymbolSet alphabet_;
            util::set_union(alphabet(a_lhs), alphabet(a_rhs), alphabet_);
            set_alphabet(nfa, alphabet_);
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(nfa, automaton_property_merge(atl::get_property(a_lhs), 
                                                                  atl::get_property(a_rhs)));
            }
            State2Map state2_map_lhs, state2_map_rhs;
            State state_lhs, state_rhs;
            copy_fa_impl::copy_states(a_lhs, nfa, state2_map_lhs, StateSet());
            copy_fa_impl::copy_transitions(a_lhs, nfa, state2_map_lhs);
            state_lhs = initial_state(nfa);
            auto final_state_set_ = final_state_set(nfa);
            clear_final_state_set(nfa);
            copy_fa_impl::copy_states(a_rhs, nfa, state2_map_rhs, StateSet());
            copy_fa_impl::copy_transitions(a_rhs, nfa, state2_map_rhs);
            state_rhs = initial_state(nfa);
            for (auto state : final_state_set_) {
                add_epsilon_transition(nfa, state, state_rhs);
            }
            set_initial_state(nfa, state_lhs);
            minimize(nfa, a_out, symbol_property_merge, state_property_merge);
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline void
    concat_fa(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out) {
        detail::concat_impl::apply(a_lhs, a_rhs, a_out, 
                                   union_merge<DFA_SYMBOL_PROP>(),
                                   union_merge<DFA_STATE_PROP>(),
                                   union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge>
    inline void
    concat_fa(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge) {
        detail::concat_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge,
                                   union_merge<DFA_STATE_PROP>(),
                                   union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge>
    inline void
    concat_fa(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge) {
        detail::concat_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge, state_property_merge,
                                   union_merge<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyMerge,
              typename StatePropertyMerge,
              typename AutomatonPropertyMerge>
    inline void
    concat_fa(const DFA& a_lhs,
              const DFA& a_rhs,
              DFA& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge,
              AutomatonPropertyMerge automaton_property_merge) {
        detail::concat_impl::apply(a_lhs, a_rhs, a_out, symbol_property_merge, state_property_merge,
                                   automaton_property_merge);
    }
};

namespace atl::detail {
    struct equal_impl {
        template <DFA_PARAMS,
                  typename SymbolPropertyEqual,
                  typename StatePropertyEqual,
                  typename AutomatonPropertyEqual>
        static bool
        apply(const DFA& a_lhs,
              const DFA& a_rhs,
              SymbolPropertyEqual symbol_property_equal,
              StatePropertyEqual state_property_equal,
              AutomatonPropertyEqual automaton_property_equal) {
            typedef typename DFA::state_property_type StateProperty;
            typedef typename DFA::symbol_property_type SymbolProperty;
            typedef typename DFA::automaton_property_type AutomatonProperty;
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                if (!automaton_property_equal(atl::get_property(a_lhs), atl::get_property(a_rhs))) return false;
                //if (atl::get_property(a_lhs) != atl::get_property(a_rhs)) return false;
            }
            if (is_empty(a_lhs) && is_empty(a_rhs)) return true;
            if (state_set(a_lhs) != state_set(a_rhs)) return false;
            if (final_state_set(a_lhs) != final_state_set(a_rhs)) return false;
            if (initial_state(a_lhs) != initial_state(a_rhs)) return false;
            const auto& transition_map_lhs = transition_map(a_lhs);
            const auto& transition_map_rhs = transition_map(a_rhs);
            for (auto state : state_set(a_lhs)) {
                ID count1 = transition_map_lhs.count(state),
                   count2 = transition_map_rhs.count(state);
                if (count1 != count2) return false;
                if (count1 == 0) continue;
                const auto& map_lhs = transition_map_lhs.at(state);
                const auto& map_rhs = transition_map_rhs.at(state);
                if (map_lhs.size() != map_rhs.size()) return false;
                auto iter_lhs = map_lhs.begin(), end_lhs = map_lhs.end(),
                     iter_rhs = map_rhs.begin();
                while (iter_lhs != end_lhs) {
                    if (iter_lhs -> first != iter_rhs -> first) return false;
                    if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                        auto state_lhs = iter_lhs -> second;
                        auto state_rhs = iter_rhs -> second;
                        if (state_lhs != state_rhs) return false;
                        if constexpr (!std::is_same<StateProperty, boost::no_property>::value) {
                            if (!state_property_equal(atl::get_property(a_lhs, state_lhs), atl::get_property(a_rhs, state_rhs))) 
                                return false;
                            //if (atl::get_property(a_lhs, state_lhs) != atl::get_property(a_rhs, state_rhs))
                            //    return false;
                        }
                    } else {
                        const auto& prop_map_lhs = iter_lhs -> second;
                        const auto& prop_map_rhs = iter_rhs -> second;
                        if (prop_map_lhs.size() != prop_map_rhs.size()) return false;
                        auto iter_lhs1 = prop_map_lhs.begin(), end_lhs1 = prop_map_lhs.end(),
                             iter_rhs1 = prop_map_rhs.begin();
                        while (iter_lhs1 != end_lhs1) {
                            if (!symbol_property_equal(iter_lhs1 -> first, iter_rhs1 -> first)) return false;
                            //if (iter_lhs1 -> first != iter_rhs1 -> first) return false;
                            auto state_lhs = iter_lhs1 -> second;
                            auto state_rhs = iter_rhs1 -> second;
                            if (state_lhs != state_rhs) return false;
                            if constexpr (!std::is_same<StateProperty, boost::no_property>::value) {
                                if (!state_property_equal(atl::get_property(a_lhs, state_lhs), atl::get_property(a_rhs, state_rhs))) 
                                    return false;
                                //if (atl::get_property(a_lhs, state_lhs) != atl::get_property(a_rhs, state_rhs)) return false;
                            }
                            iter_lhs1++;
                            iter_rhs1++;
                        }
                    }
                    iter_lhs++;
                    iter_rhs++;
                }
            }
            return true;
        }
    };
};

namespace atl {
    template <DFA_PARAMS>
    inline bool
    equal_fa(const DFA& a_lhs,
             const DFA& a_rhs) {
        return detail::equal_impl::apply(a_lhs, a_rhs, 
                                         equal<DFA_SYMBOL_PROP>(), 
                                         equal<DFA_STATE_PROP>(), 
                                         equal<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyEqual>
    inline bool
    equal_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyEqual symbol_property_equal) {
        return detail::equal_impl::apply(a_lhs, a_rhs, 
                                         symbol_property_equal,
                                         equal<DFA_STATE_PROP>(),
                                         equal<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyEqual,
              typename StatePropertyEqual>
    inline bool
    equal_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyEqual symbol_property_equal,
             StatePropertyEqual state_property_equal) {
        return detail::equal_impl::apply(a_lhs, a_rhs, 
                                         symbol_property_equal,
                                         state_property_equal,
                                         equal<DFA_AUT_PROP>());
    }

    template <DFA_PARAMS,
              typename SymbolPropertyEqual,
              typename StatePropertyEqual,
              typename AutomatonPropertyEqual>
    inline bool
    equal_fa(const DFA& a_lhs,
             const DFA& a_rhs,
             DFA& a_out,
             SymbolPropertyEqual symbol_property_equal,
             StatePropertyEqual state_property_equal,
             AutomatonPropertyEqual automaton_property_equal) {
        return detail::equal_impl::apply(a_lhs, a_rhs, 
                                         symbol_property_equal,
                                         state_property_equal,
                                         automaton_property_equal);
    }

    template <NFA_PARAMS>
    inline bool
    equal_fa(const NFA& a_lhs,
             const NFA& a_rhs) {
        typename NFA::dfa_type dfa_lhs, dfa_rhs;
        minimize(a_lhs, dfa_lhs);
        minimize(a_rhs, dfa_rhs);
        return detail::equal_impl::apply(dfa_lhs, dfa_rhs,
                                         equal<NFA_SYMBOL_PROP>(), 
                                         equal<NFA_STATE_PROP>(), 
                                         equal<NFA_AUT_PROP>());
    }

    template <NFA_PARAMS,
              typename SymbolPropertyEqual>
    inline bool
    equal_fa(const NFA& a_lhs,
             const NFA& a_rhs,
             SymbolPropertyEqual symbol_property_equal) {
        typename NFA::dfa_type dfa_lhs, dfa_rhs;
        minimize(a_lhs, dfa_lhs);
        minimize(a_rhs, dfa_rhs);
        return detail::equal_impl::apply(dfa_lhs, dfa_rhs, 
                                         symbol_property_equal,
                                         equal<NFA_STATE_PROP>(),
                                         equal<NFA_AUT_PROP>());
    }

    template <NFA_PARAMS,
              typename SymbolPropertyEqual,
              typename StatePropertyEqual>
    inline bool
    equal_fa(const NFA& a_lhs,
             const NFA& a_rhs,
             SymbolPropertyEqual symbol_property_equal,
             StatePropertyEqual state_property_equal) {
        typename NFA::dfa_type dfa_lhs, dfa_rhs;
        minimize(a_lhs, dfa_lhs);
        minimize(a_rhs, dfa_rhs);
        return detail::equal_impl::apply(dfa_lhs, dfa_rhs, 
                                         symbol_property_equal,
                                         state_property_equal,
                                         equal<NFA_AUT_PROP>());
    }

    template <NFA_PARAMS,
              typename SymbolPropertyEqual,
              typename StatePropertyEqual,
              typename AutomatonPropertyEqual>
    inline bool
    equal_fa(const NFA& a_lhs,
             const NFA& a_rhs,
             SymbolPropertyEqual symbol_property_equal,
             StatePropertyEqual state_property_equal,
             AutomatonPropertyEqual automaton_property_equal) {
        typename NFA::dfa_type dfa_lhs, dfa_rhs;
        minimize(a_lhs, dfa_lhs);
        minimize(a_rhs, dfa_rhs);
        return detail::equal_impl::apply(dfa_lhs, dfa_rhs, 
                                         symbol_property_equal,
                                         state_property_equal,
                                         automaton_property_equal);
    }
};

#endif /* atl_detail_finite_automaton_operate_hpp */

