/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

void solve() {
    int n; cin >> n;
    string s; cin >> s;

    string t;
    bool f = false;
    REP(i, n) {
        if(s[i] == '"') f ^= true;
        if(s[i] == ',') {
            if(f) t += ',';
            else t += '.';
        }
        else {
            t += s[i];
        }
    }

    print(t);
}