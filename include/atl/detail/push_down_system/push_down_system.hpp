//
//  push_down_system.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_push_down_system_hpp 
#define atl_detail_push_down_system_hpp

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using std::unordered_map, std::unordered_set;

namespace atl {
    namespace detail {
        template <class Symbol>
        struct PDSLabel {
        public:
            PDSLabel() {}
            PDSLabel(const Symbol& c, const std::vector<Symbol>& cs)
                : symbol(c),
                  symbols(cs) {}
            friend std::ostream& operator<< (std::ostream& os, const PDSLabel& x) {
                os << x.symbol << " (";
                for (ID i = 0; i < x.symbols.size(); i++) {
                    if (i < x.symbols.size() - 1) {
                        os << x.symbols[i] << " ";
                    } else {
                        os << x.symbols[i];
                    }
                }
                os << ")";
                return os;
            }
        private:
            Symbol symbol;
            std::vector<Symbol> symbols;
        };

        template <class Symbol, 
                  class SymbolProperty,
                  class StateProperty, 
                  class AutomatonProperty>
        class push_down_system_gen 
            : public automaton_gen<
                     typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                   PDSLabel<Symbol>, 
                                   Property<PDSLabel<Symbol>, 
                                            SymbolProperty> >::type,
                     typename std::conditional<std::is_same<StateProperty, no_type>::value,
                                   boost::no_property, StateProperty>::type,
                     typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type> {
        public:
            typedef PDSLabel<Symbol> label_type;
            typedef Property<label_type, SymbolProperty> transition_property;
            typedef Symbol symbol_type;
            typedef SymbolProperty symbol_property_type;
            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  PDSLabel<Symbol>, 
                                  Property<PDSLabel<Symbol>, SymbolProperty> >::type
                transition_property_type;
            typedef typename std::conditional<std::is_same<StateProperty, no_type>::value,
                                   boost::no_property, StateProperty>::type
                state_property_type;
            typedef typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type
                automaton_property_type;
            typedef automaton_gen<transition_property_type, 
                                  state_property_type, 
                                  automaton_property_type> Base;


            typedef typename Base::Transition Transition;
            typedef typename Base::State State;

            typedef unordered_set<State> StateSet;
            typedef unordered_set<Symbol> SymbolSet;

            typedef typename std::conditional<std::is_same<SymbolProperty, no_type>::value,
                                  unordered_map<State, 
                                        unordered_set<std::vector<Symbol> > > , 
                                  unordered_map<State, 
                                            unordered_map<std::vector<Symbol>, 
                                                      unordered_set<SymbolProperty> > > >::type
                State2StackMap;

            typedef unordered_map<State, unordered_map<Symbol, State2StackMap> > TransitionMap;
            typedef unordered_map<State, State> State2Map;

        public:
            push_down_system_gen(const SymbolSet alphabet = SymbolSet())
                : Base(),
                  alphabet_(alphabet) {}

            push_down_system_gen(const std::initializer_list<Symbol>& alphabet)
                : Base(),
                  alphabet_(alphabet) {}

            push_down_system_gen(const push_down_system_gen& x)
                : Base(x),
                  alphabet_(x.alphabet_),
                  state_set_(x.state_set_) {}

            push_down_system_gen& 
            operator=(const push_down_system_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    alphabet_ = x.alphabet_;
                    state_set_ = x.state_set_;
                }
                return *this;
            }

            virtual void clear() {
                Base::clear();
                state_set_.clear();
                alphabet_.clear();
            }

            virtual State 
            add_state(const state_property_type& p) {
                State state = Base::add_state(p);
                state_set_.insert(state);
                return state;
            }

            virtual State 
            add_state() {
                State state = Base::add_state();
                state_set_.insert(state);
                return state;
            }

            const StateSet&
            state_set() const {
                return state_set_;
            }

            void
            set_state_set(const StateSet& state_set) {
                state_set_ = state_set;
            }

            const SymbolSet&
            alphabet() const {
                return alphabet_;
            }

            void
            set_alphabet(const SymbolSet& alphabet) {
                alphabet_ = alphabet;
            }

            void 
            set_alphabet(const Symbol& c) {
                alphabet_.insert(c);
            }

            void 
            set_state(State state) {
                state_set_.insert(state);
            }

            const TransitionMap&
            transition_map() const {
                return transition_map_;
            }
            
            pair<Transition, bool>
            add_transition(State s, State t,
                           const Symbol& c,
                           const std::vector<Symbol>& cs,
                           const SymbolProperty& p) {
                auto pds_label = PDSLabel<Symbol>(c, cs);
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    transition_map_[s][c][t].insert(cs);
                    return Base::add_transition(s, t, pds_label);
                } else {
                    transition_map_[s][c][t][cs].insert(p);
                    return Base::add_transition(s, t, transition_property(pds_label, p));
                }
            }

            using Base::add_transition;
            pair<Transition, bool>
            add_transition(State s, State t,
                           const PDSLabel<Symbol>& l,
                           const SymbolProperty& p) {
                if constexpr (std::is_same<SymbolProperty, no_type>::value) {
                    transition_map_[s][l.symbol][t].insert(l.symbols);
                    return Base::add_transition(s, t, l);
                } else {
                    transition_map_[s][l.symbol][t][l.symbols].insert(p);
                    return Base::add_transition(s, t, transition_property(l, p));
                }
            }

        private:
            StateSet state_set_;
            SymbolSet alphabet_;
            TransitionMap transition_map_;
        };
    }

    #define PDS_PARAMS typename PDS_SYMBOL, typename PDS_SYMBOL_PROP, typename PDS_STATE_PROP, typename PDS_AUT_PROP
    #define PDS detail::push_down_system_gen<PDS_SYMBOL, PDS_SYMBOL_PROP, PDS_STATE_PROP,PDS_AUT_PROP>

    template <PDS_PARAMS>
    inline typename PDS::SymbolSet const&
    alphabet(const PDS& pds) {
        return pds.alphabet();
    }

    template <PDS_PARAMS>
    inline void
    set_alphabet(PDS& pds,
                 typename PDS::SymbolSet const& set) {
        pds.set_alphabet(set);
    }

    template <PDS_PARAMS>
    inline typename PDS::StateSet const&
    state_set(const PDS& pds) {
        return pds.state_set();
    }

    template <PDS_PARAMS>
    inline void  
    set_state_set(PDS& pds, 
                  typename PDS::StateSet const& set) {
        pds.set_state_set(set);
    }

    template <PDS_PARAMS>
    inline void  
    set_state(PDS& pds, 
              typename PDS::State state) {
        pds.set_state(state);
    }

    template <PDS_PARAMS>
    inline typename PDS::TransitionMap const&
    transition_map(const PDS& pds) {
        return pds.transition_map();
    }

    template <PDS_PARAMS>
    inline pair<typename PDS::Transition, bool>
    add_transition(PDS& pds,
                   typename PDS::State s,
                   typename PDS::State t,
                   typename PDS::symbol_type c,
                   const std::vector<typename PDS::symbol_type>& cs,
                   typename PDS::symbol_property_type p = typename PDS::symbol_property_type()) {
        return pds.add_transition(s, t, c, cs, p);
    }
    
    template <PDS_PARAMS>
    inline pair<typename PDS::Transition, bool>
    add_transition(PDS& pds,
                   typename PDS::State s,
                   typename PDS::State t,
                   typename PDS::label_type l,
                   typename PDS::symbol_property_type p) {
        return pds.add_transition(s, t, l, p);
    }
}

#endif /* atl_detail_push_down_system_hpp */
