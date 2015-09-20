/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-20
    License: LGPL 3.0
    Notes:
        Simply methods that log strings to a file.
*/

#include "LogToDisk.h"
#include "..\Utility\String\VariadicArg.h"
#include <stdio.h>
#include <memory.h>
#include <chrono>

static auto StartupTime = std::chrono::steady_clock::now();
static char Filename[] = "MicroserviceBase.log";

bool LogToFile::PrintString(const char *String)
{
    // Print to stdout.
    printf(String);
    printf("\n");

    // Log to file.
    FILE *File = fopen(Filename, "a");
    if (File)
    {
        fwrite(String, 1, strlen(String), File);
        fwrite("\n\0", 1, 1, File);
        fclose(File);
        return true;
    }

    return false;
}
bool LogToFile::PrintTimedString(const char *String)
{
    return PrintString(va("%llu | %s", (std::chrono::steady_clock::now() - StartupTime).count(), String));
}
void LogToFile::DeleteLog()
{
    std::remove(Filename);
}
