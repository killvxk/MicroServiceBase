/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        Database manager, simply manages the connection and sends queries.
*/

#pragma once
#include <vector>
#include <string>

// Database table type.
typedef std::vector<std::pair<std::string /*Column name*/, std::string /*Column value*/>> DBTable;

class SQL_Manager
{
    // Socket or filehandle.
    static struct DBHandle
    {
        void *Remote;
        class sqlite3 *Local;
    } DatabaseHandle;

protected:
    static bool DatabaseQuery_SingleResult(const char *Query, DBTable &Result, bool LocalDB);
    static bool DatabaseQuery_MultipleResult(const char *Query, std::vector<DBTable> &Results, bool LocalDB);
    friend class SQL_Statements;

public:
    static bool InitializeDatabase(const char *URI, bool LocalDB);
};
