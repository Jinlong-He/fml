//
//  fomula.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef ll_fomula_hpp 
#define ll_fomula_hpp

#include <iostream>
#include <list>
using std::string;
namespace ll {
    class fomula {
    public:
        fomula()
            : identifier_("") {}

        fomula(const string& identifier)
            : identifier_(identifier) {}

        fomula(const fomula& i)
            : identifier_(i.identifier_) {}

        fomula& operator=(const fomula& i) {
            if (this != &i) {
                identifier_ = i.identifier_;
            }
            return *this;
        }

        const string& to_string() const {
            return identifier_;
        }
    private:
        string identifier_;
    };
}

#endif /* ll_fomula_hpp */
