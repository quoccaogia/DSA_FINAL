#include <student.cpp>
#include <unordered_map>
#include <set>
#include <CmpHocBong.cpp>

class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, Student*> dshocbong;

    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<Student*, CmpHocBong> by_priority;

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

};