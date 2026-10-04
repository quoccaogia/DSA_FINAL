#include "httplib.h"
#include <nlohmann/json.hpp>
#include "ScholarshipSystem.cpp"
#include <string>
#include <vector>
#include "SQL/SQLiteHandler.h"

using namespace std;

int main() {
    httplib::Server server;

    SQLiteHandler database;   // Tạo database
    ScholarshipSystem system;  // Tạo hệ thống 

    // Khởi động: nạp sinh viên từ SQLite vào RAM
    if (!database.isFirst()) {
        for (Student* student : database.loadData()) {
            system.addStudent(student);
        }
        // [QUAN TRỌNG] Đồng bộ thứ hạng 1 lần duy nhất sau khi nạp xong DB
        system.syncAllRanks();
    }

    server.Post("/api/student", [&system, &database](const httplib::Request& req, httplib::Response& res) {

        // Nhận JSON từ FE
        nlohmann::json payload;
        try {
            payload = nlohmann::json::parse(req.body);
        }
        catch (...) {
            res.status = 400;
            res.set_content(R"({"success":false,"message":"Invalid JSON format"})", "application/json");
            return;
        }

        // Xử lý JSON REQ
        string action = payload.value("ACTION", "");

        // ================================================================
        // 1. LẤY THÔNG TIN SINH VIÊN (KÈM TOP %)
        // ================================================================
        if (action == "get_StudentInfo") {
            string mssv = payload["MSSV"].get<string>();
            Student* info = system.getStudent(mssv);

            if (info == nullptr) {
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Student not found"}
                };
                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            float topPercentAllSchool = system.getTopPercentAllSchool(info);
            float topPercentGroup = system.getTopPercentByMajorAndCohort(info);

            nlohmann::json responsePayload = {
                {"RESPONSE", "get_StudentInfo"},
                {"name", info->get_Name()},
                {"gpa_4", info->get_GPA4()},
                {"gpa_10", info->get_GPA10()},
                {"gender", info->get_Gender()},
                {"dateOfBirth", info->get_DateOfBirth()},
                {"major", info->get_Major()},
                {"cohort", info->get_Cohort()},
                {"MSSV", info->get_MSSV()},
                {"credit", info->get_Credit()},
                {"DRL", info->get_DRL()},
                {"topPercentAllSchool", topPercentAllSchool},
                {"topPercentGroup", topPercentGroup}
            };

            res.set_content(responsePayload.dump(), "application/json");
        }

        // ================================================================
        // 2. CẬP NHẬT THÔNG TIN SINH VIÊN
        // ================================================================
        else if (action == "UPDATE_STUDENT") {
            bool status = system.updateStudent(payload);

            if (!status) {
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Can't update student"}
                };
                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            // Lưu thay đổi vào DB SQLite
            database.saveStudent(system.getStudent(payload["MSSV"].get<string>()));

            // Tính lại thứ hạng sau khi điểm số hoặc ngành thay đổi
            system.syncAllRanks();

            res.set_content(R"({"success":true,"message":"Student updated"})", "application/json");
        }

        // ================================================================
        // 3. XÓA SINH VIÊN
        // ================================================================
        else if (action == "DELETE_STUDENT") {
            string mssv = payload["MSSV"].get<string>();
            bool status = system.deleteStudent(mssv);

            if (status) {
                database.deleteStudent(mssv);
                // Tính lại thứ tự sau khi bớt 1 sinh viên
                system.syncAllRanks();

                res.set_content(R"({"success":true,"message":"Student deleted"})", "application/json");
            }
            else {
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Can't delete student"}
                };
                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
            }
        }

        // ================================================================
        // 4. NHẬP DỮ LIỆU CSV HÀNG LOẠT
        // ================================================================
        else if (action == "UPLOAD BASE DATA") {
            auto students_Info = payload["CSV"];

            database.beginTransaction(); // Tăng tốc độ ghi SQLite
            for (const auto& data : students_Info) {
                // ĐÃ SỬA: cohort đứng trước mssv
                Student* student = new Student(
                    data["name"].get<string>(),
                    data["gpa_4"].get<float>(),
                    data["gpa_10"].get<float>(),
                    data["gender"].get<bool>(),
                    data["date_of_birth"].get<string>(),
                    data["major"].get<string>(),
                    data["cohort"].get<string>(),
                    data["mssv"].get<string>(),
                    data["has_failed"].get<bool>(),
                    data["credit"].get<int>(),
                    data["drl"].get<int>()
                );
                database.saveStudent(student);
                system.addStudent(student);
            }
            database.commitTransaction();

            // Chốt lại thứ hạng toàn bộ sau khi nạp CSV
            system.syncAllRanks();

            res.set_content(R"({"success":true,"message":"CSV uploaded successfully"})", "application/json");
        }

        // ================================================================
        // 5. LẤY TOP K TOÀN TRƯỜNG
        // ================================================================
        else if (action == "GET_TOP_K") {
            auto topStudents = system.getTopKAllSchool(10);
            nlohmann::json list = nlohmann::json::array();

            for (Student* s : topStudents) {
                list.push_back({
                    {"MSSV", s->get_MSSV()},
                    {"name", s->get_Name()},
                    {"major", s->get_Major()},
                    {"cohort", s->get_Cohort()},
                    {"gpa_4", s->get_GPA4()},
                    {"gpa_10", s->get_GPA10()},
                    {"DRL", s->get_DRL()},
                    {"credit", s->get_Credit()}
                    });
            }

            nlohmann::json responsePayload = { {"students", list} };
            res.set_content(responsePayload.dump(), "application/json");
        }

        // ================================================================
        // 6. LẤY TOP K THEO NGÀNH VÀ KHÓA
        // ================================================================
        else if (action == "GET_TOP_K_MAJOR_COHORT") {
            string major = payload["major"].get<string>();
            string cohort = payload["cohort"].get<string>();

            auto topStudents = system.getTopKByMajorAndCohort(major, cohort, 10);
            nlohmann::json list = nlohmann::json::array();

            for (Student* s : topStudents) {
                list.push_back({
                    {"MSSV", s->get_MSSV()},
                    {"name", s->get_Name()},
                    {"major", s->get_Major()},
                    {"cohort", s->get_Cohort()},
                    {"gpa_4", s->get_GPA4()},
                    {"gpa_10", s->get_GPA10()},
                    {"DRL", s->get_DRL()},
                    {"credit", s->get_Credit()}
                    });
            }

            nlohmann::json responsePayload = { {"students", list} };
            res.set_content(responsePayload.dump(), "application/json");
        }

        // ================================================================
        // 7. LẤY CHI TIẾT SINH VIÊN (KÈM TRẠNG THÁI RỚT MÔN)
        // ================================================================
        else if (action == "get_StudentInfo2") {
            string mssv = payload["MSSV"].get<string>();
            Student* info = system.getStudent(mssv);

            if (info == nullptr) {
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Student not found"}
                };
                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            nlohmann::json responsePayload = {
                {"RESPONSE", "get_StudentInfo2"},
                {"name", info->get_Name()},
                {"gpa_4", info->get_GPA4()},
                {"gpa_10", info->get_GPA10()},
                {"gender", info->get_Gender()},
                {"dateOfBirth", info->get_DateOfBirth()},
                {"major", info->get_Major()},
                {"cohort", info->get_Cohort()},
                {"MSSV", info->get_MSSV()},
                {"credit", info->get_Credit()},
                {"DRL", info->get_DRL()},
                {"isNgu", info->get_IsNgu()}
            };

            res.set_content(responsePayload.dump(), "application/json");
        }

        // ================================================================
        // 8. THÊM 1 SINH VIÊN
        // ================================================================
        else if (action == "ADD_STUDENT") {
            // ĐÃ SỬA: cohort đứng trước MSSV
            Student* student = new Student(
                payload["name"].get<string>(),
                payload["gpa_4"].get<float>(),
                payload["gpa_10"].get<float>(),
                payload["gender"].get<bool>(),
                payload["dateOfBirth"].get<string>(),
                payload["major"].get<string>(),
                payload["cohort"].get<string>(),
                payload["MSSV"].get<string>(),
                payload["isNgu"].get<bool>(),
                payload["credit"].get<int>(),
                payload["DRL"].get<int>()
            );

            system.addStudent(student);
            database.saveStudent(student);

            // Cập nhật lại rank toàn bộ sau khi có sinh viên mới
            system.syncAllRanks();

            res.set_content(R"({"success":true,"message":"Student added"})", "application/json");
        }
        });

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
        });

    server.Options("/api/student", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
        });

    cout << "Server dang chay tai http://localhost:8080" << endl;
    server.listen("localhost", 8080);

    return 0;
}