//
//  fomula_automaton.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_fomula_automaton_hpp 
#define atl_fomula_automaton_hpp

#include <atl/detail/fomula_automaton/fomula_automaton.hpp>
#include <atl/detail/fomula_automaton/translate.hpp>
#include <atl/detail/fomula_automaton/verify.hpp>
#include <atl/detail/fomula_automaton/parse.hpp>
#include <ll/propositional_logic.hpp>

namespace atl {
    template <class Fomula = ll::propositional_fomula>
    class fomula_automaton 
        : public detail::fomula_automaton_gen<Fomula> {
    public:
        typedef detail::fomula_automaton_gen<Fomula> Base;

        typedef typename Base::State State;
        typedef typename Base::Transition Transition;

        typedef typename Base::StateIter StateIter;
        typedef typename Base::TransitionIter TransitionIter;
        typedef typename Base::InTransitionIter InTransitionIter;
        typedef typename Base::OutTransitionIter OutTransitionIter;

        typedef unordered_set<State> StateSet;

        typedef unordered_map<State, State> State2Map;
    public:
        fomula_automaton()
            : Base() {}

        fomula_automaton(const fomula_automaton& x)
            : Base(x) {}

        fomula_automaton& 
        operator=(const fomula_automaton& x) {
            if (&x != this) {
                Base::operator=(x);
            }
            return *this;
        }
    };
}

#endif /* atl_fomula_automaton_hpp */
