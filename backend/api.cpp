#include "httplib.h"
#include "ScholarshipBackend.cpp"
#include <string>
#include <vector>
#include "SQL/SQLiteHandler.h"
using namespace std;


int main() {
    httplib::Server server;

    SQLiteHandler database;
    ScholarshipSystem system; 
    
    for(Student* student : database.loadData()){
        system.add_Student(student);
    }


    server.Post("/api/student", [&system, &database /*add whatever outside*/](const httplib::Request& req,
                                   httplib::Response& res) {

        // Nhận JSON từ FE
        nlohmann::json payload = nlohmann::json::parse(req.body);
        
        // Xử lý JSON REQ
        string action = payload["ACTION"].get<string>();
        
        if(action == "get_StudentInfo"){
            string mssv = payload["MSSV"].get<string>();

            Student* info = system.get_Student(mssv);

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
                {"MSSV", info->get_MSSV()},
                {"credit", info->get_Credit()},
                {"DRL", info->get_DRL()}
            };

            res.set_content(responsePayload.dump(), "application/json");
        }

        else if(action == "UPDATE_STUDENT"){
            bool status = system.update_Student(payload);
            
            if(status == false){
                nlohmann::json responsePayload = {
                    {"success", false},
                    {"message", "Can't update student"}
                };

                res.status = 404;
                res.set_content(responsePayload.dump(), "application/json");
                return;
            }

            database.saveStudent(system.get_Student(payload["MSSV"].get<string>()));
        }

        else if(action == "DELETE_STUDENT"){
            bool status = system.delete_Student(payload["MSSV"].get<string>());
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