//
//  item.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_detail_item_hpp 
#define ll_detail_item_hpp

#include <iostream>
#include <list>
using std::string;
namespace ll {
    namespace detail {
        class item_gen {
        public:
            item_gen()
                : identifier_(""),
                  type_("") {}

            item_gen(const string& identifier)
                : identifier_(identifier),
                  type_("") {}

            item_gen(const string& identifier, const string& type)
                : identifier_(identifier),
                  type_(type) {}

            item_gen(const item_gen& i)
                : identifier_(i.identifier_),
                  type_(i.type_) {}

            item_gen& operator=(const item_gen& i) {
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
}

#endif /* ll_detail_item_gen_hpp */
