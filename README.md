# Tanner-graph cycle counting — reproducibility materials

Code, graph definitions, realized edge lists, validation records, and timing data for exact and randomized Tanner-graph cycle counting.

## October 11 manuscript tables

- `paper_table1_exact_20261011.csv` and `paper_table2_hybrid_20261011.csv` are row-by-row snapshots of the submitted manuscript tables. They preserve the reported counts and aggregate timings; they are table snapshots, not raw per-repetition logs.
- `interleaved_optimized.cpp`, `run_optimized.py`, and `measurements_optimized.csv` provide the weighted-credit single-processor interleaved implementation, rerun script, and archived Q=4096 measurement record.
- `run_exact_table.py` reruns the exact-versus-Karimi–Banihashemi targets in Table 1 and writes fresh results to `exact_table_rerun.csv`.
- `EXPERIMENT_optimized.md` documents the settings, cost model, and relationship between the manuscript snapshots and the archived/rerun measurements.

The archived Q=4096 record uses the same eleven graph instances and reported configuration (`epsilon=0.08`, `r=101`, `Q=4096`, 100 repetitions). Its timing values differ from the manuscript's table snapshot. The snapshots preserve the values printed in the manuscript; the archived record preserves a separate measured run. Fresh reruns are machine- and load-dependent. Do not present these timing records as interchangeable or describe the table snapshots as raw repetitions.

## Earlier or separate measurements

- `interleaved_threeway.cpp`, `run_three_way.py`, and `three_way_interleaved_11.csv` record the preliminary Q=256 implementation/run. They are not the manuscript's Q=4096 weighted-credit configuration.
- `counter.cpp` includes a legacy pilot-and-cap routine. `table2_hybrid.csv` is from that earlier method and is not the current single-processor interleaved result.
- `table1_exact.csv`, `results.json`, `results_qc.json`, `export_tables.py`, and `run_tables.py` preserve earlier exact/KB and pilot-and-cap records.

## Graphs and validation

- `generate.py`, `generate_diverse.py`, `generate_qc.py`, and `manifest*.json` specify graph families and parameters.
- `graphs/` contains supplied edge lists for the benchmark and validation instances. Q410 and Q412 are generated using `generate_qc.py`; see `manifest_qc.json` for their hashes.
- `verify.py`, `verify_diverse.py`, `verify_extended.py`, `verification*.json`, `counter_qc.cpp`, and `run_qc.py` contain additional verification utilities and records.

Run `python3 run_exact_table.py` and `python3 run_optimized.py` from the repository root to generate fresh machine-dependent records. Do not compile checking programs with `-DNDEBUG`, because some validation assertions also execute checks.

All graphs are synthetic Tanner-graph instances, not standardized LDPC codes. Results from different scheduling policies, code versions, or quantum sizes are distinct measurements.