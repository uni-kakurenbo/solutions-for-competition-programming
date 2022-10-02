/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region tmplate */
#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL_JUDGE
    #include<debug>
    #define debug(...) Debug::debug(Debug::split(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
    #define DEBUG(...) do { Debug::DEBUG(nullptr, "\033[3;35m#" + to_string(__LINE__) + "\033[m  "); Debug::DEBUG(__VA_ARGS__); Debug::DEBUG(nullptr, "\033[m\n"); } while(0);
#else
    #define debug(...) { ; }
    #define DEBUG(...) { ; }
#endif

#define INF32 2147483647
#define INF64 9223372036854775807LL

#define until(...) while(!(__VA_ARGS__))

#define REP(i,n) for(int i=0, i##_length=int(n); i<i##_length; ++i)
#define REPD(i,n) for(int i=(n)-1; i>=0; --i)
#define LOOP(n) REP(_$, (n))
#define FOR(i,l,r) for(ll i=(l), i##_last=ll(r); i<=i##_last; ++i)
#define FORD(i,l,r) for(ll i=(l), i##_last=ll(r); i>=i##_last; --i)

#define ITRP(x,v) for(auto x : (v))
#define ITRR(x,v) for(auto &x : (v))
#define ITR(x,v) for(const auto &x : (v))
#define ITRMP(x,y,v) for(auto [x, y] : (v))
#define ITRMR(x,y,v) for(auto &[x, y] : (v))
#define ITRM(x,y,v) for(const auto [x, y] : (v))

#define ALL(x) begin((x)),end((x))
#define RALL(x) rbegin((x)),rend((x))

#define $F first
#define $S second

using ll = long long;
using ull = unsigned long long;
using ld = long double;

constexpr char ln = '\n';
constexpr char spc = ' ';

template<class T1, class T2> inline auto mod(T1 x, T2 r) { return (x%r+r)%r; }

template<class T> inline bool chmax(T &a, T b) { return (a<b ? a=b, true : false); }
template<class T> inline bool chmin(T &a, T b) { return (a>b ? a=b, true : false); }

template<class T> auto operator<<(ostream &out, const T &val) -> decltype(val.val(), out)& {
    return out << val.val();
}
/* #endregion */

template<class I, class T = typename iterator_traits<I>::value_type, class C = vector<T>>
struct PrefixSum : C {
    PrefixSum(I first, I last, T head = T()) {
        this->assign(distance(first,last)+1, head);
        for(auto i=size_t(1), itr=first; itr!=last; i++) {
            (*this)[i] = (*this)[i-1] + *itr++;
        }
    }
    inline T operator()(size_t left, size_t right) {
        return (*this)[right] - (*this)[left];
    }
};

signed main() {
    ll n, p, q, r; cin >> n >> p >> q >> r;
    vector<int> a(n); REP(i, n) cin >> a[i];

    PrefixSum sum(ALL(a), 0LL);

    REP(x, n) {
        auto y = lower_bound(ALL(sum), p + sum[x]);
        if (y == sum.end() or *y != p + sum[x]) continue;

        auto z = lower_bound(ALL(sum), p + q + sum[x]);
        if (z == sum.end() or *z != p + q + sum[x]) continue;

        auto w = lower_bound(ALL(sum), p + q + r + sum[x]);
        if (w == sum.end() or *w != p + q + r + sum[x]) continue;

        cout << "Yes" << ln;
        return 0;
    }

    cout << "No" << ln;
    return 0;

    return 0;
}
