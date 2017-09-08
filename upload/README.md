Uploads the test data to the MySQL database. You'll probably want to give the MySQL database location as the
first parameter, in SQLAlchemy format, e.g. "mysql://username:password@host:port/database". See
http://docs.sqlalchemy.org/en/latest/core/engines.html for full details.

Note that the input file is not located in this repository so that the repository is kept small. You could
use any SQLite file (named "inputData.sqlite" in this repository), or if you run the script without a file
present it will try to download the one I used from github.
