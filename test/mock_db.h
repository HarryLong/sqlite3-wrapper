#ifndef MOCK_DB
#define MOCK_DB

#include "../sqlite3_db.h"

class MockDB : public Sqlite3Db
{
public:
    MockDB();
    ~MockDB();

    static const std::string _PATH;
};

#endif //MOCK_DB
