
function copyDb {

	echo "Batch size $1"
	time ./convertDB.py -l mysql://ethoscope:ethoscope@localhost/ethoscope_db -o sqlite:///sqlalchemyBulkInsert_${1}.sqlite -b $1
}

copyDb 200
copyDb 2000
copyDb 20000
