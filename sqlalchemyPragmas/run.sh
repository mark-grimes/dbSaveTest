
function copyDb {
	echo "Batch size $1"
	time ./convertDB.py -l mysql://ethoscope:ethoscope@localhost/ethoscope_db -o sqlite:///sqlalchemyPragmas_${1}.sqlite -b $1
}

copyDb 20000
