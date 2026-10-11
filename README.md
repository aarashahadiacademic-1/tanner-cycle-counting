# Tanner-graph cycle counting — reproducibility materials

Code, graph generators, realized edge lists, validation tools, and measurement records for exact cycle counting and single-processor interleaved inverse-sampling experiments on Tanner graphs.

## Key files

- `counter.cpp`: exact local BFS/DFS counting and the independent Karimi–Banihashemi benchmark implementation. It also contains a **legacy pilot-and-cap** routine, which is **not** the current interleaved hybrid.
- `interleaved_threeway.cpp`: resumable, single-processor exact-versus-sampling implementation (no pilot or cap).
- `run_three_way.py` and `three_way_interleaved_11.csv`: saved eleven-graph interleaved run, with `epsilon=0.08`, `r=101`, and `Q=256`. These are a **separate measured configuration**, not the manuscript's `Q=4096` work-credit configuration; do not substitute their runtimes for manuscript entries.
- `generate.py`, `generate_diverse.py`, `generate_qc.py` and `manifest*.json`: graph definitions and parameters.
- `graphs/`: supplied edge lists for standard and correctness-test instances. Q410 and Q412 are generated using `generate_qc.py`; see `manifest_qc.json` for hashes.
- `verify.py`, `verify_diverse.py`, `verify_extended.py`, `verification*.json`: checks and recorded validation.
- `counter_qc.cpp` and `run_qc.py`: additional high-girth verification/measurement utilities.
- `results.json`, `results_qc.json`, `table1_exact.csv`, `table2_hybrid.csv`, `export_tables.py`, `run_tables.py`: preserved earlier exact/KB and pilot-and-cap measurement records. **Do not interpret `table2_hybrid.csv` as a single-processor interleaved result.**

## Re-run the preserved eleven-graph interleaved measurement

```sh
g++ -std=c++17 -O3 interleaved_threeway.cpp -o interleaved_threeway
python3 run_three_way.py
```

The runner creates `three_way_interleaved_rerun.csv` using the same graph instances and Q=256. Wall-clock measurements depend on the machine and its load. Build and run the exact/KB checks with `counter.cpp` and the supplied graph lists as appropriate. Do not compile the checking programs with `-DNDEBUG`, because some validation assertions also execute checks.

All graphs are synthetic Tanner-graph instances, not standardized LDPC codes. Data from distinct scheduling policies, code versions, or quantum sizes must not be presented as interchangeable.

Only actively referenced source, graph, validation, and measurement materials are retained at the repository root. Prior exploratory scripts, reports, and drafts were removed from the current tree; earlier commits retain their history.
