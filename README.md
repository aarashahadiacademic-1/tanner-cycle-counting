# Tanner-graph cycle-counting benchmarks

This repository contains the implementations, graph inputs, and benchmark
runners for the 20 rows in the manuscript's combined experimental table:
13 girth-only instances and 7 multiple-target instances. The manuscript table
is maintained in the paper; this repository does not carry a second table
snapshot or archived timing CSVs.

## Implementations

- counter.cpp: local exact cycle counter, canonical simple-cycle DFS
  validation, and Karimi–Banihashemi message-passing baseline (KB).
- impa_count.cpp: Li–Lin–Abdel-Ghaffar IMPA implementation.
- interleaved_optimized.cpp: exact/sampling interleaved girth-only method.
- run_exact_table.py: ten multiple-target rows in the manuscript.
- algorithm_sources/run_added_exact_targets.py: the additional
  A128r-at-8 and D1c-at-10/12/14 targets.
- run_optimized.py: all thirteen girth-only rows.
- algorithm_sources/run_impa_bench.py: IMPA on the main target set; the
  added-target runner covers its remaining targets.

The edge lists under graphs/ are the realized benchmark inputs. The
manifests and generator scripts record how those graphs were constructed.

## Fresh runs

Use a C++17 compiler and Python 3:

    g++ -std=c++17 -O3 counter.cpp -o counter
    python3 run_exact_table.py
    python3 algorithm_sources/run_added_exact_targets.py
    python3 run_optimized.py
    python3 algorithm_sources/run_impa_bench.py

Each runner writes fresh local measurements. Wall-clock times vary with
compiler version, processor, and system load. The historical raw timing
records that produced the manuscript's printed millisecond values are not
archived here, so these commands do not promise to reproduce those exact
rounded times. Graph inputs and deterministic cycle counts can be checked
with the included validation tools.

The comparisons are independent implementations based on the cited algorithm
descriptions, not measurements of the original authors' binaries.
