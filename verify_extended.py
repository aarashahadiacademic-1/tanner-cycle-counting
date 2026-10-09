import pathlib,json,collections,hashlib
P=pathlib.Path(__file__).resolve().parent
M=json.loads((P/'manifest_diverse.json').read_text())+json.loads((P/'manifest_highgirth.json').read_text());out=[]
for row in M:
 f=P/'graphs'/f"{row['name']}.edges";lines=f.read_text().splitlines();n,m,E=map(int,lines[0].split());edges=[tuple(map(int,l.split())) for l in lines[1:]]
 assert n==row['n'] and m==row['m'] and E==len(edges)==len(set(edges))==n*row['dv']==m*row['dc']
 adj=[[] for _ in range(n+m)]
 for u,w in edges:
  assert 0<=u<n and 0<=w<m
  adj[u].append(n+w);adj[n+w].append(u)
 assert all(len(adj[u])==row['dv'] for u in range(n)) and all(len(adj[n+w])==row['dc'] for w in range(m))
 seen={0};q=collections.deque([0])
 while q:
  for w in adj[q.popleft()]:
   if w not in seen:seen.add(w);q.append(w)
 assert len(seen)==n+m
 best=n+m
 for root in range(n+m):
  d={root:0};par={root:-1};q=collections.deque([root])
  while q:
   u=q.popleft()
   if 2*d[u]>=best:continue
   for w in adj[u]:
    if w not in d:d[w]=d[u]+1;par[w]=u;q.append(w)
    elif par[u]!=w and par[w]!=u:best=min(best,d[u]+d[w]+1)
  if best==6:break
 assert best==row['g']
 out.append(dict(name=row['name'],sha256=hashlib.sha256(f.read_bytes()).hexdigest(),g=best,connected=True,regular=True,simple=True))
(P/'verification_extended.json').write_text(json.dumps(out,indent=2));print('Verified all eight non-original graphs.')
