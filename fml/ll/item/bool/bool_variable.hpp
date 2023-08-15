//
//  bool_variable.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_bool_variable_hpp 
#define ll_bool_variable_hpp

#include <fml/ll/item/variable.hpp>
#include <fml/ll/item/bool/bool_item.hpp>
#include <fml/ll/item/bool/bool_value.hpp>

namespace ll {
    class bool_variable 
        : public variable,
          public bool_item {
    public:
        bool_variable()
            : item("", "boolean"),
              variable(),
              bool_item() { }

        bool_variable(const string& id)
            : item(id, "boolean"),
              variable(),
              bool_item() { }

        bool_variable(const bool_variable& v)
            : item(v),
              variable(v),
              bool_item(v) {}

        bool_variable& operator=(const bool_variable& v) {
            if (this != &v) {
                item::operator=(v);
                variable::operator=(v);
                bool_item::operator=(v);
            }
            return *this;
        }

        ~bool_variable() {}

    private:
    };
}

#endif /* ll_bool_variable_hpp */
