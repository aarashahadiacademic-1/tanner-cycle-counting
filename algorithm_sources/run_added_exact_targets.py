"""Pinned single-core reruns for the additional Table 1 targets."""
import csv
import hashlib
import json
import pathlib
import statistics
import subprocess

ROOT = pathlib.Path(__file__).resolve().parents[1]
CPU = "0"
TARGETS = [("A128r", 6, 8), ("D1c", 8, 10), ("D1c", 8, 12), ("D1c", 8, 14)]


def pinned(*args, timeout=900):
    return subprocess.run(["taskset", "-c", CPU, *map(str, args)], cwd=ROOT,
                          check=True, capture_output=True, text=True,
                          timeout=timeout)


def main():
    subprocess.run(["g++", "-std=c++17", "-O3", "counter.cpp", "-o", "counter"],
                   cwd=ROOT, check=True)
    subprocess.run(["g++", "-std=c++17", "-O3", "impa_count.cpp", "-o", "impa_count"],
                   cwd=ROOT, check=True)
    out = ROOT / "algorithm_sources" / "added_exact_targets.csv"
    fields = ["graph", "g", "T", "counts", "ours_ms", "KB_ms",
              "IMPA_runs_ms", "IMPA_median_ms", "sha256", "cpu"]
    with out.open("w", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields)
        writer.writeheader()
        for name, g, T in TARGETS:
            graph = ROOT / "graphs" / f"{name}.edges"
            validation = json.loads(pinned(ROOT / "counter", graph, g, T,
                                           "validate").stdout)
            measured = json.loads(pinned(ROOT / "counter", graph, g, T,
                                         "bench").stdout)
            counts = measured["counts"]
            if not validation.get("canonical_dfs_agrees"):
                raise RuntimeError(f"canonical DFS validation failed: {name}, T={T}")
            if validation["counts"] != counts or not measured.get("kb_agrees"):
                raise RuntimeError(f"count validation failed: {name}, T={T}")
            impa_runs = []
            for _ in range(7):
                result = json.loads(pinned(ROOT / "impa_count", graph, g, T).stdout)
                if result["counts"] != counts:
                    raise RuntimeError(f"IMPA count mismatch: {name}, T={T}")
                impa_runs.append(float(result["impa_ms"]))
            writer.writerow({
                "graph": name, "g": g, "T": T,
                "counts": json.dumps(counts, separators=(",", ":")),
                "ours_ms": measured["exact_ms"], "KB_ms": measured["kb_ms"],
                "IMPA_runs_ms": json.dumps(impa_runs, separators=(",", ":")),
                "IMPA_median_ms": statistics.median(impa_runs),
                "sha256": hashlib.sha256(graph.read_bytes()).hexdigest(),
                "cpu": CPU,
            })
            stream.flush()
            print(name, T, counts, measured["exact_ms"], measured["kb_ms"],
                  statistics.median(impa_runs), flush=True)


if __name__ == "__main__":
    main()
