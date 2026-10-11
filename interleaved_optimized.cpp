// Resumable exact girth cycle counter: each charge is one adjacency inspection.
// Bookkeeping (queue, stack, marking, root change) runs within the same tight loop.
// Sampler: eight work credits per non-backtracking neighbor draw/traversal (empirical calibration).
// Work credits are a practical cost model, not equal CPU instructions.
// Clock comparison uses same optimized exact engine as hybrid (plus recursive reference).
#define main legacy_main
#include "counter.cpp"
#undef main
static double mendo(int r){double x=r-1.;return exp(log(2.)+x*log(x)-x-lgamma(double(r)));}
static int target(double eps){for(int r=2;r<10000000;r++)if(mendo(r)<=eps)return r;throw runtime_error("eps too small");}
struct EState {
 const G& Gg; int g,a,epoch=0,v=0,qh=0,qt=0,qi=0,x=-1,z=-1,idx=0,jdx=0,stage=0;
 bool done=false;
 vector<int>tag,dep,par,br,que;ull raw=0,probes=0;
 EState(const G&gr,int gg):Gg(gr),g(gg),a(gg/2-1),tag(gr.M),dep(gr.M),par(gr.M),br(gr.M),que(gr.M){}
 void advance(uint64_t budget){uint64_t charges=0;while(!done){
  switch(stage){
   case 0:
    if(v==Gg.n){done=true;return;}
    ++epoch;qh=0;qt=1;que[0]=v;tag[v]=epoch;dep[v]=0;par[v]=-1;stage=1;break;
   case 1:
    if(qh==qt){qi=0;stage=3;break;}
    z=que[qh++];idx=0;if(dep[z]!=a)stage=2;break;
   case 2:{
    const auto&adj=Gg.a[z];if(idx==(int)adj.size()){stage=1;break;}
    if(charges>=budget)return;
    int w=adj[idx++];++charges;++probes;
    if(tag[w]!=epoch){tag[w]=epoch;dep[w]=dep[z]+1;par[w]=z;br[w]=(z==v?w:br[z]);que[qt++]=w;}
    break;}
   case 3:
    if(qi==qt){++v;stage=0;break;}
    x=que[qi++];if(dep[x]!=a)break;
    idx=0;stage=4;break;
   case 4:{
    const auto&adj=Gg.a[x];if(idx==(int)adj.size()){stage=3;break;}
    if(charges>=budget)return;
    z=adj[idx++];++charges;++probes;
    if(z!=par[x]){jdx=0;stage=5;}
    break;}
   case 5:{
    const auto&adj=Gg.a[z];if(jdx==(int)adj.size()){stage=4;break;}
    if(charges>=budget)return;
    int y=adj[jdx++];++charges;++probes;
    if(tag[y]==epoch && dep[y]==a && x<y && br[x]!=br[y] && z!=par[y])++raw;
    break;}
  }
 }}
 ull result()const {assert(done);assert((2*raw)%g==0);return (2*raw)/g;}
};
struct RState{
 const G& gr;int g,r,A=0,origin=-1,z=-1,back=-1,depth=0;uint64_t trials=0,probes=0;mt19937_64 rng;
 RState(const G&gr_,int gg,int rr,uint64_t seed):gr(gr_),g(gg),r(rr),rng(seed){}
 void advance(uint64_t budget){uint64_t usedbudget=0;while(A<r&&usedbudget+8<=budget){
  if(origin<0){origin=uniform_int_distribution<int>(0,gr.n-1)(rng);z=origin;back=-1;depth=0;}
  int d=gr.out[z].size();int j=uniform_int_distribution<int>(0,d-1-(back!=-1))(rng);if(back!=-1&&j>=back)++j;
  int e=gr.out[z][j];z=gr.to[e];back=gr.pos[e^1];++probes;usedbudget+=8;
  if(++depth==g){++trials;A+=(z==origin);origin=-1;}
 }}
};
struct Out {double value,ms;uint64_t ep,rp,trials;bool random;};
static Out race(const G& gr,int g,int r,uint64_t Q,double B,uint64_t seed){
 EState e(gr,g);RState s(gr,g,r,seed);auto start=chrono::steady_clock::now();
 while(true){e.advance(Q);if(e.done)return {double(e.result()),millis(start),e.probes,s.probes,s.trials,false};
 s.advance(Q);if(s.A==r)return {(gr.n*B/g)*(r-1)/double(s.trials-1),millis(start),e.probes,s.probes,s.trials,true};}
}
int main(int argc,char**argv){if(argc!=7){cerr<<"graph g eps reps seed Q\n";return 2;}
 G gr(argv[1]);int g=stoi(argv[2]), reps=stoi(argv[4]);double eps=stod(argv[3]),B=Bvalue(gr,g);uint64_t seed=stoull(argv[5]),Q=stoull(argv[6]);int r=target(eps);
 ull Ng=exact(gr,g,g)[g];vector<double> exacttimes,ktimes,resumetimes;ull exactprobes=0;
 for(int i=0;i<7;i++){
  auto st=chrono::steady_clock::now();auto got=exact(gr,g,g)[g];if(got!=Ng)throw runtime_error("exact mismatch");exacttimes.push_back(millis(st));
  st=chrono::steady_clock::now();EState e(gr,g);e.advance(UINT64_MAX);if(e.result()!=Ng)throw runtime_error("resumable mismatch");resumetimes.push_back(millis(st));exactprobes=e.probes;
  st=chrono::steady_clock::now();auto kbout=kb(gr,g,g)[g];if(kbout!=Ng)throw runtime_error("KB mismatch");ktimes.push_back(millis(st));
 }
 sort(exacttimes.begin(),exacttimes.end());sort(ktimes.begin(),ktimes.end());sort(resumetimes.begin(),resumetimes.end());
 double total=0,err=0,meanep=0,meanrp=0,means=0,maxopsratio=0;int rwins=0;
 for(int j=0;j<reps;j++){Out o=race(gr,g,r,Q,B,seed+j*0x9e3779b97f4a7c15ULL);
  if(!o.random&&(ull)o.value!=Ng)throw runtime_error("hybrid exact mismatch");
  total+=o.ms;err+=abs(o.value-double(Ng))/Ng;meanep+=o.ep;meanrp+=o.rp;means+=o.trials;rwins+=o.random;
  maxopsratio=max(maxopsratio,double(o.ep+8*o.rp)/double(exactprobes));
 }
 cout<<setprecision(12)<<"{\"Ng\":"<<Ng<<",\"g\":"<<g<<",\"n\":"<<gr.n<<",\"r\":"<<r<<",\"Q\":"<<Q
  <<",\"kb_ms\":"<<ktimes[3]<<",\"recursive_exact_ms\":"<<exacttimes[3]
  <<",\"resumable_exact_ms\":"<<resumetimes[3]<<",\"hybrid_ms\":"<<total/reps
  <<",\"MARE_percent\":"<<100*err/reps<<",\"random_wins\":"<<rwins<<",\"exact_wins\":"<<reps-rwins
  <<",\"exact_probes\":"<<exactprobes<<",\"mean_hybrid_exact_probes\":"<<meanep/reps
  <<",\"mean_hybrid_sampling_probes\":"<<meanrp/reps<<",\"mean_samples\":"<<means/reps
  <<",\"max_weighted_work_ratio\":"<<maxopsratio<<"}\n";
}
