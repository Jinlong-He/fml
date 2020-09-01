//
//  variable.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/1.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_variable_hpp 
#define ll_variable_hpp

#include <string>
#include "item.hpp"

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
