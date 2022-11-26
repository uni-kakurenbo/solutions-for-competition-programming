/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#include <iostream>
#include <string>
#include <valarray>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <numeric>

#include "template.hpp"

struct Graph {
  protected:
    int V;
    int E;
    std::valarray<bool> _data;
    std::valarray<int> _degrees;

    inline int pair_tp_id(const int i, const int j) const {
        assert(0 <= i and i < j and j < this->V);
        return this->E - (this->V - i) * (this->V - i - 1) / 2 + j - i - 1;
    }

  public:
    Graph(const int V = 0) : V(V), E(V*(V-1)/2) { _data.resize(this->E, 0), _degrees.resize(this->V, 0); }

    int size() const { return this->V; }

    void add_edge(const int i, const int j) {
        assert(not this->has_edge(i, j));
        this->_data[this->pair_tp_id(i, j)] = true;
        ++this->_degrees[i], ++this->_degrees[j];
    }

    bool has_edge(const int i, const int j) const {
        return this->_data[this->pair_tp_id(i, j)];
    }
    int degree(const int v) const {
        return this->_degrees[v];
    }
    auto degrees() const { return this->_degrees; }

    void from_string(const std::string &H) {
        int i = 0, j = 1;
        ITR(h, H) {
            if(h == '1') this->add_edge(i, j);
            ++j;
            if(j >= this->V) {
                ++i;
                j = i+1;
            }
        }
    }

    std::string to_string() const {
        std::string res;
        REP(i, this->V) REPF(j, i+1, this->V) {
            res += this->_data[this->pair_tp_id(i, j)] ? "1" : "0";
        }
        assert((int)res.size() == this->E);
        return res;
    }
};

struct Solver {
  private:
    std::string note = "#v";

  protected:
    constexpr static double NO_ERROR = 0.001;
    constexpr static double MANY_ERROR = 0.38;
    int M;
    float eps;
    int V;
    int threshold = -1;
    int trim_size = 0, trim_begin = 101;
    bool unuse0 = 0;

    std::valarray<int> degrees_sums;

  public:
    Solver(const int M, const float eps) : M(M), eps(eps) {
        if(this->eps <= NO_ERROR) {
            this->V = std::ceil((1.0 + std::sqrt(1.0 + 8*M)) / 2);
            const int E = V*(V-1)/2;
            assert(E >= M);
            return;
        }
        if(this->eps >= MANY_ERROR) {
            this->V = 4;
            return;
        }

        this->V = this->M;// + 1/eps;

        if(this->M < 100) {
            this->unuse0 = true;
            ++this->V;
        }

        if(this->eps >= 0.18) chmax(this->V, 70);
        if(this->eps >= 0.20) chmax(this->V, 80);
        if(this->eps >= 0.22) chmax(this->V, 90);
        if(this->eps >= 0.24) chmax(this->V, 100);

        this->V = std::clamp(this->V, 1, 100);

        this->degrees_sums.resize(this->V);

        this->threshold = this->V / 2;
        debug(this->M, this->V, this->threshold);

        this->trim_size = this->V - this->M - this->unuse0;
        this->trim_begin = this->threshold - trim_size/2;
        debug(this->trim_size, this->trim_begin);

        REP(i, this->V) this->note += " ", this->note += std::to_string(i);
    }

    int vertexes() const { return this->V; }

    std::string generate(const int _k) {
        int k = _k + this->unuse0;
        if(this->eps <= NO_ERROR) {
            const int E = this->V*(this->V-1)/2;
            return std::string(k, '1') + std::string(E-k, '0');
        }
        if(this->eps >= MANY_ERROR) {
            const int E = this->V*(this->V-1)/2;
            if(k == 0) return std::string(E, '0');
            return std::string(E, '1');
            // return std::string(E/2, '1') + std::string(E - E/2, '0');
        }

        if(this->trim_begin <= _k) {
            k += this->trim_size;
        }
        debug(_k, k);

        Graph G(this->V);

        REP(i, k) {
            REPF(j, i+1, k) G.add_edge(i, j);
        }
        if(k <= this->threshold) {
            REP(i, k) {
                REPF(j, k, G.size()) G.add_edge(i, j);
            }
        }

        auto degrees = G.degrees();
        debug(degrees);
        this->degrees_sums[k] = degrees.sum();
        // debug(k, this->degrees_sums[k]);
        debug(degrees.max() - degrees.min());

        return G.to_string();
    }

    int predict(const std::string &H) const  {
        // std::cout << this->note << std::endl;

        if(this->eps <= NO_ERROR) {
            return std::min(this->M-1, (int)std::count(ALL(H), '1'));
        }
        if(this->eps >= MANY_ERROR) {
            const int cnt0 = (int)std::count(ALL(H), '0');
            const int cnt1 = (int)std::count(ALL(H), '1');
            return cnt0 > cnt1 ? 0 : this->M-1;
            // if(this->cnt0 > 0)
        }

        Graph G(this->V);
        G.from_string(H);

        auto degrees = G.degrees();
        std::sort(ALL(degrees));
        std::reverse(ALL(degrees));
        debug(degrees, degrees.sum());

        std::vector<int> diff(degrees.size());
        std::adjacent_difference(ALL(degrees), diff.begin());
        debug(diff);

        const auto diff_min = std::min_element(ALL(diff));
        auto diff_min_rm = std::find(RALL(diff), *diff_min);
        debug(*diff_min);

        std::vector<int> cnts(100);
        std::reverse(ALL(degrees));
        REPD(d, 100) {
            cnts[d] = std::end(degrees) - std::lower_bound(ALL(degrees), d);
        }
        std::vector<int> diff2(100);
        std::adjacent_difference(ALL(cnts), diff2.begin());
        debug(cnts);
        debug(diff2);

        int res = diff_min_rm.base() - std::begin(diff) - 1;

        if(degrees.sum() - this->degrees_sums[res] < -4000)  res = 0;

        debug(res, degrees.sum(), this->degrees_sums[res]);
        res -= this->unuse0;
        if(res >= this->trim_begin) {
            res -= this->trim_size;
        }

        // if(this->trim_begin*0.9 < res and res < (this->trim_begin + this->trim_size)*1.1 and cnt >= 4) ++res;

        debug(res);

        return std::clamp(res, 0, this->M-1);
    }
};

constexpr int QUERIES = 100;

signed main() {
    int M; std::cin >> M;
    float eps; std::cin >> eps;

    Solver solver(M, eps);

    std::cout << solver.vertexes() << std::endl;
    REP(i, M) {
        std::cout << solver.generate(i) << std::endl;
    }

    REP(i, QUERIES) {
        debug(i);
        std::string H; std::cin >> H;
        std::cout << solver.predict(H) << std::endl;
    }

    return 0;
}
