// Experimental single-stage hybrid benchmark. Requires counter.cpp in same directory.
// Compile: g++ -O3 -std=c++17 hybrid_budget.cpp -o hybrid_budget
// Usage: ./hybrid_budget graphs/A128c.edges 6 0.08 0.005 100 20261010
#define main original_counter_main
#include "counter.cpp"
#undef main
static double bound(int r) {
    double k=r-1.;
    return exp(log(2.)+k*log(k)-k-lgamma(r));
}
static int stopping_target(double eps) {
    if(!(eps>0)) throw runtime_error("eps must be positive");
    int lo=2,hi=2;
    while(bound(hi)>eps && hi<100000000) hi*=2;
    if(bound(hi)>eps) throw runtime_error("target too large");
    while(lo<hi){int mid=lo+(hi-lo)/2;if(bound(mid)<=eps)hi=mid;else lo=mid+1;}
    return lo;
}
int main(int argc,char**argv){
    if(argc!=7){cerr<<"usage: hybrid_budget graph.edges girth eps c repetitions seed\n";return 2;}
    G graph(argv[1]); int g=stoi(argv[2]); double eps=stod(argv[3]),c=stod(argv[4]);
    int reps=stoi(argv[5]); mt19937_64 rng(stoull(argv[6]));
    int r=stopping_target(eps);
    int dv=graph.a[0].size(),dc=graph.a[graph.n].size();
    int L=g/2+1;
    double omega=dv*pow(dc-1,L/2)*pow(dv-1,(L-1)/2);
    unsigned long long K=(unsigned long long)floor(c*graph.n*omega);
    auto reference=exact(graph,g,g)[g]; double B=Bvalue(graph,g);
    double total=0,absolute_relative=0,mean_trials=0;int randomized=0;
    for(int j=0;j<reps;j++){
        auto start=chrono::steady_clock::now();
        unsigned long long s=0;int A=0;double estimate;
        if(K >= (unsigned long long)r)
            while(s<K && A<r){++s; A+=trial(graph,g,rng);}
        if(A==r){++randomized;estimate=(double)graph.n*B/g*(r-1)/(s-1);}
        else estimate=exact(graph,g,g)[g];
        total+=millis(start);mean_trials+=s;
        if(reference) absolute_relative+=abs(estimate-reference)/reference;
    }
    vector<double> baseline;
    for(int j=0;j<7;j++){auto start=chrono::steady_clock::now();
        auto cnt=exact(graph,g,g);baseline.push_back(millis(start));}
    sort(baseline.begin(),baseline.end());
    cout<<setprecision(12)<<"{\"n\":"<<graph.n<<",\"g\":"<<g<<
       ",\"r\":"<<r<<",\"c\":"<<c<<",\"K\":"<<K<<
       ",\"exact_ms\":"<<baseline[3]<<
       ",\"hybrid_ms\":"<<total/reps<<
       ",\"MARE_percent\":"<<100*absolute_relative/reps<<
       ",\"randomized\":"<<randomized<<
       ",\"exact_fallback\":"<<reps-randomized<<
       ",\"mean_trials\":"<<mean_trials/reps<<"}\n";
}