# Column-weight-three high-girth QC examples

These are supplementary candidates for the hybrid-estimation table. Existing
paper tables and their measured data are left intact. The construction uses
Eq. (36) of G. Zhang and X. Wang, *Girth-12 Quasi-Cyclic LDPC Codes with
Consecutive Lengths*, https://arxiv.org/abs/1001.3916.

Starting exponents (rows are check blocks, columns variable blocks):

```
0  0  0   0   0   0
0  3 14  18  24  26
0 19 62 107 170 224
```

For (3,4), retain the first four columns. Each specified circulant block
connects variable `(column,i)` to check `(row,i+exponent mod Z)`.
The examples are our modified matrices; their counts and timings are not
results quoted from Zhang and Wang. Each is a cyclic cover of a smaller QC
base: increase one exponent by the base circulant size and replace that
size by its stated integer multiple. The block was selected, among all
single-block perturbations, to retain the most girth cycles. This intentionally
studies cycle-rich graphs, and is not a uniform random LDPC ensemble or a
decoding-performance comparison.

`manifest_qc.json` records all dimensions, exponents, girths, counts and
SHA-256 hashes. Degrees are (3,4), constant throughout each graph. Their block lengths are
40,448 and 36,352; no claim is made that they are standardized codes.

## Reproduction

```
python generate_qc.py
g++ -O3 -std=c++17 -Wall -Wextra counter_qc.cpp -o counter_qc
python run_qc.py
```

The generated plaintext edge lists are deterministic and can be checked
against the manifest hashes; these large additional edge lists are generated
locally rather than committed. Python has no external package dependencies.
Never compile with `-DNDEBUG`, because correctness assertions execute the
timed calls.

`counter_qc.cpp` keeps the exact counting and randomized trial algorithms
unchanged from `counter.cpp`, as well as the pilot/inverse-sampling/fallback
logic. Exact time is the median of seven full-graph calls. Hybrid time is the
mean of 100 full-graph calls, including pilot and fallback. Each trial samples
from every variable in the complete graph. Seed: `20261009+n`; inverse target
100 successes; pilot length n; cap 10n. No symmetry is used in either timed
algorithm.

## Independent validation

The simultaneous shift `i -> i+1 mod Z` of all variable and check blocks is
an explicitly checked graph automorphism. Validation uses one representative
per cyclic orbit for BFS girth determination and exhaustive nonbacktracking
walk enumeration; each orbit has exactly Z vertices. The walk counts are
multiplied by Z and divided by g, and compared with the exact algorithm on the
complete graph. At girth g, a closed nonbacktracking walk of length g is a
simple girth cycle, so this is an independent verification of N_g. Connectivity
is checked on the complete graph. This symmetry reduction speeds only
validation and does not change reported exact or hybrid times.

No KB timing is reported for these large supplementary candidates.
`routes=[pilot fallback, cap fallback, inverse sampling]` in the raw results.
The previous KB comparisons in the main tables are unaffected.
