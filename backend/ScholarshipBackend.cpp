#include "student.cpp"
#include <unordered_map>
#include <set>
#include "CmpHocBong.cpp"
#include <nlohmann/json.hpp>
#include <vector>


class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, Student*> dshocbong;

    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<Student*, CmpHocBong> by_priority;

public:
    // các hàm...
    Student* get_Student(string mssv){
        auto it = dshocbong.find(mssv);

        if(it == dshocbong.end()){
            return nullptr;
        }
        else{
            return it->second;
        }
    }

    bool update_Student(nlohmann::json& data){
        Student* student = get_Student(data["MSSV"].get<string>());
        if(student == nullptr){
            return false;
        }

         if (!data["name"].is_null())
        student->set_Name(data["name"].get<string>());

        if (!data["gpa_4"].is_null())
            student->set_GPA4(data["gpa_4"].get<float>());

        if (!data["gpa_10"].is_null())
            student->set_GPA10(data["gpa_10"].get<float>());

        if (!data["gender"].is_null())
            student->set_Gender(data["gender"].get<bool>());

        if (!data["dateOfBirth"].is_null())
            student->set_DateOfBirth(data["dateOfBirth"].get<string>());

        if (!data["major"].is_null())
            student->set_Major(data["major"].get<string>());

        if (!data["credit"].is_null())
            student->set_Credit(data["credit"].get<int>());

        if (!data["DRL"].is_null())
            student->set_DRL(data["DRL"].get<int>());

        return true;
    }

    bool delete_Student(string mssv){
        auto it = dshocbong.find(mssv);

        if(it == dshocbong.end()){
            return false;
        }
        else{
            delete it->second;
            dshocbong.erase(it);
            return true;
        }
    }

};