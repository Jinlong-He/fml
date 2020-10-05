//
//  deterministic_letter2letter_transducer.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_deterministic_letter2letter_transducer_hpp 
#define atl_detail_deterministic_letter2letter_transducer_hpp

#include <map>
#include <boost/unordered_map.hpp>
#include <atl/detail/finite_automaton/deterministic_finite_automaton.hpp>
#include <atl/detail/letter2letter_transducer/letter2letter_transducer.hpp>

using boost::unordered_map;
namespace atl {
    template <class Symbol,
              long epsilon_,
              class LabelProperty,
              class StateProperty, 
              class AutomatonProperty>
    class nondeterministic_letter2letter_transducer;
}
namespace atl::detail {
    template <class Symbol, 
              long epsilon_,
              class LabelProperty,
              class StateProperty, 
              class AutomatonProperty>
    class deterministic_letter2letter_transducer_gen
        : public letter2letter_transducer_gen<Symbol>,
          public deterministic_finite_automaton_gen<L2LTLabel<Symbol>, epsilon_,
                                                    LabelProperty,
                                                    StateProperty,
                                                    AutomatonProperty> {
    public:
        typedef letter2letter_transducer_gen<Symbol> l2lt_type;
        typedef deterministic_finite_automaton_gen<L2LTLabel<Symbol>, epsilon_,
                                                   LabelProperty,
                                                   StateProperty,
                                                   AutomatonProperty> Base;

        typedef nondeterministic_letter2letter_transducer<Symbol, epsilon_,
                                                          LabelProperty,
                                                          StateProperty,
                                                          AutomatonProperty> nl2lt_type;

        typedef Symbol symbol_type;
        typedef L2LTLabel<Symbol> label_type;
        typedef LabelProperty label_property_type;

        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;
        typedef typename Base::transition_property_type transition_property_type;

        typedef typename Base::State State;
        typedef typename Base::StateSet StateSet;
        typedef typename Base::SymbolSet LabelSet;
        typedef typename Base::Transition Transition;
        typedef typename Base::Symbol2StateMap Symbol2StateMap;
        typedef typename Base::Symbol2StatePairMap Symbol2StatePairMap;
        typedef typename l2lt_type::SymbolSet SymbolSet;

        typedef pair<State, State> StatePair;
        typedef unordered_map<StatePair, State> StatePairMap;

        typedef typename std::conditional<std::is_same<LabelProperty, no_type>::value,
                              unordered_map<label_type, unordered_set<StatePair> >,
                              unordered_map<label_type, 
                              unordered_map<LabelProperty, 
                                            unordered_set<StatePair>> > >::type 
            Label2StatePairsMap;

        typedef typename std::conditional<std::is_same<LabelProperty, no_type>::value,
                              std::map<Symbol, std::map<Symbol, State> >,
                              std::map<Symbol, std::map<Symbol, 
                                                        std::map<LabelProperty, State> > > >::type
            Label2StateMap;

        typedef unordered_map<State, Label2StateMap> L2LTransitionMap;

    public:
        deterministic_letter2letter_transducer_gen()
            : l2lt_type(),
              Base() {}

        deterministic_letter2letter_transducer_gen(const SymbolSet& alphabet)
            : l2lt_type(alphabet),
              Base() { 
                  LabelSet label_set;
                  util::set_product(alphabet, label_set);
                  Base::set_alphabet(label_set);
              }

        deterministic_letter2letter_transducer_gen(const std::initializer_list<Symbol>& alphabet)
            : l2lt_type(alphabet),
              Base() {
                  LabelSet label_set;
                  util::set_product(alphabet, label_set);
                  Base::set_alphabet(label_set);
              }

        deterministic_letter2letter_transducer_gen(const deterministic_letter2letter_transducer_gen& x)
            : l2lt_type(x),
              Base(x),
              l2ltransition_map_(x.l2ltransition_map_) {}

        ~deterministic_letter2letter_transducer_gen() {}

        deterministic_letter2letter_transducer_gen& 
        operator=(const deterministic_letter2letter_transducer_gen& x) {
            if (&x != this) {
                l2lt_type::operator=(x);
                Base::operator=(x);
                l2ltransition_map_ = x.l2ltransition_map_;
            }
            return *this;
        }

        virtual void clear() {
            Base::clear();
            this -> symbol_set_.clear();
            l2ltransition_map_.clear();
        }

        Symbol
        epsilon_symbol() const {
            return epsilon_;
        }

        const L2LTransitionMap&
        l2ltransition_map() const {
            return l2ltransition_map_;
        }

        virtual pair<Transition, bool>
        add_transition(State s, State t, 
                       const transition_property_type& c) {
            if (Base::add_transition(s, t, c).second) {
                if constexpr (std::is_same<LabelProperty, no_type>::value) {
                    l2ltransition_map_[s][c.upper_symbol][c.lower_symbol] = t;
                } else {
                    l2ltransition_map_[s][c.default_property.upper_symbol]
                                         [c.default_property.lower_symbol][c.extended_property] = t;
                }
            }
            return std::make_pair(Transition(), false);
        }

        virtual pair<Transition, bool>
        add_transition(State s, State t,
                       const label_type& l,
                       const LabelProperty& p) {
            if (Base::add_transition(s, t, l, p).second) {
                if constexpr (!std::is_same<LabelProperty, no_type>::value) {
                    l2ltransition_map_[s][l.upper_symbol][l.lower_symbol][p] = t;
                } 
            }
            return std::make_pair(Transition(), false);
        }

        virtual void
        set_symbol_set(const SymbolSet& symbol_set) {
            l2lt_type::set_symbol_set(symbol_set);
            LabelSet label_set;
            util::set_product(symbol_set, label_set);
            Base::set_alphabet(label_set);
        }

        virtual void
        set_alphabet(const LabelSet& alphabet) {
            Base::set_alphabet(alphabet);
            for (const auto& label : alphabet) {
                this -> add_symbol(label.upper_symbol);
                this -> add_symbol(label.lower_symbol);
            }
        }
    private:
        L2LTransitionMap l2ltransition_map_;
    };
};

namespace atl {
    #define DL2LT_PARAMS typename DL2LT_SYMBOL, long DL2LT_EPSILON, typename DL2LT_LABEL_PROP, typename DL2LT_STATE_PROP, typename DL2LT_AUT_PROP
    #define DL2LT detail::deterministic_letter2letter_transducer_gen<DL2LT_SYMBOL, DL2LT_EPSILON, DL2LT_LABEL_PROP, DL2LT_STATE_PROP,DL2LT_AUT_PROP>

    template <DL2LT_PARAMS>
    inline DL2LT_SYMBOL
    epsilon_symbol(const DL2LT& dl2lt) {
        return dl2lt.epsilon_symbol();
    }

    template <DL2LT_PARAMS>
    inline typename DL2LT::L2LTransitionMap const&
    l2ltransition_map(const DL2LT& dl2lt) {
        return dl2lt.l2ltransition_map();
    }

    template <DL2LT_PARAMS>
    inline pair<typename DL2LT::Transition, bool>
    add_transition(DL2LT& dl2lt,
                   typename DL2LT::State s,
                   typename DL2LT::State t,
                   const DL2LT_SYMBOL& upper,
                   const DL2LT_SYMBOL& lower) {
        typename DL2LT::label_type l(upper, lower);
        typedef typename DL2LT::label_property_type LabelProperty;
        if constexpr (std::is_same<LabelProperty, no_type>::value) {
            return dl2lt.add_transition(s, t, l);
        } else {
            return dl2lt.add_transition(s, t, l, LabelProperty());
        }
    }
    
    template <DL2LT_PARAMS>
    inline pair<typename DL2LT::Transition, bool>
    add_transition(DL2LT& dl2lt,
                   typename DL2LT::State s,
                   typename DL2LT::State t,
                   const DL2LT_SYMBOL& upper,
                   const DL2LT_SYMBOL& lower,
                   typename DL2LT::label_property_type const& p) {
        typename DL2LT::label_type l(upper, lower);
        return dl2lt.add_transition(s, t, l, p);
    }

    template <DL2LT_PARAMS>
    inline void
    get_target_maps_in_map(const DL2LT& dl2lt,
                           typename DL2LT::State s, 
                           const DL2LT_SYMBOL& c, 
                           unordered_map<typename DL2LT::State, 
                                         unordered_set<DL2LT_SYMBOL> >& target_map) {
        const auto& l2ltransition_map_ = l2ltransition_map(dl2lt);
        auto transition_map_iter = l2ltransition_map_.find(s);
        if (transition_map_iter != l2ltransition_map_.end()) {
            const auto& map = transition_map_iter -> second;
            auto map_iter = map.find(c);
            if (map_iter != map.end()) {
                if constexpr (std::is_same<typename DL2LT::label_property_type, 
                                           no_type>::value) {
                    for (const auto& [lower, state] : map_iter -> second) {
                        target_map[state].insert(lower);
                    }
                } else {
                    for (const auto& [lower, prop_map] : map_iter -> second) {
                        for (const auto& [prop, state] : prop_map) {
                            target_map[state].insert(lower);
                        }
                    }
                }
            }
        }
    }
};

#endif /* atl_detail_deterministic_letter2letter_transducer_hpp */
