#include "TestTable.h"
#include <sqlpp11/insert.h>
#include <sqlpp11/select.h>
#include <sqlpp11/sqlite3/connection.h>
#include <sqlpp11/mysql/connection.h>
#include <iostream>

template<class T>
void copyTable( sqlpp::mysql::connection& inputDb, sqlpp::sqlite3::connection& outputDb, const T table )
{
	const std::string tableName( sqlpp::name_of<T>::template char_ptr<void>() );
	outputDb.execute("BEGIN TRANSACTION");
	outputDb.execute("DROP TABLE IF EXISTS `"+tableName+"`;");
	outputDb.execute("CREATE TABLE `"+tableName+"` ( \
						`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
						`t` INTEGER DEFAULT NULL, \
						`x` SMALLINT DEFAULT NULL, \
						`y` SMALLINT DEFAULT NULL, \
						`w` SMALLINT DEFAULT NULL, \
						`h` SMALLINT DEFAULT NULL, \
						`phi` SMALLINT DEFAULT NULL, \
						`xy_dist_log10x1000` SMALLINT DEFAULT NULL, \
						`is_inferred` SMALLINT DEFAULT NULL, \
						`has_interacted` SMALLINT DEFAULT NULL \
					);");

	auto preparedSelectAll = inputDb.prepare(sqlpp::select(all_of(table)).from(table).unconditionally());
	auto insert = insert_into(table).columns(table.t, table.x, table.y, table.w, table.h, table.phi, table.xyDistLog10x1000, table.isInferred, table.hasInteracted );

	bool hasData=false;
	for( const auto& row : inputDb(preparedSelectAll) )
	{
		hasData=true;
		insert.values.add(table.t = (int)row.t,
			table.x = (int)row.x,
			table.y = (int)row.y,
			table.w = (int)row.w,
			table.h = (int)row.h,
			table.phi = (int)row.phi,
			table.xyDistLog10x1000 = (int)row.xyDistLog10x1000,
			table.isInferred = (int)row.isInferred,
			table.hasInteracted = (int)row.hasInteracted
		);
	};
	if( hasData )
	{
		auto last_insert_rowid = outputDb(insert);
		// std::cout << "Inserted " << last_insert_rowid << " rows into " << tableName << "\n";
	}
	outputDb.execute("END TRANSACTION");
}

void mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile )
{
	// std::cout << "Starting" << std::endl;
	auto inputConfig = std::make_shared<sqlpp::mysql::connection_config>();
	inputConfig->user = user;
	inputConfig->password = password;
	inputConfig->database = database;
	inputConfig->host = host;
	sqlpp::mysql::connection inputDb(inputConfig);

	sqlpp::sqlite3::connection_config outputConfig;
	outputConfig.path_to_database = outputFile;
	outputConfig.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	sqlpp::sqlite3::connection outputDb(outputConfig);

	outputDb.execute("PRAGMA synchronous=OFF");
	outputDb.execute("PRAGMA count_changes=OFF");
	outputDb.execute("PRAGMA journal_mode=MEMORY");
	outputDb.execute("PRAGMA temp_store=MEMORY");

	copyTable( inputDb, outputDb, testtable::ROI1{} );
	copyTable( inputDb, outputDb, testtable::ROI2{} );
	copyTable( inputDb, outputDb, testtable::ROI3{} );
	copyTable( inputDb, outputDb, testtable::ROI4{} );
	copyTable( inputDb, outputDb, testtable::ROI5{} );
	copyTable( inputDb, outputDb, testtable::ROI6{} );
	copyTable( inputDb, outputDb, testtable::ROI7{} );
	copyTable( inputDb, outputDb, testtable::ROI8{} );
	copyTable( inputDb, outputDb, testtable::ROI9{} );
	copyTable( inputDb, outputDb, testtable::ROI10{} );
	copyTable( inputDb, outputDb, testtable::ROI11{} );
	copyTable( inputDb, outputDb, testtable::ROI12{} );
	copyTable( inputDb, outputDb, testtable::ROI13{} );
	copyTable( inputDb, outputDb, testtable::ROI14{} );
	copyTable( inputDb, outputDb, testtable::ROI15{} );
	copyTable( inputDb, outputDb, testtable::ROI16{} );
	copyTable( inputDb, outputDb, testtable::ROI17{} );
	copyTable( inputDb, outputDb, testtable::ROI18{} );
	copyTable( inputDb, outputDb, testtable::ROI19{} );
	copyTable( inputDb, outputDb, testtable::ROI20{} );

	// std::cout << "Finished" << std::endl;
}

int main( int argc, const char* argv[] )
{
	std::cout.sync_with_stdio(false);
	try
	{
		const char* defaultArgs[]={ "localhost", "ethoscope", "ethoscope", "ethoscope_db", "testdb.sqlite" };
		const char** args[]={ &defaultArgs[0], &defaultArgs[1], &defaultArgs[2], &defaultArgs[3], &defaultArgs[4] };
		for( int index=1; index<argc; ++index ) args[index-1]=&argv[index];
		std::cout << "Database host     = " << *args[0] << "\n"
			<< "Database username = " << *args[1] << "\n"
			<< "Database password = " << *args[2] << "\n"
			<< "Database database = " << *args[3] << "\n"
			<< "Output filename   = " << *args[4] << "\n";
		mySQLBackup( *args[0], *args[1], *args[2], *args[3], *args[4] );
		return 0;
	}
	catch( const std::exception& error )
	{
		std::cerr << "Got exception " << error.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cerr << "Got unknown exception" << std::endl;
		return -2;
	}
}
