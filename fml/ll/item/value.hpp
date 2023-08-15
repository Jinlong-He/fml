//
//  value.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_value_hpp 
#define ll_value_hpp

#include <iostream>
#include <fml/ll/item/item.hpp>

namespace ll {
    class value
        : virtual public item {
    public:
        value()
            : item() {}

        value& operator=(const value& v) {
            if (this != &v) {
                item::operator=(v);
            }
            return *this;
        }
        
        virtual ~value() {}

    private:
    };
}

#endif /* value_hpp */
