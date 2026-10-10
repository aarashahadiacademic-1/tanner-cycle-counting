// Single-processor interleaving: fixed equal elementary-step quanta.
// Compile: g++ -std=c++17 -O3 interleaved.cpp -o interleaved
// Usage: ./interleaved graphs/A128c.edges 6 0.08 10 20261010 256
#define main old_main
#include "counter.cpp"
#undef main
static double mendo(int r){double x=r-1.;return exp(log(2.)+x*log(x)-x-lgamma((double)r));}
static int target(double eps){if(!(eps>0))throw runtime_error("bad epsilon");int l=2,h=2;while(mendo(h)>eps&&h<100000000)h*=2;if(mendo(h)>eps)throw runtime_error("r too large");while(l<h){int m=l+(h-l)/2;if(mendo(m)<=eps)h=m;else l=m+1;}return l;}
struct ExactStepper{
 const G& graph;int g,a,R;vector<int>tag,dep,par,br,que;vector<unsigned char>used,blocked;vector<ull>C;int epoch=0,v=0,qh=0,qt=0,qi=0,x=-1,z=-1,idx=0,stage=0,chain=-1,check=-1;bool done=false;
 struct Frame{int z,l,idx;bool entered;};vector<Frame>stack;
 ExactStepper(const G&gr,int gg):graph(gr),g(gg),a(gg/2-1),R(2),tag(gr.M),dep(gr.M),par(gr.M),br(gr.M),que(gr.M),used(gr.M),blocked(gr.M),C(gg+1){stack.reserve(gg+3);}
 void step(){if(done)return;
 switch(stage){
 case 0:
   if(v==graph.n){assert((2*C[g])%g==0);C[g]=2*C[g]/g;done=true;return;}
   ++epoch;qh=0;qt=1;que[0]=v;tag[v]=epoch;dep[v]=0;par[v]=-1;stage=1;return;
 case 1:
   if(qh>=qt){qi=0;stage=3;return;}z=que[qh++];idx=0;if(dep[z]==a)return;stage=2;return;
 case 2:
   if(idx>=(int)graph.a[z].size()){stage=1;return;}
   {int w=graph.a[z][idx++];if(tag[w]!=epoch){tag[w]=epoch;dep[w]=dep[z]+1;par[w]=z;br[w]=(z==v?w:br[z]);que[qt++]=w;}}return;
 case 3:
   if(qi==qt){++v;stage=0;return;}x=que[qi++];if(dep[x]!=a)return;chain=par[x];stage=4;return;
 case 4:
   if(chain==-1){used[x]=1;stack.clear();stack.push_back({x,0,0,false});stage=5;return;}
   blocked[chain]=1;chain=par[chain];return;
 case 5:
   if(stack.empty()){used[x]=0;chain=par[x];stage=8;return;}
   {auto &f=stack.back();if(!f.entered){f.entered=true;
     if(f.l>=2&&!(f.l&1)&&tag[f.z]==epoch&&dep[f.z]==a&&x<f.z&&br[x]!=br[f.z]){check=par[f.z];stage=6;return;}}
    if(f.l==R||f.idx==(int)graph.a[f.z].size()){int old=f.z;stack.pop_back();if(!stack.empty())used[old]=0;return;}
    int w=graph.a[f.z][f.idx++];if(!used[w]&&!blocked[w]){used[w]=1;stack.push_back({w,f.l+1,0,false});}return;}
 case 6:
   if(check==-1){++C[g];stage=5;return;}
   if(used[check]){stage=5;return;}check=par[check];return;
 case 8:
   if(chain==-1){stage=3;return;}blocked[chain]=0;chain=par[chain];return;
 }}
};
struct SampleStepper{
 const G&graph;int g,r,A=0;unsigned long long S=0;int origin=-1,z=-1,back=-1,depth=0;mt19937_64 rng;
 SampleStepper(const G&gr,int gg,int rr,uint64_t seed):graph(gr),g(gg),r(rr),rng(seed){}
 void step(){if(A>=r)return;
  if(origin<0){origin=uniform_int_distribution<int>(0,graph.n-1)(rng);z=origin;back=-1;depth=0;return;}
  int d=graph.out[z].size(),j=uniform_int_distribution<int>(0,d-1-(back!=-1))(rng);
  if(back!=-1&&j>=back)++j;int e=graph.out[z][j];z=graph.to[e];back=graph.pos[e^1];
  if(++depth==g){++S;A+=(z==origin);origin=-1;}
 }
};
struct Result{double val,ms;unsigned long long trials;bool random;};
static Result run(const G&graph,int g,int r,int Q,double B,uint64_t seed){
 ExactStepper E(graph,g);SampleStepper R(graph,g,r,seed);auto st=chrono::steady_clock::now();
 for(;;){
  for(int i=0;i<Q&&!E.done;i++)E.step();
  if(E.done)return {(double)E.C[g],millis(st),R.S,false};
  for(int i=0;i<Q&&R.A<r;i++)R.step();
  if(R.A==r)return {((double)graph.n*B/g)*(r-1)/(R.S-1),millis(st),R.S,true};
 }
}
int main(int argc,char**argv){
 if(argc!=7){cerr<<"usage: interleaved graph.edges girth epsilon reps seed quantum\n";return 2;}
 G graph(argv[1]);int g=stoi(argv[2]);double eps=stod(argv[3]);int reps=stoi(argv[4]);auto seed=stoull(argv[5]);int Q=stoi(argv[6]);int r=target(eps);
 double B=Bvalue(graph,g);ull N=exact(graph,g,g)[g];vector<double>times;
 for(int i=0;i<7;i++){auto st=chrono::steady_clock::now();auto c=exact(graph,g,g);assert(c[g]==N);times.push_back(millis(st));}
 sort(times.begin(),times.end());double total=0,err=0,avgS=0;int Rwins=0;
 for(int j=0;j<reps;j++){auto out=run(graph,g,r,Q,B,seed+j*0x9e3779b97f4a7c15ULL);total+=out.ms;avgS+=out.trials;Rwins+=out.random;
  if(N)err+=abs(out.val-(double)N)/N;else assert(out.val==0);if(!out.random)assert((ull)out.val==N);}
 cout<<setprecision(12)<<"{\"n\":"<<graph.n<<",\"g\":"<<g<<",\"Ng\":"<<N<<",\"eps\":"<<eps<<",\"r\":"<<r<<",\"quantum\":"<<Q<<",\"exact_ms\":"<<times[3]<<",\"hybrid_ms\":"<<total/reps<<",\"MARE_percent\":"<<100*err/reps<<",\"random_wins\":"<<Rwins<<",\"exact_wins\":"<<reps-Rwins<<",\"mean_trials\":"<<avgS/reps<<"}\n";
}