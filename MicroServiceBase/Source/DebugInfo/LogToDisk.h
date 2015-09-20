/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-20
    License: LGPL 3.0
    Notes:
        Simply methods that log strings to a file.
*/

#pragma once

class LogToFile
{
public:
    static bool PrintString(const char *String);        // > String
    static bool PrintTimedString(const char *String);   // > Time | String
    static void DeleteLog();
};
