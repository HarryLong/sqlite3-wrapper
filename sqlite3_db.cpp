#include "sqlite3_db.h"
#include <iostream>
#include "sqlite3_helpers.h"

Sqlite3Db::Sqlite3Db(std::string inPath) : mPath(inPath)
{

}

Sqlite3Db::~Sqlite3Db()
{

}

sqlite3 * Sqlite3Db::open()
{
    std::cout << "Opening db " << mPath << "....";

    sqlite3 * db;
    checkIfSqlError ( sqlite3_open(mPath.c_str(), &db), __FILE__, __LINE__);
    checkIfSqlError( sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0), __FILE__, __LINE__ );

    std::cout << "OK!" << std::endl;

    return db;
}

bool Sqlite3Db::executeCreationCode()
{
    bool success(true);
    sqlite3 * db (open());
    char *error_msg = 0;
    for(auto it(mTables.begin()); it != mTables.end(); it++)
    {
        std::cout << "Executing table initialization code for table " << it->first << "...";
        std::cout << "Code " << it->second->getCreationCode() << std::endl;

        success = (success && checkIfSqlError(sqlite3_exec(db, it->second->getCreationCode().c_str(), NULL, 0, &error_msg), __FILE__, __LINE__, error_msg));
        std::cout << "OK!" << std::endl;
    }

    sqlite3_free(error_msg);
    sqlite3_close(db);
    return success;
}

void Sqlite3Db::addTable(const std::string & name, std::unique_ptr<Sqlite3Table> & inTable)
{
    mTables.emplace(name, std::move(inTable));
}

Sqlite3Table * Sqlite3Db::getTable(const std::string & inTableName)
{
    return mTables.find(inTableName)->second.get();
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Db& sqlitedb)
{
    os << sqlitedb.mPath;
}
