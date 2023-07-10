#include <fml/util/util.hpp>
#include "test.hpp"
using namespace util;

namespace test {
    bool test_combination() {
        vector<int> datas({1,2});
        vector<vector<int> > coms, pers;
        combine(datas, 2, coms);
        permut(datas, pers);
        for (auto& data : coms) {
            for (auto i : data) {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;
        for (auto& data : pers) {
            for (auto i : data) {
                cout << i << " ";
            }
            cout << endl;
        }
    }

}
