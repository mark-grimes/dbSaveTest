# Speed tests on converting MySQL database to SQLite


-------------------------------------------------------------------
| Method                     | Real       | User      | Sys       |
|----------------------------|------------|-----------|-----------|
| sqlDumpAndExec (full)      | 4m51.927s  | 5m18.130s | 0m5.070s  |
| sqlDumpAndExec (read to /dev/null) | 0m21.239s | 0m32.290s | 0m0.880s |
| sqlDumpAndExec (read to file)      | 0m29.755s | 0m32.290s | 0m0.880s |
| sqlDumpAndExec (write from file)   | 4m49.877s | 4m43.050s | 0m2.570s |
| sqlalchemy1                | gave up after 8 min |  |           |
| sqlalchemyBulkInsert 200   | 14m54.983s | 7m42.980s | 0m19.990s |
| sqlalchemyBulkInsert 2000  | 8m13.869s  | 6m44.830s | 0m3.770s  |
| sqlalchemyBulkInsert 20000 | 7m31.308s  | 6m48.250s | 0m2.700s  |
| sqlalchemyMemory     20000 | 7m7.145s   | 6m48.540s | 0m1.560s  |
| sqlalchemyPragmas    20000 | 7m8.928s   | 6m49.700s | 0m2.420s  |
| sqlalchemyReadOnly         | 2m23.909s  | 2m6.470s  | 0m0.680s  |
| customCpp                  | 1m10.478s  | 0m45.390s | 0m1.280s  |
-------------------------------------------------------------------
