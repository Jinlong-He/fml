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

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <util/util.hpp>
#include <atl/detail/automaton.hpp>
#include <atl/detail/no_type.hpp>

using boost::unordered_map;
using boost::unordered_set;

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

        letter2letter_transducer_gen(const SymbolSet& symbol_set)
            : symbol_set_(symbol_set) {}

        letter2letter_transducer_gen(const std::initializer_list<Symbol>& symbol_set)
            : symbol_set_(symbol_set) {}

        letter2letter_transducer_gen(const letter2letter_transducer_gen& x)
            : symbol_set_(x.symbol_set_) {}

        const SymbolSet& symbol_set() const {
            return symbol_set_;
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

        void add_symbol(const Symbol& c) {
            symbol_set_.insert(c);
        }

    protected:
        SymbolSet symbol_set_;
    };
};

namespace boost {
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
    typename L2LT::SymbolSet const&
    symbol_set(const L2LT& t) {
        return t.symbol_set();
    }

    template<L2LT_PARAMS>
    void
    set_symbol_set(L2LT& t,
                   typename L2LT::SymbolSet const& symbol_set) {
        return t.set_symbol_set(symbol_set);
    }
};

#endif /* atl_detail_letter2letter_transducer_hpp */
