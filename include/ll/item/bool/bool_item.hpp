//
//  bool_item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_bool_item_hpp 
#define ll_bool_item_hpp

#include "../item.hpp"

namespace ll {
    class bool_item : public item {
    public:
        bool_item()
            : item("") {}

        bool_item(const string& identifier)
            : item(identifier) {}

        bool_item(const string& identifier, const string& type)
            : item(identifier, type) {}

        bool_item(const bool_item& i)
            : item(i) {}

        bool_item& operator=(const bool_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }
    };
}

#endif /* ll_bool_item_hpp */

