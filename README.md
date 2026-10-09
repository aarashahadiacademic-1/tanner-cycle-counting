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
python3 verify_extended.py
python3 export_tables.py
```

`run_tables.py` uses the supplied edge lists. To regenerate the original and new families, run `python3 generate.py` and `python3 generate_diverse.py`; run `python3 generate_highgirth.py` for the cycle-code family.
**Do not compile with `-DNDEBUG`: the original measurement harness uses assertions that execute and verify the timed algorithms.**
`run_tables.py` overwrites `results.json`, and `verify.py` overwrites `verification.json`; retain copies if the original measurements are needed.
The stored measurements used GNU g++ 13.3.0, Python 3.12.14, and an AMD EPYC 9V74 shared container. Counting times are medians of seven runs. Hybrid times and errors average 100 runs with target r=100, pilot n trials, and second-stage cap 10n. Both pilot and exact fallback are timed. Sampling uses seed 20261009+n. New wall-clock timings depend on hardware and load and need not equal the saved values.

## Table correspondence

Table I: same-target comparisons of our exact routine with our KB implementation. Speedup = KB time / our exact time.

Table II: hybrid estimation. Speedup = median girth-only exact time / mean hybrid time. MARE (%) is the mean absolute relative error multiplied by 100 over all outputs, including exact returns.

A--D graph labels specify the base, lift size, and cyclic (`c`) or random (`r`) lift. R/P labels give variable-node counts. The generators and manifest provide construction details. The benchmarks include variable degrees 2, 3, and 4 and girths 6, 8, 10, and 12. They are synthetic Tanner graphs, not standardized LDPC codes. Cactus graphs are mathematical correctness tests. No decoding-performance or matrix-rank claim is made.

## Independent checks and limits

The KB routine is our own implementation based on Karimi and Banihashemi, arXiv:1004.3966, not the authors' original software. It is used only for targets below 2g. The unrestricted exact search is also checked against canonical simple-cycle DFS and cactus constructions.

Edge-list files start with `n m E`, followed by zero-based variable/check endpoint pairs. The C++ implementation uses unsigned 64-bit counters; the supplied benchmark counts fit, but larger inputs/targets can overflow and require arbitrary-precision counters. Very long targets can entail exponential work.

## Current high-girth, cycle-rich selection (2026-10-09)

Both tables keep their original row counts (9 exact, 13 hybrid). Earlier measurements and selections remain in files ending `_original` or `_diverse_v1`; only the current CSV files correspond to the current tables. All current selected rows were freshly timed under one protocol.

`generate_diverse.py` builds socket-pairing graphs repaired by switches (R) and our degree-constrained PEG variant (P). Repaired graphs are not claimed to be uniformly distributed. PEG prioritizes distance, then minimum degree, then random ties among checks with remaining capacity; it restarts if completion without four-cycles fails.

`generate_highgirth.py` adds four positive-rate, column-weight-two **cycle-code** benchmarks, not cactus tests:

| Label | Ordinary graph | Tanner degree pair | Lift | Girth | Exact count |
|---|---|---|---|---|---|
| Pet64 | Petersen | (2,3) | 64 | 10 | 512 |
| HS128 | Hoffman--Singleton | (2,7) | 128 | 10 | 156672 |
| F3c32 | projective-plane incidence graph over F3 | (2,4) | 32 | 12 | 6624 |
| F5c64 | projective-plane incidence graph over F5 | (2,6) | 64 | 12 | 240000 |

An ordinary edge becomes a variable and an ordinary vertex becomes a check, doubling ordinary girth. The Tanner incidence graphs are cyclically lifted, shifting the lexicographically first incidence edge by one and leaving other permutations as identity. This produces connected regular graphs with cycles inherited from their structured bases. It is a deliberate cycle-rich regime; these examples do not represent arbitrary degree-three LDPC families or establish decoding quality. We retain degree-three and degree-four samples and both longer-target rows favoring KB.

Independent cycle-count identities: the Petersen and Hoffman--Singleton graphs have 12 and 1260 girth-five cycles; an edge lies on 4 and 36, respectively. For a projective plane of prime order q, the girth-six count of its incidence graph is C(q^2+q+1,3) minus (q^2+q+1)C(q+1,3), and the cycles through an edge number six times the count divided by the edge count. A one-edge nonzero voltage removes those base girth cycles from the lifted girth count. Consequently the counts per lift copy are 8, 1224, 207, and 3750. Exhaustive non-backtracking enumeration and KB independently confirm every reported count.

The four high-girth samples use inverse sampling in all 100 runs. HS128, F3c32, and F5c64 gain in runtime; Pet64 retains the opposite outcome because exact counting is already cheap. A16c provides mixed routing, while A128r, R1200, and PEG instances demonstrate exact fallback.

`verify_extended.py` checks all eight added graphs for simplicity, connectivity, exact degrees, girth, and hashes. `verification_extended.json` records its output. `paper_table_replacements.tex` contains construction, both tables, updated interpretation, and bibliography entries.

References: Hu, Eleftheriou, and Arnold, IEEE TIT 51(1), 386--398 (2005), doi:10.1109/TIT.2004.839541 (PEG); Malema and Liebelt, EURASIP JWCN 2007, 048158, doi:10.1155/2007/48158 (column-weight-two LDPC construction). The Hoffman--Singleton generator uses the pentagon/pentagram construction documented by NetworkX; the implementation here is independent and has no third-party package dependency.
