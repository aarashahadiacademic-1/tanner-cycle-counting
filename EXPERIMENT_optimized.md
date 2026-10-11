# October 11 manuscript experiment record

This file documents the table data and rerun procedures.

## Table snapshots

`paper_table1_exact_20261011.csv` preserves the graph rows, counts, and
reported timings in Table 1. `paper_table2_hybrid_20261011.csv` is rounded to
two decimal places from the archived `measurements_optimized.csv` record and
can be compared row by row against it. The CSV measurement file stores the
higher-precision aggregate values; it is not a per-repetition log.

## Exact-versus-KB rerun

Run `python3 run_exact_table.py`. It compiles `counter.cpp`, checks each
target with the exact and Karimi--Banihashemi implementations, and writes
fresh seven-run medians to `exact_table_rerun.csv`. Timings are machine- and
load-dependent; the snapshot preserves the manuscript's reported values.

## Interleaved hybrid rerun

First generate Q410/Q412 edge lists, then run the eleven hybrid instances:

```sh
python3 generate_qc.py
python3 run_optimized.py
```

The runner compiles `interleaved_optimized.cpp` with C++17 and `-O3`, then
writes a fresh 100-run aggregate to `measurements_optimized_rerun.csv`. The
archived `measurements_optimized.csv` records the October 10 run with
`epsilon=0.08`, `r=101`, and `Q=4096`.

The scheduler assigns one credit per exact adjacency inspection and eight
credits per sampled non-backtracking edge traversal. It can suspend inside
BFS, residual-path scans, and walks. Eight credits is an empirical calibration,
not a machine-independent instruction-count equivalence. The weighted-work
ratio in the measurements is relative to the same resumable exact engine; it
does not imply a wall-clock or CPU-instruction bound.

Fresh reruns can differ in timing because of machine and system load; sampling
outcomes can also vary with the seed. The archived measurement record and the
Table 2 snapshot in this repository are aligned after rounding to two decimal
places.