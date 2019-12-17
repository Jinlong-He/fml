//
//  bool.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_bool_hpp 
#define ll_bool_hpp

#include "../detail/bool/bool_variable.hpp"

namespace ll {
    class bool_value : public detail::bool_value_gen {
    public:
        bool_value()
            : bool_value_gen() {}

        bool_value(bool v)
            : bool_value_gen(v) {}

        //bool_value(const string& id)
        //    : bool_value_gen(id) {}

        bool_value(const bool_value& v)
            : bool_value_gen(v) {}

        bool_value_gen& operator=(const bool_value& v) {
            if (this != &v) {
                bool_value_gen::operator=(v);
            }
            return *this;
        }
    };

    class bool_variable : public detail::bool_variable_gen {
    public:
        bool_variable()
            : bool_variable_gen() {}

        bool_variable(const string& id)
            : bool_variable_gen(id) {}

        bool_variable(const bool_variable& v)
            : bool_variable_gen(v) {}

        bool_variable& operator=(const bool_variable& v) {
            if (this != &v) {
                bool_variable_gen::operator=(v);
            }
            return *this;
        }
    };
}

#endif /* bool_value_hpp */
