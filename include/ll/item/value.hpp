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
using std::string;

namespace ll {
    class value {
    public:
        value() {}

        value& operator=(const value& v) {
            if (this != &v) {
            }
            return *this;
        }

    private:
    };
}

#endif /* value_hpp */