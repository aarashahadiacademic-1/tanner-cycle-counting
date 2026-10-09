import pathlib,itertools,json
P=pathlib.Path(__file__).resolve().parent
(P/'graphs').mkdir(exist_ok=True)
def petersen():
 e=set()
 for i in range(5):
  e.add(tuple(sorted((i,(i+1)%5))));e.add((i,5+i));e.add(tuple(sorted((5+i,5+(i+2)%5))))
 return 10,sorted(e)
def hoffman_singleton():
 e=set()
 for i in range(5):
  for k in range(5):
   e.add(tuple(sorted((5*i+k,5*i+(k+1)%5))))
   e.add(tuple(sorted((25+5*i+k,25+5*i+(k+2)%5))))
   for j in range(5):e.add((5*i+k,25+5*j+(i*j+k)%5))
 return 50,sorted(e)
def projective_incidence(q):
 norm=lambda x:tuple(a*pow(next(t for t in x if t),-1,q)%q for a in x)
 points=sorted({norm(x) for x in itertools.product(range(q),repeat=3) if any(x)})
 n=len(points);e=[]
 for i,x in enumerate(points):
  for j,y in enumerate(points):
   if sum(a*b for a,b in zip(x,y))%q==0:e.append((i,n+j))
 return 2*n,e
def save(name,N,e,d,s,g,base,expected):
 # Incidence Tanner graph: ordinary edges become variable nodes,
 # ordinary vertices become checks. Cyclic lift shifts the first
 # lexicographic incidence edge by one, all others by zero.
 e=sorted(e);out=[]
 for u,(a,b) in enumerate(e):
  for endpoint,w in enumerate((a,b)):
   shift=1 if u==0 and endpoint==0 else 0
   for i in range(s):out.append((u*s+i,w*s+(i+shift)%s))
 n=len(e)*s;m=N*s;out.sort()
 with (P/'graphs'/f'{name}.edges').open('w') as f:
  f.write(f'{n} {m} {len(out)}\n');f.writelines(f'{u} {w}\n' for u,w in out)
 return dict(name=name,n=n,m=m,dv=2,dc=d,g=g,T=g,base=base,lift=s,kind='cyclic',family='column-weight-two-incidence',expected_Ng=expected*s)
M=[]
N,e=petersen();M.append(save('Pet64',N,e,3,64,10,'Petersen',8))
N,e=hoffman_singleton();M.append(save('HS128',N,e,7,128,10,'Hoffman-Singleton',1224))
N,e=projective_incidence(3);M.append(save('F3c32',N,e,4,32,12,'PG(2,3) incidence',207))
N,e=projective_incidence(5);M.append(save('F5c64',N,e,6,64,12,'PG(2,5) incidence',3750))
(P/'manifest_highgirth.json').write_text(json.dumps(M,indent=2));print(json.dumps(M,indent=2))
