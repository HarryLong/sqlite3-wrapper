#include "sqlite3_column.h"
#include <sstream>

Sqlite3Column::Sqlite3Column(std::string inName, Sqlite3Type type, bool inPrimaryKey, bool inNotNull,
                             bool autoincrement) : mName(inName), mType(type), mPrimaryKey(inPrimaryKey), mAutoIncrement(autoincrement),
                            mNotNull(inNotNull), mBindName("@")
{
    mBindName.append(mName);
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
        ss << " PRIMARY KEY";
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

void Sqlite3ColumnNull::value(sqlite3_stmt * statement, int columnIdx, void * valuePtr) const
{
    //TODO
}

//-------------------------------------------------------------------------------------
#include <iostream>
int Sqlite3ColumnInteger::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_int(statement, sqlite3_bind_parameter_index(statement, mBindName.c_str()), *static_cast<int*>(valuePtr));
}

void Sqlite3ColumnInteger::value(sqlite3_stmt * statement, int columnIdx, void * valuePtr) const
{
    *static_cast<int*>(valuePtr) = sqlite3_column_int(statement,columnIdx);
}

//-------------------------------------------------------------------------------------

int Sqlite3ColumnReal::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_double(statement, sqlite3_bind_parameter_index(statement, mBindName.c_str()), *static_cast<double*>(valuePtr));
}

void Sqlite3ColumnReal::value(sqlite3_stmt * statement, int columnIdx, void * valuePtr) const
{
    *static_cast<double*>(valuePtr) = sqlite3_column_double(statement,columnIdx);
}

//-------------------------------------------------------------------------------------

int Sqlite3ColumnText::bind(sqlite3_stmt * statement, void * valuePtr) const
{
    sqlite3_bind_text(statement, sqlite3_bind_parameter_index(statement, mBindName.c_str()), static_cast<std::string*>(valuePtr)->c_str(), -1, NULL);
}

void Sqlite3ColumnText::value(sqlite3_stmt * statement, int columnIdx, void * valuePtr) const
{
    std::string value ((char * ) sqlite3_column_text(statement,columnIdx));
    *static_cast<std::string*>(valuePtr) = value;
}

//-------------------------------------------------------------------------------------

int Sqlite3ColumnBlob::bind(sqlite3_stmt * statement, void * valuePtr) const
{

}

void Sqlite3ColumnBlob::value(sqlite3_stmt * statement, int columnIdx, void * valuePtr) const
{
    //TODO
//    *static_cast<int>(valuePtr) = sqlite3_column_int(statement,columnIdx);
}


//-------------------------------------------------------------------------------------

