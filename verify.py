import hashlib, json, pathlib
P = pathlib.Path(__file__).parent
checks = []
for manifest_name in ("manifest.json", "manifest_diverse.json", "manifest_qc.json"):
    manifest = json.loads((P / manifest_name).read_text())
    for row in manifest:
        path = P / "graphs" / f"{row['name']}.edges"
        lines = path.read_text().splitlines()
        n, m, edge_count = map(int, lines[0].split())
        edges = [tuple(map(int, line.split())) for line in lines[1:]]
        assert n == row["n"] and m == row["m"]
        assert edge_count == len(edges) == len(set(edges))
        assert all(0 <= u < n and 0 <= w < m for u, w in edges)
        if "dv" in row:
            du, dw = [0] * n, [0] * m
            for u, w in edges:
                du[u] += 1; dw[w] += 1
            assert all(x == row["dv"] for x in du)
            assert all(x == row["dc"] for x in dw)
        digest = hashlib.sha256(path.read_bytes()).hexdigest()
        if row.get("sha256"):
            assert digest == row["sha256"]
        checks.append({"name": row["name"], "sha256": digest,
                       "simple": True, "degrees_checked": "dv" in row})
(P / "verification.json").write_text(json.dumps(checks, indent=2) + "\\n")
print(f"Verified {len(checks)} table graph files.")
