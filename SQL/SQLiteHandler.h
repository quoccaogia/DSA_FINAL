#ifndef SQLITE_HANDLER_H
#define SQLITE_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "Student.h" // Nhớ include file Student chuẩn của nhóm

using namespace std;

class SQLiteHandler {
private:
    sqlite3* db;
    string db_name;

public:
    // 1. Khởi tạo và Mở kết nối Database
    SQLiteHandler(const string& database_path = "scholarship.db") 
        : db_name(database_path), db(nullptr) {
        if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
            cerr << "[SQLite Error] Khong the mo Database: " << sqlite3_errmsg(db) << endl;
        } else {
            initDB();
        }
    }

    // 2. Destructor tự động đóng kết nối tránh rò rỉ tài nguyên
    ~SQLiteHandler() {
        if (db) {
            sqlite3_close(db);
        }
    }

    // 3. Khởi tạo Bảng dữ liệu (Tạo bảng nếu chưa tồn tại)
    void initDB() {
        string sql = "CREATE TABLE IF NOT EXISTS sinh_vien ("
                     "mssv TEXT PRIMARY KEY, "
                     "name TEXT NOT NULL, "
                     "gpa_4 REAL NOT NULL, "
                     "gpa_10 REAL NOT NULL, "
                     "gender INTEGER NOT NULL, "
                     "date_of_birth TEXT, "
                     "major TEXT, "
                     "has_failed INTEGER NOT NULL, "
                     "credit INTEGER NOT NULL, "
                     "drl INTEGER NOT NULL);";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "[SQLite Error] Loi tao bang: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    // 4. Load toàn bộ dữ liệu từ DB lên (Để nạp vào Hash Table / Set của Backend)
    vector<Student*> loadData() {
        vector<Student*> students;
        string sql = "SELECT mssv, name, gpa_4, gpa_10, gender, date_of_birth, major, has_failed, credit, drl FROM sinh_vien;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                string mssv = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                float gpa_4 = static_cast<float>(sqlite3_column_double(stmt, 2));
                float gpa_10 = static_cast<float>(sqlite3_column_double(stmt, 3));
                bool gender = sqlite3_column_int(stmt, 4) != 0;
                string dob = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                string major = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                bool hasFailed = sqlite3_column_int(stmt, 7) != 0;
                int credit = sqlite3_column_int(stmt, 8);
                int drl = sqlite3_column_int(stmt, 9);

                students.push_back(new Student(name, gpa_4, gpa_10, gender, dob, major, mssv, hasFailed, credit, drl));
            }
        }
        sqlite3_finalize(stmt);
        return students;
    }

    // 5. Thêm mới hoặc Cập nhật sinh viên (Dùng INSERT OR REPLACE cực tiện)
    bool saveStudent(const Student* s) {
        if (!s) return false;
        string sql = "INSERT OR REPLACE INTO sinh_vien (mssv, name, gpa_4, gpa_10, gender, date_of_birth, major, has_failed, credit, drl) "
                     "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, s->get_MSSV().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, s->get_Name().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 3, s->get_GPA4());
            sqlite3_bind_double(stmt, 4, s->get_GPA10());
            sqlite3_bind_int(stmt, 5, s->get_Gender() ? 1 : 0);
            sqlite3_bind_text(stmt, 6, s->get_DateOfBirth().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 7, s->get_Major().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 8, s->get_HasFailedSubject() ? 1 : 0);
            sqlite3_bind_int(stmt, 9, s->get_Credit());
            sqlite3_bind_int(stmt, 10, s->get_DRL());

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 6. Xóa sinh viên theo MSSV
    bool deleteStudent(const string& mssv) {
        string sql = "DELETE FROM sinh_vien WHERE mssv = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, mssv.c_str(), -1, SQLITE_TRANSIENT);
            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }
};

#endif
