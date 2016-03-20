#include "mock_db.h"
#include "mock_table.h"

const std::string MockDB::_PATH = "./mock.db";

MockDB::MockDB() : Sqlite3Db(MockDB::_PATH)
{
    {
        std::unique_ptr<Sqlite3Table> mock_table(new MockTable);
        std::unique_ptr<Sqlite3Table> mock_table_2(new MockTable2);
        addTable(MockTable::_TABLE_NAME, mock_table);
        addTable(MockTable2::_TABLE_NAME, mock_table_2);
    }

    executeCreationCode();
}

MockDB::~MockDB()
{

}

