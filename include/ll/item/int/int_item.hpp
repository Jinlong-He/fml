//
//  int_item.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_int_item_hpp 
#define ll_int_item_hpp

#include <fml/ll/item/item.hpp>

namespace ll {
    class int_item : virtual public item {
    public:
        int_item()
            : item() {}

        int_item(const int_item& i)
            : item(i) {}

        int_item& operator=(const int_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }

        virtual ~int_item() {}
    };
}

#endif /* ll_int_item_hpp */
