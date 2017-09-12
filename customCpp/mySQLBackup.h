#ifndef INCLUDEGUARD_mySQLBackup_h
#define INCLUDEGUARD_mySQLBackup_h

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Backups the MySQL database with the details given to a SQLite file with the given filename
 * @author Mark Grimes (mark.grimes@rymapt.com
 * @date 11/Sep/2017
 * @copyright Copyright Rymapt 2017, released under the MIT licence (available from https://opensource.org/licenses/MIT)
 */
void mySQLBackup( const char* host, const char* user, const char* password, const char* database, const char* outputFile );

#ifdef __cplusplus
} // end of 'extern "C"'
#endif

#endif // end of INCLUDEGUARD_mySQLBackup_h
