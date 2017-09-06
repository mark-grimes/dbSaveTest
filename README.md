# Speed tests on converting MySQL database to SQLite


-------------------------------------------------------------------
| Method                     | Real       | User      | Sys       |
|----------------------------|------------|-----------|-----------|
| sqlDumpAndExec             | 4m51.927s  | 5m18.130s | 0m5.070s  |
| sqlalchemy1                | gave up after 8 min |  |           |
| sqlalchemyBulkInsert 200   | 14m54.983s | 7m42.980s | 0m19.990s |
| sqlalchemyBulkInsert 2000  | 8m13.869s  | 6m44.830s | 0m3.770s  |
| sqlalchemyBulkInsert 20000 | 7m31.308s  | 6m48.250s | 0m2.700s  |
-------------------------------------------------------------------
