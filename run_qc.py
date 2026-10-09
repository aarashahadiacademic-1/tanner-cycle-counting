"""Supplementary QC benchmarks; exact/hybrid routines match counter.cpp.
Only validation exploits checked cyclic automorphisms. Timed algorithms
operate on the complete graph and do not exploit those automorphisms.
"""
import pathlib,json,subprocess,time
P=pathlib.Path(__file__).resolve().parent
rows=json.loads((P/'manifest_qc.json').read_text());out=[]
for row in rows:
 print('RUN',row['name'],flush=True)
 r=subprocess.run([str(P/'counter_qc'),str(P/'graphs'/f"{row['name']}.edges"),str(row['g']),str(row['g']),'qc',str(row['Z'])],text=True,capture_output=True,check=True)
 x=json.loads(r.stdout);assert x['counts'][str(row['g'])]==row['expected_Ng'];out.append(dict(row,**x))
 (P/'results_qc.json').write_text(json.dumps(out,indent=2)+'\n');print(r.stdout,flush=True)
