//
//  bool_variable.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_bool_variable_hpp 
#define ll_bool_variable_hpp

#include "../variable.hpp"
#include "bool_item.hpp"
#include "bool_value.hpp"

namespace ll {
    class bool_variable 
        : public variable,
          public bool_item {
    public:
        bool_variable()
            : variable(),
              bool_item() {
                  this -> set_type("boolean");
              }

        bool_variable(const string& id)
            : variable(),
              bool_item(id) {
                  this -> set_type("boolean");
              }

        bool_variable(const bool_variable& v)
            : variable(v),
              bool_item(v) {}

        bool_variable& operator=(const bool_variable& v) {
            if (this != &v) {
                variable::operator=(v);
                bool_item::operator=(v);
            }
            return *this;
        }

    private:
    };
}

#endif /* ll_bool_variable_hpp */
