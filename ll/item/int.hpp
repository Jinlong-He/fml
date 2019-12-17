//
//  int.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_int_hpp 
#define ll_int_hpp

#include "../detail/int/int_expr.hpp"

namespace ll {

    class int_value : public detail::int_value_gen {
    public:
        int_value()
            : int_value_gen() {}

        int_value(int v)
            : int_value_gen(v) {}

        int_value(const int_value& v)
            : int_value_gen(v) {}

        int_value_gen& operator=(const int_value& v) {
            if (this != &v) {
                int_value_gen::operator=(v);
            }
            return *this;
        }
    };

    class int_variable : public detail::int_variable_gen {
    public:
        int_variable()
            : int_variable_gen() {}

        int_variable(const string& id)
            : int_variable_gen(id) {}

        int_variable(const string& id, 
                     const int_value& min, const int_value& max)
            : int_variable_gen(id, min, max) {}

        int_variable(const string& id, std::initializer_list<detail::int_value_gen> list)
            : int_variable_gen(id, list) {}

        int_variable(const int_variable& v)
            : int_variable_gen(v) {}

        //template <typename InputIterator>
        //int_variable(const string& id, InputIterator first, InputIterator last)
        //    : int_variable_gen(id, first, last) {}

        int_variable& operator=(const int_variable& v) {
            if (this != &v) {
                int_variable_gen::operator=(v);
            }
            return *this;
        }
    };

    class int_expr : public detail::int_expr_gen {
    public:
        int_expr()
            : int_expr_gen() {}

        int_expr(const string& id)
            : int_expr_gen(id) {}

        int_expr(const int_item_gen& lhs, const int_item_gen& rhs, const string& op)
            : int_expr_gen(lhs, rhs, op) {}

        int_expr(const int_expr& v)
            : int_expr_gen(v) {}

        int_expr_gen& operator=(const int_expr_gen& v) {
            if (this != &v) {
                int_expr_gen::operator=(v);
            }
            return *this;
        }
    };
}

#endif /* int_value_hpp */
