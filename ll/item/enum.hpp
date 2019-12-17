//
//  enum.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_enum_hpp 
#define ll_enum_hpp

#include "../detail/enum/enum_variable.hpp"

namespace ll {
    class enum_value : public detail::enum_value_gen {
    public:
        enum_value()
            : enum_value_gen() {}

        enum_value(const string& id)
            : enum_value_gen(id) {}

        enum_value(const enum_value& v)
            : enum_value_gen(v) {}

        enum_value_gen& operator=(const enum_value& v) {
            if (this != &v) {
                enum_value_gen::operator=(v);
            }
            return *this;
        }
    };

    class enum_variable : public detail::enum_variable_gen {
    public:
        enum_variable()
            : enum_variable_gen() {}

        enum_variable(const string& id)
            : enum_variable_gen(id) {}

        enum_variable(const string& id, std::initializer_list<detail::enum_value_gen> list)
            : enum_variable_gen(id, list) {}

        template <typename InputIterator>
        enum_variable(const string& id, InputIterator first, InputIterator last)
            : enum_variable_gen(id, first, last) {}

        enum_variable(const enum_variable& v)
            : enum_variable_gen(v) {}

        enum_variable& operator=(const enum_variable& v) {
            if (this != &v) {
                enum_variable_gen::operator=(v);
            }
            return *this;
        }
    };
}

#endif /* enum_value_hpp */
