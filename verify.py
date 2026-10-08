import json,pathlib,hashlib,subprocess,random
P=pathlib.Path(__file__).parent
manifest=json.load(open(P/'manifest.json')); checks=[]
for row in manifest:
 f=P/'graphs'/f"{row['name']}.edges"
 with f.open() as stream:
  n,m,E=map(int,next(stream).split());edges=[tuple(map(int,l.split())) for l in stream]
 assert n==row['n'] and m==row['m'] and E==len(edges)==len(set(edges))
 du=[0]*n;dw=[0]*m
 for u,w in edges:
  assert 0<=u<n and 0<=w<m
  du[u]+=1;dw[w]+=1
 if 'dv' in row:
  assert all(d==row['dv'] for d in du) and all(d==row['dc'] for d in dw)
  assert row['dv']<row['dc']
 checks.append(dict(name=row['name'],sha256=hashlib.sha256(f.read_bytes()).hexdigest(),degree_check=True))
# Ten further irregular overlapping-cycle correctness checks, preserving the base girth.
f=P/'graphs'/'validation_B.edges'
with f.open() as stream:
 n,m,E=map(int,next(stream).split());base=[tuple(map(int,l.split())) for l in stream]
fuzz=[]
for seed in range(10):
 rng=random.Random(20261020+seed);removed=set(rng.sample(range(E),5));edges=[e for j,e in enumerate(base) if j not in removed]
 f=P/'graphs'/f'fuzz_{seed}.edges'
 with f.open('w') as stream:
  stream.write(f'{n} {m} {len(edges)}\n')
  for u,w in edges:stream.write(f'{u} {w}\n')
 r=subprocess.run([str(P/'counter'),str(f),'6','14','validate'],capture_output=True,text=True,check=True)
 fuzz.append(dict(seed=20261020+seed,**json.loads(r.stdout)))
json.dump(dict(integrity=checks,additional_correctness=fuzz),open(P/'verification.json','w'),indent=2)
print('Verified',len(checks),'graph files and ten additional unrestricted irregular instances.')
