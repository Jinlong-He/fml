//
//  nondeterministic_letter2letter_transducer.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_nondeterministic_letter2letter_transducer_hpp 
#define atl_detail_nondeterministic_letter2letter_transducer_hpp

#include <map>
#include <boost/unordered_map.hpp>
#include <atl/detail/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <atl/detail/letter2letter_transducer/letter2letter_transducer.hpp>

using boost::unordered_map;

namespace atl::detail {
    template <class Symbol, 
              long epsilon_,
              class LabelProperty,
              class StateProperty, 
              class AutomatonProperty>
    class nondeterministic_letter2letter_transducer_gen
        : public letter2letter_transducer_gen,
          public nondeterministic_finite_automaton_gen<L2LTLabel<Symbol>, epsilon_,
                                                       LabelProperty,
                                                       StateProperty,
                                                       AutomatonProperty> {
    public:
        typedef nondeterministic_finite_automaton_gen<L2LTLabel<Symbol>, epsilon_,
                                                      LabelProperty,
                                                      StateProperty,
                                                      AutomatonProperty> Base;

        typedef Symbol symbol_type;
        typedef L2LTLabel<Symbol> label_type;
        typedef LabelProperty label_property_type;

        typedef typename Base::state_property_type state_property_type;
        typedef typename Base::automaton_property_type automaton_property_type;
        typedef typename Base::transition_property_type transition_property_type;

        typedef typename Base::State State;
        typedef typename Base::StateSet StateSet;
        typedef typename Base::SymbolSet SymbolSet;
        typedef typename Base::Transition Transition;
        typedef typename Base::Symbol2StateSetMap Symbol2StateSetMap;

        typedef pair<State, State> StatePair;
        typedef unordered_map<StatePair, State> StatePairMap;

        typedef typename std::conditional<std::is_same<LabelProperty, no_type>::value,
                              std::map<Symbol, std::map<Symbol, StateSet> >,
                              std::map<Symbol, std::map<Symbol, 
                                                        std::map<LabelProperty, StateSet> > > >::type
            Label2StateSetMap;

        typedef unordered_map<State, Label2StateSetMap> L2LTransitionMap;

    public:
        nondeterministic_letter2letter_transducer_gen()
            : Base() {}

        nondeterministic_letter2letter_transducer_gen(const SymbolSet& alphabet)
            : Base(alphabet) {}

        nondeterministic_letter2letter_transducer_gen(const std::initializer_list<Symbol>& alphabet)
            : Base(alphabet) {}

        nondeterministic_letter2letter_transducer_gen(const nondeterministic_letter2letter_transducer_gen& x)
            : Base(x),
              l2ltransition_map_(x.l2ltransition_map_) {}

        ~nondeterministic_letter2letter_transducer_gen() {}

        nondeterministic_letter2letter_transducer_gen& 
        operator=(const nondeterministic_letter2letter_transducer_gen& x) {
            if (&x != this) {
                Base::operator=(x);
                l2ltransition_map_ = x.l2ltransition_map_;
            }
            return *this;
        }

        virtual void clear() {
            Base::clear();
            l2ltransition_map_.clear();
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
                    l2ltransition_map_[s][c.upper_symbol][c.lower_symbol].insert(t);
                } else {
                    l2ltransition_map_[s][c.default_property.upper_symbol]
                                         [c.default_property.lower_symbol]
                                         [c.extended_property].insert(t);
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
                    l2ltransition_map_[s][l.upper_symbol][l.lower_symbol][p].insert(t);
                }
            }
            return std::make_pair(Transition(), false);
        }

    private:
        L2LTransitionMap l2ltransition_map_;
    };
}

namespace atl {
    #define NL2LT_PARAMS typename NL2LT_SYMBOL, long NL2LT_EPSILON, typename NL2LT_LABEL_PROP, typename NL2LT_STATE_PROP, typename NL2LT_AUT_PROP
    #define NL2LT detail::nondeterministic_letter2letter_transducer_gen<NL2LT_SYMBOL, NL2LT_EPSILON, NL2LT_LABEL_PROP, NL2LT_STATE_PROP,NL2LT_AUT_PROP>

    template <NL2LT_PARAMS>
    inline typename NL2LT::L2LTransitionMap const&
    l2ltransition_map(const NL2LT& dl2lt) {
        return dl2lt.l2ltransition_map();
    }

    template <NL2LT_PARAMS>
    inline pair<typename NL2LT::Transition, bool>
    add_transition(NL2LT& nl2lt,
                   typename NL2LT::State s,
                   typename NL2LT::State t,
                   const NL2LT_SYMBOL& upper,
                   const NL2LT_SYMBOL& lower) {
        typename NL2LT::label_type l(upper, lower);
        return nl2lt.add_transition(s, t, l);
    }
    
    template <NL2LT_PARAMS>
    inline pair<typename NL2LT::Transition, bool>
    add_transition(NL2LT& nl2lt,
                   typename NL2LT::State s,
                   typename NL2LT::State t,
                   const NL2LT_SYMBOL& upper,
                   const NL2LT_SYMBOL& lower,
                   typename NL2LT::label_property_type p) {
        typename NL2LT::label_type l(upper, lower);
        return nl2lt.add_transition(s, t, l, p);
    }

    template <NL2LT_PARAMS>
    inline void
    get_target_maps_in_map(const NL2LT& dl2lt,
                          typename NL2LT::State s, 
                          const NL2LT_SYMBOL& c, 
                          unordered_map<typename NL2LT::State, 
                                        unordered_set<NL2LT_SYMBOL> >& target_map) {
        const auto& l2ltransition_map_ = l2ltransition_map(dl2lt);
        auto transition_map_iter = l2ltransition_map_.find(s);
        if (transition_map_iter != l2ltransition_map_.end()) {
            const auto& map = transition_map_iter -> second;
            auto map_iter = map.find(c);
            if (map_iter != map.end()) {
                if constexpr (std::is_same<typename NL2LT::label_property_type, 
                                           no_type>::value) {
                    for (const auto& [lower, states] : map_iter -> second) {
                        for (auto state : states) {
                            target_map[state].insert(lower);
                        }
                    }
                } else {
                    for (const auto& [lower, prop_map] : map_iter -> second) {
                        for (const auto& [prop, states] : prop_map) {
                            for (auto state : states) {
                                target_map[state].insert(lower);
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif /* atl_detail_nondeterministic_letter2letter_transducer_hpp */
