//
//  bool_value.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_bool_value_hpp 
#define ll_bool_value_hpp

#include <fml/ll/item/value.hpp>
#include <fml/ll/item/bool/bool_item.hpp>

namespace ll {
    class bool_value 
        : public value,
          public bool_item {
    public:
        bool_value()
            : item(),
              value(),
              bool_item() {}

        bool_value(bool b)
            : item(std::to_string(b), "boolean"),
              value(),
              bool_item() {
                  if (b) set_identifier("TRUE");
                  if (!b) set_identifier("FALSE");
              }

        //bool_value(const string& id)
        //    : value(),
        //      bool_item(id, "boolean") {}

        bool_value(const bool_value& v)
            : item(v),
              value(v),
              bool_item(v) {}

        bool_value& operator=(const bool_value& v) {
            if (this != &v) {
                item::operator=(v);
                value::operator=(v);
                bool_item::operator=(v);
            }
            return *this;
        }

        ~bool_value() {}

    private:
    };
}

#endif /* bool_value_hpp */
