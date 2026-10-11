import csv
import json
import pathlib
import statistics
import subprocess
import time

ROOT = pathlib.Path(__file__).resolve().parents[1]
TARGETS = [
    ("table1", "A128r", 6, 6), ("table1", "A128c", 6, 8),
    ("table1", "A128c", 6, 10), ("table1", "A128r", 6, 10),
    ("table1", "C128r", 6, 6), ("table1", "R1200", 6, 6),
    ("table1", "R2400", 6, 6), ("table1", "P1200", 8, 8),
    ("table1", "D1c", 8, 14), ("table1", "P1600", 10, 10),
    ("table2", "A16c", 6, 6), ("table2", "A128r", 6, 6),
    ("table2", "A128c", 6, 6), ("table2", "B128c", 6, 6),
    ("table2", "C128c", 6, 6), ("table2", "D16c", 8, 8),
    ("table2", "P1200", 8, 8), ("table2", "R1200", 6, 6),
    ("table2", "P1600", 10, 10), ("table2", "Q410", 10, 10),
    ("table2", "Q412", 12, 12),
]

def main():
    subprocess.run(["g++", "-std=c++17", "-O3", "impa_count.cpp", "-o", "impa_count"],
                   cwd=ROOT, check=True)
    out = ROOT / "algorithm_sources" / "impa_timings.csv"
    fields = ["table", "graph", "g", "T", "counts", "runs_ms", "median_ms"]
    with out.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fields); writer.writeheader()
        for table, name, g, T in TARGETS:
            path = ROOT / "graphs" / f"{name}.edges"
            runs, counts = [], None
            for _ in range(7):
                started = time.monotonic()
                p = subprocess.run([str(ROOT/"impa_count"), str(path), str(g), str(T)],
                                   cwd=ROOT, check=True, capture_output=True, text=True, timeout=600)
                row = json.loads(p.stdout)
                runs.append(float(row["impa_ms"]))
                if counts is None: counts = row["counts"]
                elif counts != row["counts"]: raise RuntimeError(f"count instability: {name}, {T}")
            writer.writerow({"table":table,"graph":name,"g":g,"T":T,
                             "counts":json.dumps(counts,separators=(",",":")),
                             "runs_ms":json.dumps(runs,separators=(",",":")),
                             "median_ms":statistics.median(runs)})
            f.flush()
            print(f"{table} {name} T={T}: median={statistics.median(runs):.3f} ms",flush=True)

if __name__ == "__main__": main()
