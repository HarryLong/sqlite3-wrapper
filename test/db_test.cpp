#include "db_test.h"
#include "mock_table.h"
#include "../query_data.h"
#include <stdio.h>

void DbTest::setUp()
{
    mInsert1.mIntColumns.emplace(MockTable::ColumnNames::_COLUMN1, 100);
    mInsert1.mIntColumns.emplace(MockTable::ColumnNames::_COLUMN2, 200);
    mInsert1.mStringColumns.emplace(MockTable::ColumnNames::_COLUMN3, "Column 3");
    mInsert1.mStringColumns.emplace(MockTable::ColumnNames::_COLUMN4, "Column 4");
    mInsert1.mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN5, 100.1);
    mInsert1.mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN6, 200.2);

    mValidForeignInsert1.mIntColumns.emplace(MockTable2::ColumnNames::_COLUMN1, 100);
    mValidForeignInsert1.mStringColumns.emplace(MockTable2::ColumnNames::_COLUMN2, "Column 2");

    mInValidForeignInsert1.mIntColumns.emplace(MockTable2::ColumnNames::_COLUMN1, 200);
    mInValidForeignInsert1.mStringColumns.emplace(MockTable2::ColumnNames::_COLUMN2, "Column 2");

    mContainsY.resize(3);
    mContainsY[0].mIntColumns.emplace(MockTable::ColumnNames::_COLUMN1, 100);
    mContainsY[1].mStringColumns.emplace(MockTable::ColumnNames::_COLUMN3, "Column 3");
    mContainsY[2].mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN6, 200.2);

    mContainsN.resize(3);
    mContainsN[0].mIntColumns.emplace(MockTable::ColumnNames::_COLUMN1, 101);
    mContainsN[1].mStringColumns.emplace(MockTable::ColumnNames::_COLUMN3, "Column 123");
    mContainsN[2].mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN6, 200.1);

    QueryData mContainsN;
}

void DbTest::tearDown()
{
    std::remove(MockDB::_PATH.c_str());
}

void DbTest::insert()
{
    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));
    sqlite3 * db(mMockDb.open());
    table->insert(db, mInsert1);
    sqlite3_close(db);
}

void DbTest::insertForeign()
{
    Sqlite3Table * table (mMockDb.getTable(MockTable2::_TABLE_NAME));
    sqlite3 * db(mMockDb.open());
    table->insert(db, mValidForeignInsert1);
    sqlite3_close(db);
}

void DbTest::testCreate()
{
    std::cout << "Creation test...";
    CPPUNIT_ASSERT(mMockDb.executeCreationCode());
    std::cout << "OK!";
}

void DbTest::testInsert()
{
    std::cout << "Insertion test...";
    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));

    sqlite3 * db(mMockDb.open());

    CPPUNIT_ASSERT(table->insert(db, mInsert1));

    sqlite3_close(db);

    std::cout << "OK!";
}

void DbTest::testSelect()
{
    insert();

    std::cout << "Selection test...";
    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));
    QueryData select;
    std::vector<QueryData> results;

    sqlite3 * db(mMockDb.open());

    table->select(db, select, results);
    CPPUNIT_ASSERT(results.size() == 1);

    sqlite3_close(db);

    std::cout << "OK!";
}

//void testRemove();
void DbTest::testContains()
{
    insert();

    std::cout << "Containment test...";
    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));
    sqlite3 * db(mMockDb.open());

    for(QueryData & query : mContainsY)
        CPPUNIT_ASSERT(table->contains(db, query));
    for(QueryData & query : mContainsN)
        CPPUNIT_ASSERT(!table->contains(db, query));

    sqlite3_close(db);

    std::cout << "OK!";

//    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));
//    QueryData contains;
//    contains.mIntColumns.emplace(MockTable::ColumnNames::_COLUMN1, 100);
////    insert.mIntColumns.emplace(MockTable::ColumnNames::_COLUMN2, 200);
////    insert.mStringColumns.emplace(MockTable::ColumnNames::_COLUMN3, "Column 3");
////    insert.mStringColumns.emplace(MockTable::ColumnNames::_COLUMN4, "Column 4");
////    insert.mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN5, 100.100);
////    insert.mDoubleColumns.emplace(MockTable::ColumnNames::_COLUMN6, 200.200);
//    sqlite3 * db(mMockDb.open());

//    CPPUNIT_ASSERT(table->contains(db, contains));

//    sqlite3_close(db);


////table->contains()
//    CPPUNIT_ASSERT(table->insert(mMockDb.open(), insert));
}

void DbTest::testRemove()
{
    std::cout << "Deletion test...";
    Sqlite3Table * table (mMockDb.getTable(MockTable::_TABLE_NAME));
    sqlite3 * db(mMockDb.open());

    int removed_columns(0);
    insert();
    for(QueryData & query : mContainsN)
    {
        table->remove(db, query, removed_columns);
        CPPUNIT_ASSERT(removed_columns == 0);
    }
    for(QueryData & query : mContainsY)
    {
        table->remove(db, query, removed_columns);
        CPPUNIT_ASSERT(removed_columns == 1);
        insert();
    }

    sqlite3_close(db);

    std::cout << "OK!";
}

void DbTest::testInsertWithForeignKeys()
{
    std::cout << "Insertion with foreigh keys test...";
    insert();

    Sqlite3Table * table (mMockDb.getTable(MockTable2::_TABLE_NAME));
    sqlite3 * db(mMockDb.open());

    CPPUNIT_ASSERT(table->insert(db, mValidForeignInsert1));
    CPPUNIT_ASSERT(!table->insert(db, mInValidForeignInsert1));
    sqlite3_close(db);

    std::cout << "OK!";
}

void DbTest::testRemoveWithForeignKeys()
{
    std::cout << "Deletion with foreign keys test...";
    insert();
    insertForeign();

    Sqlite3Table * table1 (mMockDb.getTable(MockTable::_TABLE_NAME));
    Sqlite3Table * table2 (mMockDb.getTable(MockTable2::_TABLE_NAME));

    int removed_columns;
    sqlite3 * db(mMockDb.open());
    int before_row_count(table2->rowCount(db));
    CPPUNIT_ASSERT(before_row_count == 1);

    QueryData remove;
    table1->remove(db, remove, removed_columns);
    CPPUNIT_ASSERT(removed_columns == 1);

    int after_row_count(table2->rowCount(db));
    CPPUNIT_ASSERT(after_row_count == 0);

    sqlite3_close(db);

    std::cout << "OK!";

}

