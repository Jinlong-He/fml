//
//  linear_temporal_logic_fomular.hpp
//  ATL 
//
//  Created by Jinlong He on 2019/11/5.
//  Copyright © 2019年 Ruting-Team. All rights reserved.
//

#ifndef linear_temporal_logic_fomular_hpp 
#define linear_temporal_logic_fomular_hpp

#include "../fomula.hpp"

namespace ll {
    class linear_temporal_logic_fomula : public virtual fomula {
        typedef std::unique_ptr<linear_temporal_logic_fomula> LTLFomulaPtr;
    public:
        linear_temporal_logic_fomula()
            : lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_(""),
              logical_operator_("") {}

        linear_temporal_logic_fomula(const string& content)
            : fomula(content),
              lhs_(nullptr),
              rhs_(nullptr),
              temporal_operator_(""),
              logical_operator_("") {}

        linear_temporal_logic_fomula(const linear_temporal_logic_fomula& p,
                                     const string& op,
                                     bool is_logical = true)
            : fomula("(" + op + p.to_string() + ")"),
              lhs_(new linear_temporal_logic_fomula(p)),
              rhs_(nullptr),
              temporal_operator_(is_logical ? "" : op),
              logical_operator_(is_logical ? op : "") {}

        linear_temporal_logic_fomula(const linear_temporal_logic_fomula& lhs,
                                     const linear_temporal_logic_fomula& rhs,
                                     const string& op,
                                     bool is_logical = true)
            : fomula (is_logical ? "(" + lhs.to_string() + op + rhs.to_string() + ")" :
                                   "[" + lhs.to_string() + op + rhs.to_string() + "]"),
              lhs_(new linear_temporal_logic_fomula(lhs)),
              rhs_(new linear_temporal_logic_fomula(rhs)),
              temporal_operator_(is_logical ? "" : op),
              logical_operator_(is_logical ? op : "") {}

        linear_temporal_logic_fomula(const linear_temporal_logic_fomula& p) 
            : fomula(p),
              lhs_((p.rhs_ ? new linear_temporal_logic_fomula(p.lhs()): nullptr)),
              rhs_((p.rhs_ ? new linear_temporal_logic_fomula(p.rhs()): nullptr)),
              temporal_operator_(p.temporal_operator_),
              logical_operator_(p.logical_operator_) {}

        linear_temporal_logic_fomula& operator=(const linear_temporal_logic_fomula& p) {
            if (this != &p) {
                fomula::operator=(p);
                lhs_ = ((p.rhs_ ? LTLFomulaPtr(new linear_temporal_logic_fomula(p.lhs())): nullptr));
                rhs_ = ((p.rhs_ ? LTLFomulaPtr(new linear_temporal_logic_fomula(p.rhs())): nullptr));
                temporal_operator_ = p.temporal_operator_;
                logical_operator_ = p.logical_operator_;
            }
            return *this;
        }

        const linear_temporal_logic_fomula& lhs() const {
            return *lhs_;
        }

        const linear_temporal_logic_fomula& rhs() const {
            return *rhs_;
        }

        const string& temporal_operator() const {
            return temporal_operator_;
        }

        const string& logical_operator() const {
            return logical_operator_;
        }

    private:
        LTLFomulaPtr lhs_;
        LTLFomulaPtr rhs_;
        string temporal_operator_;
        string logical_operator_;
    };

    inline linear_temporal_logic_fomula make_G(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "G");
    }

    inline linear_temporal_logic_fomula make_F(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "F");
    }

    inline linear_temporal_logic_fomula make_X(const linear_temporal_logic_fomula& p) {
        return linear_temporal_logic_fomula(p, "X");
    }

    inline linear_temporal_logic_fomula make_U(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "U", 0);
    }

    inline linear_temporal_logic_fomula make_W(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "W", 0);
    }

    inline linear_temporal_logic_fomula make_R(const linear_temporal_logic_fomula& lhs,
                                               const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "R", 0);
    }

    inline linear_temporal_logic_fomula operator&(const linear_temporal_logic_fomula& lhs, 
                                                  const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "&");
    }

    inline linear_temporal_logic_fomula make_and(const linear_temporal_logic_fomula& lhs,
                                                 const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "&");
    }

    inline linear_temporal_logic_fomula operator|(const linear_temporal_logic_fomula& lhs, 
                                                  const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "|");
    }

    inline linear_temporal_logic_fomula make_or(const linear_temporal_logic_fomula& lhs,
                                                const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "|");
    }

    inline linear_temporal_logic_fomula operator!(const linear_temporal_logic_fomula& f) {
        return linear_temporal_logic_fomula(f, "!");
    }

    inline linear_temporal_logic_fomula make_not(const linear_temporal_logic_fomula& f) {
        return linear_temporal_logic_fomula(f, "!");
    }

    inline linear_temporal_logic_fomula make_implies(const linear_temporal_logic_fomula& lhs,
                                                     const linear_temporal_logic_fomula& rhs) {
        return linear_temporal_logic_fomula(lhs, rhs, "->");
    }
}

#endif /* linear_temporal_logic_fomular_hpp */
