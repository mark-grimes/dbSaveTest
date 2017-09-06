# Speed tests on converting MySQL database to SQLite


-------------------------------------------------------------------
| Method                     | Real       | User      | Sys       |
|----------------------------|------------|-----------|-----------|
| sqlDumpAndExec             | 4m51.927s  | 5m18.130s | 0m5.070s  |
| sqlalchemy1                | gave up after 8 min |  |           |
| sqlalchemyBulkInsert 200   | 14m54.983s | 7m42.980s | 0m19.990s |
| sqlalchemyBulkInsert 2000  | 8m13.869s  | 6m44.830s | 0m3.770s  |
| sqlalchemyBulkInsert 20000 | 7m31.308s  | 6m48.250s | 0m2.700s  |
| sqlalchemyMemory     20000 | 7m7.145s   | 6m48.540s | 0m1.560s  |
| sqlalchemyPragmas    20000 | 7m8.928s   | 6m49.700s | 0m2.420s  |
| sqlalchemyReadOnly         | 2m23.909s  | 2m6.470s  | 0m0.680s  |
-------------------------------------------------------------------
