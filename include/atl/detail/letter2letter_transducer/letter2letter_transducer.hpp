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

namespace atl {
    namespace detail {
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

        class letter2letter_transducer_gen {
        };
    }
}

namespace boost {
    template<class Symbol>
    struct hash<atl::detail::L2LTLabel<Symbol> > {
        std::size_t operator() (const atl::detail::L2LTLabel<Symbol>& l) const {
            return boost::hash<pair<Symbol, Symbol> >()(pair(l.upper_symbol, l.lower_symbol));
        }
    };
}



#endif /* atl_detail_letter2letter_transducer_hpp */
