#ifndef SQLITE_3_TABLE_H
#define SQLITE_3_TABLE_H

#include <string>
#include "sqlite3_column.h"
#include <map>
#include <memory>
#include <vector>

struct QueryData{
    QueryData(std::map<std::string,std::string> inStringColumns,
              std::map<std::string, int> inIntColumns,
              std::map<std::string, double> inDoubleColumns,
              std::vector<std::string> inNullColumns = std::vector<std::string>()) :
        mStringColumns(inStringColumns), mIntColumns(inIntColumns), mDoubleColumns(inDoubleColumns), mNullColumns(inNullColumns) {}
    QueryData() {}

    std::map<std::string,std::string> mStringColumns;
    std::map<std::string, int> mIntColumns;
    std::map<std::string, double> mDoubleColumns;
    std::vector<std::string> mNullColumns;
};

class Sqlite3Table
{
public:
    Sqlite3Table(std::string inName);
    ~Sqlite3Table();

    void addColumn(const std::string & name, std::unique_ptr<Sqlite3Column> & inColumn);
    std::string getCreationCode();

    sqlite3_stmt * select(sqlite3 * db,
                          QueryData & inQuerydata) const;

    bool contains(sqlite3 * db,
                  QueryData & inQuerydata) const;

    friend std::ostream& operator<<(std::ostream& os, const Sqlite3Table& sqlitetable);

private:
    std::vector<std::string> extract_column_names(const QueryData & inQuerydata) const;

    void build_creation_code();
    const std::string mName;
    std::string mCreationCode;
    std::map<std::string, std::unique_ptr<Sqlite3Column>> mColumns;
};


#endif //SQLITE_3_TABLE_H
