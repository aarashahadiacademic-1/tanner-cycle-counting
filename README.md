# Tanner-graph cycle-counting experiments

This repository contains the benchmark instances, implementations, and recorded results for the consolidated table in Section 4 of the manuscript.

## Current table

- [`paper_table_current.tex`](paper_table_current.tex) is the LaTeX table snapshot.
- [`paper_table_current.csv`](paper_table_current.csv) contains the same 20 rows and values in machine-readable form.
- The table compares the local Karimi–Banihashemi (KB) and Li–Lin–Abdel-Ghaffar (IMPA) implementations with our exact and interleaved hybrid methods. The table is a selected comparison; it does not implement every algorithm from every cited paper. Halford–Chugg (H&C) results are not included.
- C128c and C128r are retained because they are benchmark graphs in the current table; they are unrelated to the removed H&C comparison artifacts.

The comparison codes are independent implementations based on the published algorithm descriptions, not the authors' original software. Low-level implementation choices can differ from those of the original authors. All listed routines are single-threaded and use the same graph inputs. The IMPA timing records were collected with g++ 13.3.0; the manuscript's general benchmark description lists g++ 14.2 for its main measurements. The included runners allow fresh measurements, whose timings depend on compiler, machine load, and CPU scheduling. Treat the archived values as the reported manuscript snapshot, not as guaranteed fresh-run timings.

## Code and data

- `counter.cpp` implements the exact local-search counter, canonical simple-cycle DFS validation, and KB message-passing baseline.
- `impa_count.cpp` implements the IMPA comparison algorithm.
- `interleaved_optimized.cpp` implements the hybrid girth-cycle routine and includes the shared exact/KB code.
- `graphs/*.edges` are the realized graph instances used by the current table. Their construction parameters and seeds are recorded in the manifests.
- `impa_timings.csv` and `added_exact_targets.csv` preserve the seven-run IMPA timing records, counts, and graph hashes for the current targets.
- `measurements_optimized.csv` and the two old split-table CSVs were removed because they did not match the current consolidated table snapshot.

## Reproduction

From the repository root, with a C++17 compiler and Python 3:

```sh
g++ -std=c++17 -O3 counter.cpp -o counter
python3 run_exact_table.py
python3 run_optimized.py
g++ -std=c++17 -O3 impa_count.cpp -o impa_count
python3 algorithm_sources/run_impa_bench.py
```

The runners write fresh measurements. They do not overwrite the archived table snapshot. Exact and KB timings use seven runs; hybrid estimates use 100 repetitions with `epsilon=0.08`, `r=101`, and `Q=4096`. Re-running on a different compiler or under different system load can produce different times and sampling outputs.

The benchmark graphs are synthetic sparse Tanner graphs, not standardized codes or decoding benchmarks. See `QC_BENCHMARKS.md` for Q410 and Q412 construction details.
