//
//  value.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_value_hpp 
#define ll_detail_value_hpp

#include <iostream>
using std::string;

namespace ll {
    namespace detail {
        class value_gen {
        public:
            value_gen() {}

            value_gen& operator=(const value_gen& v) {
                if (this != &v) {
                }
                return *this;
            }

        private:
        };
    }
}

#endif /* value_hpp */
