#include <Python.h>
#include "mySQLBackup.h"

static PyObject* sqlitedump_mySQLBackup( PyObject *self, PyObject *args, PyObject *keywords )
{
   // Argument defaults
   const char* host="localhost";
   const char* user="root";
   const char* password="root";
   const char* database="ethoscope_db";
   const char* outputFile="testdb.sqlite";
   static const char* keywordList[]={"host","user","password","database","outputFile"};
   // Parse the arguments
   if( !PyArg_ParseTupleAndKeywords( args, keywords, "|sssss", const_cast<char**>(keywordList),
         &host, &user, &password, &database, &outputFile ) ) return nullptr;

   // Call the C++ function
	mySQLBackup( host, user, password, database, outputFile );
	Py_RETURN_NONE;
}

static PyMethodDef SqlitedumpMethods[] = {
	{ "mySQLBackup", (PyCFunction)sqlitedump_mySQLBackup, METH_VARARGS | METH_KEYWORDS, "Backup a MySQL database to a SQLite file" },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initsqlitedump(void)
{
	(void) Py_InitModule( "sqlitedump", SqlitedumpMethods );
}
