echo "Testing query only, with output to /dev/null"
time (./mysql2sqlite.sh -u ethoscope -pethoscope ethoscope_db > /dev/null)
echo "Testing query only, with output to file"
time (./mysql2sqlite.sh -u ethoscope -pethoscope ethoscope_db > ./sqlDumpAndExec.sql.txt)
echo "Testing write from previous file"
time (sqlite3 sqlDumpAndExec_write.sqlite < sqlDumpAndExec.sql.txt)
echo "Testing full query and write to SQLite"
time (./mysql2sqlite.sh -u ethoscope -pethoscope ethoscope_db | sqlite3 sqlDumpAndExec.sqlite)
