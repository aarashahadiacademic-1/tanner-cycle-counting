# Tanner-graph cycle-counting benchmarks

This repository contains the implementations, graph inputs, and benchmark
runners for the manuscript's combined experimental table: 13 girth-only
instances and 7 multiple-target rows.

- `published_table.csv` is a row-for-row snapshot of the current manuscript
  table, with values rounded to the precision printed in the paper.
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
- `verify_published_table.py`: checks table rows, graph metadata, and count
  tuple structure; `--counts` also compares Exact, KB, and IMPA counts with
  every row.

The edge lists under `graphs/` are the realized benchmark inputs.
The manifests and generator scripts record their construction.

## Fresh runs

Use a C++17 compiler and Python 3:

```sh
g++ -std=c++17 -O3 counter.cpp -o counter
g++ -std=c++17 -O3 impa_count.cpp -o impa_count
python3 verify_published_table.py
python3 run_exact_table.py
python3 algorithm_sources/run_added_exact_targets.py
python3 run_optimized.py
python3 algorithm_sources/run_impa_bench.py
```

The CSV values match the printed manuscript table. The historical per-run
timing records are not archived here. Benchmark runners produce fresh local
measurements; wall-clock times vary with compiler, processor, and system load
and are not expected to reproduce the frozen manuscript timings exactly.
The optional `verify_published_table.py --counts` checks deterministic graph
counts, not historical runtime values.

The comparisons are independent implementations based on the cited algorithm
descriptions, not measurements of the original authors' binaries.
