/** @file
 * @brief Program to copy a MySQL database to an SQLite file
 * @author Mark Grimes (mark.grimes@rymapt.com)
 * @date 10/Sep/2017
 * @copyright Copyright Rymapt 2017, released under the MIT licence (available from https://opensource.org/licenses/MIT)
 *
 * @details
 * This started out as a speed test so it's a bit messy at the moment. I was getting incredibly slow
 * results using python and SQLAlchemy so thought I'd see if using the C interfaces for MySQL and
 * SQLite made any difference.
 */

#include <iostream>
#include "mySQLBackup.h"

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
	}
	catch( const std::exception& error )
	{
		std::cerr << "Exception: " << error.what() << std::endl;
		return -1;
	}

	return 0;
}
