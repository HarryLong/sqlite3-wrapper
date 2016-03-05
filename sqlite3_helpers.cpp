#include "sqlite3_helpers.h"
#include "sqlite3.h"
#include <iostream>

void exit_on_error(int p_code, int p_line,  char * p_error_msg)
{
    if(p_code != SQLITE_OK && p_code != SQLITE_DONE)
    {
        std::cerr << "Database failure!" << std::endl;
        std::cerr << "Error code: " << p_code << std::endl;
        std::cerr << "File: " << __FILE__ << std::endl;
        std::cerr << "Line: " << p_line << std::endl;
        std::cerr << "Error message:" << p_error_msg << std::endl;
        sqlite3_free(p_error_msg);
        exit(1);
    }
}
