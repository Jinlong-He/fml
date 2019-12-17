//
//  item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_item_hpp 
#define ll_item_hpp

#include "int.hpp"
#include "enum.hpp"
#include "bool.hpp"

namespace ll {
    class item : public detail::item_gen {
    public:
        item()
            : item_gen() {}

        item(const string& id)
            : item_gen(id) {}

        item(const item& i)
            : item_gen(i) {}

        item_gen& operator=(const item& v) {
            if (this != &v) {
                item_gen::operator=(v);
            }
            return *this;
        }
    };
}

#endif /* ll_item_hpp */
