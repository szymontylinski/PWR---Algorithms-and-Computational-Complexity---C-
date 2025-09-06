#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <stack>
#include <limits>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;
using namespace chrono;
typedef pair<int,int> pii;
struct Edge { int u, v, w; };

enum { INF = numeric_limits<int>::max() };

// Disjoint Set Union for Kruskal
class DSU {
public:
    DSU(int n): parent(n), rankv(n,0) { iota(parent.begin(), parent.end(), 0); } // parent - dla węzła przechowuje wskaźnik na przedstawiciela jego zbioru, rankv - przybliżenie wysokości drzewa, spłaszczanie ścieżki by przyśpieszyć kolejne zapytanie, scala dwa zbiory
    int find(int x) { return parent[x]==x ? x : parent[x] = find(parent[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rankv[a] < rankv[b]) swap(a, b);
        parent[b] = a;
        if (rankv[a] == rankv[b]) rankv[a]++;
        return true;
    }
private:
    vector<int> parent, rankv;
};
//
class Graph {
public:
    int n; bool directed; //liczba wierzchołków i czy skierowany
    vector<Edge> edges; //spis wszystkich krawędzi (u, v, w)
    vector<vector<pii>> adj; // lista sąsiedztwa (v,waga)
    vector<vector<int>> adjMatrix; // macierz sąsiedztwa (wagi lub INF)

    Graph(int _n=0, bool dir=false): n(_n), directed(dir) { init(); }
    void init() {
        edges.clear();
        adj.assign(n, {});
        adjMatrix.assign(n, vector<int>(n, INF));
        for (int i = 0; i < n; i++) adjMatrix[i][i] = 0;
    }
    void reset(int _n, bool dir) {
        n = _n; directed = dir;
        init();
    }
    void addEdge(int u, int v, int w) {
        edges.push_back({u, v, w});
        adj[u].push_back({v, w});
        adjMatrix[u][v] = w;
        if (!directed) {
            adj[v].push_back({u, w});
            adjMatrix[v][u] = w;
        }
    }
    void printList() const {
        cout << "Adjacency List:\n";
        for (int u = 0; u < n; u++) {
            cout << u << ":";
            for (auto &p : adj[u]) cout << " (" << p.first << "," << p.second << ")";
            cout << "\n";
        }
    }
    void printMatrix() const {
        cout << "Adjacency Matrix:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjMatrix[i][j] == INF) cout << "inf ";
                else cout << adjMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }

    // MST: Prim (list)
    pair<vector<Edge>, int> primListMST(int src = 0) const {
        vector<bool> used(n, false);
        used[src] = true;
        int cnt = 1;
        priority_queue<pair<int, pii>, vector<pair<int, pii>>, greater<>> pq; // inicjalizacja kolejki krawędzi wychodzących ze źródła
        for (auto &p : adj[src]) pq.push({p.second, {src, p.first}});
        vector<Edge> mst;
        int total = 0;
        while (cnt < n && !pq.empty()) {
            auto [w, uv] = pq.top(); pq.pop();
            int u = uv.first, v = uv.second;
            if (used[v]) continue;     // akceptujemy v, zapamiętujemy krawędź (u–v) i dodajemy nowe krawędzie do pq
            used[v] = true; cnt++;
            mst.push_back({u, v, w}); total += w;
            for (auto &p : adj[v]) if (!used[p.first]) pq.push({p.second, {v, p.first}});
        }
        return {mst, total};
    }
    // MST: Prim (matrix)
    pair<vector<Edge>, int> primMatrixMST(int src = 0) const {
        vector<bool> used(n, false); used[src] = true; int cnt = 1;
        vector<Edge> mst; int total = 0;
        while (cnt < n) { // przeszukujemy wszystkie pary (u,v) z used[u]=true, used[v]=false, wybieramy minimalną wagę w macierzy
            int best = INF, bu = -1, bv = -1;
            for (int u = 0; u < n; u++) if (used[u])
                for (int v = 0; v < n; v++) if (!used[v] && adjMatrix[u][v] < best)
                    best = adjMatrix[u][v], bu = u, bv = v;
            if (bv == -1) break;
            used[bv] = true; cnt++;
            mst.push_back({bu, bv, best}); total += best;
        }
        return {mst, total};
    }
    // MST: Kruskal
    pair<vector<Edge>, int> kruskalMST() const {
        vector<Edge> se = edges;
        sort(se.begin(), se.end(), [](const Edge &a, const Edge &b){ return a.w < b.w; });
        DSU d(n);
        vector<Edge> mst; int total = 0;
        for (auto &e : se) {
            if (d.unite(e.u, e.v)) {// jeśli spina dwa różne drzewa
                mst.push_back(e); total += e.w;
                if ((int)mst.size() == n-1) break;
            }
        }
        return {mst, total};
    }
    // SP: Dijkstra (list)
    pair<vector<int>, vector<int>> dijkstraListSP(int src) const {
        vector<int> dist(n, INF), parent(n, -1);
        dist[src] = 0;
        priority_queue<pii, vector<pii>, greater<>> pq;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto &p : adj[u]) {
                int v = p.first, w = p.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w; parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
        return {dist, parent};
    }
    // SP: Dijkstra (matrix)
    pair<vector<int>, vector<int>> dijkstraMatrixSP(int src) const {
        vector<int> dist(n, INF), parent(n, -1);
        vector<bool> used(n, false);
        dist[src] = 0;
        for (int i = 0; i < n; i++) {
            int u=-1, best=INF;
            for (int j=0;j<n;j++) if (!used[j] && dist[j]<best) best=dist[j], u=j;
            if (u==-1) break;
            used[u]=true;
            for (int v=0;v<n;v++) if (!used[v] && adjMatrix[u][v]<INF) {
                int nd = dist[u] + adjMatrix[u][v];
                if (nd < dist[v]) { dist[v]=nd; parent[v]=u; }
            }
        }
        return {dist, parent};
    }
    // SP: Bellman-Ford
    pair<vector<int>, bool> bellmanFordSP(int src) const { // n-1 rund relaksacji wszystkich E krawędzi   kolejna runda: jeśli da się jeszcze zrelaksować => ujemny cykl
        vector<int> dist(n, INF), parent(n, -1);
        dist[src] = 0;
        for (int i=0;i<n-1;i++){
            for (auto &e: edges) {
                if (dist[e.u]<INF && dist[e.u]+e.w<dist[e.v]) {
                    dist[e.v]=dist[e.u]+e.w; parent[e.v]=e.u;
                }
            }
        }
        for (auto &e: edges) if (dist[e.u]<INF && dist[e.u]+e.w<dist[e.v]) return {dist,true};
        return {dist,false};
    }
    // Random graph generator
    static Graph randomGraph(int n,int m,bool dir=false){ // losujemy kolejność wierzchołków verts[]
        Graph g(n,dir); random_device rd; mt19937 gen(rd());
        uniform_int_distribution<> wdist(1,100), vd(0,n-1);
        vector<int> verts(n); iota(verts.begin(), verts.end(), 0); shuffle(verts.begin(), verts.end(), gen);
        if(dir){ // utwórz cykl hamiltona żeby przejść do każdego wierzchołka
            for(int i=0;i<n;i++){int u=verts[i],v=verts[(i+1)%n]; g.addEdge(u,v,wdist(gen));}
            m=max(m,n);
        } else { // drzewa spinające najpierw drzewo rozpinające – pętla od i=1 do n-1
            for(int i=1;i<n;i++){int u=verts[i],v=verts[vd(gen)%i]; g.addEdge(u,v,wdist(gen));}
            m=max(m,n-1);
        }// potem dodajemy pozostałe losowe krawędzie aż do m
        int extra=m-(int)g.edges.size();
        while(extra-->0){int u=vd(gen),v=vd(gen); if(u==v) continue; g.addEdge(u,v,wdist(gen));}
        return g;
    }
};

int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    Graph g;
    while(true){
        cout<<"Mode: load file (f), random (r), tests (t), quit (q): "; char c; cin>>c;
        if(c=='q') break;
        if(c=='t'){
            cout<<"Test for MST (m) or shortest paths (s): "; char tp; cin>>tp;
            vector<int> Vs={100,300,500,700,900,1100,1200}; vector<int> dens={20,60,99}; const int R=50;
            if(tp=='m'){
                cout<<"V\tDens\tPrimL\tPrimM\tKruskal\n";
                for(int V:Vs) for(int d:dens){
                    int maxE=V*(V-1)/2, E=d*maxE/100;
                    double sPL=0,sPM=0,sK=0;
                    for(int i=0;i<R;i++){
                        Graph gt=Graph::randomGraph(V,E,false);
                        auto t1=high_resolution_clock::now(); gt.primListMST(); auto t2=high_resolution_clock::now(); sPL+=duration_cast<duration<double,milli>>(t2-t1).count();
                        t1=high_resolution_clock::now(); gt.primMatrixMST(); t2=high_resolution_clock::now(); sPM+=duration_cast<duration<double,milli>>(t2-t1).count();
                        t1=high_resolution_clock::now(); gt.kruskalMST(); t2=high_resolution_clock::now(); sK+=duration_cast<duration<double,milli>>(t2-t1).count();
                    }
                    cout<<V<<"\t"<<d<<"\t"<<sPL/R<<"\t"<<sPM/R<<"\t"<<sK/R<<"\n";
                }
            } else if(tp=='s'){
                cout<<"V\tDens\tDijL\tDijM\tBellman\n";
                for(int V:Vs) for(int d:dens){
                    int maxE=V*(V-1), E=d*maxE/100;
                    double sDL=0,sDM=0,sBF=0;
                    for(int i=0;i<R;i++){
                        Graph gt=Graph::randomGraph(V,E,true);
                        auto t1=high_resolution_clock::now(); gt.dijkstraListSP(0); auto t2=high_resolution_clock::now(); sDL+=duration_cast<duration<double,milli>>(t2-t1).count();
                        t1=high_resolution_clock::now(); gt.dijkstraMatrixSP(0); t2=high_resolution_clock::now(); sDM+=duration_cast<duration<double,milli>>(t2-t1).count();
                        t1=high_resolution_clock::now(); gt.bellmanFordSP(0); t2=high_resolution_clock::now(); sBF+=duration_cast<duration<double,milli>>(t2-t1).count();
                    }
                    cout<<V<<"\t"<<d<<"\t"<<sDL/R<<"\t"<<sDM/R<<"\t"<<sBF/R<<"\n";
                }
            }
            continue;
        }
        bool directed=false;
        if(c=='f'){
            string fname; cout<<"Filename: "; cin>>fname;
            ifstream in(fname);
            if(!in){ cerr<<"Cannot open file\n"; continue; }
            int m,n; in>>m>>n>>directed;
            g.reset(n,directed);
            for(int i=0,u,v,w;i<m;i++){ in>>u>>v>>w; g.addEdge(u,v,w); }
        } else if(c=='r'){
            int n; double dens; char dch;
            cout<<"Vertices: "; cin>>n;
            cout<<"Directed? (y/n): "; cin>>dch; directed=(dch=='y');
            cout<<"Density %: "; cin>>dens;
            int maxE= directed? n*(n-1) : n*(n-1)/2;
            int m=int(dens/100.0*maxE);
            g=Graph::randomGraph(n,m,directed);
        } else continue;

        g.printList(); g.printMatrix();
        cout<<"Problem: MST (m), shortest paths (s), quit(q): "; char p; cin>>p;
        if(p=='m'){
            auto [m1,w1]=g.primListMST(); cout<<"Prim(list) MST weight="<<w1<<" edges:\n";
            for(auto &e:m1) cout<<e.u<<"-"<<e.v<<"("<<e.w<<") "; cout<<"\n";
            auto [m2,w2]=g.primMatrixMST(); cout<<"Prim(matrix) MST weight="<<w2<<" edges:\n";
            for(auto &e:m2) cout<<e.u<<"-"<<e.v<<"("<<e.w<<") "; cout<<"\n";
            auto [m3,w3]=g.kruskalMST(); cout<<"Kruskal MST weight="<<w3<<" edges:\n";
            for(auto &e:m3) cout<<e.u<<"-"<<e.v<<"("<<e.w<<") "; cout<<"\n";
        } else if(p=='s'){
            int src; cout<<"Source: "; cin>>src;
            auto [d1,par1]=g.dijkstraListSP(src); cout<<"Dijkstra(list) distances:\n";
            for(int i=0;i<g.n;i++) cout<<i<<":"<<(d1[i]==INF?-1:d1[i])<<" "; cout<<"\n";
            auto [d2,par2]=g.dijkstraMatrixSP(src); cout<<"Dijkstra(matrix) distances:\n";
            for(int i=0;i<g.n;i++) cout<<i<<":"<<(d2[i]==INF?-1:d2[i])<<" "; cout<<"\n";
            auto [d3,neg]=g.bellmanFordSP(src); if(neg) cout<<"Negative cycle detected!\n";
            cout<<"Bellman-Ford distances:\n";
            for(int i=0;i<g.n;i++) cout<<i<<":"<<(d3[i]==INF?-1:d3[i])<<" "; cout<<"\n";
        }
    }
    return 0;
}
