// Two-thread exact-versus-inverse-binomial race (C++17).
// g++ -std=c++17 -O3 -pthread race_parallel.cpp -o race_parallel
// ./race_parallel graphs/A128c.edges 6 0.08 100 20261010
#define main original_counter_main
#include "counter.cpp"
#undef main

vector<ull> exact_cancellable(const G&G,int g,int T,atomic<bool>&cancel){
 int M=G.M,a=g/2-1,R=T-g+2;vector<ull>C(T+1);vector<int>tag(M),dep(M),par(M),br(M),que(M);vector<char>used(M),blocked(M);int epoch=0;
 for(int v=0;v<G.n;v++){if(cancel.load(memory_order_relaxed))return {};
  ++epoch;int qh=0,qt=0;que[qt++]=v;tag[v]=epoch;dep[v]=0;par[v]=-1;
  while(qh<qt){int z=que[qh++];if(dep[z]==a)continue;for(int w:G.a[z])if(tag[w]!=epoch){tag[w]=epoch;dep[w]=dep[z]+1;par[w]=z;br[w]=(z==v?w:br[z]);que[qt++]=w;}}
  for(int qi=0;qi<qt;qi++){if(cancel.load(memory_order_relaxed))return {};
   int x=que[qi];if(dep[x]!=a)continue;
   for(int z=par[x];z!=-1;z=par[z])blocked[z]=1;used[x]=1;
   auto dfs=[&](auto&&self,int z,int l)->void{
    if(cancel.load(memory_order_relaxed))return;
    if(l>=2&&!(l&1)&&tag[z]==epoch&&dep[z]==a&&x<z&&br[x]!=br[z]){
     bool ok=true;for(int w=par[z];w!=-1;w=par[w])if(used[w]){ok=false;break;}
     if(ok)++C[g-2+l];
    }
    if(l==R)return;
    for(int w:G.a[z])if(!used[w]&&!blocked[w]){used[w]=1;self(self,w,l+1);used[w]=0;}
   };dfs(dfs,x,0);used[x]=0;for(int z=par[x];z!=-1;z=par[z])blocked[z]=0;
  }
 }
 for(int t=g;t<=T;t+=2){assert((2*C[t])%t==0);C[t]=2*C[t]/t;}return C;
}
static double mendo_bound(int r){
 double x=r-1.;return exp(log(2.)+x*log(x)-x-lgamma((double)r));
}
static int stopping_target(double eps){
 if(!(eps>0))throw runtime_error("epsilon must be positive");
 int lo=2,hi=2;
 while(mendo_bound(hi)>eps&&hi<10000000)hi*=2;
 if(mendo_bound(hi)>eps)throw runtime_error("stopping target too large");
 while(lo<hi){int mid=lo+(hi-lo)/2;
  if(mendo_bound(mid)<=eps)hi=mid;else lo=mid+1;}
 return lo;
}
struct RaceResult{double value,ms;bool random;long long samples;};
static RaceResult race(const G&graph,int g,int r,double B,uint64_t seed){
 atomic<bool> cancel{false};atomic<int> winner{0};ull exact_count=0;
 auto st=chrono::steady_clock::now();
 thread worker([&]{
  auto c=exact_cancellable(graph,g,g,cancel);
  if(!c.empty()){exact_count=c[g];int zero=0;
   if(winner.compare_exchange_strong(zero,1))cancel.store(true,memory_order_relaxed);}
 });
 mt19937_64 rng(seed);int closures=0;long long s=0;double estimate=0;
 while(winner.load(memory_order_relaxed)==0&&closures<r){
  closures+=trial(graph,g,rng);++s;
 }
 if(closures==r){
  estimate=(double)graph.n*B/g*(r-1)/(s-1);
  int zero=0;
  if(winner.compare_exchange_strong(zero,2))cancel.store(true,memory_order_relaxed);
 }
 double elapsed=millis(st);cancel.store(true,memory_order_relaxed);worker.join();
 int win=winner.load();
 return {win==2?estimate:(double)exact_count,elapsed,win==2,s};
}
int main(int argc,char**argv){
 if(argc!=6){cerr<<"usage: race_parallel graph.edges girth epsilon repetitions seed\n";return 2;}
 G graph(argv[1]);int g=stoi(argv[2]);double eps=stod(argv[3]);
 int reps=stoi(argv[4]);uint64_t seed=stoull(argv[5]);
 int r=stopping_target(eps);double B=Bvalue(graph,g);
 vector<double> base;ull N=0;
 for(int i=0;i<7;i++){auto t=chrono::steady_clock::now();
  auto c=exact(graph,g,g);base.push_back(millis(t));N=c[g];}
 sort(base.begin(),base.end());
 double total=0,err=0,mean_trials=0;int wins=0;
 for(int i=0;i<reps;i++){
  auto out=race(graph,g,r,B,seed+(uint64_t)i*0x9e3779b97f4a7c15ULL);
  total+=out.ms;mean_trials+=out.samples;wins+=out.random;
  if(N)err+=abs(out.value-(double)N)/(double)N;
  else if(out.value!=0)throw runtime_error("nonzero estimate for zero");
 }
 cout<<setprecision(11)<<"{\"n\":"<<graph.n<<",\"dv\":"<<graph.a[0].size()
  <<",\"dc\":"<<graph.a[graph.n].size()<<",\"g\":"<<g<<",\"N_g\":"<<N
  <<",\"epsilon\":"<<eps<<",\"r\":"<<r<<",\"exact_ms\":"<<base[3]
  <<",\"race_ms\":"<<total/reps<<",\"MARE_percent\":"<<100*err/reps
  <<",\"randomized_runs\":"<<wins<<",\"exact_runs\":"<<reps-wins
  <<",\"mean_trials\":"<<mean_trials/reps<<"}\n";
}