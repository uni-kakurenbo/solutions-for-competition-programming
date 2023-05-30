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

#include <atcoder/scc.hpp>

void solve() {
    int n; cin >> n;
    valarray<int> a(n); input >> a;
    a -= 1;

    atcoder::scc_graph G(n);
    REP(i, n) G.add_edge(i, a[i]);

    i64 ans = 0;

    auto scc = G.scc();
    ITR(sub, scc) {
        if(sub.size() > 1) ans += sub.size();
        else ans += a[sub[0]] == sub[0];
    }

    print(ans);
}