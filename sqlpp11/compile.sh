SQLPP11_INCLUDE_DIR="$HOME/Documents/dbTest/sqlpp11/include"
SQLPP11SQLITE3CONNECTOR_INCLUDE_DIR="$HOME/Documents/dbTest/sqlpp11-connector-sqlite3/include"
SQLPP11SQLITE3CONNECTOR_LIB_DIR="$HOME/Documents/dbTest/sqlpp11-connector-sqlite3/build/src"
SQLPP11MYSQLCONNECTOR_INCLUDE_DIR="$HOME/Documents/dbTest/sqlpp11-connector-mysql/include"
SQLPP11MYSQLCONNECTOR_LIB_DIR="$HOME/Documents/dbTest/sqlpp11-connector-mysql/build/src"
MARIADB_LIB_DIR="$HOME/Documents/dbTest/mariadb-connector-install/lib/mariadb"
BOOST_LIB_DIR="$HOME/Documents/boost/lib"
HINNANTDATE_INCLUDE_DIR="$HOME/Documents/dbTest/date/include"

c++ --std=c++11 -I "$SQLPP11SQLITE3CONNECTOR_INCLUDE_DIR" \
                -I "$SQLPP11MYSQLCONNECTOR_INCLUDE_DIR" \
                -I "$SQLPP11_INCLUDE_DIR" \
                -I "$HINNANTDATE_INCLUDE_DIR" \
                main.cpp \
                -L "$SQLPP11SQLITE3CONNECTOR_LIB_DIR" \
                -L "$SQLPP11MYSQLCONNECTOR_LIB_DIR" \
                -L "$MARIADB_LIB_DIR" \
                -L "$BOOST_LIB_DIR" \
                -l sqlpp11-connector-sqlite3 \
                -l sqlpp-mysql \
                -l mariadb \
                -l sqlite3 \
                -l boost_thread \
                -o sqliteTest
