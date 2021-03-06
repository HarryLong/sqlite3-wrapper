#ifndef SQLITE_3_DB_H
#define SQLITE_3_DB_H

#include <string>
#include <vector>
#include <map>
#include "sqlite3_table.h"
class Sqlite3Db
{
public:
    Sqlite3Db(std::string inPath);
    ~Sqlite3Db();
    void addTable(const std::string & name, std::unique_ptr<Sqlite3Table> & inTable);
    friend std::ostream& operator<<(std::ostream& os, const Sqlite3Db& sqlitedb);

    sqlite3 * open();

    Sqlite3Table * getTable(const std::string & inTableName);

protected:
    bool executeCreationCode();

private:
    std::string mPath;
    std::map<std::string, std::unique_ptr<Sqlite3Table>> mTables;

    friend class DbTest;
};

#endif
