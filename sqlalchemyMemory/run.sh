
function copyDb {
	echo "Batch size $1, copying into memory only"
	time ../sqlalchemyBulkInsert/convertDB.py -l mysql://ethoscope:ethoscope@localhost/ethoscope_db -o sqlite:// -b $1
}

copyDb 20000
