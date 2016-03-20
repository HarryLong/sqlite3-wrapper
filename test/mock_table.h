#ifndef MOCK_TABLE_DB
#define MOCK_TABLE_DB

#include "../sqlite3_table.h"
#include <string>
#include <vector>

class MockTable : public Sqlite3Table
{
public:
    static const std::string _TABLE_NAME;

    struct ColumnNames
    {
        static const std::string _COLUMN1;
        static const std::string _COLUMN2;
        static const std::string _COLUMN3;
        static const std::string _COLUMN4;
        static const std::string _COLUMN5;
        static const std::string _COLUMN6;
    };



    MockTable();
    ~MockTable();
};

class MockTable2 : public Sqlite3Table
{
public:
    static const std::string _TABLE_NAME;

    struct ColumnNames
    {
        static const std::string _COLUMN1;
        static const std::string _COLUMN2;
    };

    MockTable2();
    ~MockTable2();
};

#endif //MOCK_TABLE_DB
