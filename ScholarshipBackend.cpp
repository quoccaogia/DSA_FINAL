#include <student.cpp>
#include <unordered_map>
#include <set>
#include <CmpHocBong.cpp>

class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, Student*> by_mssv;

    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<Student*, CmpHocBong> by_priority;

public:
    // các hàm...
};