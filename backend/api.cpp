#include "httplib.h"
#include <nlohmann/json.hpp>
#include "ScholarshipBackend.cpp"
#include <string>
#include <vector>
#include "SQL/SQLiteHandler.h"
using namespace std;


int main() {
    httplib::Server server;

    SQLiteHandler database;
    ScholarshipSystem system; 
    

    //Khoi dong thi add student vao
    if(database.isFirst() == false){
        for(Student* student : database.loadData()){
            system.addStudent(student);
        }
    }
    
    server.Post("/api/student", [&system, &database /*add whatever outside*/](const httplib::Request& req,
                                   httplib::Response& res) {

        // Nhận JSON từ FE
        nlohmann::json payload = nlohmann::json::parse(req.body);
        
        // Xử lý JSON REQ
        string action = payload["ACTION"].get<string>();
                                    
        // ==== Lay thong tin cua hoc sinh dua theo MSSV
        if(action == "get_StudentInfo"){
            string mssv = payload["MSSV"].get<string>();

            Student* info = system.getStudent(mssv);
            float topPercentAllSchool = system.getTopPercentAllSchool(info);
            float topPercentGroup = system.getTopPercentByMajorAndCohort(info);

            //Nếu ko tìm thấy sinh vien
            if(info == nullptr){
                nlohmann::json responsePayload = {
                {"success", false},
                {"message", "Student not found"}
                };

                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            //tim thay
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


        // === Update thong tin sinh vien
        else if(action == "UPDATE_STUDENT"){
            bool status = system.updateStudent(payload);
            
            if(status == false){
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Can't update student"}
                };

                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            database.saveStudent(system.getStudent(payload["MSSV"].get<string>()));
        }

        // ===== Xoa thong tin sinh vien
        else if(action == "DELETE_STUDENT"){
            bool status = system.deleteStudent(payload["MSSV"].get<string>());
            if(status == true){
                database.deleteStudent(payload["MSSV"].get<string>());
            }
            else{
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Can't delete student"}
                };

                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
            }
        }

        // ======= Nhập csv lần đầu
        else if(action == "UPLOAD BASE DATA"){
            auto students_Info = payload["CSV"];

            for(const auto& data : students_Info){
                Student* student = new Student(
                    data["name"].get<string>(),
                    data["gpa_4"].get<float>(),
                    data["gpa_10"].get<float>(),
                    data["gender"].get<bool>(),
                    data["date_of_birth"].get<string>(),
                    data["major"].get<string>(),
                    data["mssv"].get<string>(),
                    data["cohort"].get<string>(),
                    data["has_failed"].get<bool>(),
                    data["credit"].get<int>(),
                    data["drl"].get<int>()
                );
                database.saveStudent(student);
                system.addStudent(student);
            }
        }
    
        // ===== TopK
        else if(action == "GET_TOP_K"){
            auto topStudents = system.getTopKAllSchool(10);

            nlohmann::json list = nlohmann::json::array();

            for (Student* s : topStudents) {
                list.push_back({
                    {"MSSV", s->get_MSSV()},
                    {"name", s->get_Name()},
                    {"major", s->get_Major()},
                    {"gpa_4", s->get_GPA4()},
                    {"gpa_10", s->get_GPA10()},
                    {"DRL", s->get_DRL()},
                    {"credit", s->get_Credit()}
                });
            }

            nlohmann::json responsePayload = {
                {"students", list}
            };

            res.set_content(
                responsePayload.dump(),
                "application/json"
            );
        }
        else if(action == "GET_TOP_K_MAJOR_COHORT"){
            string major = payload["major"].get<string>();
            string cohort = payload["cohort"].get<string>();

            auto topStudents = system.getTopKByMajorAndCohort(major, cohort, 10);

            nlohmann::json list = nlohmann::json::array();

            for (Student* s : topStudents) {
                list.push_back({
                    {"MSSV", s->get_MSSV()},
                    {"name", s->get_Name()},
                    {"major", s->get_Major()},
                    {"gpa_4", s->get_GPA4()},
                    {"gpa_10", s->get_GPA10()},
                    {"DRL", s->get_DRL()},
                    {"credit", s->get_Credit()}
                });
            }

            nlohmann::json responsePayload = {
                {"students", list}
            };

            res.set_content(
                responsePayload.dump(),
                "application/json"
            );
        }
        
    });

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Options("/api/student", [](const httplib::Request& req,
                                  httplib::Response& res) {
    res.status = 200;
    });

    server.listen("localhost", 8080);
}