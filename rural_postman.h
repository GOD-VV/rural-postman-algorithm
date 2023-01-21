#ifndef RPP__RURAL_POSTMAN_H_
#define RPP__RURAL_POSTMAN_H_

#include <vector>
#include <set>

struct Edge {
  int u{0}, v{0}, w{0};
  Edge(int u, int v, int w) : u(u), v(v), w(w) {}
  Edge() = default;
};

class RuralPostman{
 public:
  const int INF = 1e9;

  std::pair<int64_t, std::vector<int> > Run(int n_arg,
                                            int m_arg,
                                            std::vector<int> u_arg,
                                            std::vector<int> v_arg,
                                            std::vector<int> w_arg,
                                            std::vector<int> chosen_edges);

 private:
  int n_;
  int m_;

  std::vector<std::vector<Edge> > g_;
  std::vector<Edge> chosen_edges_;
  // Saved paths between every pair of vertices
  std::vector<std::vector<std::vector<int> > > shortest_path_;
  std::set<std::pair<int, int>> chosen_vertices_;

  // Components
  std::vector<std::vector<int> > components_;
  std::vector<std::pair<int, int> > component_pairs_;
  std::vector<std::vector<Edge> > subgraph_;
  std::vector<int> subgraph_vertices_;

  // Graph transformation according to Lemma 3.2
  void TransformGraph();

  // Bruteforce search for the solution
  std::pair<int64_t, std::vector<Edge> > BruteforceSearch(int, std::vector<Edge>);

  // Check if graph is connected
  bool IsConnected(std::vector<std::vector<Edge> >, std::vector<int>);

  // Check if graph is eulerian
  std::vector<Edge> FindEulerCycle(std::vector<std::vector<Edge> >);
};

#endif //RPP__RURAL_POSTMAN_H_
