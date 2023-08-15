//
//  variable.hpp
//  ATL 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_variable_hpp 
#define ll_variable_hpp

#include <string>
#include <fml/ll/item/item.hpp>

namespace ll {
    class variable 
        : virtual public item {
    public:
        variable()
            : item() {}

        variable& operator=(const variable& v) {
            if (this != &v) {
                item::operator=(v);
            }
            return *this;
        }

        virtual ~variable() {}

    private:
    };
}

#endif /* variable_hpp */
