//
//  enum_value.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_enum_value_hpp 
#define ll_enum_value_hpp

#include <ll/item/value.hpp>
#include <ll/item/enum/enum_item.hpp>

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
