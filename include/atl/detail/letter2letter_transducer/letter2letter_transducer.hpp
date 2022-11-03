//
//  letter2letter_transducer.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_detail_letter2letter_transducer_hpp 
#define atl_detail_letter2letter_transducer_hpp

#include "atl/detail/finite_automaton/finite_automaton.hpp"
#include <unordered_set>
#include <unordered_map>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using std::unordered_map, std::unordered_set;

namespace atl::detail {
    template <class Symbol>
    struct L2LTLabel {
        Symbol upper_symbol;
        Symbol lower_symbol;

        L2LTLabel() {}

        L2LTLabel(const Symbol& symbol)
            : upper_symbol(symbol),
              lower_symbol(symbol) {}

        L2LTLabel(const Symbol& upper, const Symbol& lower)
            : upper_symbol(upper),
              lower_symbol(lower) {}

        friend std::ostream& operator<< (std::ostream& os, const L2LTLabel& x) {
            os << "<" << x.upper_symbol << "," << x.lower_symbol << ">";
            return os;
        }

        bool operator== (const L2LTLabel& x) const {
            return upper_symbol == x.upper_symbol && lower_symbol == x.lower_symbol;
        }

        bool operator!= (const L2LTLabel& x) const {
            return !(*this == x);
        }

        bool operator< (const L2LTLabel& x) const {
            return (upper_symbol < x.upper_symbol || 
                    (upper_symbol == x.upper_symbol && lower_symbol < x.lower_symbol));
        }
    };

    template <class Symbol>
    class letter2letter_transducer_gen {
    public:
        typedef unordered_set<Symbol> SymbolSet;
    public:
        letter2letter_transducer_gen()
            : symbol_set_() {}

        letter2letter_transducer_gen(const SymbolSet& symbol_set, 
                                     const Symbol& epsilon_symbol)
            : symbol_set_(symbol_set),
              epsilon_symbol_(epsilon_symbol) {}

        letter2letter_transducer_gen(const std::initializer_list<Symbol>& symbol_set,
                                     const Symbol& epsilon_symbol)
            : symbol_set_(symbol_set),
              epsilon_symbol_(epsilon_symbol) {}

        letter2letter_transducer_gen(const letter2letter_transducer_gen& x)
            : symbol_set_(x.symbol_set_),
              epsilon_symbol_(x.epsilon_symbol_) {}

        const SymbolSet& 
        symbol_set() const {
            return symbol_set_;
        }

        const Symbol&
        epsilon_symbol() const {
            return epsilon_symbol_;
        }

        letter2letter_transducer_gen& 
        operator=(const letter2letter_transducer_gen& x) {
            if (&x != this) {
                symbol_set_ = x.symbol_set_;
            }
            return *this;
        }

        virtual void set_symbol_set(const SymbolSet& symbol_set) {
            symbol_set_ = symbol_set;
        }

        void set_epsilon_symbol(const Symbol& epsilon_symbol) {
            epsilon_symbol_ = epsilon_symbol;
        }

        void add_symbol(const Symbol& c) {
            symbol_set_.insert(c);
        }

    protected:
        SymbolSet symbol_set_;
        Symbol epsilon_symbol_;
    };
};

namespace std {
    template<class Symbol>
    struct hash<atl::detail::L2LTLabel<Symbol> > {
        std::size_t operator() (const atl::detail::L2LTLabel<Symbol>& l) const {
            return boost::hash<pair<Symbol, Symbol> >()(pair(l.upper_symbol, l.lower_symbol));
        }
    };
};

namespace atl {
    #define L2LT_PARAMS typename L2LT_SYMBOL
    #define L2LT detail::letter2letter_transducer_gen<L2LT_SYMBOL>

    template<L2LT_PARAMS>
    inline typename L2LT::SymbolSet const&
    symbol_set(const L2LT& t) {
        return t.symbol_set();
    }

    template <L2LT_PARAMS>
    inline L2LT_SYMBOL const&
    epsilon_symbol(const L2LT& t) {
        return t.epsilon_symbol();
    }

    template<L2LT_PARAMS>
    inline void
    set_symbol_set(L2LT& t,
                   typename L2LT::SymbolSet const& symbol_set) {
        return t.set_symbol_set(symbol_set);
    }

    template<L2LT_PARAMS>
    inline void
    set_epsilon_symbol(L2LT& t,
                       const L2LT_SYMBOL& epsilon_symbol) {
        return t.set_epsilon_symbol(epsilon_symbol);
    }
};

#endif /* atl_detail_letter2letter_transducer_hpp */
