
if [ $# -ge 1 ]; then
	DB_LOCATION=$1
else
	DB_LOCATION="mysql://root:root@localhost/ethoscope_db"
fi

if [ ! -f inputData.sqlite ]; then
	echo "No input file present. Attempting to download from https://github.com/mark-grimes/dbSaveTest/releases/download/v0.0.1/inputData.sqlite"
	wget "https://github.com/mark-grimes/dbSaveTest/releases/download/v0.0.1/inputData.sqlite"
fi

echo "Uploading SQLite file to $DB_LOCATION"
time ../sqlalchemyBulkInsert/convertDB.py -l sqlite:///inputData.sqlite -o "$DB_LOCATION" -b 20000
