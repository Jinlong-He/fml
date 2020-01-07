//
//  algorithm.hpp
//  atl 
//
//  Created by Jinlong He on 2020/01/03.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_regular_expression_algorithm_hpp 
#define atl_regular_expression_algorithm_hpp
#include <iostream>
#include "basic_regular_expression.hpp"
#include "../finite_automaton/nondeterministic_finite_automaton.hpp"
using std::cout;
using std::endl;

namespace atl {
    template <typename Symbol>
    struct NFARegularSymbol {
        detail::RegularSymbol<Symbol> regular_symbol;
        nondeterministic_finite_automaton<Symbol>* nfa;

        NFARegularSymbol() 
            : regular_symbol(),
              nfa(nullptr) {}

        NFARegularSymbol(const detail::RegularSymbol<Symbol>& c)
            : regular_symbol(c),
              nfa(nullptr) {}

        NFARegularSymbol(const detail::RegularSymbol<Symbol>& c,
                         const nondeterministic_finite_automaton<Symbol>& a)
            : regular_symbol(c),
              nfa(&a) {}
    };

    struct make_fa_impl {
        template <typename Symbol>
        static void
        union_automaton(NFARegularSymbol<Symbol>& c_lhs,
                        NFARegularSymbol<Symbol>& c_rhs,
                        char opt) {
            auto& a_lhs = c_lhs.nfa;
            auto& a_rhs = c_rhs.nfa;
            if (a_lhs == nullptr && a_rhs == nullptr) {
                a_lhs = new nondeterministic_finite_automaton<Symbol>();
                auto i_state = add_initial_state(*a_lhs);
                auto f_state = add_final_state(*a_lhs);
                add_transition(*a_lhs, i_state, f_state, c_lhs.regular_symbol.symbol);
                add_transition(*a_lhs, i_state, f_state, c_rhs.regular_symbol.symbol);
            } else if (a_lhs == nullptr && a_rhs != nullptr) {
                a_lhs = a_rhs;
                for (auto state : final_state_set(*a_lhs)) {
                    add_transition(*a_lhs, initial_state(*a_lhs), state, 
                                   c_lhs.regular_symbol.symbol);
                }
            } else if (a_lhs != nullptr && a_rhs == nullptr) {
                for (auto state : final_state_set(*a_lhs)) {
                    add_transition(*a_lhs, initial_state(*a_lhs), state, 
                                   c_rhs.regular_symbol.symbol);
                }
            } else {
                typedef typename nondeterministic_finite_automaton<Symbol>::StateSet StateSet;
                auto state_lhs = initial_state(*a_lhs);
                typename nondeterministic_finite_automaton<Symbol>::State2Map state2_map;
                copy_fa_impl::copy_states(*a_rhs, *a_lhs, state2_map, StateSet());
                copy_fa_impl::copy_transitions(*a_rhs, *a_lhs, state2_map);
                auto state_rhs = initial_state(*a_lhs);
                auto i_state = add_initial_state(*a_lhs);
                add_transition(*a_lhs, i_state, state_lhs, epsilon(*a_lhs));
                add_transition(*a_lhs, i_state, state_rhs, epsilon(*a_lhs));
                delete a_rhs;
            }
        }
        template <typename Symbol>
        static void
        concat_automaton(NFARegularSymbol<Symbol>& c_lhs,
                         NFARegularSymbol<Symbol>& c_rhs,
                         char opt) {
            auto& a_lhs = c_lhs.nfa;
            auto& a_rhs = c_rhs.nfa;
            if (a_lhs == nullptr && a_rhs == nullptr) {
                a_lhs = new nondeterministic_finite_automaton<Symbol>();
                auto i_state = add_initial_state(*a_lhs);
                auto f_state = add_final_state(*a_lhs);
                auto state = add_state(*a_lhs);
                add_transition(*a_lhs, i_state, state, c_lhs.regular_symbol.symbol);
                add_transition(*a_lhs, state, f_state, c_rhs.regular_symbol.symbol);
            } else if (a_lhs == nullptr && a_rhs != nullptr) {
                a_lhs = a_rhs;
                auto i_state = add_state(*a_lhs);
                add_transition(*a_lhs, i_state, initial_state(*a_lhs), 
                               c_lhs.regular_symbol.symbol);
                set_initial_state(*a_lhs, i_state);
            } else if (a_lhs != nullptr && a_rhs == nullptr) {
                auto f_state = add_state(*a_lhs);
                for (auto state : final_state_set(*a_lhs)) {
                    add_transition(*a_lhs, state, f_state, c_rhs.regular_symbol.symbol);
                }
                clear_finale_state_set(*a_lhs);
                set_final_state(*a_lhs, f_state);
            } else {
                typedef typename nondeterministic_finite_automaton<Symbol>::StateSet StateSet;
                auto i_state = initial_state(*a_lhs);
                auto final_state_set_ = final_state_set(*a_lhs);
                typename nondeterministic_finite_automaton<Symbol>::State2Map state2_map;
                copy_fa_impl::copy_states(*a_rhs, *a_lhs, state2_map, StateSet());
                copy_fa_impl::copy_transitions(*a_rhs, *a_lhs, state2_map);
                for (auto state : final_state_set(*a_lhs)) {
                    add_transition(*a_lhs, state, initial_state(*a_lhs), epsilon(*a_lhs));
                }
                set_initial_state(*a_lhs, i_state);
                delete a_rhs;
            }
        }

        template <typename Symbol>
        static void
        repeat_automaton(NFARegularSymbol<Symbol>& c,
                         char opt) {
            auto& a = c.nfa;
            if (a == nullptr) {
                a = new nondeterministic_finite_automaton<Symbol>();
                auto initial_state = add_initial_state(*a);
                if (opt == '*') {
                    set_final_state(*a, initial_state);
                    add_transition(*a, initial_state, initial_state, c.regular_symbol.symbol);
                } else if (opt == '+') {
                    auto final_state = add_final_state(*a);
                    add_transition(*a, initial_state, final_state, c.regular_symbol.symbol);
                    add_transition(*a, final_state, final_state, c.regular_symbol.symbol);
                } else if (opt == '?') {
                    set_final_state(*a, initial_state);
                    auto final_state = add_final_state(*a);
                    add_transition(*a, initial_state, final_state, c.regular_symbol.symbol);
                } else {
                    return;
                }
            } else {
                auto f_state = add_state(*a);
                auto i_state = add_state(*a);
                if (opt == '*') {
                    for (auto state : final_state_set(*a)) {
                        add_transition(*a, state, initial_state(*a), epsilon(*a));
                    }
                    add_transition(*a, i_state, f_state, epsilon(*a));
                } else if (opt == '+') {
                    for (auto state : final_state_set(*a)) {
                        add_transition(*a, state, initial_state(*a), epsilon(*a));
                    }
                }
                add_transition(*a, i_state, initial_state(*a), epsilon(*a));
                for (auto state : final_state_set(*a)) {
                    add_transition(*a, state, f_state, epsilon(*a));
                }
                clear_finale_state_set(*a);
                set_final_state(*a, f_state);
                set_initial_state(*a, i_state);
            }
        }

        template <typename Symbol>
        static void
        apply(const basic_regular_expression<Symbol>& re,
              deterministic_finite_automaton<Symbol>& dfa) {
            std::stack<NFARegularSymbol<Symbol> > symbol_stack;
            typename deterministic_finite_automaton<Symbol>::SymbolSet alphabet;
            for (const auto& c : re.postfix_expression()) {
                if (c.opt == 0) {
                    symbol_stack.push(c);
                    alphabet.insert(c.symbol);
                } else {
                    if (c.priority == 3) {
                        repeat_automaton(symbol_stack.top(), c.opt);
                    } else if (c.priority == 2) {
                        auto& rs = symbol_stack.top();
                        symbol_stack.pop();
                        concat_automaton(symbol_stack.top(), rs, c.opt);
                    } else {
                        auto& rs = symbol_stack.top();
                        symbol_stack.pop();
                        union_automaton(symbol_stack.top(), rs, c.opt);
                    }
                }
            }
            minimize(*(symbol_stack.top().nfa), dfa);
            set_alphabet(dfa, alphabet);
            delete symbol_stack.top().nfa;
        }
    };

    template <typename Symbol>
    inline void
    make_fa(const basic_regular_expression<Symbol>& re,
            deterministic_finite_automaton<Symbol>& dfa) {
        make_fa_impl::apply(re, dfa);
    }
}

#endif /* atl_regular_expression_algorithm_hpp */
