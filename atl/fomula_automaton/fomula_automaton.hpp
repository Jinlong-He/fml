//
//  fomula_automaton.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/10/27.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_fomula_automaton_hpp 
#define atl_fomula_automaton_hpp

#include "../detail/fomula_automaton/fomula_automaton.hpp"

namespace atl {
    class fomula_automaton 
        : public detail::fomula_automaton_gen {
    public:
        typedef detail::fomula_automaton_gen Base;

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
