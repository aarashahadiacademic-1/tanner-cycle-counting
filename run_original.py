import subprocess,json,pathlib,time
P=pathlib.Path(__file__).parent
M=json.load(open(P/'manifest.json'));out=[]
M += [dict(row,name=row['name'],T=T,comparison=True) for row in M if row['name'] in ['A128c','A128r','D1c','D16r'] for T in ([8,10] if row['name'].startswith('A') else [10,14] if row['name']=='D1c' else [10])]
for row in M:
 name=row['name'];g=10 if name.startswith('cactus10') else 12 if name.startswith('cactus12') else 8 if name.startswith('D') else 6
 mode='cactus' if name.startswith('cactus') else 'validate' if row.get('validation') else 'bench'
 cmd=[str(P/'counter'),str(P/'graphs'/f'{name}.edges'),str(g),str(row['T']),mode]
 if mode=='cactus':cmd.append(str(row['expected']))
 st=time.time();print('RUN',name,flush=True)
 r=subprocess.run(cmd,capture_output=True,text=True,check=True)
 result=dict(row,**json.loads(r.stdout));out.append(result)
 json.dump(out,open(P/'results_original.json','w'),indent=2)
 print(name,round(time.time()-st,2),r.stdout.strip(),flush=True)
