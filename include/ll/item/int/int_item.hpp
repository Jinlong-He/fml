//
//  int_item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_int_item_hpp 
#define ll_int_item_hpp

#include "../item.hpp"

namespace ll {
    class int_expr;
    class int_item : public item {
    public:
        int_item()
            : item() {}

        int_item(const string& identifier)
            : item(identifier) {}

        int_item(const string& identifier, const string& type)
            : item(identifier, type) {}

        int_item(const int_item& i)
            : item(i) {}

        int_item& operator=(const int_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }
    };
}

#endif /* ll_int_item_hpp */
