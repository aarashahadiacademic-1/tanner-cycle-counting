"""Run the interleaved girth-only method on every girth-only table instance.

This writes fresh machine-dependent timings to measurements_optimized.csv.
"""
import csv
import hashlib
import json
import pathlib
import subprocess

ROOT = pathlib.Path(__file__).resolve().parent
GRAPHS = [
    ("A16c", 6), ("A128r", 6), ("A128c", 6), ("B128c", 6),
    ("C128c", 6), ("D16c", 8), ("R1200", 6), ("P1200", 8),
    ("P1600", 10), ("Q410", 10), ("Q412", 12),
    # Additional girth-only rows present in the current manuscript table.
    ("C128r", 6), ("R2400", 6),
]
FIELDS = [
    "graph", "dv", "dc", "g", "n", "Ng", "kb_ms",
    "recursive_exact_ms", "resumable_exact_ms", "hybrid_ms",
    "MARE_percent", "random_wins", "exact_wins", "exact_probes",
    "mean_hybrid_exact_probes", "mean_hybrid_sampling_probes",
    "mean_samples", "max_weighted_work_ratio", "r", "Q",
    "repetitions", "sha256",
]

def main():
    subprocess.run([
        "g++", "-std=c++17", "-O3", "interleaved_optimized.cpp",
        "-o", "benchmark",
    ], cwd=ROOT, check=True)
    output = ROOT / "measurements_optimized.csv"
    with output.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=FIELDS)
        writer.writeheader()
        for i, (name, girth) in enumerate(GRAPHS):
            graph = ROOT / "graphs" / f"{name}.edges"
            proc = subprocess.run([
                str(ROOT / "benchmark"), str(graph), str(girth),
                "0.08", "100", str(20261010 + i * 101), "4096",
            ], cwd=ROOT, check=True, capture_output=True, text=True, timeout=300)
            row = json.loads(proc.stdout)
            row["graph"] = name
            row["Q"] = 4096
            row["repetitions"] = 100
            row["sha256"] = hashlib.sha256(graph.read_bytes()).hexdigest()
            with graph.open() as source:
                n, m, edges = map(int, source.readline().split())
            row["n"] = n
            row["dv"] = edges // n
            row["dc"] = edges // m
            writer.writerow(row)
            stream.flush()
            print(name, row["hybrid_ms"], row["MARE_percent"], flush=True)

if __name__ == "__main__":
    main()
