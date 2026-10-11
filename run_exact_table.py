"""Re-run all exact-versus-KB targets in the October 11 manuscript table."""
import csv
import hashlib
import json
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parent
TARGETS = [
    ("A128r", 6, 6), ("A128c", 6, 8), ("A128c", 6, 10),
    ("A128r", 6, 10), ("C128r", 6, 6), ("R1200", 6, 6),
    ("R2400", 6, 6), ("P1200", 8, 8), ("D1c", 8, 14),
    ("P1600", 10, 10),
]
FIELDS = [
    "graph", "dv", "dc", "g", "n", "T", "counts", "ours_ms",
    "KB_ms", "speedup", "sha256",
]

def main():
    subprocess.run(["g++", "-std=c++17", "-O3", "counter.cpp", "-o", "counter"],
                   cwd=ROOT, check=True)
    output = ROOT / "exact_table_rerun.csv"
    with output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS)
        writer.writeheader()
        for graph_name, girth, target in TARGETS:
            graph = ROOT / "graphs" / f"{graph_name}.edges"
            proc = subprocess.run([
                str(ROOT / "counter"), str(graph), str(girth), str(target),
                "bench",
            ], cwd=ROOT, check=True, capture_output=True, text=True, timeout=300)
            result = json.loads(proc.stdout)
            with graph.open() as source:
                n, m, edges = map(int, source.readline().split())
            counts = tuple(result["counts"][str(t)]
                           for t in range(girth, target + 1, 2))
            ours = result["exact_ms"]
            kb = result["kb_ms"]
            writer.writerow({
                "graph": graph_name, "dv": edges // n,
                "dc": edges // m, "g": girth, "n": n,
                "T": target, "counts": counts, "ours_ms": ours,
                "KB_ms": kb, "speedup": kb / ours,
                "sha256": hashlib.sha256(graph.read_bytes()).hexdigest(),
            })
            stream.flush()
            print(graph_name, target, counts, ours, kb, flush=True)

if __name__ == "__main__":
    main()
