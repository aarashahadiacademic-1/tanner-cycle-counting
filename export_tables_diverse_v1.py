import csv,json,pathlib
P=pathlib.Path(__file__).resolve().parent
rows=json.loads((P/'results.json').read_text())
lookup={(r['name'],r['T']):r for r in rows}
EXACT=[('A128r',6),('C128r',6),('R1200',6),('R2400',6),('P1200',8),('P1600',10),('A128r',10),('A128c',10),('D1c',14)]
HYBRID=['A32c','A128r','A128c','B128r','B128c','C128r','C128c','D16r','D16c','R1200','R2400','P1200','P1600']
with (P/'table1_exact.csv').open('w',newline='') as f:
 w=csv.writer(f);w.writerow(['graph','dv','dc','g','n','T','count_tuple','ours_ms','KB_ms','speedup'])
 for key in EXACT:
  r=lookup[key];assert r['kb_agrees'] and r['nb_agrees']
  counts=tuple(r['counts'][str(t)] for t in range(r['g'],r['T']+1,2))
  w.writerow([r['name'],r['dv'],r['dc'],r['g'],r['n'],r['T'],str(counts),f"{r['exact_ms']:.3f}",f"{r['kb_ms']:.3f}",f"{r['kb_ms']/r['exact_ms']:.2f}"])
with (P/'table2_hybrid.csv').open('w',newline='') as f:
 w=csv.writer(f);w.writerow(['graph','dv','dc','g','n','N_g','lambda','speedup','mare_percent'])
 for name in HYBRID:
  r=next(r for r in rows if r['name']==name and r['T']==r['g']);assert sum(r['routes'])==100
  w.writerow([name,r['dv'],r['dc'],r['g'],r['n'],r['counts'][str(r['g'])],(f"{r['lambda']:.3f}" if r['lambda']<0.05 else f"{r['lambda']:.2f}"),f"{r['exact_ms']/r['hybrid_ms']:.2f}",f"{r['MARE_percent']:.1f}"])
print('Exported 9 exact-comparison rows and 13 hybrid rows.')
