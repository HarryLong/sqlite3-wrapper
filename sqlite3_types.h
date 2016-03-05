#ifndef SQLITE_3_TYPES_H
#define SQLITE_3_TYPES_H

#include <string>
#include <ostream>

class Sqlite3Type
{
public:
    Sqlite3Type(std::string inName);
    ~Sqlite3Type();
    std::string name() const;

    friend std::ostream& operator<<(std::ostream& os, const Sqlite3Type& sqlitetype);

private:
    const std::string mName;
};

class Sqlite3Null : public Sqlite3Type
{
public:
    Sqlite3Null() : Sqlite3Type("NULL") {}
};

class Sqlite3Integer : public Sqlite3Type
{
public:
    Sqlite3Integer() : Sqlite3Type("INTEGER") {}
};

class Sqlite3Real : public Sqlite3Type
{
public:
    Sqlite3Real() : Sqlite3Type("REAL") {}
};

class Sqlite3Text : public Sqlite3Type
{
public:
    Sqlite3Text() : Sqlite3Type("TEXT") {}
};

class Sqlite3Blob : public Sqlite3Type
{
public:
    Sqlite3Blob() : Sqlite3Type("BLOB") {}
};

#endif // SQLITE_3_TYPES_H

//#include <sqlite3.h>
//#include <map>
//#include <set>
//#include <string>
//#include <vector>




///*******************
// * DATABASE SCHEMA *
// *******************/
//class DbColumn
//{
//public:
//    BaseDbColumn(std::string name);
//    ~BaseDbColumn();

//private:
//    std::string mName;
//};

//class BaseDbTable
//{
//public:
//    BaseDbTable(std::string name);
//    ~BaseDbTable();
//};
