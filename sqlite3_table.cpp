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

    bool first_append(true);
    std::map<std::string, ForeignKeyProperties>::iterator it2;
    for(auto it(mColumns.begin()); it != mColumns.end(); it++)
    {
        if(!first_append)
            ss << ", ";
        ss << (it->second->getCreationCode());
        if((it2 = mForeignKeysAndReferences.find(it->second->name())) != mForeignKeysAndReferences.end())
        {
            ss << " REFERENCES " << it2->second.reference_table << "(" << it2->second.reference_column << ") ";
            if(it2->second.cascade_on_delete)
                ss << "ON DELETE CASCADE ";
        }

        first_append = false;
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

void Sqlite3Table::addColumn(const std::string & name, std::unique_ptr<Sqlite3Column> & inColumn, const ForeignKeyProperties & inProperties)
{
    addColumn(name, inColumn);
    mForeignKeysAndReferences.emplace(name, inProperties);
}

Sqlite3Column * Sqlite3Table::getColumn(const std::string & name)
{
    return mColumns.find(name)->second.get();
}

int Sqlite3Table::rowCount(sqlite3 * db)
{
    QueryData dummy_query;
    std::vector<QueryData> out;
    select(db, dummy_query, out);
    return out.size();
}


bool Sqlite3Table::insert(sqlite3 * db,
                            QueryData & inQuerydata) const
{

    std::vector<std::string> all_columns (extract_column_names(inQuerydata));
    if(all_columns.empty())
    {
        std::cerr << "No insertion values !" << std::endl;
        return false;
    }

    std::stringstream _mainQuerySs;
    std::stringstream _subQuerySs;

    _mainQuerySs << "INSERT INTO " + mName + " (";

    bool first_add(true);
    for(auto it(all_columns.begin()); it != all_columns.end(); it++)
    {
        if(!first_add)
        {
            _mainQuerySs << ", ";
            _subQuerySs << ", ";
        }
        _mainQuerySs << *it;
        _subQuerySs << "@" << *it;
        first_add = false;
    }

    _mainQuerySs << ") " << "VALUES (" << _subQuerySs.str() << ");";

    bool _success(true);

    sqlite3_stmt * statement;

    std::cout << "Insert: " << _mainQuerySs.str() << std::endl;

    // Prepare the statement
    checkIfSqlError(sqlite3_prepare_v2(db, _mainQuerySs.str().c_str(),-1/*null-terminated*/,&statement,NULL), __FILE__, __LINE__);

    // PERFORM BINDING
    for(auto it(inQuerydata.mStringColumns.begin()); it != inQuerydata.mStringColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mIntColumns.begin()); it != inQuerydata.mIntColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mDoubleColumns.begin()); it != inQuerydata.mDoubleColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mNullColumns.begin()); it != inQuerydata.mNullColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(*it)->second->bind(statement, nullptr), __FILE__, __LINE__));
    if(!_success) // Binding failed
        return false;

    _success = (_success && checkIfSqlError(sqlite3_step(statement), __FILE__, __LINE__));
    sqlite3_finalize(statement);

    return _success;
}

bool Sqlite3Table::select(sqlite3 * db,
                            QueryData & inQuerydata,
                            std::vector<QueryData> & outQuerydata) const
{
    //    exit_on_error(sqlite3_step(statement), __LINE__);


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
    ss << ";";

    // Prepare the statement
    sqlite3_stmt * statement;
    bool _success(true);
    _success = (_success && checkIfSqlError(sqlite3_prepare_v2(db, ss.str().c_str(),-1/*null-terminated*/,&statement,NULL), __FILE__, __LINE__));

    // PERFORM BINDING
    for(auto it(inQuerydata.mStringColumns.begin()); it != inQuerydata.mStringColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mIntColumns.begin()); it != inQuerydata.mIntColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mDoubleColumns.begin()); it != inQuerydata.mDoubleColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mNullColumns.begin()); it != inQuerydata.mNullColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(*it)->second->bind(statement, nullptr), __FILE__, __LINE__));


    // Fill return data
    int _columnCount(sqlite3_column_count(statement));
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        QueryData row_result;
        for(int c (0); c < _columnCount; c++)
        {
            std::string column_name(sqlite3_column_name(statement, c));
            Sqlite3Column * column(mColumns.find(column_name)->second.get());
            Sqlite3Type column_type(column->getType());
            if(column_type == Sqlite3Null::_NAME)
            {
                row_result.mNullColumns.push_back(column_name);
            }
            else if(column_type == Sqlite3Integer::_NAME)
            {
                int * v = new int;
                column->value(statement, c, v);
                row_result.mIntColumns.emplace(column_name, *v);
                delete v;
            }
            else if(column_type == Sqlite3Real::_NAME)
            {
                double * v = new double;
                column->value(statement, c, v);
                row_result.mDoubleColumns.emplace(column_name, *v);
                delete v;
            }
            else if(column_type == Sqlite3Text::_NAME)
            {
                std::string * v = new std::string;
                column->value(statement, c, v);
                row_result.mStringColumns.emplace(column_name, *v);
                delete v;
            }
            else
            {
                _success = false;
                std::cerr << "Unimplemented type!" << std::endl;
            }
        }
        outQuerydata.push_back(row_result);
    }

    sqlite3_finalize(statement);

    return _success;
}

bool Sqlite3Table::contains(sqlite3 * db,
                            QueryData & inQuerydata) const
{
    std::vector<QueryData> result;

    bool _success(select(db, inQuerydata, result)); // First do the selection
    _success = (_success && (result.size() > 0)); // Now check there is data in the result

    return _success;
}

bool Sqlite3Table::remove(sqlite3 * db,
                         QueryData & inQuerydata,
                         int & outColumnsRemoved) const
{
    std::vector<std::string> all_columns (extract_column_names(inQuerydata));

    std::stringstream ss;
    ss << "DELETE FROM " + mName;

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
    ss << ";";

    // Prepare the statement
    sqlite3_stmt * statement;
    bool _success(true);
    _success = (_success && checkIfSqlError(sqlite3_prepare_v2(db, ss.str().c_str(),-1/*null-terminated*/,&statement,NULL), __FILE__, __LINE__));

    // PERFORM BINDING
    for(auto it(inQuerydata.mStringColumns.begin()); it != inQuerydata.mStringColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mIntColumns.begin()); it != inQuerydata.mIntColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mDoubleColumns.begin()); it != inQuerydata.mDoubleColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(it->first)->second->bind(statement, &it->second), __FILE__, __LINE__));
    for(auto it(inQuerydata.mNullColumns.begin()); it != inQuerydata.mNullColumns.end(); it++)
        _success = (_success && checkIfSqlError(mColumns.find(*it)->second->bind(statement, nullptr), __FILE__, __LINE__));

    sqlite3_step(statement);
    outColumnsRemoved = sqlite3_changes(db);

    sqlite3_finalize(statement);

    return _success;
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

    return all_columns;
}

std::ostream& operator<<(std::ostream& os, const Sqlite3Table& sqlitetable)
{
    os << sqlitetable.mName;
}
