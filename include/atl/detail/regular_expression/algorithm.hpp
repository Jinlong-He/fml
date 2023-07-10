//
//  algorithm.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_regular_expression_algorithm_hpp 
#define atl_detail_regular_expression_algorithm_hpp
#include "basic_regular_expression.hpp"
#include <fml/atl/finite_automaton/nondeterministic_finite_automaton.hpp>
#include <fml/atl/finite_automaton/deterministic_finite_automaton.hpp>

namespace atl::detail {
    template <typename Symbol>
    struct NFARegularSymbol {
        RegularSymbol<Symbol> regular_symbol;
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
                        NFARegularSymbol<Symbol>& c_rhs) {
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
                auto f_state = add_final_state(*a_lhs);
                add_transition(*a_lhs, initial_state(*a_lhs), f_state, 
                               c_lhs.regular_symbol.symbol);
            } else if (a_lhs != nullptr && a_rhs == nullptr) {
                auto f_state = add_final_state(*a_lhs);
                add_transition(*a_lhs, initial_state(*a_lhs), f_state, 
                               c_rhs.regular_symbol.symbol);
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
                         NFARegularSymbol<Symbol>& c_rhs) {
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
                clear_final_state_set(*a_lhs);
                set_final_state(*a_lhs, f_state);
            } else {
                typedef typename nondeterministic_finite_automaton<Symbol>::StateSet StateSet;
                auto i_state = initial_state(*a_lhs);
                auto final_state_set_ = final_state_set(*a_lhs);
                typename nondeterministic_finite_automaton<Symbol>::State2Map state2_map;
                clear_final_state_set(*a_lhs);
                copy_fa_impl::copy_states(*a_rhs, *a_lhs, state2_map, StateSet());
                copy_fa_impl::copy_transitions(*a_rhs, *a_lhs, state2_map);
                for (auto state : final_state_set_) {
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
                } else if (opt == '?') {
                    add_transition(*a, i_state, f_state, epsilon(*a));
                }
                add_transition(*a, i_state, initial_state(*a), epsilon(*a));
                for (auto state : final_state_set(*a)) {
                    add_transition(*a, state, f_state, epsilon(*a));
                }
                clear_final_state_set(*a);
                set_final_state(*a, f_state);
                set_initial_state(*a, i_state);
            }
        }

        template <typename Symbol>
        static void
        apply(const basic_regular_expression_gen<Symbol>& re,
              deterministic_finite_automaton<Symbol>& dfa) {
            if (re.postfix_expression().size() == 1) {
                const auto& c = re.postfix_expression().front();
                typename deterministic_finite_automaton<Symbol>::SymbolSet alphabet({c.symbol});
                set_alphabet(dfa, alphabet);
                add_transition(dfa, add_initial_state(dfa), add_final_state(dfa), c.symbol);
                return;
            }
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
                        concat_automaton(symbol_stack.top(), rs);
                    } else {
                        auto& rs = symbol_stack.top();
                        symbol_stack.pop();
                        union_automaton(symbol_stack.top(), rs);
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
    make_fa(const detail::basic_regular_expression_gen<Symbol>& re,
            deterministic_finite_automaton<Symbol>& dfa) {
        make_fa_impl::apply(re, dfa);
    }
}

#endif /* atl_detail_regular_expression_algorithm_hpp */
