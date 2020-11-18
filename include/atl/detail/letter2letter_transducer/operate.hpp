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
#include <atl/detail/finite_automaton/copy.hpp>
#include <atl/detail/finite_automaton/merge.hpp>

namespace atl::detail {
    struct composite_impl {
        template <DL2LT_PARAMS,
                  typename SymbolPropertyMerge>
        static void 
        get_composite_map(const DL2LT& a_lhs,
                          const DL2LT& a_rhs,
                          typename DL2LT::State old_state_rhs,
                          typename DL2LT::Label2StatePairsMap& map,
                          typename DL2LT::Label2StateMap const& map_lhs,
                          typename DL2LT::Label2StateMap const& map_rhs,
                          SymbolPropertyMerge symbol_property_merge) {
            typedef typename DL2LT::StatePair StatePair;
            typedef typename DL2LT::label_type Label;
            typedef typename DL2LT::label_property_type LabelProperty;
            for (const auto& [upper, map_lhs1] : map_lhs) {
                if constexpr (std::is_same<LabelProperty, no_type>::value) {
                    for (const auto& [symbol, state_lhs] : map_lhs1) {
                        if (symbol == epsilon_symbol(a_lhs)) {
                            map[Label(upper, epsilon_symbol(a_lhs))].insert(StatePair(state_lhs, old_state_rhs));
                        }
                        auto iter_rhs = map_rhs.find(symbol);
                        if (iter_rhs != map_rhs.end()) {
                            const auto& map_rhs1 = iter_rhs -> second;
                            for (const auto& [lower, state_rhs] : map_rhs1) {
                                map[Label(upper, lower)].insert(StatePair(state_lhs, state_rhs));
                            }
                        }
                    }
                } else {
                    for (const auto& [symbol, prop_map_lhs] : map_lhs1) {
                        if (symbol == epsilon_symbol(a_lhs)) {
                            for (const auto& [p_lhs, state_lhs] : prop_map_lhs) {
                                map[Label(upper, epsilon_symbol(a_lhs))]
                                   [symbol_property_merge(p_lhs, LabelProperty())].insert(StatePair(state_lhs, old_state_rhs));
                            }
                        }
                        auto iter_rhs = map_rhs.find(symbol);
                        if (iter_rhs != map_rhs.end()) {
                            const auto& map_rhs1 = iter_rhs -> second;
                            for (const auto& [p_lhs, state_lhs] : prop_map_lhs) {
                                for (const auto& [lower, prop_map_rhs] : map_rhs1) {
                                    for (const auto& [p_rhs, state_rhs] : prop_map_rhs) {
                                        map[Label(upper, lower)]
                                           [symbol_property_merge(p_lhs, p_rhs)].insert(StatePair(state_lhs, state_rhs));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        template <DL2LT_PARAMS,
                  typename SymbolPropertyMerge>
        static void 
        get_composite_map(const DL2LT& a_lhs,
                          const DL2LT& a_rhs,
                          typename DL2LT::State state_lhs,
                          typename DL2LT::State state_rhs,
                          typename DL2LT::Label2StatePairsMap& map,
                          SymbolPropertyMerge symbol_property_merge) {
            const auto& transition_map_lhs = a_lhs.l2ltransition_map();
            const auto& transition_map_rhs = a_rhs.l2ltransition_map();
            auto iter_lhs = transition_map_lhs.find(state_lhs);
            auto iter_rhs = transition_map_rhs.find(state_rhs);
            if (iter_lhs != transition_map_lhs.end() && iter_rhs != transition_map_rhs.end()) {
                const auto& map_lhs = iter_lhs -> second;
                const auto& map_rhs = iter_rhs -> second;
                get_composite_map(a_lhs, a_rhs, state_rhs, map, map_lhs, map_rhs, 
                                  symbol_property_merge);
            }
        }

        template <DL2LT_PARAMS,
                  typename StatePropertyMerge,
                  typename SymbolPropertyMerge>
        static void
        add_composite_transition(const DL2LT& a_lhs,
                                 const DL2LT& a_rhs,
                                 typename DL2LT::nl2lt_type& a_out,
                                 typename DL2LT::State source,
                                 typename DL2LT::StatePair const& state_pair,
                                 typename DL2LT::StatePairMap& pair_map,
                                 typename DL2LT::transition_property_type const& t,
                                 StatePropertyMerge state_property_merge,
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
                    target = add_state(a_out, state_property_merge(atl::get_property(a_lhs, new_state_lhs), 
                                                          atl::get_property(a_rhs, new_state_rhs)));
                }
                pair_map[state_pair] = target;
                do_composite(a_lhs, a_rhs, a_out, 
                             new_state_lhs, new_state_rhs, target,
                             pair_map, state_property_merge, symbol_property_merge);
            }
            add_transition(a_out, source, target, t);
        }

        template <DL2LT_PARAMS,
                  typename StatePropertyMerge,
                  typename SymbolPropertyMerge>
        static void 
        do_composite(const DL2LT& a_lhs,
                     const DL2LT& a_rhs,
                     typename DL2LT::nl2lt_type& a_out,
                     typename DL2LT::State state_lhs,
                     typename DL2LT::State state_rhs,
                     typename DL2LT::State state_out,
                     typename DL2LT::StatePairMap& pair_map,
                     StatePropertyMerge state_property_merge,
                     SymbolPropertyMerge symbol_property_merge) {
            typedef typename DL2LT::symbol_property_type SymbolProperty;
            typedef typename DL2LT::transition_property_type TransitionProperty;
            if (is_final_state(a_lhs, state_lhs) && is_final_state(a_rhs, state_rhs)) 
                set_final_state(a_out, state_out);
            typename DL2LT::Label2StatePairsMap map;
            get_composite_map(a_lhs, a_rhs, state_lhs, state_rhs, map, symbol_property_merge);
            for (auto& map_pair : map) {
                auto& symbol = map_pair.first;
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    for (auto& state_pair : map_pair.second) {
                        add_composite_transition(a_lhs, a_rhs, a_out,
                                                 state_out, state_pair, pair_map, symbol,
                                                 state_property_merge, symbol_property_merge);
                    }
                } else {
                    for (auto& map_pair1 : map_pair.second) {
                        auto& symbol_property = map_pair1.first;
                        for (auto& state_pair : map_pair1.second) {
                        add_composite_transition(a_lhs, a_rhs, a_out,
                                                     state_out, state_pair, pair_map,
                                                     TransitionProperty(symbol, symbol_property),
                                                     state_property_merge, symbol_property_merge);
                        }
                    }
                }
            }
        }

        template <DL2LT_PARAMS,
                  typename SymbolPropertyMerge,
                  typename StatePropertyMerge,
                  typename AutomatonPropertyMerge>
        static void 
        apply(const DL2LT& a_lhs,
              const DL2LT& a_rhs,
              typename DL2LT::nl2lt_type& a_out,
              SymbolPropertyMerge symbol_property_merge,
              StatePropertyMerge state_property_merge,
              AutomatonPropertyMerge automaton_property_merge) {
            typedef typename DL2LT::State State;
            typedef typename DL2LT::StatePair StatePair;
            typedef typename DL2LT::StatePairMap StatePairMap;
            typedef typename DL2LT::state_property_type StateProperty;
            typedef typename DL2LT::automaton_property_type AutomatonProperty;
            if constexpr (!std::is_same<AutomatonProperty, boost::no_property>::value) {
                atl::set_property(a_out, automaton_property_merge(atl::get_property(a_lhs), 
                                                  atl::get_property(a_rhs)));
            }
            typename DL2LT::SymbolSet symbol_set_;
            util::set_intersection(symbol_set(a_lhs), symbol_set(a_rhs), symbol_set_);
            set_symbol_set(a_out, symbol_set_);
            
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
            do_composite(a_lhs, a_rhs, a_out, 
                         initial_state_lhs, initial_state_rhs, 
                         a_out.initial_state(), pair_map, state_property_merge, symbol_property_merge);
            if (final_state_set(a_out).size() == 0) clear(a_out);
        }
    };
}

namespace atl {
    template <DL2LT_PARAMS>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out) {
        typename DL2LT::nl2lt_type nl2lt;
        detail::composite_impl::apply(a_lhs, a_rhs, nl2lt, 
                              intersect_merge<typename DL2LT::symbol_property_type>(),
                              intersect_merge<typename DL2LT::state_property_type>(),
                              intersect_merge<typename DL2LT::automaton_property_type>());
        minimize(nl2lt, a_out);
    }

    template <DL2LT_PARAMS,
              typename LabelPropertyMerge>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   LabelPropertyMerge label_property_merge) {
        typename DL2LT::nl2lt_type nl2lt;
        detail::composite_impl::apply(a_lhs, a_rhs, nl2lt, 
                                      label_property_merge, 
                                      intersect_merge<typename DL2LT::state_property_type>(),
                                      intersect_merge<typename DL2LT::automaton_property_type>());
        minimize(nl2lt, a_out);
    }

    template <DL2LT_PARAMS,
              typename LabelPropertyMerge,
              typename StatePropertyMerge>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   LabelPropertyMerge label_property_merge,
                   StatePropertyMerge state_property_merge) {
        typename DL2LT::nl2lt_type nl2lt;
        detail::composite_impl::apply(a_lhs, a_rhs, nl2lt, 
                                      label_property_merge, 
                                      state_property_merge,
                                      intersect_merge<typename DL2LT::automaton_property_type>());
        minimize(nl2lt, a_out);
    }

    template <DL2LT_PARAMS,
              typename LabelPropertyMerge,
              typename StatePropertyMerge,
              typename AutomatonPropertyMerge>
    inline void
    composite_l2lt(const DL2LT& a_lhs,
                   const DL2LT& a_rhs,
                   DL2LT& a_out,
                   LabelPropertyMerge label_property_merge,
                   StatePropertyMerge state_property_merge,
                   AutomatonPropertyMerge automaton_property_merge) {
        typename DL2LT::nl2lt_type nl2lt;
        detail::composite_impl::apply(a_lhs, a_rhs, nl2lt, 
                                      label_property_merge, 
                                      state_property_merge,
                                      automaton_property_merge);
        minimize(nl2lt, a_out);
    }
}

namespace atl::detail {
    struct translate_impl {
        template <DL2LT_PARAMS>
        static void
        do_translate(const DL2LT& dl2lt,
                     typename DL2LT::State state,
                     const std::vector<DL2LT_SYMBOL>& uppers, ID index,
                     const std::vector<std::vector<DL2LT_SYMBOL> >& words_in,
                     unordered_map<typename DL2LT::State,
                                   std::vector<std::vector<DL2LT_SYMBOL > > >& words_out) {
            unordered_map<typename DL2LT::State, unordered_set<DL2LT_SYMBOL> > map;
            get_target_maps_in_map(dl2lt, state, uppers[index], map);
            std::vector<std::vector<DL2LT_SYMBOL> > words;
            for (auto& [target, lowers] : map) {
                for (auto lower : lowers) {
                    for (const auto& word : words_in) {
                        words.clear();
                        std::vector<DL2LT_SYMBOL> new_word = word;
                        new_word.emplace_back(lower);
                        words.emplace_back(new_word);
                    }
                    if (index == uppers.size() - 1) {
                        for (auto& new_word : words) {
                            words_out[target].emplace_back(new_word);
                        }
                        continue;
                    }
                    do_translate(dl2lt, target, uppers, index + 1, words, words_out);
                }
            }
        }

        template <NL2LT_PARAMS>
        static void
        do_translate(const NL2LT& nl2lt,
                     typename NL2LT::State state,
                     const std::vector<NL2LT_SYMBOL>& uppers, ID index,
                     const std::vector<std::vector<NL2LT_SYMBOL> >& words_in,
                     unordered_map<typename NL2LT::State,
                                   std::vector<std::vector<NL2LT_SYMBOL > > >& words_out) {
            unordered_map<typename NL2LT::State, unordered_set<NL2LT_SYMBOL> > map;
            get_target_maps_in_map(nl2lt, state, uppers[index], map);
            std::vector<std::vector<NL2LT_SYMBOL> > words;
            for (auto& [target, lowers] : map) {
                for (auto lower : lowers) {
                    words.clear();
                    for (const auto& word : words_in) {
                        std::vector<NL2LT_SYMBOL> new_word = word;
                        new_word.emplace_back(lower);
                        words.emplace_back(new_word);
                    }
                    if (index == uppers.size() - 1) {
                        for (auto& new_word : words) {
                            words_out[target].emplace_back(new_word);
                        }
                        continue;
                    }
                    do_translate(nl2lt, target, uppers, index + 1, words, words_out);
                }
            }
        }

        template <DL2LT_PARAMS>
        static void
        apply(const DL2LT& dl2lt,
              const std::vector<DL2LT_SYMBOL>& word_in,
              unordered_map<typename DL2LT::State,
                            std::vector<std::vector<DL2LT_SYMBOL> > >& words_out) {
            do_translate(dl2lt, initial_state(dl2lt), word_in, 0,
                         std::vector<std::vector<DL2LT_SYMBOL> >({std::vector<DL2LT_SYMBOL>()}),
                         words_out);
        }

        template <NL2LT_PARAMS>
        static void
        apply(const NL2LT& nl2lt,
              const std::vector<NL2LT_SYMBOL>& word_in,
              unordered_map<typename NL2LT::State,
                            std::vector<std::vector<NL2LT_SYMBOL> > >& words_out) {
            do_translate(nl2lt, initial_state(nl2lt), word_in, 0,
                         std::vector<std::vector<NL2LT_SYMBOL> >({std::vector<NL2LT_SYMBOL>()}),
                         words_out);
        }
    };
}

namespace atl {
    template <DL2LT_PARAMS>
    inline void
    translate(const DL2LT& dl2lt,
              const std::vector<DL2LT_SYMBOL>& word_in,
              std::vector<std::vector<DL2LT_SYMBOL> >& words_out) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(dl2lt, word_in, map);
        for (auto& [state, words] : map) {
            for (auto& word : words) {
                words_out.emplace_back(word);
            }
        }
    }

    template <DL2LT_PARAMS>
    inline void
    translate(const DL2LT& dl2lt,
              const std::string& word_in,
              std::vector<std::string>& words_out) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        std::vector<DL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(dl2lt, word, map);
        for (auto& [state, words] : map) {
            for (auto& w : words) {
                words_out.emplace_back(std::string(w.begin(), w.end()));
            }
        }
    }

    template <NL2LT_PARAMS>
    inline void
    translate(const NL2LT& nl2lt,
              const std::vector<NL2LT_SYMBOL>& word_in,
              std::vector<std::vector<NL2LT_SYMBOL> >& words_out) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(nl2lt, word_in, map);
        for (auto& [state, words] : map) {
            for (auto& word : words) {
                words_out.emplace_back(word);
            }
        }
    }

    template <NL2LT_PARAMS>
    inline void
    translate(const NL2LT& nl2lt,
              const std::string& word_in,
              std::vector<std::string>& words_out) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        std::vector<NL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(nl2lt, word, map);
        for (auto& [state, words] : map) {
            for (auto& w : words) {
                words_out.emplace_back(std::string(w.begin(), w.end()));
            }
        }
    }

    template <DL2LT_PARAMS>
    inline bool 
    accept(const DL2LT& dl2lt,
           const std::vector<DL2LT_SYMBOL>& word_in) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(dl2lt, word_in, map);
        typename DL2LT::StateSet states;
        for (auto& [state, words] : map) {
            states.insert(state);
        }
        if (has_final_state(dl2lt, states)) return true;
        return false;
    }

    template <DL2LT_PARAMS>
    inline bool 
    accept(const DL2LT& dl2lt,
           const std::string& word_in) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        std::vector<DL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(dl2lt, word, map);
        typename DL2LT::StateSet states;
        for (auto& [state, words] : map) {
            states.insert(state);
        }
        if (has_final_state(dl2lt, states)) return true;
        return false;
    }

    template <NL2LT_PARAMS>
    inline bool 
    accept(const NL2LT& nl2lt,
           const std::vector<NL2LT_SYMBOL>& word_in) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(nl2lt, word_in, map);
        typename NL2LT::StateSet states;
        for (auto& [state, words] : map) {
            states.insert(state);
        }
        if (has_final_state(nl2lt, states)) return true;
        return false;
    }

    template <NL2LT_PARAMS>
    inline bool 
    accept(const NL2LT& nl2lt,
           const std::string& word_in) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        std::vector<NL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(nl2lt, word, map);
        typename NL2LT::StateSet states;
        for (auto& [state, words] : map) {
            states.insert(state);
        }
        if (has_final_state(nl2lt, states)) return true;
        return false;
    }

    template <DL2LT_PARAMS>
    inline void 
    left_quotient(const DL2LT& a_in,
                  const std::vector<DL2LT_SYMBOL>& word_in,
                  DL2LT& a_out) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(a_in, word_in, map);
        typename DL2LT::StateSet states;
        typename DL2LT::nl2lt_type nl2lt;
        typename DL2LT::State2Map state2_map;
        a_out.clear();
        copy_fa(a_in, nl2lt, state2_map);
        auto initial_state = add_initial_state(nl2lt);
        for (auto& [state, words] : map) {
            add_epsilon_transition(nl2lt, initial_state, state2_map.at(state));
        }
        minimize(nl2lt, a_out);
    }

    template <DL2LT_PARAMS>
    inline void 
    left_quotient(const DL2LT& a_in,
                  const std::string& word_in,
                  DL2LT& a_out) {
        unordered_map<typename DL2LT::State, 
                      std::vector<std::vector<DL2LT_SYMBOL> > > map;
        std::vector<DL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(a_in, word, map);
        a_out.clear();
        typename DL2LT::StateSet states;
        typename DL2LT::nl2lt_type nl2lt;
        typename DL2LT::State2Map state2_map;
        copy_fa(a_in, nl2lt, state2_map);
        auto initial_state = add_initial_state(nl2lt);
        for (auto& [state, words] : map) {
            add_epsilon_transition(nl2lt, initial_state, state2_map.at(state));
        }
        minimize(nl2lt, a_out);
    }

    template <NL2LT_PARAMS>
    inline void 
    left_quotient(const NL2LT& a_in,
                  const std::vector<NL2LT_SYMBOL>& word_in,
                  typename NL2LT::dl2lt_type& a_out) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        detail::translate_impl::apply(a_in, word_in, map);
        typename NL2LT::StateSet states;
        typename NL2LT::State2Map state2_map;
        typename NL2LT nl2lt;
        a_out.clear();
        copy_fa(a_in, nl2lt, state2_map);
        auto initial_state = add_initial_state(nl2lt);
        for (auto& [state, words] : map) {
            add_epsilon_transition(nl2lt, initial_state, state2_map.at(state));
        }
        minimize(nl2lt, a_out);
    }

    template <NL2LT_PARAMS>
    inline void 
    left_quotient(const NL2LT& a_in,
                  const std::string& word_in,
                  typename NL2LT::dl2lt_type& a_out) {
        unordered_map<typename NL2LT::State, 
                      std::vector<std::vector<NL2LT_SYMBOL> > > map;
        std::vector<NL2LT_SYMBOL> word(word_in.begin(), word_in.end());
        detail::translate_impl::apply(a_in, word, map);
        typename NL2LT::StateSet states;
        typename NL2LT::State2Map state2_map;
        typename NL2LT nl2lt;
        a_out.clear();
        copy_fa(a_in, nl2lt, state2_map);
        auto initial_state = add_initial_state(nl2lt);
        for (auto& [state, words] : map) {
            add_epsilon_transition(nl2lt, initial_state, state2_map.at(state));
        }
        minimize(nl2lt, a_out);
    }
}

namespace atl::detail {
    struct inverse_impl {
        template <DL2LT_PARAMS>
        static void 
        do_incerse(const DL2LT& a_in,
                   DL2LT& a_out,
                   typename DL2LT::State2Map const& state2_map) {
            typedef typename DL2LT::label_property_type LabelProperty;
            typedef typename DL2LT::label_type Label;
            typename DL2LT::OutTransitionIter t_it, t_end;
            for (const auto& [state, source] : state2_map) {
                for (tie(t_it, t_end) = out_transitions(a_in, state); t_it != t_end; t_it++) {
                    auto target = atl::target(a_in, *t_it);
                    if (state2_map.count(target) > 0) {
                        auto new_target = state2_map.at(target);
                        const auto& t = atl::get_property(a_in, *t_it);
                        if constexpr (std::is_same<LabelProperty, no_type>::value) {
                            add_transition(a_out, source, new_target, 
                                           Label(t.lower_symbol, t.upper_symbol));
                        } else {
                            add_transition(a_out, source, new_target, 
                                           Label(t.default_property.lower_symbol, 
                                                 t.default_property.upper_symbol),
                                           t.extended_property);
                        }
                    }
                }
            }
        }

        template <DL2LT_PARAMS>
        static void 
        apply(const DL2LT& a_in,
              DL2LT& a_out) {
            typename DL2LT::nl2lt_type nl2lt;
            copy_states(a_in, nl2lt);
        }
    };
}

#endif /* atl_detail_letter2letter_transducer_operate_hpp */

