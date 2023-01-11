#include "min_matching.h"

MinMatching::MinMatching(int n) : n_(n) {
    g_ = std::vector<std::vector<Edge> >(n+1, std::vector<Edge>(n+1));
    flower_from_ = std::vector<std::vector<int> >(n+1, std::vector<int>(n+1));
    flower_ = std::vector<std::vector<int> >(n+1);
    match_ = std::vector<int>(n+1);
    st_ = std::vector<int>(n+1);
    lab_ = std::vector<int>(n+1);
    slack_ = std::vector<int>(n+1);
    s_ = std::vector<int>(n+1);
    pa_ = std::vector<int>(n+1);
    vis_ = std::vector<int>(n+1);
}

void MinMatching::update_slack(int u, int x) {
    if (!slack_[x] || DIST(g_[u][x]) < DIST(g_[slack_[x]][x]))
        slack_[x] = u;
}

void MinMatching::set_slack(int x) {
    slack_[x] = 0;
    for (int u = 1; u <= n_; ++u)
        if (g_[u][x].w > 0 && st_[u] != x && s_[st_[u]] == 0)
            update_slack(u, x);
}

void MinMatching::q_push(int x) {
    if (x <= n_)
        return q_.push_back(x);
    for (int i : flower_[x])
        q_push(i);
}

void MinMatching::set_st(int x, int b) {
    st_[x] = b;
    if (x <= n_)
        return;
    for (int i : flower_[x])
        set_st(i, b);
}


int MinMatching::get_pr(int b, int xr) {
    int pr = find(flower_[b].begin(), flower_[b].end(), xr) - flower_[b].begin();
    if (pr % 2 == 1) {
        reverse(flower_[b].begin() + 1, flower_[b].end());
        return static_cast<int>(flower_[b].size() - pr);
    } else
        return pr;
}

void MinMatching::set_match(int u, int v) {
    match_[u] = g_[u][v].v;
    if (u <= n_)
        return;
    Edge e = g_[u][v];
    int xr = flower_from_[u][e.u];
    int pr = get_pr(u, xr);
    for (int i = 0; i < pr; ++i)
        set_match(flower_[u][i], flower_[u][i ^ 1]);
    set_match(xr, v);
    rotate(flower_[u].begin(), flower_[u].begin() + pr, flower_[u].end());
}
void MinMatching::augment(int u, int v) {
    int xnv = st_[match_[u]];
    set_match(u, v);
    if (!xnv)
        return;
    set_match(xnv, st_[pa_[xnv]]);
    augment(st_[pa_[xnv]], xnv);
}

int MinMatching::get_lca(int u, int v) {
    static int t = 0;
    for (++t; u || v; std::swap(u, v)) {
        if (u == 0)
            continue;
        if (vis_[u] == t)
            return u;
        vis_[u] = t;
        u = st_[match_[u]];
        if (u)
            u = st_[pa_[u]];
    }
    return 0;
}

void MinMatching::add_blossom(int u, int lca, int v) {
    int b = n_ + 1;
    while (b <= n_x_ && st_[b])
        ++b;
    if (b > n_x_)
        ++n_x_;
    lab_[b] = 0, s_[b] = 0;
    match_[b] = match_[lca];
    flower_[b].clear();
    flower_[b].push_back(lca);
    for (int x = u, y; x != lca; x = st_[pa_[y]]) {
        flower_[b].push_back(x);
        flower_[b].push_back(y = st_[match_[x]]);
        q_push(y);
    }
    reverse(flower_[b].begin() + 1, flower_[b].end());
    for (int x = v, y; x != lca; x = st_[pa_[y]]) {
        flower_[b].push_back(x);
        flower_[b].push_back(y = st_[match_[x]]);
        q_push(y);
    }
    set_st(b, b);
    for (int x = 1; x <= n_x_; ++x)
        g_[b][x].w = g_[x][b].w = 0;
    for (int x = 1; x <= n_; ++x)
        flower_from_[b][x] = 0;
    for (int i = 0; i < flower_[b].size(); ++i) {
        int xs = flower_[b][i];
        for (int x = 1; x <= n_x_; ++x)
            if (g_[b][x].w == 0 || DIST(g_[xs][x]) < DIST(g_[b][x])) {
                g_[b][x] = g_[xs][x];
                g_[x][b] = g_[x][xs];
            }
        for (int x = 1; x <= n_; ++x)
            if (flower_from_[xs][x])
                flower_from_[b][x] = xs;
    }
    set_slack(b);
}

void MinMatching::expand_blossom(int b) {
    for (int i : flower_[b])
        set_st(i, i);
    int xr = flower_from_[b][g_[b][pa_[b]].u];
    int pr = get_pr(b, xr);
    for (int i = 0; i < pr; i += 2) {
        int xs = flower_[b][i];
        int xns = flower_[b][i + 1];
        pa_[xs] = g_[xns][xs].u;
        s_[xs] = 1;
        s_[xns] = 0;
        slack_[xs] = 0;
        set_slack(xns);
        q_push(xns);
    }
    s_[xr] = 1;
    pa_[xr] = pa_[b];
    for (int i = pr + 1; i < flower_[b].size(); ++i) {
        int xs = flower_[b][i];
        s_[xs] = -1;
        set_slack(xs);
    }
    st_[b] = 0;
}

bool MinMatching::on_found_Edge(const Edge& e) {
    int u = st_[e.u], v = st_[e.v];
    if (s_[v] == -1) {
        pa_[v] = e.u;
        s_[v] = 1;
        int nu = st_[match_[v]];
        slack_[v] = slack_[nu] = 0;
        s_[nu] = 0;
        q_push(nu);
    } else if (s_[v] == 0) {
        int lca = get_lca(u, v);
        if (!lca)
            return augment(u, v), augment(v, u), 1;
        else add_blossom(u, lca, v);
    }
    return false;
}

bool MinMatching::matching() {
    std::fill(s_.begin(), s_.begin() + n_x_ + 1, -1);
    std::fill(slack_.begin(), slack_.begin() + n_x_ + 1, 0);
    q_.clear();
    for (int x = 1; x <= n_x_; ++x)
        if (st_[x] == x && !match_[x]) {
            pa_[x] = 0;
            s_[x] = 0;
            q_push(x);
        }
    if (q_.empty())
        return false;
    for (;;) {
        while (!q_.empty()) {
            int u = q_.front();
            q_.pop_front();
            if (s_[st_[u]] == 1)
                continue;
            for (int v = 1; v <= n_; ++v)
                if (g_[u][v].w > 0 && st_[u] != st_[v]) {
                    if (DIST(g_[u][v]) == 0) {
                        if (on_found_Edge(g_[u][v]))
                            return true;
                    } else update_slack(u, st_[v]);
                }
        }
        int d = INF;
        for (int b = n_ + 1; b <= n_x_; ++b)
            if (st_[b] == b && s_[b] == 1)
                d = std::min(d, lab_[b] / 2);
        for (int x = 1; x <= n_x_; ++x)
            if (st_[x] == x && slack_[x]) {
                if (s_[x] == -1)
                    d = std::min(d, DIST(g_[slack_[x]][x]));
                else if (s_[x] == 0)
                    d = std::min(d, DIST(g_[slack_[x]][x]) / 2);
            }
        for (int u = 1; u <= n_; ++u) {
            if (s_[st_[u]] == 0) {
                if (lab_[u] <= d)
                    return false;
                lab_[u] -= d;
            } else if (s_[st_[u]] == 1)
                lab_[u] += d;
        }
        for (int b = n_ + 1; b <= n_x_; ++b)
            if (st_[b] == b) {
                if (s_[st_[b]] == 0)
                    lab_[b] += d * 2;
                else if (s_[st_[b]] == 1)
                    lab_[b] -= d * 2;
            }
        q_.clear();
        for (int x = 1; x <= n_x_; ++x)
            if (st_[x] == x && slack_[x] && st_[slack_[x]] != x && DIST(g_[slack_[x]][x]) == 0)
                if (on_found_Edge(g_[slack_[x]][x]))
                    return true;
        for (int b = n_ + 1; b <= n_x_; ++b)
            if (st_[b] == b && s_[b] == 1 && lab_[b] == 0)
                expand_blossom(b);
    }
    return false;
}

std::pair<int64_t, int> MinMatching::weight_blossom() {
    std::fill(match_.begin(), match_.begin() + n_ + 1, 0);
    n_x_ = n_;
    int n_matches = 0;
    int64_t tot_w = 0;
    for (int u = 0; u <= n_; ++u) {
        st_[u] = u;
        flower_[u].clear();
    }
    int w_max = 0;
    for (int u = 1; u <= n_; ++u)
        for (int v = 1; v <= n_; ++v) {
            flower_from_[u][v] = (u == v ? u : 0);
            w_max = std::max(w_max, g_[u][v].w);
        }
    for (int u = 1; u <= n_; ++u)
        lab_[u] = w_max;
    while (matching())
        ++n_matches;
    for (int u = 1; u <= n_; ++u)
        if (match_[u] && match_[u] < u)
            tot_w += g_[u][match_[u]].w;
    return std::make_pair(tot_w, n_matches);
}

std::pair<int64_t, std::vector<int> >
MinMatching::Run(int n_arg, int m_arg, std::vector<int> u_arg, std::vector<int> v_arg, std::vector<int> w_arg) {
    n_ = n_arg;
    m_ = m_arg;
    for (int u = 0; u <= n_; ++u)
        for (int v = 0; v <= n_; ++v)
            g_[u][v] = Edge(u, v, INF);
    for (int i = 0, u, v, w; i < m_; ++i) {
        u = u_arg[i];
        v = v_arg[i];
        w = w_arg[i];
        g_[u][v].w = g_[v][u].w = INF - w;
    }
    std::pair<int64_t, std::vector<int>> ans(0, std::vector<int>(n_, 0));
    auto res = weight_blossom();
    if (res.first / res.second == INF) {
        ans.first = -1;
        return ans;
    }
    ans.first = res.second * INF - res.first;
    for (int i = 1; i <= n_; ++i) {
        ans.second[i - 1] = match_[i];
    }
    return ans;
}

