#include <iostream>
#include "min_matching.h"
#include "rural_postman.h"

auto StartTime() {
    return std::chrono::high_resolution_clock::now();
}

void EndTime(std::chrono::high_resolution_clock::time_point start) {
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

void RunMockTestOne() {
    int n = 4;
    int m = 6;
    std::vector<int> u = {1, 1, 1, 2, 2, 3}; // starting node
    std::vector<int> v = {2, 3, 4, 3, 4, 4}; // ending node
    std::vector<int> w = {3, 1, 1, 1, 1, 1}; // weight
    std::vector<int> chosen_edges = {0, 5}; // indexes of chosen edges
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunMockTestTwo() {
    int n = 4;
    int m = 6;
    std::vector<int> u = {1, 1, 1, 2, 2, 3}; // starting node
    std::vector<int> v = {2, 3, 4, 3, 4, 4}; // ending node
    std::vector<int> w = {3, 1, 1, 1, 1, 1}; // weight
    std::vector<int> chosen_edges = {0, 4, 5}; // indexes of chosen edges
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunMockTestThree() {
    int n = 4;
    int m = 6;
    std::vector<int> u = {1, 1}; // starting node
    std::vector<int> v = {2, 3}; // ending node
    std::vector<int> w = {3, 1}; // weight
    std::vector<int> chosen_edges = {0}; // indexes of chosen edges
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunOneComponentRandomGraph(int n) {
    int m = n * (n - 1) / 2;
    std::vector<int> u(m), v(m), w(m);
    int k = 0;
    std::vector<int> chosen_edges;
    for (int i = 1; i <= n - 1; ++i) {
        chosen_edges.push_back(k);
        for (int j = i + 1; j <= n; ++j) {
            u[k] = i;
            v[k] = j;
            w[k] = rand() % 100 + 1;
            ++k;
        }
    }
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunTwoComponentGraph(int n) {
    int m = n * (n - 1) / 2;
    std::vector<int> u(m), v(m), w(m);
    int k = 0;
    std::vector<int> chosen_edges;
    for (int i = 1; i <= n - 1; ++i) {
        if (i != n / 2) {
            chosen_edges.push_back(k);
        }
        for (int j = i + 1; j <= n; ++j) {
            u[k] = i;
            v[k] = j;
            w[k] = rand() % 100 + 1;
            ++k;
        }
    }
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunFourComponentGraph(int n) {
    int m = n * (n - 1) / 2;
    std::vector<int> u(m), v(m), w(m);
    int k = 0;
    std::vector<int> chosen_edges;
    for (int i = 1; i <= n - 1; ++i) {
        if (i != n / 2 && i != n / 4 && i != 3 * n / 4) {
            chosen_edges.push_back(k);
        }
        for (int j = i + 1; j <= n; ++j) {
            u[k] = i;
            v[k] = j;
            w[k] = rand() % 100 + 1;
            ++k;
        }
    }
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

void RunManyComponentGraph(int n) {
    int m = n * (n - 1) / 2;
    std::vector<int> u(m), v(m), w(m);
    int k = 0;
    std::vector<int> chosen_edges;
    for (int i = 1; i <= n - 1; ++i) {
        if (i % 2 == 1) {
            chosen_edges.push_back(k);
        }
        for (int j = i + 1; j <= n; ++j) {
            u[k] = i;
            v[k] = j;
            w[k] = rand() % 100 + 1;
            ++k;
        }
    }
    auto ans = RuralPostman().Run(n, m, u,
                                  v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    /////////////////////MAX MATCHING////////////////////////
    /*
    int n, m;
    std::cin >> n >> m;
    std::vector<int> u(m), v(m), w(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> u[i] >> v[i] >> w[i];
    }
    auto ans = MinMatching(n).Run(n, m, u, v, w);
    std::cout << ans.first << "\n";
    for (int i = 0; i < n; ++i) {
        std::cout << ans.second[i] << " ";
    }
    std::cout << "\n";
     */
    //////////////////////////////////////////////////////////

    /////////////////////MOCK TESTS////////////////////////
    /*

    // Mock test 1 from the paper
    RunMockTestOne();

    // Mock test 2 from the paper
    RunMockTestTwo();

    // Mock test 3 from the paper
    RunMockTestThree();
     */
    //////////////////////////////////////////////////////////

    ////////////////////////RANDOM GRAPHS TIME MEASUREMENT/////////////////////
    std::vector<int> n_values = {8, 16, 32, 64, 128, 256};

    // Random graph with one component
/*
    n_values = {8, 12, 16, 20, 32, 64, 128, 256};
    for (int i = 0; i < n_values.size(); ++i) {
        auto start = StartTime();
        std::cout << "n = " << n_values[i] << "\n";
        RunOneComponentRandomGraph(n_values[i]);
        EndTime(start);
    }
     */

    // Random graph with two components
/*
    n_values = {8, 12, 16, 20, 32, 64, 128};
    for (int i = 0; i < n_values.size(); ++i) {
        auto start = StartTime();
        std::cout << "n = " << n_values[i] << "\n";
        RunTwoComponentGraph(n_values[i]);
        EndTime(start);
    }
*/

    // Random graph with four components
    /*
    n_values = {8, 12, 16, 20, 32};
    for (int i = 0; i < n_values.size(); ++i) {
        auto start = StartTime();
        std::cout << "n = " << n_values[i] << "\n";
        RunFourComponentGraph(n_values[i]);
        EndTime(start);
    }
    */


    // Random graph with many components
    /*
    n_values = {8, 12, 16, 20};
    for (int i = 0; i < n_values.size(); ++i) {
        auto start = StartTime();
        std::cout << "n = " << n_values[i] << "\n";
        RunManyComponentGraph(n_values[i]);
        EndTime(start);
    }
     */
    //////////////////////////////////////////////////////////

    ////////////////////////RURAL POSTMAN/////////////////////
    /*int n, m;
    std::cin >> n >> m;
    std::vector<int> u(m), v(m), w(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> u[i] >> v[i] >> w[i];
    }
    int k;
    std::cin >> k;
    std::vector<int> chosen_edges(k);
    for (int i = 0; i < k; ++i) {
        std::cin >> chosen_edges[i];
    }
    auto ans = RuralPostman().Run(n, m, u, v, w, chosen_edges);
    std::cout << "Cost: " << ans.first << "\n";
    std::cout << "Cycle traversal: ";
    for (int i = 0; i < ans.second.size(); ++i) {
        std::cout << ans.second[i] << " ";
    }*/
    //////////////////////////////////////////////////////////
    return 0;
}