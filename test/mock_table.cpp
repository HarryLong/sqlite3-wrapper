#include "mock_table.h"

const std::string MockTable::_TABLE_NAME = "mock_table";

const std::string MockTable::ColumnNames::_COLUMN1 = "C1";
const std::string MockTable::ColumnNames::_COLUMN2 = "C2";
const std::string MockTable::ColumnNames::_COLUMN3 = "C3";
const std::string MockTable::ColumnNames::_COLUMN4 = "C4";
const std::string MockTable::ColumnNames::_COLUMN5 = "C5";
const std::string MockTable::ColumnNames::_COLUMN6 = "C6";

MockTable::MockTable() : Sqlite3Table(_TABLE_NAME)
{
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnInteger(ColumnNames::_COLUMN1, true, true, true));
        addColumn(ColumnNames::_COLUMN1, column);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnInteger(ColumnNames::_COLUMN2, false, false, false));
        addColumn(ColumnNames::_COLUMN2, column);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnText(ColumnNames::_COLUMN3, false, false, false));
        addColumn(ColumnNames::_COLUMN3, column);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnText(ColumnNames::_COLUMN4, false, false, false));
        addColumn(ColumnNames::_COLUMN4, column);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnReal(ColumnNames::_COLUMN5, false, false, false));
        addColumn(ColumnNames::_COLUMN5, column);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnReal(ColumnNames::_COLUMN6, false, false, false));
        addColumn(ColumnNames::_COLUMN6, column);
    }
}

MockTable::~MockTable()
{

}


//--------------------------------------------------------
const std::string MockTable2::_TABLE_NAME = "mock_table_2";

const std::string MockTable2::ColumnNames::_COLUMN1 = "C1";
const std::string MockTable2::ColumnNames::_COLUMN2 = "C2";
MockTable2::MockTable2() : Sqlite3Table(_TABLE_NAME)
{
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnInteger(ColumnNames::_COLUMN1, false, false, false));
        ForeignKeyProperties properties;
        properties.cascade_on_delete = true;
        properties.reference_table = MockTable::_TABLE_NAME;
        properties.reference_column = MockTable::ColumnNames::_COLUMN1;
        addColumn(ColumnNames::_COLUMN1, column, properties);
    }
    {
        std::unique_ptr<Sqlite3Column> column(new Sqlite3ColumnText(ColumnNames::_COLUMN2, false, false, false));
        addColumn(ColumnNames::_COLUMN2, column);
    }
}

MockTable2::~MockTable2()
{

}
