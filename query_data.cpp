#include "query_data.h"
#include <algorithm>

QueryData::QueryData(std::map<std::string,std::string> inStringColumns,
          std::map<std::string, int> inIntColumns,
          std::map<std::string, double> inDoubleColumns,
          std::vector<std::string> inNullColumns) :
    mStringColumns(inStringColumns), mIntColumns(inIntColumns), mDoubleColumns(inDoubleColumns), mNullColumns(inNullColumns)
{

}

QueryData::QueryData()
{

}

int QueryData::columnCount()
{
    return mStringColumns.size() +
            mIntColumns.size() +
            mDoubleColumns.size() +
            mNullColumns.size();
}
