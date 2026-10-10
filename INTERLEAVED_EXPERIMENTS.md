# Single-processor interleaved hybrid benchmark (preliminary)

Use `interleaved.cpp` with the original `counter.cpp`. This implements a resumable BFS/DFS exact girth search and non-backtracking inverse sampling on **one thread**, with alternating fixed elementary-step budgets. It does not use the old pilot, cap, or two-core race.

Compile: `g++ -std=c++17 -O3 interleaved.cpp -o interleaved`
Run: `./interleaved graphs/A128c.edges 6 0.08 10 20261010 256`

The accompanying `interleaved_results_preliminary.csv` records 10 independent repetitions on each of 13 older benchmark graphs, epsilon=0.08, target r=101 and Q=256. These were run locally; results are preliminary. Comparisons with the optimized standalone exact implementation include substantial state-machine overhead and should not be presented as tuned throughput. No claim of uniform speedups is made. The existing official experiment CSVs and exact/KB results are preserved.

`section4_interleaved.tex` is a complete Section IV replacement, leaving the benchmark construction and exact-method comparisons substantively unchanged. The main manuscript has not been pushed; replace Section IV only after verifying against the latest Section III and rerunning larger repeated batches.