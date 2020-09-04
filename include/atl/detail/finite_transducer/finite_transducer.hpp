//
//  finite_transducer.hpp
//  ATL 
//
//  Created by Jinlong He on 2020/08/01.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_finite_transducer_hpp 
#define atl_detail_finite_transducer_hpp

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using boost::unordered_map;
using boost::unordered_set;

namespace atl {
        namespace detail {

        template <class UpperSymbol, 
                  class LowerSymbol>
            struct FTLable {
            public:
                FTLable() {}
                FTLable(const UpperSymbol& upper, const LowerSymbol& lower)
                    : upper_symbol(upper),
                      lower_symbols(lower) {}

                FTLable(const UpperSymbol& upper, const std::initializer_list<LowerSymbol>& lowers)
                    : upper_symbol(upper),
                      lower_symbols(lowers) {}

                FTLable(const UpperSymbol& upper, const std::vector<LowerSymbol>& lowers)
                    : upper_symbol(upper),
                      lower_symbols(lowers) {}

            friend std::ostream& operator<< (std::ostream& os, const FTLable& x) {
                os << "<" << x.upper_symbol << "," << "[";
                for (const auto& lower_symbol : x.lower_symbols) {
                    os << x.lower_symbol << " ";
                }
                os << "]";
                return os;
            }

            private:
                UpperSymbol upper_symbol;
                std::vector<LowerSymbol> lower_symbols;
            };

        
        template <class UpperSymbol, 
                  class LowerSymbol,
                  long upper_epsilon_,
                  long lower_epsilon_,
                  class LabelProperty,
                  class StateProperty, 
                  class AutomatonProperty>
        class finite_transducer_gen 
            : public automaton_gen<
                     typename std::conditional<std::is_same<LabelProperty, no_type>::value,
                                   FTLable<UpperSymbol, LowerSymbol>, 
                                   Property<FTLable<UpperSymbol, LowerSymbol>, LabelProperty> >::type,
                     typename std::conditional<std::is_same<StateProperty, no_type>::value,
                                   boost::no_property, StateProperty>::type,
                     typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type> {
        public:
            typedef FTLable<UpperSymbol, LowerSymbol> label_type;
            typedef LabelProperty label_property_type;
            typedef Property<label_type, label_property_type> transition_property;

            typedef typename std::conditional<std::is_same<LabelProperty, no_type>::value,
                                  label_type, transition_property>::type 
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
            typedef unordered_set<UpperSymbol> UpperSymbolSet;
            typedef unordered_set<LowerSymbol> LowerSymbolSet;

            typedef unordered_map<State, State> State2Map;

        public:
            finite_transducer_gen(const UpperSymbolSet& upper_alphabet,
                                  const LowerSymbolSet& lower_alphabet)
                : Base(),
                  initial_state_(-1),
                  upper_alphabet_(upper_alphabet),
                  lower_alphabet_(lower_alphabet) {}

            finite_transducer_gen(const std::initializer_list<UpperSymbol>& upper_alphabet,
                                  const std::initializer_list<LowerSymbol>& lower_alphabet)
                : Base(),
                  initial_state_(-1),
                  upper_alphabet_(upper_alphabet),
                  lower_alphabet_(lower_alphabet) {}

            finite_transducer_gen(const finite_transducer_gen& x)
                : Base(x),
                  initial_state_(x.initial_state_),
                  final_state_set_(x.final_state_set_),
                  state_set_(x.state_set_),
                  upper_alphabet_(x.upper_alphabet_),
                  lower_alphabet_(x.lower_alphabet_) {}

            ~finite_transducer_gen() {}

            finite_transducer_gen& 
            operator=(const finite_transducer_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    initial_state_ = x.initial_state_;
                    final_state_set_ = x.final_state_set_;
                    state_set_ = x.state_set_;
                    upper_alphabet_ = x.upper_alphabet_;
                    lower_alphabet_ = x.lower_alphabet_;
                }
                return *this;
            }

            virtual void clear() {
                Base::clear();
                initial_state_ = -1;
                final_state_set_.clear();
                state_set_.clear();
                upper_alphabet_.clear();
                lower_alphabet_.clear();
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

            UpperSymbol
            upper_epsilon() const {
                return upper_epsilon_;
            }

            LowerSymbol
            lower_epsilon() const {
                return lower_epsilon_;
            }

            label_type
            epsilon() const {
                return label_type(upper_epsilon_, lower_epsilon_);
            }

            transition_property
            epsilon_transition() const {
                return transition_property(epsilon());
            }

            State
            initial_state() const {
                return initial_state_;
            }

            void 
            set_initial_state(State state) {
                initial_state_ = state;
            }

            const StateSet&
            state_set() const {
                return state_set_;
            }

            void
            set_state_set(const StateSet& state_set) {
                state_set_ = state_set;
            }

            const StateSet&
            final_state_set() const {
                return final_state_set_;
            }

            void
            set_final_state_set(const StateSet& final_state_set) {
                final_state_set_ = final_state_set;
            }

            void
            clear_finale_state_set() {
                final_state_set_.clear();
            }

            const UpperSymbolSet&
            upper_alphabet() const {
                return upper_alphabet_;
            }

            const LowerSymbolSet&
            lower_alphabet() const {
                return lower_alphabet_;
            }

            void
            set_upper_alphabet(const UpperSymbolSet& alphabet) {
                upper_alphabet_ = alphabet;
            }

            void
            set_lower_alphabet(const LowerSymbolSet& alphabet) {
                lower_alphabet_ = alphabet;
            }

            void 
            set_upper_alphabet(const UpperSymbol& c) {
                upper_alphabet_.insert(c);
            }

            void 
            set_lower_alphabet(const LowerSymbol& c) {
                lower_alphabet_.insert(c);
            }

            void 
            set_state(State state) {
                state_set_.insert(state);
            }

            void 
            set_final_state(State state) {
                final_state_set_.insert(state);
            }

            void 
            remove_final_state(State state) {
                final_state_set_.erase(state);
            }
            
            using Base::add_transition;
            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const transition_property_type& c) {
                if constexpr (std::is_same<LabelProperty, no_type>::value) {
                    if (c == epsilon()) this -> set_flag(4, 1);
                } else {
                    if (c.default_property == epsilon()) this -> set_flag(4, 1);
                }
                return Base::add_transition(s, t, c);
            }

            virtual pair<Transition, bool>
            add_transition(State s, State t,
                           const UpperSymbol& upper,
                           const LowerSymbol& lower,
                           const LabelProperty& p) {
                if constexpr (std::is_same<LabelProperty, no_type>::value) {
                    auto label = label_type(upper, lower);
                    if (label == epsilon()) this -> set_flag(4, 1);
                    return Base::add_transition(s, t, label);
                } else {
                    auto label = label_type(upper, lower);
                    if (label == epsilon()) this -> set_flag(4, 1);
                    return Base::add_transition(s, t, transition_property(label, p));
                }
            }

        private:
            State initial_state_;
            StateSet final_state_set_;
            StateSet state_set_;
            UpperSymbolSet upper_alphabet_;
            LowerSymbolSet lower_alphabet_;
        };
    };

    #define FT_PARAMS typename FT_USYMBOL, typename FT_LSYMBOL, long FT_UEPSILON, long FT_LEPSILON,  typename FT_LABEL_PROP, typename FT_STATE_PROP, typename FT_AUT_PROP
    #define FT detail::finite_transducer_gen<FT_USYMBOL, FT_LSYMBOL, FT_UEPSILON, FT_LEPSILON,FT_LABEL_PROP, FT_STATE_PROP,FT_AUT_PROP>

    template <FT_PARAMS>
    inline typename FT::SymbolSet const&
    alphabet(const FT& fa) {
        return fa.alphabet();
    }

    template <FT_PARAMS>
    inline void
    set_alphabet(FT& fa,
                 typename FT::SymbolSet const& set) {
        fa.set_alphabet(set);
    }

    template <FT_PARAMS>
    inline typename FT::State
    initial_state(const FT& fa) {
        return fa.initial_state();
    }

    template <FT_PARAMS>
    inline void
    set_initial_state(FT& fa, 
                      typename FT::State state) {
        fa.set_initial_state(state);
    }

    template <FT_PARAMS>
    inline typename FT::StateSet const&
    state_set(const FT& fa) {
        return fa.state_set();
    }

    template <FT_PARAMS>
    inline void  
    set_state_set(FT& fa, 
                  typename FT::StateSet const& set) {
        fa.set_state_set(set);
    }

    template <FT_PARAMS>
    inline void  
    set_state(FT& fa, 
              typename FT::State state) {
        fa.set_state(state);
    }

    template <FT_PARAMS>
    inline typename FT::StateSet const&
    final_state_set(const FT& fa) {
        return fa.final_state_set();
    }

    template <FT_PARAMS>
    inline void  
    set_final_state_set(FT& fa, 
                        typename FT::StateSet const& set) {
        fa.set_final_state_set(set);
    }

    template <FT_PARAMS>
    inline void  
    set_final_state(FT& fa, 
                    typename FT::State state) {
        fa.set_final_state(state);
    }
    
    template <FT_PARAMS>
    inline void  
    remove_final_state(FT& fa, 
                       typename FT::State state) {
        fa.remove_final_state(state);
    }

    template <FT_PARAMS>
    inline void  
    clear_finale_state_set(FT& fa) {
        fa.clear_finale_state_set();
    }

    template <FT_PARAMS>
    inline typename FT::symbol_type
    epsilon(const FT& fa) {
        return fa.epsilon();
    }

    template <FT_PARAMS>
    inline typename FT::transition_property
    epsilon_transition(const FT& fa) {
        return fa.epsilon_transition();
    }

    template <FT_PARAMS>
    inline typename FT::State
    add_initial_state(FT& fa,
                      typename FT::state_property_type const& p) {
        typename FT::State s = add_state(fa, p);
        fa.set_initial_state(s);
        return s;
    }

    template <FT_PARAMS>
    inline typename FT::State
    add_initial_state(FT& fa) {
        typename FT::State s = add_state(fa);
        fa.set_initial_state(s);
        return s;
    }

    template <FT_PARAMS>
    inline typename FT::State
    add_final_state(FT& fa,
                    typename FT::state_property_type const& p) {
        typename FT::State s = add_state(fa, p);
        fa.set_final_state(s);
        return s;
    }

    template <FT_PARAMS>
    inline typename FT::State
    add_final_state(FT& fa) {
        typename FT::State s = add_state(fa);
        fa.set_final_state(s);
        return s;
    }

    template <FT_PARAMS>
    inline pair<typename FT::Transition, bool>
    add_transition(FT& fa,
                   typename FT::State s,
                   typename FT::State t,
                   typename FT::symbol_type c,
                   typename FT::symbol_property_type p) {
        return fa.add_transition(s, t, c, p);
    }
    
    template <FT_PARAMS>
    inline bool
    is_initial_state(const FT& fa,
                     typename FT::State s) {
        return (s == initial_state(fa));
    }

    template <FT_PARAMS>
    inline bool
    is_final_state(const FT& fa,
                   typename FT::State s) {
        return fa.final_state_set().count(s);
    }

    template <FT_PARAMS>
    inline bool
    has_final_state(const FT& fa,
                   typename FT::StateSet const& set) {
        typename FT::StateSet res;
        util::set_intersection(fa.final_state_set(), set, res);
        return res.size();
    }

    template <FT_PARAMS>
    inline void 
    set_forward_reachable_flag(FT& a, bool b = true) {
        a.set_flag(1, b);
        a.set_flag(0, 0);
    }

    template <FT_PARAMS>
    inline bool
    is_forward_reachable(const FT& a) {
        return (a.flag(1) & !a.flag(0));
    }

    template <FT_PARAMS>
    inline void 
    set_minimal_flag(FT& a, bool b = true) {
        a.set_flag(2, b);
        a.set_flag(1, 1);
        a.set_flag(0, 0);
    }

    template <FT_PARAMS>
    inline bool
    is_minimal(const FT& a) {
        return (a.flag(2) & !a.flag(0));
    }

    template <FT_PARAMS>
    inline void 
    set_undeterministic_flag(FT& a, bool b = true) {
        a.set_flag(3, b);
    }

    template <FT_PARAMS>
    inline bool
    is_undeterministic(const FT& a) {
        return (a.flag(3) | a.flag(4));
    }

    template <FT_PARAMS>
    inline void 
    set_epsilon_flag(FT& a, bool b = true) {
        a.set_flag(4, b);
    }

    template <FT_PARAMS>
    inline bool
    has_epsilon_transition(const FT& a) {
        return a.flag(4);
    }

    template <FT_PARAMS>
    inline bool
    is_empty(const FT& a) {
        if (is_forward_reachable(a)) {
            if (a.final_state_set().size() == 0) return true;
            return false;
        } else {
            typename FT::StateSet states;
            reachable_closure(a, states);
            if (!has_final_state(a, states)) return true;
            return false;
        }
    }
}

#endif /* atl_detail_finite_transducer_hpp */
