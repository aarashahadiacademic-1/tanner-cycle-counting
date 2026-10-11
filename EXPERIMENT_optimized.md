# Section 4 experiment record

`paper_table_current.tex` and `paper_table_current.csv` are the consolidated manuscript table snapshots. The CSV has one row per graph and target. `Girth only` rows report only the target `T=g`; `Multiple cycle lengths` rows report the listed count tuple through `T`.

The table compares selected algorithms for which published descriptions and implementation details were sufficiently accessible for an independent implementation and count validation. It is not an exhaustive implementation of every method cited in the paper. KB, IMPA, and our exact method count the same requested targets. The hybrid experiment counts only `N_g` and reports mean runtime and MARE over 100 runs; KB and standalone Exact are medians of seven runs.

The comparison implementations follow the cited algorithms but are not the original authors' software. Technical implementation choices can differ from the authors' own code. The main exact/KB/hybrid measurements and the recorded IMPA measurements have their compiler and run details in their respective source records; specifically, the IMPA timing record was produced with g++ 13.3.0. The manuscript's general benchmark description lists g++ 14.2 for its main measurements. Fresh runs are provided for audit and may have different times due to compiler, CPU scheduling, and machine load.

## Reruns

- `python3 run_exact_table.py` compiles `counter.cpp` and measures all exact-table graph/target pairs, including the KB baseline.
- `python3 run_optimized.py` compiles `interleaved_optimized.cpp` and runs 100 hybrid repetitions for each girth-only graph in the table.
- `python3 algorithm_sources/run_impa_bench.py` recompiles `impa_count.cpp` and records seven IMPA timing runs per configured target.

All graph construction is untimed. The data files preserve the table snapshot and the IMPA timing records; a fresh runner output is a new measurement, not a replacement for the reported snapshot.
