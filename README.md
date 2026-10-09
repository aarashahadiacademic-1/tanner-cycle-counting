# Exact and hybrid cycle counting in Tanner graphs

Reproducibility package for the two experimental tables of the manuscript **Near-Girth Cycle Counting in Tanner Graphs: Exact Algorithms and Hybrid Estimation**.

## Contents

- `counter.cpp`: proposed exact search, independent canonical simple-cycle DFS, independent non-backtracking enumeration, our implementation of Karimi--Banihashemi message passing, and capped hybrid sampling.
- `generate.py`, `manifest.json`, `graphs/`: graph generators, construction parameters, seeds, and the realized edge lists.
- `results.json`, `run_tables.log`: the fresh 2026-10-09 measurements for the revised tables. `results_original.json`, `run.log`, and CSV files ending in `_original` retain the earlier experiments.
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
python3 run_tables.py
python3 verify.py
python3 verify_diverse.py
python3 export_tables.py
```

`run_tables.py` uses the supplied edge lists. To regenerate the original and new families, run `python3 generate.py` and `python3 generate_diverse.py`, respectively.
**Do not compile with `-DNDEBUG`: the original measurement harness uses assertions that execute and verify the timed algorithms.**
`run_tables.py` overwrites `results.json`, and `verify.py` overwrites `verification.json`; retain copies if the original measurements are needed.
The stored measurements used GNU g++ 13.3.0, Python 3.12.14, and an AMD EPYC 9V74 shared container. Counting times are medians of seven runs. Hybrid times and errors average 100 runs with target r=100, pilot n trials, and second-stage cap 10n. Both pilot and exact fallback are timed. Sampling uses seed 20261009+n. New wall-clock timings depend on hardware and load and need not equal the saved values.

## Table correspondence

Table I: same-target comparisons of our exact routine with our KB implementation. Speedup = KB time / our exact time.

Table II: hybrid estimation. Speedup = median girth-only exact time / mean hybrid time. MARE (%) is the mean absolute relative error multiplied by 100 over all outputs, including exact returns.

A--D graph labels specify the base, lift size, and cyclic (`c`) or random (`r`) lift. R/P labels give variable-node counts. The generators and manifest provide construction details. The regular benchmarks are synthetic Tanner graphs with degree pairs (3,6), (3,4), and (4,8), not standardized LDPC codes. Cactus graphs are mathematical correctness tests. No decoding-performance or matrix-rank claim is made.

## Independent checks and limits

The KB routine is our own implementation based on Karimi and Banihashemi, arXiv:1004.3966, not the authors' original software. It is used only for targets below 2g. The unrestricted exact search is also checked against canonical simple-cycle DFS and cactus constructions.

Edge-list files start with `n m E`, followed by zero-based variable/check endpoint pairs. The C++ implementation uses unsigned 64-bit counters; the supplied benchmark counts fit, but larger inputs/targets can overflow and require arbitrary-precision counters. Very long targets can entail exponential work.

## Broadened benchmark selection (2026-10-09)

Four base construction families are represented: affine incidence (A--C), split symplectic generalized quadrangle (D), random socket pairing repaired by switches (R), and our degree-constrained PEG variant (P).

Table I retains 9 rows: A256r at T=6, A128r at T=8, D16r at T=8, and D16r at T=10 are replaced by R1200, R2400, P1200, and P1600 at their girths. The two longer-target rows favoring KB remain.
Table II retains 13 rows: A8c, A16c, A256r, and A256c are replaced by the same four new graphs. The matched A128, B128, C128, and D16 cyclic/random pairs remain. All selected rows were freshly measured under the same protocol; no old/new timing mixture is used. The new samples use fixed predeclared seeds and are not selected for runtime outcomes.

`generate_diverse.py` constructs the four R/P samples. Socket pairing rejects parallel edges; degree-preserving two-switches remove four-cycles without introducing new ones. These repaired graphs are **not claimed to be uniform draws** from the girth-constrained ensemble. PEG attaches edges at maximum distance among checks with remaining degree capacity, breaks ties by minimum degree then random selection, and restarts if completion without four-cycles becomes impossible. This is our implementation of a degree-constrained variant, not the original authors' code or standardized codes.

The R1200/R2400 degree pairs are (4,8)/(3,6); P1200/P1600 use (3,6)/(3,4) and realized girths 8/10. All four new samples route to exact fallback in all 100 runs, illustrating limits of hybrid acceleration rather than concealing them. `verification_diverse.json` records independent degree, girth, connectivity, simplicity, and edge-hash checks. The C++ harness additionally checks count equality against exhaustive NB enumeration and KB.

PEG reference: X.-Y. Hu, E. Eleftheriou, and D. M. Arnold, IEEE Transactions on Information Theory 51(1), 386--398 (2005), doi:10.1109/TIT.2004.839541.

`paper_table_replacements.tex` supplies the construction subsection, revised tables, interpretation paragraph, and PEG bibliography entry.
