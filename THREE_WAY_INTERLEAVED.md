# Three-way, same-target experiment (October 10, 2026)

The original **11** graphs are A16c, A128r, A128c, B128c, C128c, D16c, R1200, P1200, P1600, Q410 and Q412. No degree-two graphs are used. Table I of the older two-table experiment is retained as archival longer-target evidence; the new Table I compares **only N_g** among KB, our standalone exact routine and our operation-interleaved single-processor hybrid.

Protocol: GNU g++ 14.2, -O3, AMD EPYC 9V74 shared container, 7 median runs for each exact method and 100 hybrid repetitions per graph; epsilon=0.08, r=101, operation-state-machine quantum Q=256. The accuracy is empirical MARE over all outputs. Fixed-quantum C++ state-machine transitions approximate elementary-operation allocation; transition costs and overheads are not equal in wall-clock time. The single-core implementation is slower than optimized exact on some sparse graphs and can be slower even when the random branch wins. No uniform wall-clock dominance is claimed.

Source `interleaved_threeway.cpp` and `run_three_way.py` reproduce the benchmark. Build: `g++ -O3 -std=c++17 interleaved_threeway.cpp -o interleaved_threeway`; run `python3 run_three_way.py`. The original edge lists are used. `generate_qc.py` and `generate_diverse.py` reproduce the QC and R/P graphs, respectively. All 11 N_g values were independently checked against the target counts; Q410/Q412 generated SHA256 hashes match `manifest_qc.json`. The results file `three_way_interleaved_11.csv` contains measured values, not estimates.

Implementation note: ExactGirth is resumable for T=g through explicit BFS/DFS frames and bookkeeping. A sampler trial is resumable, with fresh uniform root and nonbacktracking walk. The first completed procedure wins. This practical state-machine version currently incurs significant overhead on some closure-sparse instances.

Avoid using the old `table2_hybrid.csv` as an interleaved benchmark; those results belong to a previous pilot+cap algorithm.
