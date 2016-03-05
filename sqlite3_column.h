#ifndef SQLITE_3_COLUMN_H
#define SQLITE_3_COLUMN_H

#include <string>

#include <sqlite3.h>
#include "sqlite3_types.h"

class Sqlite3Column
{
public:
    Sqlite3Column(std::string inName, Sqlite3Type type, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false);
    ~Sqlite3Column();

    std::string getCreationCode() const;
    virtual int bind(sqlite3_stmt * statement, void * valuePtr) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Sqlite3Column& sqlitecolumn);
protected:
    const std::string mName;
    const Sqlite3Type mType;
    const bool mPrimaryKey, mNotNull, mAutoIncrement;
};

class Sqlite3ColumnNull : public Sqlite3Column
{
public:
    Sqlite3ColumnNull(std::string name, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false) :
        Sqlite3Column(name, Sqlite3Null(), inPrimaryKey, inNotNull, autoincrement) {}
    int bind(sqlite3_stmt * statement, void * valuePtr) const;
};

class Sqlite3ColumnInteger : public Sqlite3Column
{
public:
    Sqlite3ColumnInteger(std::string name, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false) :
        Sqlite3Column(name, Sqlite3Integer(), inPrimaryKey, inNotNull, autoincrement) {}
    int bind(sqlite3_stmt * statement, void * valuePtr) const;
};

class Sqlite3ColumnReal : public Sqlite3Column
{
public:
    Sqlite3ColumnReal(std::string name, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false) :
        Sqlite3Column(name,  Sqlite3Real(), inPrimaryKey, inNotNull, autoincrement) {}
    int bind(sqlite3_stmt * statement, void * valuePtr) const;
};

class Sqlite3ColumnText : public Sqlite3Column
{
public:
    Sqlite3ColumnText(std::string name, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false) :
        Sqlite3Column(name, Sqlite3Text(), inPrimaryKey, inNotNull, autoincrement) {}
    int bind(sqlite3_stmt * statement, void * valuePtr) const;
};

class Sqlite3ColumnBlob : public Sqlite3Column
{
public:
    Sqlite3ColumnBlob(std::string name, bool inPrimaryKey = false, bool inNotNull = false, bool autoincrement = false) :
        Sqlite3Column(name, Sqlite3Blob(), inPrimaryKey, inNotNull, autoincrement) {}
    int bind(sqlite3_stmt * statement, void * valuePtr) const;
};


#endif //SQLITE_3_COLUMN_H
