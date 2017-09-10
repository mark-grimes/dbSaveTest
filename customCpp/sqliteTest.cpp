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
#include <string>
#include <functional>
#include <vector>
#include <sqlite3.h>
#include <mysql/mysql.h>
#include <cstring> // required for memset

const char* liteResultCodeToString( int result )
{
	switch( result )
	{
		case SQLITE_OK: return "SQLITE_OK";
		case SQLITE_ERROR: return "SQLITE_ERROR";
		case SQLITE_INTERNAL: return "SQLITE_INTERNAL";
		case SQLITE_PERM: return "SQLITE_PERM";
		case SQLITE_ABORT: return "SQLITE_ABORT";
		case SQLITE_BUSY: return "SQLITE_BUSY";
		case SQLITE_LOCKED: return "SQLITE_LOCKED";
		case SQLITE_NOMEM: return "SQLITE_NOMEM";
		case SQLITE_READONLY: return "SQLITE_READONLY";
		case SQLITE_INTERRUPT: return "SQLITE_INTERRUPT";
		case SQLITE_IOERR: return "SQLITE_IOERR";
		case SQLITE_CORRUPT: return "SQLITE_CORRUPT";
		case SQLITE_NOTFOUND: return "SQLITE_NOTFOUND";
		case SQLITE_FULL: return "SQLITE_FULL";
		case SQLITE_CANTOPEN: return "SQLITE_CANTOPEN";
		case SQLITE_PROTOCOL: return "SQLITE_PROTOCOL";
		case SQLITE_EMPTY: return "SQLITE_EMPTY";
		case SQLITE_SCHEMA: return "SQLITE_SCHEMA";
		case SQLITE_TOOBIG: return "SQLITE_TOOBIG";
		case SQLITE_CONSTRAINT: return "SQLITE_CONSTRAINT";
		case SQLITE_MISMATCH: return "SQLITE_MISMATCH";
		case SQLITE_MISUSE: return "SQLITE_MISUSE";
		case SQLITE_NOLFS: return "SQLITE_NOLFS";
		case SQLITE_AUTH: return "SQLITE_AUTH";
		case SQLITE_FORMAT: return "SQLITE_FORMAT";
		case SQLITE_RANGE: return "SQLITE_RANGE";
		case SQLITE_NOTADB: return "SQLITE_NOTADB";
		case SQLITE_NOTICE: return "SQLITE_NOTICE";
		case SQLITE_WARNING: return "SQLITE_WARNING";
		case SQLITE_ROW: return "SQLITE_ROW";
		case SQLITE_DONE: return "SQLITE_DONE";
		case SQLITE_IOERR_READ: return "SQLITE_IOERR_READ";
		case SQLITE_IOERR_SHORT_READ: return "SQLITE_IOERR_SHORT_READ";
		case SQLITE_IOERR_WRITE: return "SQLITE_IOERR_WRITE";
		case SQLITE_IOERR_FSYNC: return "SQLITE_IOERR_FSYNC";
		case SQLITE_IOERR_DIR_FSYNC: return "SQLITE_IOERR_DIR_FSYNC";
		case SQLITE_IOERR_TRUNCATE: return "SQLITE_IOERR_TRUNCATE";
		case SQLITE_IOERR_FSTAT: return "SQLITE_IOERR_FSTAT";
		case SQLITE_IOERR_UNLOCK: return "SQLITE_IOERR_UNLOCK";
		case SQLITE_IOERR_RDLOCK: return "SQLITE_IOERR_RDLOCK";
		case SQLITE_IOERR_DELETE: return "SQLITE_IOERR_DELETE";
		case SQLITE_IOERR_BLOCKED: return "SQLITE_IOERR_BLOCKED";
		case SQLITE_IOERR_NOMEM: return "SQLITE_IOERR_NOMEM";
		case SQLITE_IOERR_ACCESS: return "SQLITE_IOERR_ACCESS";
		case SQLITE_IOERR_CHECKRESERVEDLOCK: return "SQLITE_IOERR_CHECKRESERVEDLOCK";
		case SQLITE_IOERR_LOCK: return "SQLITE_IOERR_LOCK";
		case SQLITE_IOERR_CLOSE: return "SQLITE_IOERR_CLOSE";
		case SQLITE_IOERR_DIR_CLOSE: return "SQLITE_IOERR_DIR_CLOSE";
		case SQLITE_IOERR_SHMOPEN: return "SQLITE_IOERR_SHMOPEN";
		case SQLITE_IOERR_SHMSIZE: return "SQLITE_IOERR_SHMSIZE";
		case SQLITE_IOERR_SHMLOCK: return "SQLITE_IOERR_SHMLOCK";
		case SQLITE_IOERR_SHMMAP: return "SQLITE_IOERR_SHMMAP";
		case SQLITE_IOERR_SEEK: return "SQLITE_IOERR_SEEK";
		case SQLITE_IOERR_DELETE_NOENT: return "SQLITE_IOERR_DELETE_NOENT";
		case SQLITE_IOERR_MMAP: return "SQLITE_IOERR_MMAP";
		case SQLITE_IOERR_GETTEMPPATH: return "SQLITE_IOERR_GETTEMPPATH";
		case SQLITE_IOERR_CONVPATH: return "SQLITE_IOERR_CONVPATH";
		//case SQLITE_IOERR_VNODE: return "SQLITE_IOERR_VNODE";
		//case SQLITE_IOERR_AUTH: return "SQLITE_IOERR_AUTH";
		case SQLITE_LOCKED_SHAREDCACHE: return "SQLITE_LOCKED_SHAREDCACHE";
		case SQLITE_BUSY_RECOVERY: return "SQLITE_BUSY_RECOVERY";
		case SQLITE_BUSY_SNAPSHOT: return "SQLITE_BUSY_SNAPSHOT";
		case SQLITE_CANTOPEN_NOTEMPDIR: return "SQLITE_CANTOPEN_NOTEMPDIR";
		case SQLITE_CANTOPEN_ISDIR: return "SQLITE_CANTOPEN_ISDIR";
		case SQLITE_CANTOPEN_FULLPATH: return "SQLITE_CANTOPEN_FULLPATH";
		case SQLITE_CANTOPEN_CONVPATH: return "SQLITE_CANTOPEN_CONVPATH";
		case SQLITE_CORRUPT_VTAB: return "SQLITE_CORRUPT_VTAB";
		case SQLITE_READONLY_RECOVERY: return "SQLITE_READONLY_RECOVERY";
		case SQLITE_READONLY_CANTLOCK: return "SQLITE_READONLY_CANTLOCK";
		case SQLITE_READONLY_ROLLBACK: return "SQLITE_READONLY_ROLLBACK";
		case SQLITE_READONLY_DBMOVED: return "SQLITE_READONLY_DBMOVED";
		case SQLITE_ABORT_ROLLBACK: return "SQLITE_ABORT_ROLLBACK";
		case SQLITE_CONSTRAINT_CHECK: return "SQLITE_CONSTRAINT_CHECK";
		case SQLITE_CONSTRAINT_COMMITHOOK: return "SQLITE_CONSTRAINT_COMMITHOOK";
		case SQLITE_CONSTRAINT_FOREIGNKEY: return "SQLITE_CONSTRAINT_FOREIGNKEY";
		case SQLITE_CONSTRAINT_FUNCTION: return "SQLITE_CONSTRAINT_FUNCTION";
		case SQLITE_CONSTRAINT_NOTNULL: return "SQLITE_CONSTRAINT_NOTNULL";
		case SQLITE_CONSTRAINT_PRIMARYKEY: return "SQLITE_CONSTRAINT_PRIMARYKEY";
		case SQLITE_CONSTRAINT_TRIGGER: return "SQLITE_CONSTRAINT_TRIGGER";
		case SQLITE_CONSTRAINT_UNIQUE: return "SQLITE_CONSTRAINT_UNIQUE";
		case SQLITE_CONSTRAINT_VTAB: return "SQLITE_CONSTRAINT_VTAB";
		case SQLITE_CONSTRAINT_ROWID: return "SQLITE_CONSTRAINT_ROWID";
		case SQLITE_NOTICE_RECOVER_WAL: return "SQLITE_NOTICE_RECOVER_WAL";
		case SQLITE_NOTICE_RECOVER_ROLLBACK: return "SQLITE_NOTICE_RECOVER_ROLLBACK";
		case SQLITE_WARNING_AUTOINDEX: return "SQLITE_WARNING_AUTOINDEX";
		case SQLITE_AUTH_USER: return "SQLITE_AUTH_USER";
		//case SQLITE_OK_LOAD_PERMANENTLY: return "SQLITE_OK_LOAD_PERMANENTLY";
		default: return "<unknown>";
	}
}

class SQLiteStatement
{
public:
	~SQLiteStatement();
	template<typename T> void bind( const T value, int position );
	void reset();
	void step();
private:
	friend class SQLiteDatabase;
	SQLiteStatement( sqlite3_stmt* pStatement, sqlite3* pDatabase );
	sqlite3_stmt* pStatement_;
	sqlite3* pDatabase_; // required to get error messages
};

SQLiteStatement::SQLiteStatement( sqlite3_stmt* pStatement, sqlite3* pDatabase )
	: pStatement_(pStatement), pDatabase_(pDatabase)
{
	// No operation besides the initialiser list
}

SQLiteStatement::~SQLiteStatement()
{
	sqlite3_finalize(pStatement_);
}

template<> void SQLiteStatement::bind( const double value, int position )
{
	int result;
	if( (result=sqlite3_bind_double( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind double to SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

template<> void SQLiteStatement::bind( const float value, int position )
{
	int result;
	if( (result=sqlite3_bind_double( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind float to SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

template<> void SQLiteStatement::bind( const int value, int position )
{
	int result;
	if( (result=sqlite3_bind_int( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind int to SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

template<> void SQLiteStatement::bind( const short value, int position )
{
	int result;
	if( (result=sqlite3_bind_int( pStatement_, position, value ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind short to SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

template<> void SQLiteStatement::bind( char* value, int position )
{
	int result;
	if( (result=sqlite3_bind_text( pStatement_, position, value, -1, nullptr ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't bind short to SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

void SQLiteStatement::reset()
{
	sqlite3_reset(pStatement_);
}

void SQLiteStatement::step()
{
	int result;
	if( (result=sqlite3_step(pStatement_))!=SQLITE_DONE )
	{
		throw std::runtime_error( std::string("Can't step SQLiteStatement: ")+liteResultCodeToString(result) );
	}
}

class SQLiteDatabase
{
public:
	SQLiteDatabase( const char* pFilename );
	SQLiteDatabase( const std::string& filename ) : SQLiteDatabase(filename.c_str()) {}
	~SQLiteDatabase();

	void execute( const char* pCommand );
	void execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback );
	SQLiteStatement prepareStatement( const char* pStatement );
private:
	void execute_( const char* pCommand, void* userCallback );
	static int callback_( void *pMyFunction, int argc, char **argv, char **azColName );
	sqlite3* pDatabase_;
};

SQLiteDatabase::SQLiteDatabase( const char* pFilename )
	: pDatabase_(nullptr)
{
	int result=sqlite3_open( pFilename, &pDatabase_ );
	if( result!=SQLITE_OK )
	{
		sqlite3_close( pDatabase_ );
		throw std::runtime_error( std::string("Can't open database: ")+sqlite3_errmsg(pDatabase_) );
	}
}

SQLiteDatabase::~SQLiteDatabase()
{
	sqlite3_close( pDatabase_ );
}

void SQLiteDatabase::execute( const char* pCommand )
{
	execute_( pCommand, nullptr );
}

void SQLiteDatabase::execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback )
{
	execute_( pCommand, &resultsCallback );
}

void SQLiteDatabase::execute_( const char* pCommand, void* userCallback )
{
	//std::cout << "Executing command " << pCommand << std::endl;
	char* pErrorMsg=0;
	int result=sqlite3_exec( pDatabase_, pCommand, (userCallback ? callback_ : nullptr), userCallback, &pErrorMsg );
	if( result!=SQLITE_OK )
	{
		std::string errorMessage(pErrorMsg);
		sqlite3_free( pErrorMsg );
		throw std::runtime_error(std::string("Error executing command '")+pCommand+"': "+errorMessage);
	}
}

SQLiteStatement SQLiteDatabase::prepareStatement( const char* pCommand )
{
	sqlite3_stmt* pStatement=nullptr;
	int result;
	if( (result=sqlite3_prepare_v2( pDatabase_, pCommand, std::char_traits<char>::length(pCommand), &pStatement, NULL ))!=SQLITE_OK )
	{
		throw std::runtime_error( std::string("Can't prepare SQLiteStatement '")+pCommand+"': "+liteResultCodeToString(result) );
	}
	return SQLiteStatement( pStatement, pDatabase_ );
}

int SQLiteDatabase::callback_( void* pMyFunction, int argc, char** argv, char** azColName )
{
	if( pMyFunction==nullptr ) return 0;
	std::function<bool(int,int,char*,char*)>& callback=*reinterpret_cast<std::function<bool(int,int,char*,char*)>*>(pMyFunction);

	bool keepGoing=true;
	for( int index=0; index<argc && keepGoing; ++index )
	{
		keepGoing=callback( index, argc, argv[index], azColName[index] );
	}

	return (keepGoing ? 0 : 1);
}

class MySQLStatement
{
public:
	~MySQLStatement();
	template<typename T> void bind( const T value, int position );
	void reset();
	void step();
	void bindResults( std::vector<MYSQL_BIND>& results );
	operator MYSQL_STMT*() { return pStatement_; }
private:
	friend class MySQLDatabase;
	MySQLStatement( MYSQL_STMT* pStatement );
	MYSQL_STMT* pStatement_;
};

MySQLStatement::MySQLStatement( MYSQL_STMT* pStatement )
	: pStatement_(pStatement)
{
	// No operation besides the initialiser list
}

MySQLStatement::~MySQLStatement()
{
	mysql_stmt_free_result(pStatement_);
	mysql_stmt_close(pStatement_);
}

template<typename T> void MySQLStatement::bind( const T value, int position )
{

}

void MySQLStatement::reset()
{

}

void MySQLStatement::step()
{
	if( mysql_stmt_execute(pStatement_)!=0 )
	{
		throw std::runtime_error( std::string("Error performing query: ")+mysql_stmt_error(pStatement_) );
	}
}

void MySQLStatement::bindResults( std::vector<MYSQL_BIND>& results )
{
	if( mysql_stmt_bind_result( pStatement_, results.data() )!=0 )
	{
		throw std::runtime_error( "Unable to bind results" );
	}
}

class MySQLResult
{
public:
	MySQLResult( MYSQL_RES* pResult ) : pResult_(pResult) { if(pResult_==nullptr) throw std::runtime_error("Got null result pointer"); }
	~MySQLResult() { mysql_free_result(pResult_); }
	int numberOfFields() { return mysql_num_fields(pResult_); }
	void showResults( std::function<bool(int,int,char*,char*)> resultsCallback )
	{
		int num_fields=numberOfFields();
		MYSQL_FIELD* fields=mysql_fetch_fields(pResult_);

		MYSQL_ROW row;
		bool keepGoing=true;
		while( (row=mysql_fetch_row(pResult_)) && keepGoing )
		{
			for( int fieldIndex=0; fieldIndex<num_fields && keepGoing; ++fieldIndex )
			{
				keepGoing=resultsCallback( fieldIndex, num_fields, row[fieldIndex], fields[fieldIndex].name );
			}
		}
	}
	operator MYSQL_RES*() { return pResult_; }
private:
	MYSQL_RES* pResult_;
};

class MySQLDatabase
{
public:
	MySQLDatabase( const char* host, const char* user, const char* password, const char* database );
	~MySQLDatabase();

	void execute( const char* pCommand );
	MySQLResult execute_new( const char* pCommand );
	void execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback );
	MySQLStatement prepareStatement( const char* pStatement );
	MYSQL* rawHandle() { return pDatabase_; }
private:
	MYSQL* pDatabase_;
};

MySQLDatabase::MySQLDatabase( const char* host, const char* user, const char* password, const char* database )
	: pDatabase_(nullptr)
{
	pDatabase_=mysql_init(nullptr);

	if( pDatabase_==nullptr )
	{
		throw std::runtime_error( std::string("Error initiating MySQL: ")+mysql_error(pDatabase_) );
	}

	if( mysql_real_connect( pDatabase_, host, user, password, database, 0, nullptr, 0 )==nullptr )
	{
		throw std::runtime_error( std::string("Can't open database: ")+mysql_error(pDatabase_));
	}
}

MySQLDatabase::~MySQLDatabase()
{
	mysql_close(pDatabase_);
}

void MySQLDatabase::execute( const char* pCommand )
{
	//std::cout << "Executing command " << pCommand << std::endl;
	if( mysql_query( pDatabase_, pCommand ) )
	{
		throw std::runtime_error( std::string("Error performing query: ")+mysql_error(pDatabase_) );
	}
}

MySQLResult MySQLDatabase::execute_new( const char* pCommand )
{
	execute( pCommand );
	return MySQLResult( mysql_store_result(pDatabase_) );
}

void MySQLDatabase::execute( const char* pCommand, std::function<bool(int,int,char*,char*)> resultsCallback )
{
	execute( pCommand );

	MySQLResult result( mysql_store_result(pDatabase_) );
	if( result==nullptr )
	{
		throw std::runtime_error(std::string("Error getting result: ")+mysql_error(pDatabase_) );
	}

	int num_fields=mysql_num_fields(result);
	MYSQL_FIELD* fields=mysql_fetch_fields(result);

	MYSQL_ROW row;

	bool keepGoing=true;
	while( (row=mysql_fetch_row(result)) && keepGoing )
	{
		for( int fieldIndex=0; fieldIndex<num_fields && keepGoing; ++fieldIndex )
		{
			keepGoing=resultsCallback( fieldIndex, num_fields, row[fieldIndex], fields[fieldIndex].name );
		}
	}
}

MySQLStatement MySQLDatabase::prepareStatement( const char* pStatement )
{
	MYSQL_STMT* pHandle=mysql_stmt_init(pDatabase_);
	if( pHandle==nullptr ) throw std::runtime_error("Couldn't initialise the statement handler");
	if( mysql_stmt_prepare( pHandle, pStatement, std::char_traits<char>::length(pStatement) )!=0 )
	{
		throw std::runtime_error( std::string("Could not prepare statement: ")+mysql_stmt_error(pHandle) );
	}
	return MySQLStatement(pHandle);
}

void printCharVector( const std::vector<char>& data )
{
	for( int index=0; index<data.size() && index<128; ++index )
	{
		if( data[index]>=32 && data[index]<=125 ) std::cout << data[index];
		else std::cout << ".";
	}
	std::cout << "\n";
}

const char* stmt_fetch_errorCode( int errorCode )
{
	switch( errorCode )
	{
		case 0: return "Success";
		case 1: return "Error (call mysql_stmt_error)";
		case MYSQL_NO_DATA: return "MYSQL_NO_DATA";
		case MYSQL_DATA_TRUNCATED: return "MYSQL_DATA_TRUNCATED";
		default: return "unknown";
	}
}

class BufferDump
{
public:
	BufferDump( const MYSQL_BIND& bind ) : bind_(bind) {}
private:
	const MYSQL_BIND& bind_;
	friend std::ostream& operator<<( std::ostream& stream, const BufferDump& buffer );
};

std::ostream& operator<<( std::ostream& stream, const BufferDump& buffer )
{
	switch( buffer.bind_.buffer_type )
	{
		case MYSQL_TYPE_TINY: stream << (signed char*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_SHORT: stream << *(short int*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_INT24: stream << *(int*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_LONG: stream << *(int*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_LONGLONG: stream << *(long long int*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_FLOAT: stream << *(float*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_DOUBLE: stream << *(double*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_NEWDECIMAL: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_TIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_DATE: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_DATETIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_TIMESTAMP: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_STRING: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_VAR_STRING: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_TINY_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_MEDIUM_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_LONG_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_BIT: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		default: stream << "<unknown>";
	}
	return stream;
}

const char* typeAsString( int type );

void bindToSQLite( const MYSQL_BIND& bind, SQLiteStatement& statement, const int fieldNumber )
{
	switch( bind.buffer_type )
	{
		//case MYSQL_TYPE_TINY: stream << (signed char*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_SHORT: statement.bind( *(short int*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_INT24: statement.bind( *(int*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_LONG: statement.bind( *(int*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_LONGLONG: statement.bind( *(long long int*)bind.buffer, fieldNumber ); break;
		case MYSQL_TYPE_FLOAT: statement.bind( *(float*)bind.buffer, fieldNumber ); break;
		case MYSQL_TYPE_DOUBLE: statement.bind( *(double*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_NEWDECIMAL: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_TIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_DATE: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_DATETIME: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_TIMESTAMP: stream << (MYSQL_TIME*)buffer.bind_.buffer; break;
		case MYSQL_TYPE_STRING: statement.bind( (char*)bind.buffer, fieldNumber+1 ); break;
		case MYSQL_TYPE_VAR_STRING: statement.bind( (char*)bind.buffer, fieldNumber+1 ); break;
		//case MYSQL_TYPE_TINY_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_MEDIUM_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_LONG_BLOB: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		//case MYSQL_TYPE_BIT: stream << (char*)buffer.bind_.buffer; break;//stream << (char[]*)buffer.bind_.buffer; break;
		default: throw std::runtime_error("Unable to bind SQLite parameter to MySQL result "+std::string(typeAsString(bind.buffer_type)));
	}
}

const char* typeAsString( int type )
{
	switch( type )
	{
		case MYSQL_TYPE_TINY: return "MYSQL_TYPE_TINY";
		case MYSQL_TYPE_SHORT: return "MYSQL_TYPE_SHORT";
		case MYSQL_TYPE_INT24: return "MYSQL_TYPE_INT24";
		case MYSQL_TYPE_LONG: return "MYSQL_TYPE_LONG";
		case MYSQL_TYPE_LONGLONG: return "MYSQL_TYPE_LONGLONG";
		case MYSQL_TYPE_FLOAT: return "MYSQL_TYPE_FLOAT";
		case MYSQL_TYPE_DOUBLE: return "MYSQL_TYPE_DOUBLE";
		case MYSQL_TYPE_NEWDECIMAL: return "MYSQL_TYPE_NEWDECIMAL";
		case MYSQL_TYPE_TIME: return "MYSQL_TYPE_TIME";
		case MYSQL_TYPE_DATE: return "MYSQL_TYPE_DATE";
		case MYSQL_TYPE_DATETIME: return "MYSQL_TYPE_DATETIME";
		case MYSQL_TYPE_TIMESTAMP: return "MYSQL_TYPE_TIMESTAMP";
		case MYSQL_TYPE_STRING: return "MYSQL_TYPE_STRING";
		case MYSQL_TYPE_VAR_STRING: return "MYSQL_TYPE_VAR_STRING";
		case MYSQL_TYPE_TINY_BLOB: return "MYSQL_TYPE_TINY_BLOB";
		case MYSQL_TYPE_BLOB: return "MYSQL_TYPE_BLOB";
		case MYSQL_TYPE_MEDIUM_BLOB: return "MYSQL_TYPE_MEDIUM_BLOB";
		case MYSQL_TYPE_LONG_BLOB: return "MYSQL_TYPE_LONG_BLOB";
		case MYSQL_TYPE_BIT: return "MYSQL_TYPE_BIT";
		default: return "<unknown>";
	}
}

const char* sqlTypeString( const MYSQL_FIELD& field )
{
	switch( field.type )
	{
		case MYSQL_TYPE_TINY: return "INT";
		case MYSQL_TYPE_SHORT: return "SHORT";
		case MYSQL_TYPE_INT24: return "LONG INT";
		case MYSQL_TYPE_LONG: return "LONG";
		case MYSQL_TYPE_LONGLONG: return "LONGLONG";
		case MYSQL_TYPE_FLOAT: return "FLOAT";
		case MYSQL_TYPE_DOUBLE: return "DOUBLE";
		case MYSQL_TYPE_NEWDECIMAL: return "NEWDECIMAL";
		case MYSQL_TYPE_TIME: return "TIME";
		case MYSQL_TYPE_DATE: return "DATE";
		case MYSQL_TYPE_DATETIME: return "DATETIME";
		case MYSQL_TYPE_TIMESTAMP: return "TIMESTAMP";
		case MYSQL_TYPE_STRING: return "STRING";
		case MYSQL_TYPE_VAR_STRING: return "VAR_STRING";
		case MYSQL_TYPE_TINY_BLOB: return "TINY_BLOB";
		case MYSQL_TYPE_BLOB: return "BLOB";
		case MYSQL_TYPE_MEDIUM_BLOB: return "MEDIUM_BLOB";
		case MYSQL_TYPE_LONG_BLOB: return "LONG_BLOB";
		case MYSQL_TYPE_BIT: return "BIT";
		default: throw std::runtime_error("Unknown type (sqlTypeString)");
	}
}

int preparedStatement( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		std::cerr << "Usage: " << argv[0] << " DATABASE" << std::endl;
		return 1;
	}

	try
	{
		SQLiteDatabase database( argv[1] );
		database.execute( "BEGIN TRANSACTION" );
		auto statement=database.prepareStatement( "INSERT OR IGNORE INTO blah VALUES (?1, ?2, ?3)" );
		statement.bind( 11, 1 );
		statement.bind( 12, 2 );
		statement.bind( 13, 3 );
		statement.step();
		database.execute( "COMMIT TRANSACTION" );

		auto printResults=[]( int fieldIndex, int numFields, char* value, char* field )
		{
			std::cout << field << "=" << value << (fieldIndex==numFields-1 ? "\n" : ", ");
			return true;
		};
		database.execute( "SELECT * FROM blah", printResults );
	}
	catch( const std::exception& error )
	{
		std::cerr << "Exception: " << error.what() << std::endl;
		return -1;
	}

	return 0;
}

int mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile )
{
	MySQLDatabase inputDatabase( host, user, password, database );
	SQLiteDatabase outputDatabase( outputFile );
	outputDatabase.execute("PRAGMA synchronous=OFF");
	outputDatabase.execute("PRAGMA count_changes=OFF");
	outputDatabase.execute("PRAGMA journal_mode=MEMORY");
	outputDatabase.execute("PRAGMA temp_store=MEMORY");

	MySQLResult tableNamesResult( mysql_list_tables( inputDatabase.rawHandle(), nullptr ) );
	MYSQL_ROW tableNames;
	while( (tableNames=mysql_fetch_row(tableNamesResult)) )
	{
		for( int fieldIndex=0; fieldIndex<tableNamesResult.numberOfFields(); ++fieldIndex )
		{
			std::string tableName(tableNames[fieldIndex]);
			std::cout << "Table " << tableName << std::endl;

			auto selectStatement=inputDatabase.prepareStatement( (std::string("SELECT * FROM ")+tableName).c_str() );
			if( mysql_stmt_execute(selectStatement)!=0 ) throw std::runtime_error("Failed executing select statement");
			MySQLResult metadata( mysql_stmt_result_metadata(selectStatement) );
			const int numberOfFields=metadata.numberOfFields();
			MYSQL_FIELD* fields=mysql_fetch_fields(metadata);
			/*for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
			{
				std::cout << "   "
					<< (fields[columnNameIndex].name ? fields[columnNameIndex].name : "null")
					<< ", " << typeAsString(fields[columnNameIndex].type)
					<< ", " << fields[columnNameIndex].length
					<< ", " << fields[columnNameIndex].max_length
					<< "\n";
			}*/
			std::vector<MYSQL_BIND> results(numberOfFields);
			std::vector<std::vector<char>> buffers(numberOfFields);
			std::vector<long unsigned int> lengths(numberOfFields);
			std::string createStatementSQL("CREATE TABLE '"+tableName+"'(");
			for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
			{
				buffers[columnNameIndex].resize(fields[columnNameIndex].length+1);
				results[columnNameIndex].length=&lengths[columnNameIndex];
				results[columnNameIndex].buffer=buffers[columnNameIndex].data();
				results[columnNameIndex].buffer_type=fields[columnNameIndex].type;
				results[columnNameIndex].buffer_length=buffers[columnNameIndex].size()-1;
				if( columnNameIndex!=0 ) createStatementSQL+=", '";
				else createStatementSQL+="'";
				createStatementSQL+=fields[columnNameIndex].name;
				createStatementSQL+="' ";
				createStatementSQL+=sqlTypeString(fields[columnNameIndex]);
			}
			createStatementSQL+=")";
			outputDatabase.execute("BEGIN TRANSACTION");
			outputDatabase.execute(createStatementSQL.c_str());
			// Prepare the INSERT statement for the output database
			std::string insertStatementSQL("INSERT INTO "+tableName+" VALUES(");
			for( int index=0; index<numberOfFields; ++index )
			{
				if( index!=0 ) insertStatementSQL+=", ";
				insertStatementSQL+="?"+std::to_string(index+1);
			}
			insertStatementSQL+=")";
			auto insertStatement=outputDatabase.prepareStatement(insertStatementSQL.c_str());
			//if( mysql_stmt_bind_param(selectStatement,params.data())!=0 ) throw std::runtime_error("Failed binding param");
			if( mysql_stmt_bind_result(selectStatement,results.data())!=0 ) throw std::runtime_error("Failed binding result");
			if( mysql_stmt_store_result(selectStatement)!=0 ) throw std::runtime_error("Failed storing result");
			int fetchResult;
			while( (fetchResult=mysql_stmt_fetch(selectStatement))!=MYSQL_NO_DATA )
			{
				if( fetchResult!=0 ) std::cout << "Didn't work " << stmt_fetch_errorCode(fetchResult) << std::endl;//throw std::runtime_error("Failed fetching result: '"+std::string(mysql_stmt_error(selectStatement))+"'-'"+std::string(mysql_error(inputDatabase.rawHandle()))+"'");
				for( int columnNameIndex=0; columnNameIndex<numberOfFields; ++columnNameIndex )
				{
					//std::cout << (columnNameIndex==0 ? "  " : ", ") << fields[columnNameIndex].name << "=" << BufferDump(results[columnNameIndex]);
					bindToSQLite( results[columnNameIndex], insertStatement, columnNameIndex );
				}
				//std::cout << "\n";
				insertStatement.step();
				insertStatement.reset();
			}
			outputDatabase.execute("END TRANSACTION");
		} // end of loop over fields in the table names call (pretty sure there is only one)
	} // end of loop over table names
} // end of function mySQLTest

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
