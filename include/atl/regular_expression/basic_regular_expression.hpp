//
//  basic_regular_expression.hpp
//  atl 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef atl_basic_regular_expression_hpp 
#define atl_basic_regular_expression_hpp
#include <atl/detail/regular_expression/algorithm.hpp>

namespace atl {
    template <typename Symbol = char>
    class basic_regular_expression 
        : public detail::basic_regular_expression_gen<Symbol> {
        public:
            typedef detail::basic_regular_expression_gen<Symbol> Base;
        public:
            basic_regular_expression()
                : Base() {}

            basic_regular_expression(const std::vector<Symbol>& re)
                : Base(re) {}

            basic_regular_expression(const std::string& re)
                : Base(re) {}

            basic_regular_expression(const std::vector<Symbol>& re, 
                                     const unordered_map<char, Symbol>& operator_map) 
                : Base(re, operator_map) {}

            basic_regular_expression(const std::string& re, 
                                     const unordered_map<char, char>& operator_map) 
                : Base(re, operator_map) {}
    };
}

#endif /* atl_basic_regular_expression_hpp */
