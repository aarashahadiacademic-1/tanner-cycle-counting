# Exact and hybrid cycle counting in Tanner graphs

Reproducibility package for the two experimental tables of the manuscript **Near-Girth Cycle Counting in Tanner Graphs: Exact Algorithms and Hybrid Estimation**.

## Contents

- `counter.cpp`: proposed exact search, independent canonical simple-cycle DFS, independent non-backtracking enumeration, our implementation of Karimi--Banihashemi message passing, and capped hybrid sampling.
- `generate.py`, `manifest.json`, `graphs/`: graph generators, construction parameters, seeds, and the realized edge lists.
- `results.json`, `run.log`: saved results of the executed experiments, including results beyond the rows selected for the manuscript.
- `verify.py`, `verification.json`: degree and graph-integrity checks, hashes, and independent correctness checks.
- `export_tables.py`: exports exactly the selected rows of Tables I and II to CSV, using the saved unrounded results.
- `table1_exact.csv`, `table2_hybrid.csv`: exported manuscript rows. Speedups are calculated from unrounded times, not rounded display values.

## Reproduce the tables from saved results

```bash
python3 export_tables.py
```

## Compile and run new measurements

Requires Python 3 and GNU g++ with C++17 support; no additional Python packages.

```bash
g++ -std=c++17 -O3 -Wall -Wextra counter.cpp -o counter
python3 run.py
python3 verify.py
python3 export_tables.py
```

`run.py` uses the supplied edge lists. To regenerate them first, run `python3 generate.py`.
**Do not compile with `-DNDEBUG`: the original measurement harness uses assertions that execute and verify the timed algorithms.**
`run.py` overwrites `results.json`, and `verify.py` overwrites `verification.json`; retain copies if the original measurements are needed.
The stored measurements used GNU g++ 13.3.0, Python 3.12.14, and an AMD EPYC 9V74 shared container. Counting times are medians of seven runs. Hybrid times and errors average 100 runs with target r=100, pilot n trials, and second-stage cap 10n. Both pilot and exact fallback are timed. Sampling uses seed 20261009+n. New wall-clock timings depend on hardware and load and need not equal the saved values.

## Table correspondence

Table I: same-target comparisons of our exact routine with our KB implementation. Speedup = KB time / our exact time.

Table II: hybrid estimation. Speedup = median girth-only exact time / mean hybrid time. MARE (%) is the mean absolute relative error multiplied by 100 over all outputs, including exact returns.

The graph labels specify the base A--D, lift size, and cyclic (`c`) or random (`r`) lift. The generators and manifest provide construction details. The regular benchmarks are synthetic Tanner graphs with degree pairs (3,6), (3,4), and (4,8), not standardized LDPC codes. Cactus graphs are mathematical correctness tests. No decoding-performance or matrix-rank claim is made.

## Independent checks and limits

The KB routine is our own implementation based on Karimi and Banihashemi, arXiv:1004.3966, not the authors' original software. It is used only for targets below 2g. The unrestricted exact search is also checked against canonical simple-cycle DFS and cactus constructions.

Edge-list files start with `n m E`, followed by zero-based variable/check endpoint pairs. The C++ implementation uses unsigned 64-bit counters; the supplied benchmark counts fit, but larger inputs/targets can overflow and require arbitrary-precision counters. Very long targets can entail exponential work.
