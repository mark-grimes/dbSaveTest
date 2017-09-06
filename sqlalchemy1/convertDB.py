#! /usr/bin/env python

from optparse import OptionParser
import sqlalchemy
import sqlalchemy.ext.compiler
import sqlalchemy.dialects.mysql

# Define the custom mappings from MySQL to SQLite that aren't covered by the defaults
@sqlalchemy.ext.compiler.compiles(sqlalchemy.dialects.mysql.TINYINT, 'sqlite')
def compile_TINYINT(element, compiler, **kw):
    """Treat MySQL TINYINT as normal integer in SQLite"""
    return compiler.visit_integer(element, **kw)

@sqlalchemy.ext.compiler.compiles(sqlalchemy.dialects.mysql.LONGBLOB, 'sqlite')
def compile_LONGBLOB(element, compiler, **kw):
    """Treat MySQL LONGBLOB as large binary in SQLite"""
    return compiler.visit_large_binary(element, **kw)

def copyDatabase(inputEngine, outputEngine):
    inputMetadata = sqlalchemy.MetaData(bind=inputEngine)
    inputMetadata.reflect(inputEngine)
    outputMetadata = sqlalchemy.MetaData(bind=outputEngine)

    for tableName, inputTable in inputMetadata.tables.iteritems():
        #print tableName
        #
        # First copy the schema for the table
        #
        outputTable = sqlalchemy.Table(inputTable.name, outputMetadata)
        for inputColumn in inputTable.columns:
            outputTable.append_column(inputColumn.copy())
        outputTable.create()
        #
        # Then copy the data
        #
        insert = outputTable.insert()
        for row in inputTable.select().execute():
            insert.execute(row)

if __name__ == "__main__":

    parser = OptionParser()
    parser.add_option("-l", "--location", dest="sqlLocation", type="string", help="The sqlalchemy path to the input database")
    parser.add_option("-o", "--output", dest="output", type="string", help="The sqlalchmy path of the output database")

    (options, arguments) = parser.parse_args()
    if options.sqlLocation == None:
        raise ValueError("No database was specified with the '--location' option")
    if options.output == None:
        raise ValueError("No output database was specified with the '--output' option")

    inputEngine = sqlalchemy.create_engine(options.sqlLocation)
    outputEngine = sqlalchemy.create_engine(options.output)

    #metadata = sqlalchemy.MetaData()
    #metadata.reflect( bind=inputEngine )
    #metadata.create_all( bind=outputEngine )
    copyDatabase(inputEngine, outputEngine)
