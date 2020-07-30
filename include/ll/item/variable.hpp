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
using std::string;

namespace ll {
    class variable {
    public:
        variable() {}

        variable& operator=(const variable& v) {
            if (this != &v) {
            }
            return *this;
        }

    private:
    };
}

#endif /* variable_hpp */
