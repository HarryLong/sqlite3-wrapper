#ifndef QUERY_DATA_H
#define QUERY_DATA_H

#include <map>
#include <vector>

class QueryData{
public:
    QueryData(std::map<std::string,std::string> inStringColumns,
              std::map<std::string, int> inIntColumns,
              std::map<std::string, double> inDoubleColumns,
              std::vector<std::string> inNullColumns = std::vector<std::string>());
    QueryData();

    std::map<std::string,std::string> mStringColumns;
    std::map<std::string, int> mIntColumns;
    std::map<std::string, double> mDoubleColumns;
    std::vector<std::string> mNullColumns;

    int columnCount();
};

#endif //QUERY_DATA_H
