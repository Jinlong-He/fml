//
//  value.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_value_hpp 
#define ll_value_hpp

#include <iostream>
#include "item.hpp"

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
