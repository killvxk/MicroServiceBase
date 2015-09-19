/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        Database manager, simply manages the connection and sends queries.
*/

#pragma once

class SQL_Manager
{
    // Socket or filehandle.
    static void *DatabaseHandle;

protected:
    static void *QueryDatabase(const char *Query);
    static bool QueryDatabase_NoResponse(const char *Query);
    friend class SQL_Statements;
};
