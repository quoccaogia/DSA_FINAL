#include <student.cpp>
#include <unordered_map>
#include <set>
#include <CmpHocBong.cpp>

class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, Student*> dshocbong;

    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<Student*, CmpHocBong> cayhocbong;


public:
    // các hàm...
    Student* get_Student(string mssv){
        return dshocbong[mssv];
    }

    bool regrade_Req(string mssv){
        //Từ từ chờ fe
    }

    bool delete_Student(string mssv){
        delete dshocbong[mssv];
    }

    /// HÀM CẬP NHẬT THÔNG TIN C++
    bool updateScore(const string& mssv,
        float gpaHe4Moi,
        float gpaHe10Moi,
        int diemRenLuyenMoi) {

        auto it = bangBam.find(mssv);
        if (it == bangBam.end()) {
            return false;
        }

        Student* sinhVien = it->second;

        // Xóa khỏi Set trước khi sửa
        danhSachHocBong.erase(sinhVien);

        // Cập nhật điểm
        sinhVien->setGPA4(gpaHe4Moi);
        sinhVien->setGPA10(gpaHe10Moi);
        sinhVien->setDRL(diemRenLuyenMoi);

        // Chèn lại vào Set
        danhSachHocBong.insert(sinhVien);

        return true;
    }

    // 2. HÀM NHẬN JSON TỪ FRONTEND (gọi updateScore)
// =====================================================
    bool capNhatTuJSON(const json& duLieuJSON) {
        if (!duLieuJSON.contains("MSSV") || duLieuJSON["MSSV"].is_null()) {
            return false;
        }

        string mssv = duLieuJSON["MSSV"];

        auto it = bangBam.find(mssv);
        if (it == bangBam.end()) {
            return false;
        }

        Student* sinhVien = it->second;

        float gpa4Moi = sinhVien->getGPA4();
        float gpa10Moi = sinhVien->getGPA10();
        int drlMoi = sinhVien->getDRL();

        // Chỉ lấy giá trị mới nếu Frontend gửi lên
        if (duLieuJSON.contains("gpa_4") && !duLieuJSON["gpa_4"].is_null()) {
            gpa4Moi = duLieuJSON["gpa_4"];
        }
        if (duLieuJSON.contains("gpa_10") && !duLieuJSON["gpa_10"].is_null()) {
            gpa10Moi = duLieuJSON["gpa_10"];
        }
        if (duLieuJSON.contains("DRL") && !duLieuJSON["DRL"].is_null()) {
            drlMoi = duLieuJSON["DRL"];
        }

        return updateScore(mssv, gpa4Moi, gpa10Moi, drlMoi);
    }

    // 3. HÀM LẤY TOP-K (chỉ dùng Set)
// =====================================================
    vector<Student*> getTopK(int soLuongCanLay) {
        vector<Student*> danhSachKetQua;
        int soLuongDaLay = 0;

        for (Student* sinhVien : danhSachHocBong) {
            if (soLuongDaLay >= soLuongCanLay) break;
            danhSachKetQua.push_back(sinhVien);
            ++soLuongDaLay;
        }
        return danhSachKetQua;
    }

    // =====================================================
    // 4. HÀM LỌC THEO KHOẢNG ĐIỂM (chỉ dùng Set)
    // =====================================================

    // Lọc theo khoảng GPA hệ 4
    vector<Student*> filterByGPA4(float gpaThapNhat, float gpaCaoNhat) {
        vector<Student*> danhSachKetQua;

        for (Student* sinhVien : danhSachHocBong) {
            if (sinhVien->getGPA4() >= gpaThapNhat &&
                sinhVien->getGPA4() <= gpaCaoNhat) {
                danhSachKetQua.push_back(sinhVien);
            }
        }
        return danhSachKetQua;
    }

    // 5. HÀM XÓA SINH VIÊN (dùng cả Hash Table + Set)
// =====================================================
    bool xoaSinhVien(const string& mssv) {
        auto it = bangBam.find(mssv);
        if (it == bangBam.end()) {
            return false;
        }

        Student* sinhVien = it->second;

        // Xóa khỏi cả hai cấu trúc
        danhSachHocBong.erase(sinhVien);
        bangBam.erase(it);

        // Nếu cần giải phóng bộ nhớ:
        // delete sinhVien;

        return true;
    }





};