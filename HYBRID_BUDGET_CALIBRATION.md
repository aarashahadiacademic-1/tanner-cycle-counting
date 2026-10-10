# Preliminary cost-budget calibration (2026-10-10)

This is an **experimental branch of the methodology**, not a replacement for the manuscript tables.

We examine the single-stage inverse-binomial method: choose r as the smallest integer satisfying 2(r-1)^(r-1) exp(-(r-1))/(r-1)! <= epsilon, sample until r closures or K=floor(c n omega) trials, and fall back to exact girth counting if the cap is reached. For T=g, omega=d_v (d_c-1)^ceil(g/4) (d_v-1)^floor(g/4).

A preliminary run on 17 graphs from the October 8 reproducibility archive used epsilon=0.08, c=0.005, and 100 independent runs per graph. The accompanying C++ benchmark provides the reproducible procedure; original paper tables and baseline code remain unchanged.

The coefficient c=0.005 is a **candidate, not a proven or cross-family optimum**. Some sparse instances remain significantly slower than exact counting. Additional held-out experiments, especially on the newer g=10 and g=12 benchmark instances, must be completed before replacing Section IV or the official CSVs. Timing outcomes vary by hardware, and the ratio of hidden constants is not universal.

Sample locally:

```bash
g++ -std=c++17 -O3 hybrid_budget.cpp -o hybrid_budget
./hybrid_budget graphs/A128c.edges 6 0.08 0.005 100 20261010
```

This benchmark intentionally does not overwrite results.json or table2_hybrid.csv.
