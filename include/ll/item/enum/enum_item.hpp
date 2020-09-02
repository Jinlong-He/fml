//
//  enum_item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_enum_item_hpp 
#define ll_enum_item_hpp

#include <ll/item/item.hpp>

namespace ll {
    class enum_item 
        : virtual public item {
    public:
        enum_item()
            : item("") {}

        enum_item(const enum_item& i)
            : item(i) {}

        enum_item& operator=(const enum_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }

        virtual ~enum_item() {}
    };
}

#endif /* ll_enum_item_hpp */

