#ifndef SQLITE_3_HELPERS_H
#define SQLITE_3_HELPERS_H
#include "sqlite3_helpers.h"
#include "sqlite3.h"
#include <iostream>

inline bool checkIfSqlError(int p_code, std::string file, int line, char * p_error_msg = nullptr)
{
    if(p_code != SQLITE_OK && p_code != SQLITE_DONE && p_code != SQLITE_ROW)
    {
        std::cerr << "Database failure!" << std::endl;
        std::cerr << "Error code: " << p_code << std::endl;
        std::cerr << "File: " << file << std::endl;
        std::cerr << "Line: " << line << std::endl;
        if(p_error_msg != nullptr)
        {
            std::cerr << "Error message:" << p_error_msg << std::endl;
            sqlite3_free(p_error_msg);
        }
        return false;
    }
    return true;
}

#endif
