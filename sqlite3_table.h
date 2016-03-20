#ifndef SQLITE_3_TABLE_H
#define SQLITE_3_TABLE_H

#include <string>
#include "sqlite3_column.h"
#include <map>
#include <memory>
#include <vector>
#include "query_data.h"

class Sqlite3Table
{
protected:
    struct ForeignKeyProperties
    {
        std::string reference_table;
        std::string reference_column;
        bool cascade_on_delete;
    };

public:
    Sqlite3Table(std::string inName);
    ~Sqlite3Table();

    void addColumn(const std::string & name, std::unique_ptr<Sqlite3Column> & inColumn);
    void addColumn(const std::string & name, std::unique_ptr<Sqlite3Column> & inColumn, const ForeignKeyProperties & inProperties);

    std::string getCreationCode();

    bool select(sqlite3 * db,
                QueryData & inQuerydata,
                std::vector<QueryData> & outQuerydata) const;

    bool insert(sqlite3 * db,
                QueryData & inQuerydata) const;

    bool contains(sqlite3 * db,
                  QueryData & inQuerydata) const;

    bool remove(sqlite3 * db,
                QueryData & inQuerydata,
                int & outColumnsRemoved) const;

    int rowCount(sqlite3 * db);

    friend std::ostream& operator<<(std::ostream& os, const Sqlite3Table& sqlitetable);

    Sqlite3Column * getColumn(const std::string & name);

private:
    std::vector<std::string> extract_column_names(const QueryData & inQuerydata) const;

    void build_creation_code();
    const std::string mName;
    std::string mCreationCode;
    std::map<std::string, std::unique_ptr<Sqlite3Column>> mColumns;
    std::map<std::string, ForeignKeyProperties> mForeignKeysAndReferences;
};


#endif //SQLITE_3_TABLE_H
