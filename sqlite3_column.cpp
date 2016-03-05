#include "sqlite3_column.h"
#include <sstream>

Sqlite3Column::Sqlite3Column(std::string inName, Sqlite3Type type, bool inPrimaryKey, bool inNotNull,
                             bool autoincrement) : mName(inName), mType(type), mPrimaryKey(inPrimaryKey), mAutoIncrement(autoincrement),
                            mNotNull(inNotNull)
{

}

Sqlite3Column::~Sqlite3Column()
{

}

std::string Sqlite3Column::getCreationCode() const
{
    std::stringstream ss;
    ss << mName << " " << mType;
    if(mPrimaryKey)
    {
        if(mAutoIncrement)
        ss << " AUTOINCREMENT";
    }
    else if(mNotNull)
        ss << " NOT NULL";

    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Column& sqlitecolumn)
{
    os << sqlitecolumn.mName;
}

int Sqlite3ColumnNull::bind(sqlite3_stmt * statement, void *  valuePtr) const
{
    sqlite3_bind_null(statement, sqlite3_bind_parameter_index(statement, mName.c_str()));
}

int Sqlite3ColumnInteger::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_int(statement, sqlite3_bind_parameter_index(statement, mName.c_str()), *static_cast<int*>(valuePtr));
}

int Sqlite3ColumnReal::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_double(statement, sqlite3_bind_parameter_index(statement, mName.c_str()), *static_cast<double*>(valuePtr));
}

int Sqlite3ColumnText::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_text(statement, sqlite3_bind_parameter_index(statement, mName.c_str()), static_cast<std::string*>(valuePtr)->c_str(), -1, NULL);
}

int Sqlite3ColumnBlob::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    //TODO
}

