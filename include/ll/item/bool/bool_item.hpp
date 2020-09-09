//
//  bool_item.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_bool_item_hpp 
#define ll_bool_item_hpp

#include <ll/item/item.hpp>

namespace ll {
    class bool_item 
        : virtual public item {
    public:
        bool_item()
            : item("") {}

        bool_item(const bool_item& i)
            : item(i) {}

        bool_item& operator=(const bool_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }

        virtual ~bool_item() {}
    };
}

#endif /* ll_bool_item_hpp */

