# Tanner-graph cycle-counting benchmarks

This repository contains the implementations, graph inputs, and benchmark
runners for the manuscript's combined experimental table: 13 girth-only
instances and 7 multiple-target rows. The table itself remains in the paper;
the repository does not carry a second table snapshot or archived timing CSVs.

## Implementations and runners

- `counter.cpp`: local exact counter, canonical simple-cycle DFS validation,
  and Karimi–Banihashemi (KB) message-passing baseline.
- `impa_count.cpp`: Li–Lin–Abdel-Ghaffar (IMPA) implementation.
- `interleaved_optimized.cpp`: exact/sampling interleaved girth-only method.
- `run_exact_table.py`: ten base exact/KB targets, including girth-only and
  multiple-target cases.
- `algorithm_sources/run_added_exact_targets.py`: A128r at (T=8) and D1c
  at (T=10,12,14); it measures Exact, KB, and IMPA.
- `run_optimized.py`: all thirteen girth-only rows.
- `algorithm_sources/run_impa_bench.py`: IMPA on the main target set; the
  added-target runner covers its remaining targets.

The edge lists under `graphs/` are the realized benchmark inputs.
The manifests and generator scripts record their construction.

## Fresh runs

Use a C++17 compiler and Python 3:

```sh
g++ -std=c++17 -O3 counter.cpp -o counter
python3 run_exact_table.py
python3 algorithm_sources/run_added_exact_targets.py
python3 run_optimized.py
python3 algorithm_sources/run_impa_bench.py
```

Each runner writes fresh local measurements. Wall-clock times vary with
compiler version, processor, and system load, so a fresh run is not expected
to reproduce the manuscript's printed millisecond values exactly. The table's
reported timing values came from separate benchmark runs; those raw runs are
not archived here. Exact counts and graph instances are deterministic and
can be checked with the included validation tools.

The comparisons are independent implementations based on the cited algorithm
descriptions, not measurements of the original authors' binaries.
