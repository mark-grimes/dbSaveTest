from distutils.core import setup, Extension

module1 = Extension('sqlitedump',
                    sources = ['sqlitedumpmodule.cpp','mySQLBackup.cpp'],
                    libraries = ['sqlite3','mysqlclient'],
                    extra_compile_args=['-std=c++11'] )

setup (name = 'sqlitedump',
       version = '0.1',
       description = 'This is a demo package',
       ext_modules = [module1])
