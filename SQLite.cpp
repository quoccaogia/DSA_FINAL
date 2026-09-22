#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

// Struct SinhVien khớp với thiết kế bảng trong SQLite
struct Student {
    string mssv;
    string ho_ten;
    double gpa;
    int diem_rl;
    int diem_ctxh;
    int so_hoat_dong;
};

class SQLiteHandler {
private:
    sqlite3* db;
    string db_name;

public:
    SQLiteHandler(const string& database_path = "scholarship.db") : db_name(database_path), db(nullptr) {
        if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
            cerr << "Lỗi kết nối SQLite: " << sqlite3_errmsg(db) << endl;
        }
        else {
            initDB();
        }
    }

    ~SQLiteHandler() {
        if (db) {
            sqlite3_close(db);
        }
    }

    // 1. Thiết kế & Khởi tạo bảng sinh_vien trong SQLite
    void initDB() {
        string sql = "CREATE TABLE IF NOT EXISTS sinh_vien ("
            "mssv TEXT PRIMARY KEY, "
            "ho_ten TEXT NOT NULL, "
            "gpa REAL NOT NULL, "
            "diem_rl INTEGER NOT NULL, "
            "diem_ctxh INTEGER NOT NULL, "
            "so_hoat_dong INTEGER NOT NULL);";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Lỗi tạo bảng: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    // 2. Viết hàm load dữ liệu từ SQLite vào danh sách (để nạp vào Hash Table)
    vector<Student> loadData() {
        vector<Student> students;
        string sql = "SELECT mssv, ho_ten, gpa, diem_rl, diem_ctxh, so_hoat_dong FROM sinh_vien;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Student s;
                s.mssv = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                s.ho_ten = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                s.gpa = sqlite3_column_double(stmt, 2);
                s.diem_rl = sqlite3_column_int(stmt, 3);
                s.diem_ctxh = sqlite3_column_int(stmt, 4);
                s.so_hoat_dong = sqlite3_column_int(stmt, 5);

                students.push_back(s);
            }
        }
        sqlite3_finalize(stmt);
        return students; // Trả về danh sách sinh viên cho Hash Table dùng
    }

    // 3. Viết hàm thêm sinh viên xuống SQLite
    bool insertStudent(const Student& s) {
        string sql = "INSERT INTO sinh_vien (mssv, ho_ten, gpa, diem_rl, diem_ctxh, so_hoat_dong) VALUES (?, ?, ?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, s.mssv.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, s.ho_ten.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 3, s.gpa);
            sqlite3_bind_int(stmt, 4, s.diem_rl);
            sqlite3_bind_int(stmt, 5, s.diem_ctxh);
            sqlite3_bind_int(stmt, 6, s.so_hoat_dong);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 4. Viết hàm cập nhật sinh viên trên SQLite
    bool updateStudent(const Student& s) {
        string sql = "UPDATE sinh_vien SET ho_ten = ?, gpa = ?, diem_rl = ?, diem_ctxh = ?, so_hoat_dong = ? WHERE mssv = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, s.ho_ten.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 2, s.gpa);
            sqlite3_bind_int(stmt, 3, s.diem_rl);
            sqlite3_bind_int(stmt, 4, s.diem_ctxh);
            sqlite3_bind_int(stmt, 5, s.so_hoat_dong);
            sqlite3_bind_text(stmt, 6, s.mssv.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 5. Viết hàm xóa sinh viên trên SQLite
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

    // 6. Viết hàm sàng lọc đủ / không đủ điều kiện học bổng
    void filterEligibility(const vector<Student>& allStudents, double min_gpa, int min_rl, int min_ctxh, int min_hd,
        vector<Student>& eligible, vector<Student>& ineligible) {
        eligible.clear();
        ineligible.clear();

        for (const auto& s : allStudents) {
            bool is_qualified = (s.gpa >= min_gpa) &&
                (s.diem_rl >= min_rl) &&
                (s.diem_ctxh >= min_ctxh) &&
                (s.so_hoat_dong >= min_hd);
            if (is_qualified) {
                eligible.push_back(s);
            }
            else {
                ineligible.push_back(s);
            }
        }
    }
};
