#include "student.cpp"
#include <unordered_map>
#include <set>
#include "CmpHocBong.cpp"

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

    bool regrade_Req(string mssv){
        //Từ từ chờ fe
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
        }
    }

};