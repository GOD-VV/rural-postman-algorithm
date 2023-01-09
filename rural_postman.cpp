#include "rural_postman.h"
#include "min_matching.h"
#include <iostream>

std::pair<int64_t, std::vector<int> > RuralPostman::Run(int n_arg,
                                                        int m_arg,
                                                        std::vector<int> u_arg,
                                                        std::vector<int> v_arg,
                                                        std::vector<int> w_arg,
                                                        std::vector<int> chosen_edges) {
    n_ = n_arg;
    m_ = m_arg;
    g_ = std::vector<std::vector<Edge> >(n_ + 1, std::vector<Edge>(n_ + 1));
    for (int u = 0; u <= n_; ++u) {
        for (int v = 0; v <= n_; ++v) {
            if (u == v) {
                g_[u][v] = Edge(u, v, 0);
            } else {
                g_[u][v] = Edge(u, v, INF);
            }
        }
    }

    for (int i = 0, u, v, w; i < m_; ++i) {
        u = u_arg[i];
        v = v_arg[i];
        w = w_arg[i];
        g_[u][v].w = g_[v][u].w = w;
    }

    for (int i: chosen_edges) {
        chosen_edges_.emplace_back(u_arg[i], v_arg[i], w_arg[i]);
    }

    // Graph transformation according to Lemma 3.2
    shortest_path_ = std::vector<std::vector<std::vector<int> > >(n_ + 1, std::vector<std::vector<int> >(n_ + 1));
    TransformGraph();
    /*
    std::cout << "Transformed graph:" << std::endl;
    for (int u = 1; u <= n_; ++u) {
        for (int v = 1; v <= n_; ++v) {
            std::cout << u << " " << v << " " << g_[u][v].w << std::endl;
            for (int v : shortest_path_[u][v]) {
                std::cout << v << " ";
            }
            if (shortest_path_[u][v].size() > 0) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }*/

    // Subgraph on the vertices of chosen edges
    subgraph_ = std::vector<std::vector<Edge> >(n_ + 1, std::vector<Edge>(n_ + 1));
    for (int u = 0; u <= n_; ++u) {
        for (int v = 0; v <= n_; ++v) {
            if (u == v) {
                subgraph_[u][v] = Edge(u, v, 0);
            } else {
                subgraph_[u][v] = Edge(u, v, INF);
            }
        }
    }
    for (Edge e: chosen_edges_) {
        subgraph_[e.u][e.v].w = subgraph_[e.v][e.u].w = e.w;
    }

    // Finding all components_ of the subgraph_
    std::vector<bool> used(n_ + 1, false);
    for (int u = 1; u <= n_; ++u) {
        if (!used[u]) {
            std::vector<int> component;
            std::vector<int> stack;
            stack.push_back(u);
            used[u] = true;
            while (!stack.empty()) {
                int v = stack.back();
                stack.pop_back();
                component.push_back(v);
                for (int i = 1; i <= n_; ++i) {
                    if (subgraph_[v][i].w != INF && !used[i]) {
                        stack.push_back(i);
                        used[i] = true;
                    }
                }
            }
            components_.push_back(component);
        }
    }


    for (int i = 0; i < components_.size() - 1; ++i) {
        for (int j = i + 1; j < components_.size(); ++j) {
            component_pairs_.emplace_back(i, j);
        }
    }

    auto ans = BruteforceSearch(0, std::vector<Edge>());
    if (ans.second.empty()) {
        return std::make_pair(-1, std::vector<int>());
    }
    std::vector<int> ans_cycle;
    for (Edge e: ans.second) {
        ans_cycle.push_back(e.u);
    }
    ans_cycle.push_back(ans.second.back().v);
    return std::make_pair(ans.first, ans_cycle);
}

bool RuralPostman::IsConnected(std::vector<std::vector<Edge>>) {
    std::vector<bool> used(n_ + 1, false);
    std::vector<int> stack;
    stack.push_back(1);
    used[1] = true;
    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        for (int i = 1; i <= n_; ++i) {
            if (g_[v][i].w != INF && !used[i]) {
                stack.push_back(i);
                used[i] = true;
            }
        }
    }
    for (int i = 1; i <= n_; ++i) {
        if (!used[i]) {
            return false;
        }
    }
    return true;
}

std::vector<Edge> RuralPostman::FindEulerCycle(std::vector<std::vector<Edge> > g) {
    std::vector<Edge> cycle;
    std::vector<int> min_used(n_ + 1, 0);
    int v = 1;
    bool found = true;
    while (found) {
        found = false;
        for (int to = min_used[v]; to <= n_; ++to) {
            if (to != v && g[v][to].w != INF) {
                found = true;
                min_used[v] = to + 1;
                cycle.push_back(g[v][to]);
                g[v][to].w = g[to][v].w = INF;
                v = to;
                break;
            }
        }
    }
    bool is_cycle = true;
    for (int i = 1; i <= n_; ++i) {
        for (int j = 1; j <= n_; ++j) {
            if (i != j && g[i][j].w != INF) {
                is_cycle = false;
                break;
            }
        }
    }
    if (!is_cycle) {
        cycle.clear();
    }
    return cycle;
}

std::pair<int64_t, std::vector<Edge> > RuralPostman::BruteforceSearch(int num,
                                                                      std::vector<Edge> current_edges) {
    if (num == component_pairs_.size()) {
        std::vector<std::vector<Edge> > g_new = subgraph_;
        for (Edge e: current_edges) {
            g_new[e.u][e.v].w = g_new[e.v][e.u].w = e.w;
        }
        if (IsConnected(g_new)) {
            std::vector<int> odd_vertices;
            for (int i = 1; i <= n_; ++i) {
                int degree = 0;
                for (int j = 1; j <= n_; ++j) {
                    if (i != j && g_new[i][j].w != RuralPostman::INF) {
                        ++degree;
                    }
                }
                if (degree % 2 == 1) {
                    odd_vertices.push_back(i);
                }
            }
            std::vector<Edge> odd_vertices_edges;
            for (int i = 1; i < odd_vertices.size() - 1; ++i) {
                for (int j = i + 1; j < odd_vertices.size(); ++j) {
                    if (g_new[odd_vertices[i]][odd_vertices[j]].w == INF
                        && g_[odd_vertices[i]][odd_vertices[j]].w != INF) {
                        odd_vertices_edges.emplace_back(odd_vertices[i],
                                                        odd_vertices[j],
                                                        g_[odd_vertices[i]][odd_vertices[j]].w);
                    }
                }
            }
            int m = odd_vertices_edges.size();
            std::vector<int> u(m), v(m), w(m);
            for (int i = 0; i < m; ++i) {
                u[i] = odd_vertices_edges[i].u;
                v[i] = odd_vertices_edges[i].v;
                w[i] = odd_vertices_edges[i].w;
            }
            auto ans = MinMatching(odd_vertices.size()).Run(odd_vertices.size(), m,
                                                            u, v, w);
            if (ans.first == -1) {
                return std::make_pair(RuralPostman::INF, std::vector<Edge>());
            } else {
                for (int i = 0; i < ans.second.size(); ++i) {
                    int cur_u = odd_vertices[i];
                    int cur_v = odd_vertices[ans.second[i]];
                    g_new[cur_u][cur_v].w = g_new[cur_v][cur_u].w = g_[cur_u][cur_v].w;
                }
                std::vector<Edge> cycle = FindEulerCycle(g_new);
                if (cycle.empty()) {
                    return std::make_pair(RuralPostman::INF, std::vector<Edge>());
                } else {
                    int64_t cycle_weight = 0;
                    for (Edge e: cycle) {
                        cycle_weight += e.w;
                    }
                    return std::make_pair(cycle_weight, cycle);
                }
            }
        } else {
            return std::make_pair(RuralPostman::INF, std::vector<Edge>());
        }
    } else {
        int i = component_pairs_[num].first;
        int j = component_pairs_[num].second;
        std::vector<Edge> edges = current_edges;
        std::pair<int64_t, std::vector<Edge> > ans = std::make_pair(RuralPostman::INF,
                                                                    std::vector<Edge>());
        for (int u : components_[i]) {
            for (int v : components_[j]) {
                if (g_[u][v].w != INF) {
                    edges.push_back(g_[u][v]);
                    auto cur_ans = BruteforceSearch(num + 1, edges);
                    if (cur_ans.first < ans.first) {
                        ans = cur_ans;
                    }
                    edges.pop_back();
                }
            }
        }
    }
    return std::make_pair(RuralPostman::INF, std::vector<Edge>());
}

void RuralPostman::TransformGraph() {
    // The shortest path between every pair of vertices using Floyd-Warshall algorithm
    for (int k = 0; k < n_; ++k) {
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < n_; ++j) {
                if (g_[i][k].w + g_[k][j].w < g_[i][j].w) {
                    g_[i][j].w = g_[i][k].w + g_[k][j].w;
                    shortest_path_[i][j] = shortest_path_[i][k];
                    shortest_path_[i][j].push_back(k);
                    shortest_path_[i][j].push_back(j);
                }
            }
        }
    }
}
