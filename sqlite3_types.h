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

    bool operator ==(const Sqlite3Type& other) {return this->name() == other.name();}
    bool operator ==(const std::string& other) {return this->name() == other;}

private:
    const std::string mName;
};

class Sqlite3Null : public Sqlite3Type
{
public:
    static const std::string _NAME;
    Sqlite3Null() : Sqlite3Type(_NAME) {}
};

class Sqlite3Integer : public Sqlite3Type
{
public:
    static const std::string _NAME;
    Sqlite3Integer() : Sqlite3Type(_NAME) {}
};

class Sqlite3Real : public Sqlite3Type
{
public:
    static const std::string _NAME;
    Sqlite3Real() : Sqlite3Type(_NAME) {}
};

class Sqlite3Text : public Sqlite3Type
{
public:
    static const std::string _NAME;
    Sqlite3Text() : Sqlite3Type(_NAME) {}
};

class Sqlite3Blob : public Sqlite3Type
{
public:
    static const std::string _NAME;
    Sqlite3Blob() : Sqlite3Type(_NAME) {}
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
