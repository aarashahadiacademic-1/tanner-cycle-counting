# Q410 and Q412 graph construction

Q410 and Q412 are the modified quasi-cyclic Tanner graphs used in the current consolidated manuscript table. Their rows and timings are in `paper_table_current.csv`; this note documents their graph construction and does not attribute the measured results to Zhang and Wang.

The starting exponent matrix is Eq. (36) of G. Zhang and X. Wang, “Girth-12 Quasi-Cyclic LDPC Codes with Consecutive Lengths” (https://arxiv.org/abs/1001.3916):

```
0  0  0   0   0   0
0  3 14  18  24  26
0 19 62 107 170 224
```

For these (3,4) instances, the construction retains the first four columns and modifies one shift in each graph, as recorded in `manifest_qc.json`. The matrices and resulting graphs are specified by `generate_qc.py` and `graphs/Q410.edges`, `graphs/Q412.edges`.

Regenerate the graphs with:

```sh
python3 generate_qc.py
python3 run_exact_table.py
python3 run_optimized.py
```

The edge-list hashes and expected girth-cycle counts are recorded in `manifest_qc.json`. Fresh timing results are machine- and load-dependent; the consolidated manuscript values remain in `paper_table_current.csv`.
