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
    class bool_item 
        : virtual public item {
    public:
        bool_item()
            : item("") {}

        bool_item(const bool_item& i)
            : item(i) {}

        bool_item& operator=(const bool_item& i) {
            if (this != &i) {
                item::operator=(i);
            }
            return *this;
        }

        virtual ~bool_item() {}
    };
}

#endif /* ll_bool_item_hpp */

