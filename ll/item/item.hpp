//
//  item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_item_hpp 
#define ll_item_hpp

#include <iostream>
#include <list>
using std::string;
namespace ll {
    class item {
    public:
        item()
            : identifier_(""),
              type_("") {}

        item(const string& identifier)
            : identifier_(identifier),
              type_("") {}

        item(const string& identifier, const string& type)
            : identifier_(identifier),
              type_(type) {}

        item(const item& i)
            : identifier_(i.identifier_),
              type_(i.type_) {}

        item& operator=(const item& i) {
            if (this != &i) {
                identifier_ = i.identifier_;
                type_ = i.type_;
            }
            return *this;
        }

        const string& to_string() const {
            return identifier_;
        }

        const string& type() const {
            return type_;
        }

        void set_identifier(const string& identifier) {
            identifier_ = identifier;
        }

        void set_type(const string& type) {
            type_ = type;
        }
    private:
        string identifier_;
        string type_;
    };
}

#endif /* ll_item_hpp */
