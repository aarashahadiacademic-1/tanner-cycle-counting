## Broadened benchmark selection (2026-10-09)

Four base construction families are represented: affine incidence (A--C), split symplectic generalized quadrangle (D), random socket pairing repaired by switches (R), and our degree-constrained PEG variant (P).

Table I retains 9 rows: A256r at T=6, A128r at T=8, D16r at T=8, and D16r at T=10 are replaced by R1200, R2400, P1200, and P1600 at their girths. The two longer-target rows favoring KB remain.
Table II retains 13 rows: A8c, A16c, A256r, and A256c are replaced by the same four new graphs. The matched A128, B128, C128, and D16 cyclic/random pairs remain. All selected rows were freshly measured under the same protocol; no old/new timing mixture is used. The new samples use fixed predeclared seeds and are not selected for runtime outcomes.

`generate_diverse.py` constructs the four R/P samples. Socket pairing rejects parallel edges; degree-preserving two-switches remove four-cycles without introducing new ones. These repaired graphs are **not claimed to be uniform draws** from the girth-constrained ensemble. PEG attaches edges at maximum distance among checks with remaining degree capacity, breaks ties by minimum degree then random selection, and restarts if completion without four-cycles becomes impossible. This is our implementation of a degree-constrained variant, not the original authors' code or standardized codes.

The R1200/R2400 degree pairs are (4,8)/(3,6); P1200/P1600 use (3,6)/(3,4) and realized girths 8/10. All four new samples route to exact fallback in all 100 runs, illustrating limits of hybrid acceleration rather than concealing them. `verification_diverse.json` records independent degree, girth, connectivity, simplicity, and edge-hash checks. The C++ harness additionally checks count equality against exhaustive NB enumeration and KB.

PEG reference: X.-Y. Hu, E. Eleftheriou, and D. M. Arnold, IEEE Transactions on Information Theory 51(1), 386--398 (2005), doi:10.1109/TIT.2004.839541.

`paper_table_replacements.tex` supplies the construction subsection, revised tables, interpretation paragraph, and PEG bibliography entry.
