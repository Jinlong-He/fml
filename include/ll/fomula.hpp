//
//  fomula.hpp
//  ll 
//
//  Created by Jinlong He on 2019/12/6.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
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
    private:
        string content_;
    };
}

#endif /* ll_fomula_hpp */
