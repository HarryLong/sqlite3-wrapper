#include "sqlite3_types.h"

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
