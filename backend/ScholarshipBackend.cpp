#include "student.cpp"
#include <unordered_map>
#include <set>
#include "CmpHocBong.cpp"
#include <nlohmann/json.hpp>
#include <vector>


class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, Student*> dshocsinh;
                                                
    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<Student*, CmpHocBong> by_priority;

public:
    void add_Student(Student* student){
        dshocsinh[student->get_MSSV()] = student;

        if(student->get_IsNgu() == false){
            by_priority.insert(student);
        }
    }
    
    Student* get_Student(string mssv){
        auto it = dshocsinh.find(mssv);

        if(it == dshocsinh.end()){
            return nullptr;
        }
        else{
            return it->second;
        }
    }

    bool update_Student(nlohmann::json& data)/*hash*/{
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

    bool delete_Student(string mssv)/*Xoa ca hash va set*/{
        auto it = dshocsinh.find(mssv);

        if(it == dshocsinh.end()){ //O tim thay
            return false;
        }
        else{
            by_priority.erase(it->second); //Xoa set
            delete it->second;//Xoa object student
            dshocsinh.erase(it);//Xoa khoi hash

            return true;
        }
    }

    vector<Student*> get_TopK(int soLuongCanLay) {
        vector<Student*> danhSachKetQua;
        int soLuongDaLay = 0;

        for (auto& entry : dshocsinh) {
            Student* sinhVien = entry.second;

            if (soLuongDaLay >= soLuongCanLay){
                break;
            }
            
            danhSachKetQua.push_back(sinhVien);
            ++soLuongDaLay;
        }

        return danhSachKetQua;
    }

    vector<Student*> filter_By_GPA4(float gpaThapNhat, float gpaCaoNhat) {
        vector<Student*> danhSachKetQua;

        for (auto& entry : dshocsinh) {

            Student* sinhVien = entry.second;
            
            if (sinhVien->get_GPA4() >= gpaThapNhat && sinhVien->get_GPA4() <= gpaCaoNhat) {
                danhSachKetQua.push_back(sinhVien);
            }
        }
        return danhSachKetQua;
    }
};