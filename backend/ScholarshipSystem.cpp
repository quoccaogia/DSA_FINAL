#include <unordered_map>
#include <set>
#include <vector>
#include <nlohmann/json.hpp>
#include "Student.cpp"
#include "CmpHocBong.cpp" // File chứa struct/class so sánh để sort học bổng

class ScholarshipSystem {
private:
    // 1. Hash Table chính: Tra cứu nhanh sinh viên theo MSSV -> O(1)
    unordered_map<string, Student*> studentMap;

    // 2. Set tổng toàn trường: Tự động sort học bổng cho toàn trường -> O(log N)
    set<Student*, CmpHocBong> allSchoolPriority;

    // 3. Index phụ (Multi-index): Gom nhóm theo "Ngành_Khóa" 
    // Key: "Major_Cohort" (Ví dụ: "CNTT_K65")
    // Value: Một set riêng biệt đã tự động sort sẵn học bổng cho riêng ngành/khóa đó!
    unordered_map<string, set<Student*, CmpHocBong>> majorCohortIndex;

public:
    // ======================Thêm sinh viên vào hệ thống (Cập nhật đồng thời vào các cấu trúc Index)
    void addStudent(Student* student) {
        if (!student) return;

        // Lưu vào Hash Table chính
        studentMap[student->get_MSSV()] = student;

        // Nếu sinh viên đủ điều kiện xét học bổng (không rớt môn)
        if (!student->get_IsNgu()) {
            // Đưa vào set toàn trường
            allSchoolPriority.insert(student);

            // Đưa vào Index phụ theo Ngành và Khóa
            string key = student->get_Major() + "_" + student->get_Cohort();
            majorCohortIndex[key].insert(student);
        }
    }

    // ====================Cập nhật sinh viên
    bool updateStudent(nlohmann::json& data){
        Student* student = getStudent(data["MSSV"].get<string>());
        if(student == nullptr){ //Ko tìm thấy sv = ko có sinh viên để update -> trả về false
            return false;
        }

        //Kiểm tra liệu student này có trong index ko
        if (!student->get_IsNgu()) { 
            allSchoolPriority.erase(student);

            string oldKey =
                student->get_Major() + "_" + student->get_Cohort();

            auto it = majorCohortIndex.find(oldKey);

            if (it != majorCohortIndex.end()) {
                it->second.erase(student);

                if (it->second.empty())
                    majorCohortIndex.erase(it);
            }
        }


        if (!data["name"].is_null()){ //Kiểm tra xem liệu trường này có trống ko?, nếu trogn61 thì bỏ qua
        student->set_Name(data["name"].get<string>()); //Nếu ko trống thì cập nhật = setter
        }

        if (!data["gpa_4"].is_null()){
            student->set_GPA4(data["gpa_4"].get<float>());
        }

        if (!data["gpa_10"].is_null()){
            student->set_GPA10(data["gpa_10"].get<float>());
        }

        if (!data["gender"].is_null()){
            student->set_Gender(data["gender"].get<bool>());
        }

        if (!data["dateOfBirth"].is_null()){
            student->set_DateOfBirth(data["dateOfBirth"].get<string>());
        }

        if (!data["major"].is_null()){
            student->set_Major(data["major"].get<string>());
        }

        if (!data["cohort"].is_null()){
            student->set_Cohort(data["cohort"].get<string>());
        }

        if (!data["credit"].is_null()){
            student->set_Credit(data["credit"].get<int>());
        }

        if (!data["DRL"].is_null()){
            student->set_DRL(data["DRL"].get<int>());
        }

        if(!data["isNgu"].is_null()){
            student->set_IsNgu(data["isNgu"].get<bool>());
        }
        

        //Cập nhật lại index
        if (!student->get_IsNgu()) {
            allSchoolPriority.insert(student);

            string newKey =
                student->get_Major() + "_" + student->get_Cohort();

            majorCohortIndex[newKey].insert(student);
        }

        return true;
    }

    // Tìm kiếm sinh viên theo MSSV -> O(1)
    Student* getStudent(const string& mssv) {
        auto it = studentMap.find(mssv);

        if (it == studentMap.end()) {
            return nullptr;
        }

        return it->second;
    }

	// ================== 1. Lấy Top % TOÀN TRƯỜNG của sinh viên
    float getTopPercentAllSchool(Student* sv) {
        if (!sv || sv->get_IsNgu()) return -1.0f; // Không đủ điều kiện xét học bổng

        // Tìm vị trí của sinh viên trong cây toàn trường -> O(log N)
        auto it = allSchoolPriority.find(sv);
        if (it == allSchoolPriority.end()) {
            return -1.0f; // Không tìm thấy sinh viên trong danh sách xét học bổng
        }

        // Đếm thứ hạng (tính từ 1)
        int rank = distance(allSchoolPriority.begin(), it) + 1;
        int totalStudents = allSchoolPriority.size();

        if (totalStudents == 0) return 0.0f;

        // Công thức tính phần trăm: (Thứ hạng / Tổng số sinh viên) * 100
        return (static_cast<float>(rank) / totalStudents) * 100.0f;
    }

    // ======================== 2. Lấy Top % THEO NGÀNH VÀ KHÓA của sinh viên
    float getTopPercentByMajorAndCohort(Student* sv) {
        if (!sv || sv->get_IsNgu()) return -1.0f;

        // Tạo key để tra cứu index phụ
        string key = sv->get_Major() + "_" + sv->get_Cohort();
        auto mapIt = majorCohortIndex.find(key);
        if (mapIt == majorCohortIndex.end()) {
            return -1.0f; // Ngành/khóa này không tồn tại
        }

        const auto& targetSet = mapIt->second;
        auto it = targetSet.find(sv);
        if (it == targetSet.end()) {
            return -1.0f; // Sinh viên không có trong nhóm ngành này
        }

        // Đếm thứ hạng trong riêng nhóm đó
        int rank = distance(targetSet.begin(), it) + 1;
        int totalInGroup = targetSet.size();

        if (totalInGroup == 0) return 0.0f;

        // Công thức tính phần trăm theo nhóm
        return (static_cast<float>(rank) / totalInGroup) * 100.0f;
    }

    // =================== Xóa sinh viên khỏi hệ thống (Xóa sạch ở cả Map và các Set/Index)
    bool deleteStudent(const string& mssv) {
        auto it = studentMap.find(mssv);
        if (it == studentMap.end()) {
            return false;
        }

        Student* student = it->second;

        // Xóa khỏi set toàn trường
        allSchoolPriority.erase(student);

        // Xóa khỏi Index phụ Ngành_Khóa
        string key = student->get_Major() + "_" + student->get_Cohort();
        auto indexIt = majorCohortIndex.find(key);
        if (indexIt != majorCohortIndex.end()) {
            indexIt->second.erase(student);
            // (Tùy chọn) Nếu set của ngành đó trống thì có thể xóa luôn key để tiết kiệm RAM
            if (indexIt->second.empty()) {
                majorCohortIndex.erase(indexIt);
            }
        }

        // Giải phóng bộ nhớ và xóa khỏi bảng băm chính
        delete student;
        studentMap.erase(it);

        return true;
    }

    // =============================  Lấy Top K học bổng TOÀN TRƯỜNG -> O(K) cực nhanh
    vector<Student*> getTopKAllSchool(int topK) {
        vector<Student*> result;
        int count = 0;

        for (Student* sv : allSchoolPriority) {
            if (count >= topK) break;
            result.push_back(sv);
            count++;
        }

        return result;
    }

    // ================== Lấy Top K học bổng THEO NGÀNH VÀ KHÓA -> O(log M + K) cực kỳ tối ưu, không sợ "đáy"
    vector<Student*> getTopKByMajorAndCohort(const string& major, const string& cohort, int topK) {
        vector<Student*> result;
        string key = major + "_" + cohort;

        // Kiểm tra xem ngành và khóa này có tồn tại trong hệ thống index không
        auto it = majorCohortIndex.find(key);
        if (it == majorCohortIndex.end()) {
            return result; // Trả về vector rỗng
        }

        // Truy xuất thẳng vào set riêng của ngành/khóa đó (đã sort sẵn)
        const auto& targetSet = it->second;
        int count = 0;

        for (Student* sv : targetSet) {
            if (count >= topK) break;
            result.push_back(sv);
            count++;
        }

        return result;
    }
};