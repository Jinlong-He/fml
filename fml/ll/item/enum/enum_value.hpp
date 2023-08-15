//
//  enum_value.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_enum_value_hpp 
#define ll_enum_value_hpp

#include <fml/ll/item/value.hpp>
#include <fml/ll/item/enum/enum_item.hpp>

namespace ll {
    class enum_value 
        : public value,
          public enum_item {
    public:
        enum_value()
            : item(),
              value(),
              enum_item() {}

        enum_value(const string& id)
            : item(id, "enum"),
              value(),
              enum_item() {}

        enum_value(const enum_value& v)
            : item(v),
              value(v),
              enum_item(v) {}

        enum_value& operator=(const enum_value& v) {
            if (this != &v) {
                item::operator=(v);
                value::operator=(v);
                enum_item::operator=(v);
            }
            return *this;
        }

        ~enum_value() {}
    private:
    };
}

#endif /* enum_value_hpp */
