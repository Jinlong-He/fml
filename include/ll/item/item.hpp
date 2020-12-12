//
//  item.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_item_hpp 
#define ll_item_hpp

#include <string>
#include <memory>
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

        virtual ~item() {}

        const string& identifier() const {
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

        friend std::ostream& operator<< (std::ostream& os, const item& x) {
            if (x.type_ == "") {
                os << x.identifier_;
            } else {
                os << x.type_ + " " + x.identifier_;
            }
            return os;
        }
    protected:
        string identifier_;
        string type_;
    };
}

#endif /* ll_item_hpp */
