#include "sqlite3_db.h"
#include <iostream>
#include "sqlite3_helpers.h"

Sqlite3Db::Sqlite3Db(std::string inName, std::string inPath) : mName(inName), mPath(inPath)
{

}

Sqlite3Db::~Sqlite3Db()
{

}

sqlite3 * Sqlite3Db::open()
{
    std::cout << "Opening db " << mPath << "....";

    sqlite3 * db;
    exit_on_error ( sqlite3_open(mPath.c_str(), &db), __LINE__);
    exit_on_error( sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0), __LINE__);

    std::cout << "OK!" << std::endl;

    return db;
}

void Sqlite3Db::executeCreationCode()
{
    sqlite3 * db (open());
    char *error_msg = 0;
    for(auto it(mTables.begin()); it != mTables.end(); it++)
    {
        std::cout << "Executing table initialization code for table " << it->first << "...";
        exit_on_error(sqlite3_exec(db, it->second->getCreationCode().c_str(), NULL, 0, &error_msg), __LINE__, error_msg);
        std::cout << "OK!" << std::endl;
    }

    sqlite3_free(error_msg);
}

void Sqlite3Db::addTable(const std::string & name, std::unique_ptr<Sqlite3Table> & inTable)
{
    mTables.emplace(name, std::move(inTable));
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Db& sqlitedb)
{
    os << sqlitedb.mName;
}
