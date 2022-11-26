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
#include <random>
#include <chrono>

#include "template.hpp"

using Time = int32_t;
class Timer {
  private:
    const Time time_limit;
    std::chrono::system_clock::time_point clock_start;

  public:
    Timer(const Time time_limit = 0) : time_limit(time_limit), clock_start(std::chrono::system_clock::now()) {}

    inline Timer* reset() {
        clock_start = std::chrono::system_clock::now();
        return this;
    }

    inline Time elapsed() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->clock_start).count();
    }
    inline Time remaining () const {
        return time_limit - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - this->clock_start).count();
    }

    inline bool expired() const { return this->elapsed() > time_limit; }
};


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

struct Config {
    int type = -1;
    int V = -1;
    int score = -1;
};

struct Solver {
  protected:
    int M;
    float eps;
    int V;
    bool specialize = false;
    int threshold = -1;
    int trim_size = 0, trim_begin = 101;
    bool unuse0 = 0;

    std::valarray<int> degrees_sums;

  public:
    Solver(const int M, const float eps, const Config opt = {}) : M(M), eps(eps) {
        if(this->eps == 0 or opt.type == 0) {
            this->specialize = true;
            this->V = std::ceil((1.0 + std::sqrt(1.0 + 8*M)) / 2);
            const int E = V*(V-1)/2;
            assert(E >= M);
        }
        else {
            this->V = this->M;// + 1/eps;


            if(this->M < 100) this->unuse0 = true;
            ++this->V;

            if(this->eps >= 0.18) chmax(this->V, 70);
            if(this->eps >= 0.20) chmax(this->V, 80);
            if(this->eps >= 0.22) chmax(this->V, 90);
            if(this->eps >= 0.24) chmax(this->V, 100);

            if(opt.V >= 0) this->V = opt.V;

            this->V = std::clamp(this->V, 1, 100);

            this->degrees_sums.resize(this->V);

            this->threshold = this->V / 2;
            debug(this->M, this->V, this->threshold);

            this->trim_size = this->V - this->M - this->unuse0;
            this->trim_begin = this->threshold - trim_size/2;
            debug(this->trim_size, this->trim_begin);
        }
    }

    int vertexes() const { return this->V; }

    std::string generate(const int _k) {
        int k = _k + this->unuse0;
        if(this->specialize) {
            const int E = this->V*(this->V-1)/2;
            return std::string(k, '1') + std::string(E-k, '0');
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
        if(this->specialize) {
            return std::min(this->M-1, (int)std::count(ALL(H), '1'));
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
        const auto diff_min_rm = std::find(RALL(diff), *diff_min);
        debug(*diff_min);

        int res = diff_min_rm.base() - std::begin(diff) - 1;
        if(degrees.sum() - this->degrees_sums[res] < -4000)  res = 0;

        debug(res, degrees.sum(), this->degrees_sums[res]);
        res -= this->unuse0;
        if(res >= this->trim_begin) {
            res -= this->trim_size;
        }
        debug(res);

        return std::clamp(res, 0, this->M-1);
    }
};

std::mt19937 random_engine(998244353);
std::uniform_real_distribution distr_01(0.0, 1.0);
std::uniform_int_distribution distr_10(-3, 3);

struct Tuner {
  protected:
    int M;
    double eps;

  public:
    Tuner(const int M, const double eps) : M(M), eps(eps) {}

    Config test(const Timer *timer, const Config init = {}) {
        Config option = init;
        option.score = this->run(option, 1);

        Config res = option;

        double start_temp = 50, end_temp = 10;
        while (true) {
            if (timer->expired()) break;

            Config new_option = option;

            new_option.V += distr_10(random_engine);
            new_option.V = std::clamp(new_option.V, this->M, 100);

            new_option.score = this->run(new_option, 1);

            double temp = start_temp + (end_temp - start_temp) * (timer->elapsed()) / 3000;
            double prob = std::exp((new_option.score - option.score)/temp);

            if (prob > distr_01(random_engine)) {
                option = new_option;
                if(res.score < option.score) {
                    res = option;
                    std::cerr << res.score << std::endl;
                }
            }
        }
        std::cerr << res.score << std::endl;
        std::cerr << res.V << std::endl;
        return res;
    }

//   protected:
    ll run(const Config opt, const int times) {
        Solver solver(this->M, this->eps, opt);
        const int N = solver.vertexes();

        std::vector<std::string> G(this->M);
        REP(k, this->M) G[k] = solver.generate(k);

        int err = 0;
        LOOP(times) {
            REP(s, this->M) {
                std::string H = G[s];
                REP(i, (int)H.size()) {
                    if(distr_01(random_engine) >= this->eps) continue;
                    if(H[i] == '1') H[i] = '0';
                    else H[i] = '1';
                }
                const int t = solver.predict(H);
                debug(s, t);
                if(s != t) ++err;
            }
        }

        debug(err);
        return 1000000000LL * std::pow(0.9, err) / N;
    }
};

constexpr int QUERIES = 100;

signed main() {
    int M; std::cin >> M;
    float eps; std::cin >> eps;

    Tuner tuner(M, eps);

    Config opt;
    std::cerr << tuner.run(opt, 1) << std::endl;
    {
        // Timer timer0(1000);
        // const Config res0 = tuner.test(&timer0, Config{ 0, M, -1 });
        // Timer timer1(3000);
        // const Config res1 = tuner.test(&timer1, Config{ 1, M, -1 });
        // if(res0.score > res1.score) opt = res0;
        // else opt = res1;
    }
    std::cerr << opt.type << std::endl;
    std::cerr << opt.V << std::endl;
    std::cerr << opt.score << std::endl;

    Solver solver(M, eps, opt);

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
