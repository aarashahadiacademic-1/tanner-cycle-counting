"""Same-target KB, exact and operation-interleaved hybrid benchmark (11 original graphs)."""
import subprocess, pathlib, json, hashlib, csv
P=pathlib.Path(__file__).resolve().parent
NAMES=[('A16c',6),('A128r',6),('A128c',6),('B128c',6),('C128c',6),('D16c',8),('R1200',6),('P1200',8),('P1600',10),('Q410',10),('Q412',12)]
SEEDS={'A16c':20261010,'A128r':20261111,'A128c':20261212,'B128c':20261313,'C128c':20261414,'D16c':20261515,'R1200':20261010,'P1200':20261010,'P1600':20261010,'Q410':20261010,'Q412':20261011}
FIELDS=['graph','g','n','Ng','eps','r','quantum','exact_ms','kb_ms','hybrid_ms','MARE_percent','random_wins','exact_wins','mean_trials','graph_sha256']
def main():
  records=[]
  for name,g in NAMES:
    path=P/'graphs'/f'{name}.edges'
    args=[str(P/'interleaved_threeway'),str(path),str(g),'0.08','100',str(SEEDS[name]),'256']
    out=json.loads(subprocess.check_output(args,text=True))
    out=dict(graph=name,**out,graph_sha256=hashlib.sha256(path.read_bytes()).hexdigest())
    records.append(out)
    print(name,out,flush=True)
    with (P/'three_way_interleaved_rerun.csv').open('w') as f:
      w=csv.DictWriter(f,fieldnames=FIELDS);w.writeheader();w.writerows(records)
if __name__=='__main__':main()
