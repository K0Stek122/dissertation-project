#include "SqliteOutput.h"

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <filesystem>

static std::string to_iso8601(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm_buf;
    gmtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

static const char* CREATE_TABLE_SQL =
    "CREATE TABLE IF NOT EXISTS alerts ("
    "  id              INTEGER PRIMARY KEY AUTOINCREMENT,"
    "  rule_id         TEXT NOT NULL,"
    "  severity        TEXT NOT NULL,"
    "  src_ip          TEXT NOT NULL,"
    "  dst_ip          TEXT NOT NULL,"
    "  src_port        INTEGER NOT NULL,"
    "  dst_port        INTEGER NOT NULL,"
    "  matched_pattern TEXT NOT NULL,"
    "  match_offset    INTEGER NOT NULL,"
    "  tcp_flags       TEXT NOT NULL,"
    "  timestamp       TEXT NOT NULL"
    ");";

static const char* INSERT_SQL =
    "INSERT INTO alerts "
    "(rule_id, severity, src_ip, dst_ip, src_port, dst_port, matched_pattern, match_offset, tcp_flags, timestamp) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

SqliteOutput::SqliteOutput(const std::string& path) {
    auto parent = std::filesystem::path(path).parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error(std::string("Failed to open SQLite DB: ") + sqlite3_errmsg(db));
    }
    char* err = nullptr;
    if (sqlite3_exec(db, CREATE_TABLE_SQL, nullptr, nullptr, &err) != SQLITE_OK) {
        std::string msg = err;
        sqlite3_free(err);
        throw std::runtime_error("Failed to create alerts table: " + msg);
    }
}

SqliteOutput::~SqliteOutput() {
    if (db) {
        sqlite3_close(db);
    }
}

void SqliteOutput::write(const Alert& alert) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, INSERT_SQL, -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    const std::string src_ip  = alert.flow.srcIp.toString();
    const std::string dst_ip  = alert.flow.dstIp.toString();
    const std::string ts      = to_iso8601(alert.timestamp);

    sqlite3_bind_text(stmt, 1,  alert.rule_id.c_str(),         -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2,  alert.severity.c_str(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3,  src_ip.c_str(),                -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4,  dst_ip.c_str(),                -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 5,  alert.flow.srcPort);
    sqlite3_bind_int (stmt, 6,  alert.flow.dstPort);
    sqlite3_bind_text(stmt, 7,  alert.matched_pattern.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 8,  alert.match_offset);
    sqlite3_bind_text(stmt, 9,  alert.tcp_flags.c_str(),       -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, ts.c_str(),                    -1, SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}
