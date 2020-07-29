//
//  enum_item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_enum_item_hpp 
#define ll_enum_item_hpp

#include "../item.hpp"

namespace ll {
    class enum_item : public item {
    public:
        enum_item()
            : item("") {}

        enum_item(const string& identifier)
            : item(identifier) {}

        enum_item(const string& identifier, const string& type)
            : item(identifier, type) {}

        enum_item(const enum_item& i)
            : item(i) {}

        enum_item& operator=(const enum_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }
    };
}

#endif /* ll_enum_item_hpp */

