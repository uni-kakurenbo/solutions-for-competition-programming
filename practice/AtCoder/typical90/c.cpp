/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
#include "template/standard.hpp"

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        debug_("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "graph/shortest_path.hpp"
#include "graph/tree_diamiter.hpp"

void solve() {
    i32 n; input >> n;
    lib::graph<i32> graph(n); graph.read_bidirectionally(n - 1);

    auto diamiter = lib::tree_diamiter(graph);
    print(diamiter.first + 1);
}
