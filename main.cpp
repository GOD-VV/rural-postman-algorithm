#include <iostream>
#include "min_matching.h"
#include "rural_postman.h"

int main() {

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



    int n, m;
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
    std::cout << ans.first << "\n";
    for (int i = 0; i < k; ++i) {
        std::cout << ans.second[i] << " ";
    }
    return 0;
}