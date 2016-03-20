#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include "mock_db.h"

class DbTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( DbTest );
    CPPUNIT_TEST( testCreate );
    CPPUNIT_TEST( testInsert );
    CPPUNIT_TEST( testContains );
    CPPUNIT_TEST( testSelect );
    CPPUNIT_TEST( testRemove );
    CPPUNIT_TEST( testInsertWithForeignKeys );
    CPPUNIT_TEST( testRemoveWithForeignKeys );

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testCreate();
    void testInsert();
    void testSelect();
    void testRemove();
    void testContains();
    void testInsertWithForeignKeys();
    void testRemoveWithForeignKeys();

private:
    void insert();
    void insertForeign();
    MockDB mMockDb;
    QueryData mInsert1;
    QueryData mValidForeignInsert1;
    QueryData mInValidForeignInsert1;
    std::vector<QueryData> mContainsY;
    std::vector<QueryData> mContainsN;
};

CPPUNIT_TEST_SUITE_REGISTRATION( DbTest );
