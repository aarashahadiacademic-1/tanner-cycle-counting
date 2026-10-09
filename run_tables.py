import pathlib,json,subprocess,hashlib
P=pathlib.Path(__file__).resolve().parent
EXACT=[('A128r',6),('C128r',6),('R1200',6),('R2400',6),('P1200',8),('P1600',10),('A128r',10),('A128c',10),('D1c',14)]
HYBRID=['A32c','A128r','A128c','B128r','B128c','C128r','C128c','D16r','D16c','R1200','R2400','P1200','P1600']
M=json.loads((P/'manifest.json').read_text())+json.loads((P/'manifest_diverse.json').read_text())
by_name={r['name']:r for r in M}
requested=list(EXACT)
for name in HYBRID:
 row=by_name[name];g=row.get('g',8 if name.startswith('D') else 6)
 if (name,g) not in requested:requested.append((name,g))
out=[]
with (P/'run_tables.log').open('w') as log:
 for name,T in requested:
  row=by_name[name];g=row.get('g',8 if name.startswith('D') else 6)
  graph=P/'graphs'/f'{name}.edges'
  print('RUN',name,'T=',T,flush=True)
  result=subprocess.run([str(P/'counter'),str(graph),str(g),str(T),'bench'],text=True,capture_output=True,check=True)
  r=dict(row,**json.loads(result.stdout));r['edge_sha256']=hashlib.sha256(graph.read_bytes()).hexdigest();out.append(r)
  (P/'results.json').write_text(json.dumps(out,indent=2));log.write(result.stdout);log.flush();print(result.stdout.strip(),flush=True)
