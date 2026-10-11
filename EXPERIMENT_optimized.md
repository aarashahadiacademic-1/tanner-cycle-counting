# October 11 manuscript experiment record

This directory records the experiments for the October 11 manuscript tables.

## Current table snapshots

`paper_table1_exact_20261011.csv` and `paper_table2_hybrid_20261011.csv` are
faithful data snapshots of the two tables in the manuscript. They preserve the
published aggregate timings and counts so the repository and submitted source
can be checked row by row. They are snapshots, not raw per-repetition logs.

## Exact-versus-KB rerun

Run `python3 run_exact_table.py`. It compiles `counter.cpp`, checks each target
with the exact and Karimi--Banihashemi implementations, and writes fresh
seven-run medians to `exact_table_rerun.csv`. Timings are machine- and
load-dependent; the manuscript snapshot remains unchanged.

## Interleaved hybrid rerun

Compile and run all 11 hybrid instances with:

```sh
python3 run_optimized.py
```

The script compiles `interleaved_optimized.cpp` with C++17 and `-O3`, then
writes a fresh 100-run record to `measurements_optimized_rerun.csv`. The
archived `measurements_optimized.csv` records the October 10 run with
`epsilon=0.08`, `r=101`, and `Q=4096`.

The scheduler assigns one credit per exact adjacency inspection and eight
credits per sampled non-backtracking edge traversal. It can suspend inside
BFS, residual-path scans, and walks. Eight credits is an empirical calibration,
not a machine-independent instruction-count equivalence. The weighted-work
ratio in the measurements is relative to the same resumable exact engine; it
does not imply a wall-clock or CPU-instruction bound.

The archived Q=4096 run uses the same 11 graph instances and cost model as the
manuscript, but its measured times are not identical to the manuscript table
snapshot. Use the snapshots to identify reported values and the scripts to
re-run the stated procedures; do not substitute one timing record for the
other. The legacy `table2_hybrid.csv` is from the earlier pilot-and-cap method,
and `three_way_interleaved_11.csv` is the preliminary Q=256 run.
