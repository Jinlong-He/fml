//
//  operate.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_letter2letter_transducer_operate_hpp 
#define atl_detail_letter2letter_transducer_operate_hpp 

#include <atl/detail/letter2letter_transducer/deterministic_letter2letter_transducer.hpp>
#include <atl/detail/letter2letter_transducer/nondeterministic_letter2letter_transducer.hpp>
#include <atl/detail/finite_automaton/merge.hpp>

namespace atl::detail {
    struct composite_impl {
        template <DL2LT_PARAMS,
                  typename Merge>
        static void 
        get_composite_map(const DL2LT& a_lhs,
                          const DL2LT& a_rhs,
                          typename DL2LT::State old_state_rhs,
                          typename DL2LT::Symbol2StatePairMap& map,
                          typename DL2LT::Label2StateMap const& map_lhs,
                          typename DL2LT::Label2StateMap const& map_rhs,
                          Merge merge) {
            typedef typename DL2LT::StatePair StatePair;
            typedef typename DL2LT::label_type Label;
            typedef typename DL2LT::label_property_type LabelProperty;
            for (const auto& [upper, map_lhs1] : map_lhs) {
                if constexpr (std::is_same<LabelProperty, no_type>::value) {
                    for (const auto& [symbol, state_lhs] : map_lhs1) {
                        if (symbol == epsilon_symbol(a_lhs)) {
                            map[Label(upper, epsilon_symbol(a_lhs))] = 
                                StatePair(state_lhs, old_state_rhs);
                        }
                        auto iter_rhs = map_rhs.find(symbol);
                        if (iter_rhs != map_rhs.end()) {
                            const auto& map_rhs1 = iter_rhs -> second;
                            for (const auto& [lower, state_rhs] : map_rhs1) {
                                map[Label(upper, lower)] = StatePair(state_lhs, state_rhs);
                            }
                        }
                    }
                } else {
                    for (const auto& [symbol, prop_map_lhs] : map_lhs1) {
                        if (symbol == epsilon_symbol(a_lhs)) {
                            for (const auto& [p_lhs, state_lhs] : prop_map_lhs) {
                                map[Label(upper, epsilon_symbol(a_lhs))][merge(p_lhs, LabelProperty())] =
                                    StatePair(state_lhs, old_state_rhs);
                            }
                        }
                        auto iter_rhs = map_rhs.find(symbol);
                        if (iter_rhs != map_rhs.end()) {
                            const auto& map_rhs1 = iter_rhs -> second;
                            for (const auto& [p_lhs, state_lhs] : prop_map_lhs) {
                                for (const auto& [lower, prop_map_rhs] : map_rhs1) {
                                    for (const auto& [p_rhs, state_rhs] : prop_map_rhs) {
                                        map[Label(upper, lower)][merge(p_lhs, p_rhs)] =
                                            StatePair(state_lhs, state_rhs);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        template <DL2LT_PARAMS,
                  typename Merge>
        static void 
        get_composite_map(const DL2LT& a_lhs,
                          const DL2LT& a_rhs,
                          typename DL2LT::State state_lhs,
                          typename DL2LT::State state_rhs,
                          typename DL2LT::Symbol2StatePairMap& map,
                          Merge merge) {
            const auto& transition_map_lhs = a_lhs.l2ltransition_map();
            const auto& transition_map_rhs = a_rhs.l2ltransition_map();
            auto iter_lhs = transition_map_lhs.find(state_lhs);
            auto iter_rhs = transition_map_rhs.find(state_rhs);
            if (iter_lhs != transition_map_lhs.end() && iter_rhs != transition_map_rhs.end()) {
                const auto& map_lhs = iter_lhs -> second;
                const auto& map_rhs = iter_rhs -> second;
                get_composite_map(a_lhs, a_rhs, state_rhs, map, map_lhs, map_rhs, merge);
            }
        }

        template <DL2LT_PARAMS,
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void
        add_composite_transition(const DL2LT& a_lhs,
                                 const DL2LT& a_rhs,
                                 DL2LT& a_out,
                                 typename DL2LT::State source,
                                 typename DL2LT::StatePair const& state_pair,
                                 typename DL2LT::StatePairMap& pair_map,
                                 typename DL2LT::transition_property_type const& t,
                                 StateMerge state_merge,
                                 SymbolPropertyMerge symbol_property_merge) {
            typedef typename DL2LT::State State;
            typedef typename DL2LT::state_property_type StateProperty;
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
                    target = add_state(a_out, state_merge(atl::get_property(a_lhs, new_state_lhs), 
                                                          atl::get_property(a_rhs, new_state_rhs)));
                }
                pair_map[state_pair] = target;
                do_composite(a_lhs, a_rhs, a_out, 
                             new_state_lhs, new_state_rhs, target,
                             pair_map, state_merge, symbol_property_merge);
            }
            add_transition(a_out, source, target, t);
        }

        template <DL2LT_PARAMS,
                  typename StateMerge,
                  typename SymbolPropertyMerge>
        static void 
        do_composite(const DL2LT& a_lhs,
                     const DL2LT& a_rhs,
                     DL2LT& a_out,
                     typename DL2LT::State state_lhs,
                     typename DL2LT::State state_rhs,
                     typename DL2LT::State state_out,
                     typename DL2LT::StatePairMap& pair_map,
                     StateMerge state_merge,
                     SymbolPropertyMerge symbol_property_merge) {
            typedef typename DL2LT::symbol_property_type SymbolProperty;
            typedef typename DL2LT::transition_property_type TransitionProperty;
            if (is_final_state(a_lhs, state_lhs) && is_final_state(a_rhs, state_rhs)) 
                set_final_state(a_out, state_out);
            typename DL2LT::Symbol2StatePairMap map;
            get_composite_map(a_lhs, a_rhs, state_lhs, state_rhs, map, symbol_property_merge);
            for (auto& map_pair : map) {
                auto& symbol = map_pair.first;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    auto& state_pair = map_pair.second;
                    add_composite_transition(a_lhs, a_rhs, a_out,
                                             state_out, state_pair, pair_map, symbol,
                                             state_merge, symbol_property_merge);
                } else {
                    for (auto& map_pair1 : map_pair.second) {
                        auto& symbol_property = map_pair1.first;
                        auto& state_pair = map_pair1.second;
                        add_composite_transition(a_lhs, a_rhs, a_out,
                                                 state_out, state_pair, pair_map,
                                                 TransitionProperty(symbol, symbol_property),
                                                 state_merge, symbol_property_merge);
                    }
                }
            }
        }

        template <DL2LT_PARAMS,
                  typename SymbolPropertyMerge,
                  typename StateMerge,
                  typename FAMerge>
        static void 
        apply(const DL2LT& a_lhs,
              const DL2LT& a_rhs,
              DL2LT& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StateMerge state_merge,
              FAMerge fa_merge) {
            typedef typename DL2LT::State State;
            typedef typename DL2LT::StatePair StatePair;
            typedef typename DL2LT::StatePairMap StatePairMap;
            typedef typename DL2LT::state_property_type StateProperty;
            typedef typename DL2LT::automaton_property_type AutomatonProperty;
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, fa_merge(atl::get_property(a_lhs), 
                                                  atl::get_property(a_rhs)));
            }
            typename DL2LT::SymbolSet alphabet_;
            util::set_intersection(alphabet(a_lhs), alphabet(a_rhs), alphabet_);
            set_alphabet(a_out, alphabet_);
            
            State initial_state_lhs = initial_state(a_lhs),
                  initial_state_rhs = initial_state(a_rhs),
                  initial_state_out = -1;
            if constexpr (std::is_same<StateProperty, boost::no_property>::value) {
                  initial_state_out = add_initial_state(a_out);
            } else {
                  initial_state_out = add_initial_state(a_out, 
                                      state_merge(atl::get_property(a_lhs, initial_state_lhs),
                                                  atl::get_property(a_rhs, initial_state_rhs)));
            }

            StatePairMap pair_map({{StatePair(initial_state_lhs, initial_state_rhs),
                                    initial_state_out}}); 
            do_composite(a_lhs, a_rhs, a_out, 
                         initial_state_lhs, initial_state_rhs, 
                         a_out.initial_state(), pair_map, state_merge, symbol_property_merge);
            if (final_state_set(a_out).size() == 0) clear(a_out);
        }
    };

    template <DL2LT_PARAMS>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                 const DL2LT& a_rhs,
                 DL2LT& a_out) {
        composite_impl::apply(a_lhs, a_rhs, a_out, 
                              intersect_merge<typename DL2LT::symbol_property_type>(),
                              intersect_merge<typename DL2LT::state_property_type>(),
                              intersect_merge<typename DL2LT::automaton_property_type>());
    }

    template <DL2LT_PARAMS,
              typename Merge>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   Merge merge) {
        if constexpr (std::is_same<typename DL2LT::symbol_property_type, no_type>::value) {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge, merge,
                                  intersect_merge<typename DL2LT::automaton_property_type>());
        } else {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge,
                                  intersect_merge<typename DL2LT::state_property_type>(),
                                  intersect_merge<typename DL2LT::automaton_property_type>());
        }
    }

    template <DL2LT_PARAMS,
              typename Merge1,
              typename Merge2>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   Merge1 merge1,
                   Merge2 merge2) {
        if constexpr (std::is_same<typename DL2LT::symbol_property_type, no_type>::value) {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge1, merge1, merge2);
        } else {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge1, merge2,
                                  intersect_merge<typename DL2LT::automaton_property_type>());
        }
    }

    template <DL2LT_PARAMS,
              typename Merge1,
              typename Merge2,
              typename Merge3>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   Merge1 merge1,
                   Merge2 merge2,
                   Merge3 merge3) {
        if constexpr (std::is_same<typename DL2LT::symbol_property_type, no_type>::value) {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge1, merge1, merge2);
        } else {
            composite_impl::apply(a_lhs, a_rhs, a_out, merge1, merge2, merge3);
        }
    }

    struct translate_impl {
        template <DL2LT_PARAMS>
        static void
        apply(const DL2LT& dl2lt,
              const std::basic_string<typename DL2LT::symbol_type>& word_in,
              std::basic_string<typename DL2LT::symbol_type>& word_out,
              std::basic_string<typename DL2LT::label_property_type>& props,
              typename DL2LT::StateSet& state_set) {
        }
    };
}

#endif /* atl_detail_letter2letter_transducer_operate_hpp */

