#pragma once

#include <string>
#include <sqlite3.h>

#include "Alert.h"

class SqliteOutput {
private:
    sqlite3* db = nullptr;

public:
    explicit SqliteOutput(const std::string& path);
    ~SqliteOutput();

    SqliteOutput(const SqliteOutput&) = delete;
    SqliteOutput& operator=(const SqliteOutput&) = delete;

    void write(const Alert& alert);
};
