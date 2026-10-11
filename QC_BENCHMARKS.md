# Q410 and Q412 graph construction

Q410 and Q412 are the two modified quasi-cyclic Tanner graphs used in the
October 11 manuscript. Their measured hybrid results are in
`measurements_optimized.csv`; their table values are preserved in
`paper_table2_hybrid_20261011.csv`. This note documents graph construction
only and does not report timings from the earlier pilot-and-cap method.

The construction starts from Eq. (36) of G. Zhang and X. Wang,
“Girth-12 Quasi-Cyclic LDPC Codes with Consecutive Lengths”
(https://arxiv.org/abs/1001.3916). The starting exponent matrix is:

```
0  0  0   0   0   0
0  3 14  18  24  26
0 19 62 107 170 224
```

For the (3,4) examples, retain the first four columns. A block with exponent
(a) and circulant size (Z) connects variable ((j,i)) to check
((k,i+a mod Z)). The instances use one modified shift each, as recorded
in `manifest_qc.json`; they are modified matrices and their results are not
attributed to Zhang and Wang.

## Regeneration and checks

Run:

```sh
python3 generate_qc.py
python3 run_optimized.py
```

The generator writes deterministic edge lists under `graphs/`. Their
SHA-256 hashes and expected girth-cycle counts are recorded in
`manifest_qc.json`; the Q410/Q412 rows in `measurements_optimized.csv`
also record the edge-list hashes and exact counts. The hybrid timing protocol
and weighted-credit scheduler are described in `EXPERIMENT_optimized.md`.