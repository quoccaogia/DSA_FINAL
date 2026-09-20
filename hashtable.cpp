class ScholarshipSystem {
private:
    // 1. Hash Table - tra cứu theo MSSV
    unordered_map<string, HocSinh*> by_mssv;

    // 2. Set - xếp hạng theo nhiều tiêu chí
    set<HocSinh*, CmpHocBong> by_priority;

public:
    // các hàm...
};