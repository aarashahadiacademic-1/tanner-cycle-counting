#include <bits/stdc++.h>
using namespace std;using ull=unsigned long long;
struct G{int n,m,M;vector<vector<int>>a;vector<int>fr,to,pos;vector<vector<int>>out;
 G(string path){ifstream f(path);int E,u,w;f>>n>>m>>E;M=n+m;a.resize(M);out.resize(M);while(f>>u>>w){w+=n;a[u].push_back(w);a[w].push_back(u);int j=fr.size();pos.push_back(out[u].size());pos.push_back(out[w].size());fr.push_back(u);to.push_back(w);fr.push_back(w);to.push_back(u);out[u].push_back(j);out[w].push_back(j+1);} }
};
vector<ull> exact(const G&G,int g,int T){
 int M=G.M,a=g/2-1,R=T-g+2;vector<ull>C(T+1);vector<int>tag(M),dep(M),par(M),br(M),que(M);vector<char>used(M),blocked(M);int epoch=0;
 for(int v=0;v<G.n;v++){
  ++epoch;int qh=0,qt=0;que[qt++]=v;tag[v]=epoch;dep[v]=0;par[v]=-1;
  while(qh<qt){int z=que[qh++];if(dep[z]==a)continue;for(int w:G.a[z])if(tag[w]!=epoch){tag[w]=epoch;dep[w]=dep[z]+1;par[w]=z;br[w]=(z==v?w:br[z]);que[qt++]=w;}}
  for(int qi=0;qi<qt;qi++){int x=que[qi];if(dep[x]!=a)continue;
   for(int z=par[x];z!=-1;z=par[z]){blocked[z]=1;}used[x]=1;
   auto dfs=[&](auto&&self,int z,int l)->void{
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
// Independent canonical simple-cycle DFS: root is minimum vertex, orientation divided by two.
vector<ull> reference(const G&G,int T){vector<ull>C(T+1);vector<char>used(G.M);
 for(int s=0;s<G.M;s++){used[s]=1;auto dfs=[&](auto&&self,int z,int l)->void{for(int w:G.a[z]){if(w==s&&l>=3)++C[l+1];else if(l+1<T&&w>s&&!used[w]){used[w]=1;self(self,w,l+1);used[w]=0;}}};dfs(dfs,s,0);used[s]=0;}
 for(auto&x:C){assert(x%2==0);x/=2;}return C;}
ull nbref(const G&G,int g,int Z=1){ull count=0;for(int s=0;s<G.n;s+=Z){auto dfs=[&](auto&&self,int z,int prev,int l)->void{if(l==g){count+=z==s;return;}for(int w:G.a[z])if(w!=prev)self(self,w,z,l+1);};dfs(dfs,s,-1,0);}assert((count*Z)%g==0);return count*Z/g;}
int girth(const G&G,int limit,int Z=1){vector<int>tag(G.M),d(G.M),p(G.M),q(G.M);int epoch=0,best=limit+2;for(int s=0;s<G.M;s+=Z){++epoch;int h=0,t=0;q[t++]=s;tag[s]=epoch;d[s]=0;p[s]=-1;while(h<t){int v=q[h++];if(2*d[v]>=limit)continue;for(int w:G.a[v]){if(tag[w]!=epoch){tag[w]=epoch;d[w]=d[v]+1;p[w]=v;q[t++]=w;}else if(p[v]!=w&&p[w]!=v)best=min(best,d[v]+d[w]+1);}}}return best;}
// Karimi--Banihashemi exponent-vector message passing: deactivate earlier roots;
// propagate only nonzero messages; form node sums once, subtract reverse-edge input.
vector<ull> kb(const G&G,int g,int T){assert(T<2*g);int D=0;for(int u=0;u<G.n;u++)D=max(D,(int)G.a[u].size());int E=G.fr.size();vector<ull>C(T+1),cur(E*D),nxt(E*D),sum(G.M*D);vector<int>active,na,nodes;vector<char>touch(G.M);
 for(int root=0;root<G.n;root++){
  active.clear();for(int j=0;j<(int)G.out[root].size();j++){int e=G.out[root][j];cur[e*D+j]=1;active.push_back(e);}
  for(int l=1;l<T;l++){
   nodes.clear();for(int e:active){int w=G.to[e];if(!touch[w]){touch[w]=1;nodes.push_back(w);}for(int j=0;j<D;j++)sum[w*D+j]+=cur[e*D+j];}
   na.clear();for(int w:nodes){if(w==root)continue;for(int e:G.out[w]){int z=G.to[e];if(z<G.n&&z<root)continue;bool nonzero=false;for(int j=0;j<D;j++){ull val=sum[w*D+j]-cur[(e^1)*D+j];nxt[e*D+j]=val;nonzero|=val!=0;}if(nonzero)na.push_back(e);}}
   if((l+1)%2==0&&l+1>=g){for(int j=0;j<(int)G.out[root].size();j++){int e=G.out[root][j]^1;for(int k=0;k<D;k++)if(k!=j)C[l+1]+=nxt[e*D+k];}}
   for(int e:active)for(int j=0;j<D;j++)cur[e*D+j]=0;
   for(int w:nodes){touch[w]=0;for(int j=0;j<D;j++)sum[w*D+j]=0;}
   cur.swap(nxt);active.swap(na);
  }
  for(int e:active)for(int j=0;j<D;j++)cur[e*D+j]=0;
 }
 for(auto&x:C){assert(x%2==0);x/=2;}return C;
}
bool trial(const G&G,int g,mt19937_64&rng){int s=uniform_int_distribution<int>(0,G.n-1)(rng),z=s,back=-1;for(int i=0;i<g;i++){int d=G.out[z].size();int j=uniform_int_distribution<int>(0,d-1-(back!=-1))(rng);if(back!=-1&&j>=back)++j;int e=G.out[z][j];z=G.to[e];back=G.pos[e^1];}return z==s;}
double Bvalue(const G&G,int g){return G.a[0].size()*pow(G.a[0].size()-1,g/2-1)*pow(G.a[G.n].size()-1,g/2);}
double millis(chrono::steady_clock::time_point t){return chrono::duration<double,milli>(chrono::steady_clock::now()-t).count();}
int main(int argc,char**argv){if(argc<5)return 1;G G(argv[1]);int g=stoi(argv[2]),T=stoi(argv[3]);string mode=argv[4];int Z=mode=="qc"?stoi(argv[5]):1;if(Z>1){assert(G.n%Z==0&&G.m%Z==0);for(int u=0;u<G.n;u++){auto a=G.a[u];for(int &w:a)w=G.n+((w-G.n)/Z)*Z+((w-G.n+1)%Z);sort(a.begin(),a.end());auto b=G.a[(u/Z)*Z+(u+1)%Z];sort(b.begin(),b.end());assert(a==b);}}assert(girth(G,g,Z)==g);vector<char>seen(G.M);queue<int>q;q.push(0);seen[0]=1;while(!q.empty()){int v=q.front();q.pop();for(int w:G.a[v])if(!seen[w]){seen[w]=1;q.push(w);}}assert(count(seen.begin(),seen.end(),1)==G.M);
 auto counts=exact(G,g,T);cout<<"{\"g\":"<<g<<",\"T\":"<<T<<",\"counts\":{";bool first=true;for(int t=g;t<=T;t+=2){if(!first)cout<<",";first=false;cout<<"\""<<t<<"\":"<<counts[t];}cout<<"}";
 if(mode=="validate"){auto ref=reference(G,T);assert(ref==counts);cout<<",\"canonical_dfs_agrees\":true";}
 else if(mode=="cactus"){ull expected=stoull(argv[5]);for(int t=g;t<=T;t+=2)assert(counts[t]==expected);}
 else {assert(nbref(G,g,Z)==counts[g]);if(mode!="qc"&&T<2*g){assert(kb(G,g,T)==counts);cout<<",\"kb_agrees\":true";}cout<<",\"nb_agrees\":true";}
 vector<double>times;for(int i=0;i<7;i++){auto st=chrono::steady_clock::now();assert(exact(G,g,T)==counts);times.push_back(millis(st));}sort(times.begin(),times.end());cout<<",\"exact_ms\":"<<setprecision(10)<<times[3];
 if(mode=="bench"||mode=="qc"){
  if(mode=="bench"){times.clear();for(int i=0;i<7;i++){auto st=chrono::steady_clock::now();assert(kb(G,g,T)==counts);times.push_back(millis(st));}sort(times.begin(),times.end());cout<<",\"kb_ms\":"<<times[3];}
  if(T==g){double B=Bvalue(G,g),totalms=0,err=0,avg=0;int routes[3]={};mt19937_64 rng(20261009+G.n);for(int run=0;run<100;run++){auto st=chrono::steady_clock::now();int A0=0;for(int i=0;i<G.n;i++)A0+=trial(G,g,rng);double val;int A=0,S=0;if(A0<=5){val=exact(G,g,g)[g];++routes[0];}else{while(A<100&&S<10*G.n){++S;A+=trial(G,g,rng);}if(A<100){val=exact(G,g,g)[g];++routes[1];}else{val=G.n*B/g*99/(S-1);++routes[2];}}totalms+=millis(st);avg+=val;err+=abs(val-counts[g])/counts[g];}cout<<",\"hybrid_ms\":"<<totalms/100<<",\"mean_estimate\":"<<avg/100<<",\"MARE_percent\":"<<err<<",\"lambda\":"<<g*counts[g]/B<<",\"routes\":["<<routes[0]<<","<<routes[1]<<","<<routes[2]<<"]";}
 }
 cout<<"}\n";
}
