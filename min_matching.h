#ifndef RPP__MIN_MATCHING_H_
#define RPP__MIN_MATCHING_H_

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <deque>
#define DIST(e) (lab_[e.u]+lab_[e.v]-g_[e.u][e.v].w*2)

class MinMatching {
 public:
  static const int INF = 1e9;

  MinMatching(int n);

  std::pair<int64_t, std::vector<int> > Run(int n_1,
                                            int m_1,
                                            std::vector<int> u_arg,
                                            std::vector<int> v_arg,
                                            std::vector<int> w_arg);

 private:
  struct Edge {
    int u{0}, v{0}, w{0};
    Edge(int u, int v, int w) : u(u), v(v), w(w) {}
    Edge() = default;
  };

  std::vector<std::vector<Edge> > g_;
  std::vector<std::vector<int> > flower_from_, flower_;
  std::vector<int> match_, st_, lab_, slack_, s_, pa_, vis_;
  int n_, m_, n_x_;
  std::deque<int> q_;

  void update_slack(int u, int x);

  void set_slack(int x);

  void q_push(int x);

  void set_st(int x, int b);

  int get_pr(int b, int xr);

  void set_match(int u, int v);

  void augment(int u, int v);

  int get_lca(int u, int v);

  void add_blossom(int u, int lca, int v);

  void expand_blossom(int b);

  bool on_found_Edge(const Edge& e);

  bool matching();

  std::pair<int64_t, int> weight_blossom();
};

#endif //RPP__MIN_MATCHING_H_
