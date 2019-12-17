//
//  variable.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_variable_hpp 
#define ll_detail_variable_hpp

#include <iostream>
using std::string;

namespace ll {
    namespace detail {
        class variable_gen {
        public:
            variable_gen() {}

            variable_gen& operator=(const variable_gen& v) {
                if (this != &v) {
                }
                return *this;
            }

        private:
        };
    }
}

#endif /* variable_hpp */
