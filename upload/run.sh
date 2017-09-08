
if [ $# -ge 1 ]; then
	DB_LOCATION=$1
else
	DB_LOCATION="mysql://root:root@localhost/ethoscope_db"
fi

echo "Uploading SQLite file to $DB_LOCATION"
time ../sqlalchemyBulkInsert/convertDB.py -l sqlite:///inputData.sqlite -o "$DB_LOCATION" -b 20000
