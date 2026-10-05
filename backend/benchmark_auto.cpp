#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>

#include "ScholarshipSystem.cpp"

#include "../SQL/SQLiteHandler.h"

using namespace std;
using namespace std::chrono;

const int REPEAT_FAST = 10000;
const int REPEAT_SLOW = 1;

template<typename F>
double timeIt(F func, int repeat) {
    func();
    auto t0 = high_resolution_clock::now();
    for (int i = 0; i < repeat; ++i) func();
    auto t1 = high_resolution_clock::now();
    return duration<double, milli>(t1 - t0).count() / repeat;
}

int main() {
    cout << "===== BENCHMARK =====\n";

    SQLiteHandler db("../scholarship.db");
    vector<Student*> data = db.loadData();

    int N = (int)data.size();
    cout << "Da nap " << N << " sinh vien\n";
    if (N == 0) {
        cout << "DB rong! Upload CSV truoc.\n";
        return 1;
    }

    ScholarshipSystem sys;
    auto tb0 = high_resolution_clock::now();
    for (auto s : data) sys.addStudent(s);
    auto tb1 = high_resolution_clock::now();
    double t_bulk = duration<double, milli>(tb1 - tb0).count();

    double t_sync = timeIt([&] { sys.syncAllRanks(); }, REPEAT_SLOW);

    string target = data[N / 2]->get_MSSV();
    double t_mc1 = timeIt([&] { volatile auto p = sys.getStudent(target); (void)p; }, REPEAT_FAST);

    double t_mc2 = timeIt([&] { volatile auto v = sys.getTopKAllSchool(10); (void)v; }, REPEAT_FAST);

    string mj = data[N / 2]->get_Major();
    string ch = data[N / 2]->get_Cohort();
    double t_r1 = timeIt([&] { volatile auto v = sys.getTopKByMajorAndCohort(mj, ch, 10); (void)v; }, REPEAT_FAST);

    Student* sample = data[N / 2];
    double t_r2_all = timeIt([&] { volatile float p = sys.getTopPercentAllSchool(sample); (void)p; }, REPEAT_FAST);
    double t_r2_grp = timeIt([&] { volatile float p = sys.getTopPercentByMajorAndCohort(sample); (void)p; }, REPEAT_FAST);

    double t_add = timeIt([&] {
        Student* s = new Student("Test", 3.0f, 7.5f, true, "2004-01-01",
            "CNTT", "K65", "99999999", false, 100, 80);
        sys.addStudent(s);
        }, REPEAT_SLOW);

    double t_update = timeIt([&] {
        sys.deleteStudent("99999999");
        Student* n = new Student("Test2", 3.5f, 8.0f, true, "2004-01-01",
            "CNTT", "K65", "99999999", false, 100, 85);
        sys.addStudent(n);
        }, REPEAT_SLOW);

    double t_del = timeIt([&] { sys.deleteStudent("99999999"); }, REPEAT_SLOW);

    cout << "\n===== KET QUA (N = " << N << ") =====\n";
    cout << "  Bulk load          : " << t_bulk << " ms\n";
    cout << "  syncAllRanks       : " << t_sync << " ms\n";
    cout << "  MC1  (tra MSSV)    : " << t_mc1 << " ms\n";
    cout << "  MC2  (Top K)       : " << t_mc2 << " ms\n";
    cout << "  R1   (Top K nhom)  : " << t_r1 << " ms\n";
    cout << "  R2   (Top % all)   : " << t_r2_all << " ms\n";
    cout << "  R2   (Top % nhom)  : " << t_r2_grp << " ms\n";
    cout << "  R3   (add)         : " << t_add << " ms\n";
    cout << "  R3   (update)      : " << t_update << " ms\n";
    cout << "  R3   (delete)      : " << t_del << " ms\n";

    ofstream out("benchmark_result.csv");
    out << "N,BulkLoad,syncAllRanks,MC1,MC2,R1,R2_All,R2_Group,R3_Add,R3_Update,R3_Delete\n";
    out << N << "," << t_bulk << "," << t_sync << ","
        << t_mc1 << "," << t_mc2 << "," << t_r1 << ","
        << t_r2_all << "," << t_r2_grp << ","
        << t_add << "," << t_update << "," << t_del << "\n";
    out.close();

    cout << "\nDa luu: benchmark_result.csv\n";

    for (auto s : data) delete s;
    return 0;
}