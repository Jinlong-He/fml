//
//  fomula.hpp
//  ll 
//
//  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//  SPDX-License-Identifier: MIT
//  Copyright (c) 2020 Jinlong He.
//

#ifndef ll_fomula_hpp 
#define ll_fomula_hpp

#include <string>
using std::string;
namespace ll {
    class fomula {
    public:
        fomula()
            : content_("") {}

        fomula(const string& content)
            : content_(content) {}

        fomula(const fomula& f)
            : content_(f.content_) {}

        fomula& operator=(const fomula& f) {
            if (this != &f) {
                content_ = f.content_;
            }
            return *this;
        }

        const string& to_string() const {
            return content_;
        }

        void set_content(const string& content) {
            content_ = content;
        }

        friend std::ostream& operator<< (std::ostream& os, const fomula& x) {
            os << x.content_;
            return os;
        }
    private:
        string content_;
    };
}

#endif /* ll_fomula_hpp */
