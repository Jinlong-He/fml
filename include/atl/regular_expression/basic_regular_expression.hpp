//
//  basic_regular_expression.hpp
//  atl 
//
//  Created by Jinlong He on 2020/01/03.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef atl_basic_regular_expression_hpp 
#define atl_basic_regular_expression_hpp
#include "../detail/regular_expression/basic_regular_expression.hpp"

namespace atl {
    template <typename Symbol = char>
    class basic_regular_expression 
        : public detail::basic_regular_expression_gen<Symbol> {
        public:
            typedef detail::basic_regular_expression_gen<Symbol> Base;
        public:
            basic_regular_expression()
                : Base() {}

            basic_regular_expression(const std::basic_string<Symbol>& re)
                : Base(re) {}

            basic_regular_expression(const std::basic_string<Symbol>& re, 
                                     const unordered_map<Symbol, char>& operator_map) 
                : Base(re, operator_map) {}
    };
}

#endif /* atl_basic_regular_expression_hpp */
