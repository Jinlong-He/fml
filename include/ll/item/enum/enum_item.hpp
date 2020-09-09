//
//  enum_item.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_enum_item_hpp 
#define ll_enum_item_hpp

#include <ll/item/item.hpp>

namespace ll {
    class enum_item 
        : virtual public item {
    public:
        enum_item()
            : item("") {}

        enum_item(const enum_item& i)
            : item(i) {}

        enum_item& operator=(const enum_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }

        virtual ~enum_item() {}
    };
}

#endif /* ll_enum_item_hpp */

