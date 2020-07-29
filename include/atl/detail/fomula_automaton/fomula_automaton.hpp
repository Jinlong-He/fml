//
//  fomula_automaton.hpp
//  atl 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_detail_fomula_automaton_hpp 
#define atl_detail_fomula_automaton_hpp

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../../../util/util.hpp"
#include "../automaton.hpp"
#include "../no_type.hpp"
#include "../../../ll/propositional_logic.hpp"

using boost::unordered_map;
using boost::unordered_set;

namespace atl {
    namespace detail {

        template <class FomulaProperty, 
                  class StateVarProperty,
                  class AutomatonProperty>
        class fomula_automaton_gen 
            : public automaton_gen<
                     typename std::conditional<std::is_same<FomulaProperty, no_type>::value,
                                   ll::propositional_fomula, 
                                   Property<ll::propositional_fomula, FomulaProperty> >::type,
                     typename std::conditional<std::is_same<StateVarProperty, no_type>::value,
                                   ll::item, Property<ll::item, StateVarProperty> >::type,
                     typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                   boost::no_property, AutomatonProperty>::type> {
        public:
            typedef FomulaProperty fomula_property_type;
            typedef StateVarProperty statevar_property_type;

            typedef Property<ll::propositional_fomula, 
                             FomulaProperty> fomula_property;
            typedef Property<ll::item, StateVarProperty> statevar_property;

            typedef typename std::conditional<std::is_same<FomulaProperty, no_type>::value,
                                  ll::propositional_fomula, 
                                  fomula_property>::type transition_property_type;
            typedef typename std::conditional<std::is_same<StateVarProperty, no_type>::value,
                                  ll::item, statevar_property>::type state_property_type;
            typedef typename std::conditional<std::is_same<AutomatonProperty, no_type>::value,
                                  boost::no_property, 
                                  AutomatonProperty>::type automaton_property_type;

            typedef automaton_gen<transition_property_type,
                                  state_property_type,
                                  automaton_property_type> Base;

            typedef typename Base::State State;
            typedef typename Base::Transition Transition;

            typedef typename Base::StateIter StateIter;
            typedef typename Base::TransitionIter TransitionIter;
            typedef typename Base::InTransitionIter InTransitionIter;
            typedef typename Base::OutTransitionIter OutTransitionIter;

            typedef unordered_set<State> StateSet;

            typedef unordered_map<State, State> State2Map;
        public:
            fomula_automaton_gen()
                : Base() {}

            fomula_automaton_gen(const fomula_automaton_gen& x)
                : Base(x),
                  state_set_(x.state_set_),
                  control_state_set_(x.control_state_set_),
                  input_state_set_(x.input_state_set_) {}

            fomula_automaton_gen& 
            operator=(const fomula_automaton_gen& x) {
                if (&x != this) {
                    Base::operator=(x);
                    state_set_ = x.state_set_;
                    control_state_set_ = x.control_state_set_;
                    input_state_set_ = x.input_state_set_;
                }
                return *this;
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
            control_state_set() const {
                return control_state_set_;
            }

            void 
            set_control_state_set(const StateSet& control_state_set) {
                control_state_set_ = control_state_set;
            }

            const StateSet&
            input_state_set() const {
                return input_state_set_;
            }

            void 
            set_input_state_set(const StateSet& input_state_set) {
                input_state_set_ = input_state_set;
            }

            const std::list<ll::propositional_fomula>&
            init_list() const {
                return init_list_;
            }

            void set_init_list(const std::list<ll::propositional_fomula>& list) {
                init_list_ = list;
            }

            void 
            set_state(State state) {
                state_set_.insert(state);
            }

            void 
            set_control_state(State state) {
                control_state_set_.insert(state);
            }

            void 
            set_input_state(State state) {
                input_state_set_.insert(state);
            }

            void 
            add_init_list(const ll::propositional_fomula& f) {
                init_list_.push_back(f);
            }

            State
            add_state(const state_property_type& p) {
                if constexpr (std::is_same<StateVarProperty, boost::no_property>::value) {
                    if (p.type() == "integer") this -> set_flag(1, 1);
                } else {
                    if (p.default_property.type() == "integer") this -> set_flag(1, 1);
                }
                return Base::add_state(p);
            }
            
            pair<Transition, bool>
            add_transition(State s, State t,
                           const ll::propositional_fomula& f,
                           const FomulaProperty& p) {
                if constexpr (std::is_same<FomulaProperty, no_type>::value) {
                    return Base::add_transition(s, t, f);
                } else {
                    return Base::add_transition(s, t, transition_property(f, p));
                }
            }

        private:
            StateSet state_set_;
            StateSet control_state_set_;
            StateSet input_state_set_;
            std::list<ll::propositional_fomula> init_list_;
        };
    }

    #define FOA_PARAMS typename FP, typename SP, typename AP
    #define FOA detail::fomula_automaton_gen<FP, SP, AP>

    template <FOA_PARAMS>
    const typename FOA::StateSet&
    control_state_set(const FOA& foa) {
        return foa.control_state_set();
    }

    template <FOA_PARAMS>
    const typename FOA::StateSet&
    input_state_set(const FOA& foa) {
        return foa.input_state_set();
    }

    template <FOA_PARAMS>
    inline const std::list<ll::propositional_fomula>& 
    init_list(const FOA& foa) {
        return foa.init_list();
    }

    template <FOA_PARAMS>
    inline typename FOA::State
    add_control_state(FOA& foa,
                      typename FOA::state_property_type const& p,
                      const ll::propositional_fomula& f) {
        typedef typename FOA::statevar_property_type StateVarProperty;
        typename FOA::State s = add_state(foa, p);
        foa.set_control_state(s);
        foa.add_init_list(f);
        return s;
    }

    template <FOA_PARAMS>
    inline typename FOA::State
    add_control_state(FOA& foa,
                      const ll::item& v,
                      typename FOA::statevar_property_type const& p,
                      const ll::propositional_fomula& f) {
        typedef typename FOA::statevar_property_type StateVarProperty;
        if constexpr (std::is_same<StateVarProperty, boost::no_property>::value) {
            return add_control_state(foa, v);
        } else {
            return add_control_state(foa, Property(v, p));
        }
    }

    template <FOA_PARAMS>
    inline typename FOA::State
    add_input_state(FOA& foa,
                    typename FOA::state_property_type const& p) {
        typename FOA::State s = add_state(foa, p);
        typedef typename FOA::statevar_property_type StateVarProperty;
        foa.set_input_state(s);
        return s;
    }
    
    template <FOA_PARAMS>
    inline typename FOA::State
    add_input_state(FOA& foa,
                    const ll::item& v,
                    typename FOA::statevar_property_type const& p) {
        typedef typename FOA::statevar_property_type StateVarProperty;
        if constexpr (std::is_same<StateVarProperty, boost::no_property>::value) {
            return add_input_state(foa, v);
        } else {
            return add_input_state(foa, Property(v, p));
        }
    }

    template <FOA_PARAMS>
    inline pair<typename FOA::Transition, bool>
    add_transition(FOA& foa,
                   typename FOA::State s, 
                   typename FOA::State t,
                   const ll::propositional_fomula& f,
                   typename FOA::fomula_property_type const& p) {
        return foa.add_transition(s, t, f, p);
    }

    template <FOA_PARAMS>
    inline void 
    set_infinite_flag(FOA& foa, bool b = true) {
        foa.set_flag(1, b);
    }

    template <FOA_PARAMS>
    inline bool
    is_infinite(const FOA& foa) {
        return foa.flag(1);
    }
}

#endif /* atl_detail_fomula_automaton_hpp */
