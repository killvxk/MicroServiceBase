/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        Database manager, simply manages the connection and sends queries.
*/

#include "SQL_Manager.h"
#include "SQLite\sqlite3.h"
#include "..\STDInclude.h"
#include <stdio.h>
#include <mutex>

// Static variables.
struct SQL_Manager::DBHandle SQL_Manager::DatabaseHandle { nullptr, nullptr };
static std::mutex ThreadsafeQuery;
static std::vector<DBTable> CallbackStorage;

// Query result callback.
static int ResultAccumulator(void *Unused, int ResultCount, char **ColumnValue, char **ColumnName)
{
    DBTable LocalTable;

    for (int i = 0; i < ResultCount; ++i)
        LocalTable.push_back({ ColumnName[i], ColumnValue[i] ? ColumnValue[i] : "NULL" });
        
    // Append the table to the results.
    CallbackStorage.push_back(LocalTable);

    // No error.
    return 0;
}

// Protected methods.
bool SQL_Manager::DatabaseQuery_SingleResult(const char *Query, DBTable &Result, bool LocalDB)
{
    char *ErrorMessage = nullptr;
    bool Success = false;

    ThreadsafeQuery.lock();
    if (LocalDB)
    {
        if (DatabaseHandle.Local != nullptr)
        {
            CallbackStorage.clear();
            if (sqlite3_exec(DatabaseHandle.Local, Query, ResultAccumulator, nullptr, &ErrorMessage) != SQLITE_OK)
            {
                fprintf(stderr, "%s: SQL error - %s\n", __func__, ErrorMessage);
                sqlite3_free(ErrorMessage);
            }
            else
            {
                if(CallbackStorage.size())
                    Result = CallbackStorage.front();
                Success = true;
            }
        }
    }
    else
    {
        // TODO: Connect to a MariaDB instance.
    }
    ThreadsafeQuery.unlock();

    return Success;
}
bool SQL_Manager::DatabaseQuery_MultipleResult(const char *Query, std::vector<DBTable> &Results, bool LocalDB)
{
    char *ErrorMessage = nullptr;
    bool Success = false;

    ThreadsafeQuery.lock();
    if (LocalDB)
    {
        if (DatabaseHandle.Local != nullptr)
        {
            CallbackStorage.clear();
            if (sqlite3_exec(DatabaseHandle.Local, Query, ResultAccumulator, nullptr, &ErrorMessage) != SQLITE_OK)
            {
                fprintf(stderr, "%s: SQL error - %s\n", __func__, ErrorMessage);
                sqlite3_free(ErrorMessage);
            }
            else
            {
                if (CallbackStorage.size())
                {
                    for (auto Iterator = CallbackStorage.begin(); Iterator != CallbackStorage.end(); ++Iterator)
                        Results.push_back(*Iterator);
                }
                Success = true;
            }
        }
    }
    else
    {
        // TODO: Connect to a MariaDB instance.
    }
    ThreadsafeQuery.unlock();

    return Success;
}

// Public methods.
bool SQL_Manager::InitializeDatabase(const char *URI, bool LocalDB)
{
    if (LocalDB)
    {
        if (sqlite3_open(URI, &DatabaseHandle.Local))
            fprintf(stderr, "%s: Database error - %s\n", __FUNCTION__, sqlite3_errmsg(DatabaseHandle.Local));
        else
            return true;            
    }
    else
    {
        // TODO: Connect to a MariaDB instance.
    }

    return false;
}
