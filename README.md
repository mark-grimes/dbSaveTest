# Speed tests on converting MySQL database to SQLite

Obviously all times are dependant on hardware, so only consider these relative to each other. There
will always be slight variation but the figures shown here are pretty stable.

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
| sqlalchemyFetchAll         | 7m5.355s   | 6m19.440s | 0m3.340s  |
| customCpp                  | 1m10.478s  | 0m45.390s | 0m1.280s  |
-------------------------------------------------------------------

After these tests I got a new laptop (MacBookPro with 2.3 GHz Intel Core i5 and SSD), so the following
can't be compared to the table above.

--------------------------------------------------------------------
| Method                      | Real       | User      | Sys       |
|-----------------------------|------------|-----------|-----------|
| customCpp (native)          | 0m4.441s   | 0m2.493s  | 0m0.323s  |
| customCpp (docker container)| 0m7.08s    | 0m 4.04s  | 0m 0.98s  |
| sqlpp11                     | 0m28.708s  | 0m24.231s | 0m3.449s  |
--------------------------------------------------------------------

Interestingly the docker container adds a slight overhead; this time doesn't include spinning up the container.
I suspect this is either better compiler optimisation or performance of the docker I/O driver. It's minimal
compared to the code differences so I can't be bothered figuring out more details.

sqlpp11 doesn't copy all the data either. See the readme in that folder.
