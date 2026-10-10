# Two-thread racing hybrid — preliminary experimental update

The original exact-vs-KB experiments and files remain untouched.
The added `race_parallel.cpp` runs exact girth counting concurrently with length-g inverse-binomial sampling, using two `std::thread` workers in one process. The first result wins and the exact worker supports cooperative cancellation. No pilot screening and no cap c*n*omega are used.

The user-specified MARE tolerance is epsilon = 0.08 for these experiments; r = 101 is the smallest integer satisfying the Mendo stopping-target inequality. 100 runs are performed on each of the 13 graphs previously selected for Table II, with independent seeds. Speedup is the median seven-run exact time divided by mean first-winner latency. MARE includes exact returns. The CSV contains the full measured data, including the number of exact/random wins and the average number of trial walks.

**Important measurement limitation:** The current timer stops upon the first winner **before** `join()` completes. Reported time is first-winner latency rather than total process completion time. Cancellation, processor contention and multicore availability affect timings. On sparse graphs, race time can be longer than exact-only time. Do not interpret these data as a single-processor guarantee or a general wall-clock bound.

Run:
```sh
g++ -std=c++17 -O3 -pthread race_parallel.cpp -o race_parallel
./race_parallel graphs/A128c.edges 6 0.08 100 20261010
```
The companion `section4_hybrid_race.tex` contains a replacement for only the hybrid experimental subsection of Section IV; the benchmark-construction and exact-comparison subsections should remain unchanged. The old official paper tables/results have not been overwritten.
