//
//  int_value.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_int_value_hpp 
#define ll_int_value_hpp

#include <fml/ll/item/value.hpp>
#include <fml/ll/item/int/int_item.hpp>

namespace ll {
    class int_value 
        : public value,
          public int_item {
    public:
        int_value()
            : value(),
              int_item() {}

        int_value(int v)
            : item(std::to_string(v), "integer"),
              value(),
              int_item() {}

        int_value(const int_value& v)
            : item(v),
              value(v),
              int_item(v) {}

        int_value& operator=(const int_value& v) {
            if (this != &v) {
                item::operator=(v);
                value::operator=(v);
                int_item::operator=(v);
            }
            return *this;
        }

        ~int_value() {}
    private:
    };
}

#endif /* int_value_hpp */
