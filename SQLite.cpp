#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <sqlite3.h>

using namespace std;
struct Student {
    string mssv;
    string ho_ten;
    double gpa;
    int diem_rl;
    int diem_ctxh;
    int so_hoat_dong;
};

// So sánh độ ưu tiên Học Bổng để sắp xếp trong BST (std::set)[cite: 1]
// Thứ tự ưu tiên: GPA -> Điểm RL -> Điểm CTXH -> Số HĐ[cite: 1]
struct CmpScholarship {
    bool operator()(const Student* a, const Student* b) const {
        if (a->gpa != b->gpa) return a->gpa > b->gpa;
        if (a->diem_rl != b->diem_rl) return a->diem_rl > b->diem_rl;
        if (a->diem_ctxh != b->diem_ctxh) return a->diem_ctxh > b->diem_ctxh;
        if (a->so_hoat_dong != b->so_hoat_dong) return a->so_hoat_dong > b->so_hoat_dong;
        return a->mssv < b->mssv; // Nếu bằng hết thì so sánh MSSV để tránh trùng lặp trong set
    }
};

class ScholarshipSystem {
private:
    sqlite3* db;

    // In-Memory Core theo đúng sơ đồ[cite: 1]
    unordered_map<string, Student*> by_mssv;             // Hash Table: O(1) lookup[cite: 1]
    set<Student*, CmpScholarship> by_priority;           // BST (set): Tự động sắp xếp học bổng[cite: 1]

    void clearMemory() {
        for (auto& pair : by_mssv) {
            delete pair.second;
        }
        by_mssv.clear();
        by_priority.clear();
    }

public:
    ScholarshipSystem(const string& db_name) {
        if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
            cerr << "Lỗi mở Database: " << sqlite3_errmsg(db) << endl;
        } else {
            initDB();
        }
    }

    ~ScholarshipSystem() {
        clearMemory();
        sqlite3_close(db);
    }

    // 1. Thiết kế bảng sinh_vien trong SQLite[cite: 1]
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

    // 2. Viết hàm load dữ liệu từ SQLite vào RAM (In-Memory Core)[cite: 1]
    void loadData() {
        clearMemory();
        string sql = "SELECT mssv, ho_ten, gpa, diem_rl, diem_ctxh, so_hoat_dong FROM sinh_vien;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                Student* s = new Student();
                s->mssv = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                s->ho_ten = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                s->gpa = sqlite3_column_double(stmt, 2);
                s->diem_rl = sqlite3_column_int(stmt, 3);
                s->diem_ctxh = sqlite3_column_int(stmt, 4);
                s->so_hoat_dong = sqlite3_column_int(stmt, 5);

                // Lưu vào RAM
                by_mssv[s->mssv] = s;
                by_priority.insert(s);
            }
        }
        sqlite3_finalize(stmt);
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
                loadData(); // Đọc lại dữ liệu để đồng bộ RAM
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 4. Viết hàm cập nhật sinh viên trên SQLite
    bool updateStudent(const string& mssv, double gpa, int diem_rl, int diem_ctxh, int so_hoat_dong) {
        string sql = "UPDATE sinh_vien SET gpa = ?, diem_rl = ?, diem_ctxh = ?, so_hoat_dong = ? WHERE mssv = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_double(stmt, 1, gpa);
            sqlite3_bind_int(stmt, 2, diem_rl);
            sqlite3_bind_int(stmt, 3, diem_ctxh);
            sqlite3_bind_int(stmt, 4, so_hoat_dong);
            sqlite3_bind_text(stmt, 5, mssv.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                loadData(); // Cập nhật lại RAM
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
                loadData(); // Cập nhật lại RAM
                return true;
            }
        }
        sqlite3_finalize(stmt);
        return false;
    }

    // 6. Tạo dữ liệu mẫu để dùng cho demo[cite: 1]
    void seedData() {
        vector<Student> samples = {
            {"2021001", "Nguyễn Văn A", 3.45, 80, 60, 3},
            {"2021002", "Trần Thị B", 3.78, 88, 70, 6},
            {"2021003", "Lê Văn C", 3.65, 85, 65, 4},
            {"2021056", "Phạm Đăng D", 3.85, 90, 75, 4},
            {"2021123", "Hoàng Anh E", 3.92, 95, 80, 5}
        };

        for (const auto& s : samples) {
            insertStudent(s);
        }
    }

    // 7. Viết hàm sàng lọc đủ / không đủ điều kiện xét học bổng
    void filterEligibility(double min_gpa, int min_rl, int min_ctxh, int min_hd,
                           vector<Student*>& eligible, vector<Student*>& ineligible) {
        eligible.clear();
        ineligible.clear();

        for (const auto& pair : by_mssv) {
            Student* s = pair.second;
            bool is_qualified = (s->gpa >= min_gpa) &&
                                (s->diem_rl >= min_rl) &&
                                (s->diem_ctxh >= min_ctxh) &&
                                (s->so_hoat_dong >= min_hd);
            if (is_qualified) {
                eligible.push_back(s);
            } else {
                ineligible.push_back(s);
            }
        }
    }

    // In danh sách xếp hạng theo học bổng (Xử lý bằng BST - Top-K)[cite: 1]
    void displayTopK(int k) {
        cout << "\n=== TOP " << k << " HỌC BỔNG (ĐƯỢC SẮP XẾP BỞI BST) ===" << endl;
        int count = 0;
        for (const auto* s : by_priority) {
            if (++count > k) break;
            cout << count << ". [" << s->mssv << "] " << s->ho_ten 
                 << " | GPA: " << s->gpa 
                 << " | ĐRL: " << s->diem_rl 
                 << " | ĐCTXH: " << s->diem_ctxh 
                 << " | HĐ: " << s->so_hoat_dong << endl;
        }
    }
};

// ==========================================
// CHƯƠNG TRÌNH DEMO CHẠY THỬ
// ==========================================
int main() {
    ScholarshipSystem app("scholarship.db");

    // 1. Tạo dữ liệu mẫu
    app.seedData();
    cout << "-> Đã khởi tạo dữ liệu mẫu thành công." << endl;

    // 2. Thêm mới sinh viên
    Student sNew = {"2021099", "Lê Thị F", 3.10, 75, 40, 2};
    app.insertStudent(sNew);
    cout << "-> Đã thêm sinh viên mới: 2021099" << endl;

    // 3. Cập nhật sinh viên
    app.updateStudent("2021001", 3.60, 82, 65, 4);
    cout << "-> Đã cập nhật sinh viên: 2021001" << endl;

    // 4. Xóa sinh viên
    app.deleteStudent("2021003");
    cout << "-> Đã xóa sinh viên: 2021003" << endl;

    // 5. Hiển thị danh sách ưu tiên Top-K Học bổng
    app.displayTopK(5);

    // 6. Sàng lọc Đủ / Không đủ điều kiện
    vector<Student*> eligible;
    vector<Student*> ineligible;
    // Tiêu chuẩn: GPA >= 3.2, ĐRL >= 80, ĐCTXH >= 50, HĐ >= 3
    app.filterEligibility(3.2, 80, 50, 3, eligible, ineligible);

    cout << "\n=== DANH SÁCH ĐỦ ĐIỀU KIỆN (" << eligible.size() << " SV) ===" << endl;
    for (const auto* s : eligible) {
        cout << "- [" << s->mssv << "] " << s->ho_ten << " | GPA: " << s->gpa << endl;
    }

    cout << "\n=== DANH SÁCH KHÔNG ĐỦ ĐIỀU KIỆN (" << ineligible.size() << " SV) ===" << endl;
    for (const auto* s : ineligible) {
        cout << "- [" << s->mssv << "] " << s->ho_ten << " | GPA: " << s->gpa << endl;
    }

    return 0;
}
