#include "sqlite3_types.h"

const std::string Sqlite3Null::_NAME = "NULL";
const std::string Sqlite3Integer::_NAME = "INTEGER";
const std::string Sqlite3Real::_NAME = "REAL";
const std::string Sqlite3Text::_NAME = "TEXT";
const std::string Sqlite3Blob::_NAME = "BLOB";

Sqlite3Type::Sqlite3Type(std::string inName) : mName(inName)
{

}

Sqlite3Type::~Sqlite3Type()
{

}

std::string Sqlite3Type::name() const
{
    return mName;
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Type& sqlitetype)
{
    return os << sqlitetype.mName;
}
