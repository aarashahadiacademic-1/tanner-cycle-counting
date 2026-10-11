#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

struct Graph {
    int n, m, M;
    vector<vector<int>> out;
    vector<int> from, to, rev;
    explicit Graph(const string& path) {
        ifstream in(path);
        if (!in) throw runtime_error("cannot open graph: " + path);
        int E; in >> n >> m >> E; M = n + m; out.resize(M);
        from.reserve(2*E); to.reserve(2*E); rev.reserve(2*E);
        int u, w;
        while (in >> u >> w) {
            w += n;
            int a = (int)from.size(), b = a + 1;
            from.push_back(u); to.push_back(w); rev.push_back(b);
            from.push_back(w); to.push_back(u); rev.push_back(a);
            out[u].push_back(a); out[w].push_back(b);
        }
        if ((int)from.size() != 2*E) throw runtime_error("edge count mismatch");
    }
};

// Li--Lin--Abdel-Ghaffar IMPA path-message implementation:
// each root-edge label is carried as one message coordinate; at each
// meeting vertex, pairs of paths with distinct root edges are counted.
// Earlier variable roots are deactivated, so each cycle is counted only
// from its minimum-index variable vertex, as in their sequential IMPA.
vector<ull> impa(const Graph& G, int g, int T) {
    if (T < g || (T&1) || T > 2*g-2) throw runtime_error("IMPA target must be even and g <= T <= 2g-2");
    int maxdeg=0;
    for (int u=0;u<G.n;u++) maxdeg=max(maxdeg,(int)G.out[u].size());
    const int D=maxdeg, A=(int)G.from.size();
    vector<ull> cur((size_t)A*D), nxt((size_t)A*D);
    vector<int> active, next_active;
    vector<unsigned char> touched(A);
    vector<vector<int>> incoming(G.M);
    vector<ull> total(T+1);

    for (int root=0; root<G.n; ++root) {
        active.clear();
        for (int k=0;k<(int)G.out[root].size();++k) {
            int e=G.out[root][k];
            cur[(size_t)e*D+k]=1;
            active.push_back(e);
        }
        // A path of p edges from the root is paired with another such path
        // at its endpoint, giving a 2p-cycle.
        for (int p=2;p<=T/2;++p) {
            next_active.clear();
            for (int e:active) {
                int v=G.to[e];
                size_t base=(size_t)e*D;
                for (int f:G.out[v]) if (f!=G.rev[e]) {
                    // IMPA removes all earlier variable nodes after each
                    // root. This suppresses duplicate cycle counts.
                    if (G.to[f] < G.n && G.to[f] <= root) continue;
                    size_t dst=(size_t)f*D;
                    if (!touched[f]) { touched[f]=1; next_active.push_back(f); }
                    for (int k=0;k<D;++k) nxt[dst+k]+=cur[base+k];
                }
            }
            for (int e:active) {
                size_t base=(size_t)e*D;
                for (int k=0;k<D;++k) cur[base+k]=0;
            }
            // The new messages are paths of length p, oriented into endpoints.
            for (int e:next_active) incoming[G.to[e]].push_back(e);
            for (int v=0;v<G.M;++v) if (!incoming[v].empty()) {
                const auto& es=incoming[v];
                for (size_t a=0;a<es.size();++a) {
                    size_t ba=(size_t)es[a]*D;
                    for (size_t b=a+1;b<es.size();++b) {
                        size_t bb=(size_t)es[b]*D;
                        ull ways=0;
                        for (int k=0;k<D;++k) {
                            ull x=nxt[ba+k];
                            if (!x) continue;
                            for (int l=0;l<D;++l) if (l!=k)
                                ways += x*nxt[bb+l];
                        }
                        total[2*p]+=ways;
                    }
                }
                incoming[v].clear();
            }
            // Clear the old message buffer before swapping; it becomes nxt.
            for (int e:next_active) touched[e]=0;
            cur.swap(nxt);
            active.swap(next_active);
        }
        // Clear the last layer so the next root starts with a clean buffer.
        for (int e:active) {
            size_t base=(size_t)e*D;
            for (int k=0;k<D;++k) cur[base+k]=0;
            touched[e]=0;
        }
    }
    // Each cycle survives only for its smallest-index variable root.
    return total;
}

int main(int argc,char**argv) {
    if(argc!=4){cerr<<"usage: impa_count GRAPH G T\n";return 2;}
    try {
        Graph G(argv[1]); int g=stoi(argv[2]), T=stoi(argv[3]);
        auto t0=chrono::steady_clock::now();
        auto C=impa(G,g,T);
        double ms=chrono::duration<double,milli>(chrono::steady_clock::now()-t0).count();
        cout<<"{\"counts\":{";
        bool first=true;
        for(int t=g;t<=T;t+=2){if(!first)cout<<',';first=false;cout<<'\"'<<t<<"\":"<<C[t];}
        cout<<"},\"impa_ms\":"<<setprecision(12)<<ms<<"}\n";
    } catch(const exception& e){cerr<<e.what()<<'\n';return 1;}
}
