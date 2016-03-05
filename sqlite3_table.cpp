#include "sqlite3_table.h"
#include <sstream>
#include <stdarg.h>
#include <sqlite3.h>

#include "sqlite3_helpers.h"

Sqlite3Table::Sqlite3Table(std::string inName) : mName(inName)
{
    build_creation_code();
}

Sqlite3Table::~Sqlite3Table()
{

}

void Sqlite3Table::build_creation_code()
{
    std::stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " + mName + "( ";

    for(auto it(mColumns.begin()); it != mColumns.end(); it++)
    {
        ss << (it->second->getCreationCode());
    }

    ss << ");";
    mCreationCode = ss.str();
}

std::string Sqlite3Table::getCreationCode()
{
    build_creation_code();
    return mCreationCode;
}

void Sqlite3Table::addColumn(const std::string & name, std::unique_ptr<Sqlite3Column> & inColumn)
{
    mColumns.emplace(name, std::move(inColumn));
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Table& sqlitetable)
{
    os << sqlitetable.mName;
}

sqlite3_stmt * Sqlite3Table::select(sqlite3 * db,
                                    QueryData & inQuerydata) const
{
    std::vector<std::string> all_columns (extract_column_names(inQuerydata));

    std::stringstream ss;
    ss << "SELECT * FROM " + mName;

    if(!all_columns.empty())
    {
        ss <<  + " WHERE ";
    }

    bool first_add(true);
    for(auto it(all_columns.begin()); it != all_columns.end(); it++)
    {
        if(!first_add)
        {
            ss << " AND ";
            first_add = false;
        }
        ss << *it << " = @" << *it;
    }

    sqlite3_stmt * statement;
    // Prepare the statement
    exit_on_error(sqlite3_prepare_v2(db, ss.str().c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    // PERFORM BINDING
    for(auto it(inQuerydata.mStringColumns.begin()); it != inQuerydata.mStringColumns.end(); it++)
        exit_on_error(mColumns.find(it->first)->second->bind(statement, &it->second), __LINE__);
    for(auto it(inQuerydata.mIntColumns.begin()); it != inQuerydata.mIntColumns.end(); it++)
        exit_on_error(mColumns.find(it->first)->second->bind(statement, &it->second), __LINE__);
    for(auto it(inQuerydata.mDoubleColumns.begin()); it != inQuerydata.mDoubleColumns.end(); it++)
        exit_on_error(mColumns.find(it->first)->second->bind(statement, &it->second), __LINE__);
    for(auto it(inQuerydata.mNullColumns.begin()); it != inQuerydata.mNullColumns.end(); it++)
        exit_on_error(mColumns.find(*it)->second->bind(statement, nullptr), __LINE__);

    return statement;
}

bool Sqlite3Table::contains(sqlite3 * db,
                            QueryData & inQuerydata) const
{
    sqlite3_stmt * statement (select(db, inQuerydata));

    bool contains(sqlite3_step(statement) == SQLITE_ROW);

    sqlite3_finalize(statement);
    return contains;
        //    {

//    sqlite3 * db (open_db());

//    char *error_msg = 0;
//    sqlite3_stmt * statement;

//    // Prepare the statement
//    exit_on_error(sqlite3_prepare_v2(db, m_contains_statement.c_str(),-1/*null-terminated*/,&statement,NULL), __LINE__);

    //    /***********
    //     * BINDING *
    //     ***********/
    //    // SPECIES
    //    bind_text(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SPECIES), set_to_string(entry.species));
    //    // SLOPE
    //    bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_SLOPE), entry.slope);
    //    // HUMIDITY & TEMPERATURE & ILLUMINATION
    //    for(int i (0); i < 12; i++)
    //    {
    //        // Humidity
    //        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_HUMIDITIES,i), entry.humidities[i]);
    //        // Illumination
    //        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_ILLUMINATIONS,i), entry.illuminations[i]);
    //        // Temperature
    //        bind_int(statement, _SCHEMA.columns.get(DatabaseSchema::Columns::_TEMPERATURES,i), entry.temperatures[i]);
    //    }

    //    dir_name = -1;
    //    while(sqlite3_step(statement) == SQLITE_ROW)
    //    {
    //        for(int c (0); c < sqlite3_column_count(statement); c++)
    //        {
    //            std::string column_name(sqlite3_column_name(statement, c));
    //            if(column_name == _SCHEMA.columns.get(DatabaseSchema::Columns::_ID))
    //            {
    //                dir_name = sqlite3_column_int(statement,c);
    //            }
    //        }
    //    }

    //    // finalise the statement
    //    sqlite3_finalize(statement);
    //    sqlite3_close(db);

    //    return dir_name != -1;
}

std::vector<std::string> Sqlite3Table::extract_column_names(const QueryData & inQuerydata) const
{
    std::vector<std::string> all_columns;

    for(auto it(inQuerydata.mStringColumns.cbegin()); it != inQuerydata.mStringColumns.cend(); it++)
        all_columns.push_back(it->first);
    for(auto it(inQuerydata.mIntColumns.cbegin()); it != inQuerydata.mIntColumns.cend(); it++)
        all_columns.push_back(it->first);
    for(auto it(inQuerydata.mDoubleColumns.cbegin()); it != inQuerydata.mDoubleColumns.cend(); it++)
        all_columns.push_back(it->first);
    for(auto it(inQuerydata.mNullColumns.cbegin()); it != inQuerydata.mNullColumns.cend(); it++)
        all_columns.push_back(*it);
}
