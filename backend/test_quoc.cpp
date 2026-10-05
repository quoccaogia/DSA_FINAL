
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include "ScholarshipSystem.cpp"

using namespace std;

// ==================== BIẾN ĐẾM ====================
int passed = 0, failed = 0;

void check(bool condition, const string& testName) {
    if (condition) {
        cout << "  [PASS] " << testName << endl;
        passed++;
    }
    else {
        cout << "  [FAIL] " << testName << endl;
        failed++;
    }
}

void header(const string& name) {
    cout << "\n========== " << name << " ==========\n";
}

// ==================== HELPER ====================
Student* makeSV(const string& mssv, const string& major, const string& cohort,
    float gpa4, float gpa10, int drl, bool isNgu = false) {
    return new Student("Ten" + mssv, gpa4, gpa10, true, "2004-01-01",
        major, cohort, mssv, isNgu, 100, drl);
}

// ============================================================================
// NHÓM 1 — MC2: TOP K TOÀN TRƯỜNG
// ============================================================================

void test_topk_k5_thu_tu() {
    cout << "\n[TEST] MC2 - Top K=5, thu tu dung\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));
    sys.addStudent(makeSV("004", "CNTT", "K65", 3.7, 9.0, 85));
    sys.addStudent(makeSV("005", "CNTT", "K65", 3.0, 7.5, 70));
    sys.addStudent(makeSV("006", "CNTT", "K65", 2.5, 6.0, 60));

    auto top = sys.getTopKAllSchool(5);
    check(top.size() == 5, "Tra dung 5 phan tu");
    check(top[0]->get_MSSV() == "002", "Hang 1 = 002");
    check(top[1]->get_MSSV() == "004", "Hang 2 = 004");
    check(top[2]->get_MSSV() == "001", "Hang 3 = 001");
}

void test_topk_k1() {
    cout << "\n[TEST] MC2 - Top K=1\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));

    auto top = sys.getTopKAllSchool(1);
    check(top.size() == 1, "Chi tra 1 phan tu");
    check(top[0]->get_MSSV() == "002", "Dung nguoi dung dau");
}

void test_topk_k_bang_n() {
    cout << "\n[TEST] MC2 - Top K = N\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));

    auto top = sys.getTopKAllSchool(3);
    check(top.size() == 3, "K = N -> tra het");
}

void test_topk_k_lon_hon_n() {
    cout << "\n[TEST] MC2 - Top K > N\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));

    auto top = sys.getTopKAllSchool(1000);
    check(top.size() == 2, "K > N -> tra toi da");
}

void test_topk_k_0() {
    cout << "\n[TEST] MC2 - Top K = 0\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    auto top = sys.getTopKAllSchool(0);
    check(top.empty(), "K = 0 -> rong");
}

void test_topk_k_am() {
    cout << "\n[TEST] MC2 - Top K am\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    auto top = sys.getTopKAllSchool(-5);
    check(top.empty(), "K < 0 -> rong, khong crash");
}

void test_topk_he_thong_rong() {
    cout << "\n[TEST] MC2 - He thong rong\n";
    ScholarshipSystem sys;
    auto top = sys.getTopKAllSchool(10);
    check(top.empty(), "He thong rong -> rong");
}

void test_topk_bo_qua_rot_mon() {
    cout << "\n[TEST] MC2 - Bo qua SV rot mon\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90, false));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95, true));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80, false));

    auto top = sys.getTopKAllSchool(10);
    check(top.size() == 2, "Chi 2 SV du dieu kien");
    bool coRot = false;
    for (auto s : top) if (s->get_MSSV() == "002") coRot = true;
    check(!coRot, "SV 002 (rot) khong co trong Top K");
}

// ============================================================================
// NHÓM 2 — R1: TOP K THEO NGÀNH/KHÓA
// ============================================================================

void test_r1_dung_nhom() {
    cout << "\n[TEST] R1 - Top K dung nhom\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));
    sys.addStudent(makeSV("004", "KT", "K65", 4.0, 10.0, 100));
    sys.addStudent(makeSV("005", "KT", "K65", 3.8, 9.0, 90));

    auto top = sys.getTopKByMajorAndCohort("CNTT", "K65", 10);
    check(top.size() == 3, "Chi 3 SV CNTT-K65 (khong lan KT)");
    check(top[0]->get_MSSV() == "002", "Hang 1 dung");
    check(top[1]->get_MSSV() == "001", "Hang 2 dung");
    check(top[2]->get_MSSV() == "003", "Hang 3 dung");
}

void test_r1_nhom_rong() {
    cout << "\n[TEST] R1 - Nhom khong ton tai\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    auto top = sys.getTopKByMajorAndCohort("XXX", "K00", 10);
    check(top.empty(), "Nhom khong ton tai -> rong");
}

void test_r1_nhom_chi_rot_mon() {
    cout << "\n[TEST] R1 - Nhom chi co SV rot mon\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90, true));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95, true));

    auto top = sys.getTopKByMajorAndCohort("CNTT", "K65", 10);
    check(top.empty(), "Ca nhom rot mon -> rong");
}

void test_r1_nhieu_nhom_doc_lap() {
    cout << "\n[TEST] R1 - Nhieu nhom khong lan nhau\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K66", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "KT", "K65", 3.2, 8.0, 80));
    sys.addStudent(makeSV("004", "KT", "K66", 3.7, 9.0, 85));

    auto c65 = sys.getTopKByMajorAndCohort("CNTT", "K65", 10);
    auto c66 = sys.getTopKByMajorAndCohort("CNTT", "K66", 10);
    auto k65 = sys.getTopKByMajorAndCohort("KT", "K65", 10);
    auto k66 = sys.getTopKByMajorAndCohort("KT", "K66", 10);

    check(c65.size() == 1 && c65[0]->get_MSSV() == "001", "CNTT-K65 -> 001");
    check(c66.size() == 1 && c66[0]->get_MSSV() == "002", "CNTT-K66 -> 002");
    check(k65.size() == 1 && k65[0]->get_MSSV() == "003", "KT-K65 -> 003");
    check(k66.size() == 1 && k66[0]->get_MSSV() == "004", "KT-K66 -> 004");
}

void test_r1_k_lon_hon_nhom() {
    cout << "\n[TEST] R1 - K > so SV nhom\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));

    auto top = sys.getTopKByMajorAndCohort("CNTT", "K65", 100);
    check(top.size() == 2, "K > nhom -> toi da");
}

// ============================================================================
// NHÓM 3 — R2: TOP %
// ============================================================================

void test_r2_hang1() {
    cout << "\n[TEST] R2 - Top % hang 1 = 1/N\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));
    sys.addStudent(makeSV("004", "CNTT", "K65", 3.7, 9.0, 85));
    sys.syncAllRanks();

    Student* hang1 = sys.getStudent("002");
    float p = sys.getTopPercentAllSchool(hang1);
    float expect = (1.0f / 4.0f) * 100.0f;
    check(fabs(p - expect) < 0.01, "Hang 1 -> Top 25%");
}

void test_r2_hang_cuoi() {
    cout << "\n[TEST] R2 - Top % hang cuoi = 100%\n";
    ScholarshipSystem sys;
    for (int i = 0; i < 10; ++i) {
        char mssv[10]; sprintf(mssv, "%03d", i);
        sys.addStudent(makeSV(mssv, "CNTT", "K65", 3.0f + i * 0.1f, 7.0f, 70 + i));
    }
    sys.syncAllRanks();

    Student* hangCuoi = sys.getStudent("000");
    check(fabs(sys.getTopPercentAllSchool(hangCuoi) - 100.0f) < 0.01, "Hang cuoi -> 100%");
}

void test_r2_hang_giua() {
    cout << "\n[TEST] R2 - Top % hang giua\n";
    ScholarshipSystem sys;
    for (int i = 0; i < 10; ++i) {
        char mssv[10]; sprintf(mssv, "%03d", i);
        sys.addStudent(makeSV(mssv, "CNTT", "K65", 3.0f + i * 0.1f, 7.0f, 70 + i));
    }
    sys.syncAllRanks();

    Student* hang5 = sys.getStudent("005");
    check(fabs(sys.getTopPercentAllSchool(hang5) - 50.0f) < 0.01, "Hang 5/10 -> 50%");
}

void test_r2_nhom() {
    cout << "\n[TEST] R2 - Top % nhom\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));
    for (int i = 0; i < 5; ++i) {
        char mssv[10]; sprintf(mssv, "10%d", i);
        sys.addStudent(makeSV(mssv, "KT", "K65", 3.0f, 7.0f, 70));
    }
    sys.syncAllRanks();

    Student* sv = sys.getStudent("002");
    float expect = (1.0f / 3.0f) * 100.0f;
    check(fabs(sys.getTopPercentByMajorAndCohort(sv) - expect) < 0.01, "Top % nhom = 1/3 (khong phai 1/8)");
}

void test_r2_rot_mon() {
    cout << "\n[TEST] R2 - SV rot mon -> -1\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90, false));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95, true));
    sys.syncAllRanks();

    Student* svRot = sys.getStudent("002");
    check(sys.getTopPercentAllSchool(svRot) == -1.0f, "Top % all = -1");
    check(sys.getTopPercentByMajorAndCohort(svRot) == -1.0f, "Top % nhom = -1");
}

void test_r2_1_sv_duy_nhat() {
    cout << "\n[TEST] R2 - Chi 1 SV -> Top 100%\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.syncAllRanks();

    Student* sv = sys.getStudent("001");
    check(fabs(sys.getTopPercentAllSchool(sv) - 100.0f) < 0.01, "1 SV -> 100%");
}

// ============================================================================
// NHÓM 4 — COMPARATOR
// ============================================================================

void test_cmp_gpa4() {
    cout << "\n[TEST] Comparator - GPA4 uu tien chinh\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 9.5, 100));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 7.0, 70));

    auto top = sys.getTopKAllSchool(2);
    check(top[0]->get_MSSV() == "002", "GPA4 cao dung truoc");
}

void test_cmp_drl() {
    cout << "\n[TEST] Comparator - DRL pha hoa\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 70));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.5, 8.5, 95));

    auto top = sys.getTopKAllSchool(2);
    check(top[0]->get_MSSV() == "002", "DRL cao dung truoc");
}

void test_cmp_gpa10() {
    cout << "\n[TEST] Comparator - GPA10 pha hoa\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 9.0, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.5, 9.5, 90));

    auto top = sys.getTopKAllSchool(2);
    check(top[0]->get_MSSV() == "002", "GPA10 cao dung truoc");
}

void test_cmp_mssv() {
    cout << "\n[TEST] Comparator - MSSV pha hoa cuoi\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    auto top = sys.getTopKAllSchool(2);
    check(top[0]->get_MSSV() == "001", "MSSV nho hon dung truoc");
}

void test_cmp_hoa_hoan_toan() {
    cout << "\n[TEST] Comparator - 2 SV hoa hoan toan\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.5, 8.5, 90));

    auto top = sys.getTopKAllSchool(10);
    check(top.size() == 2, "Ca 2 deu co trong Top K");
    check(sys.getStudent("001") != nullptr, "001 ton tai");
    check(sys.getStudent("002") != nullptr, "002 ton tai");
}

// ============================================================================
// NHÓM 5 — SYNCALLRANKS
// ============================================================================

void test_sync_rank_school() {
    cout << "\n[TEST] syncAllRanks - rankSchool dung\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "CNTT", "K65", 3.2, 8.0, 80));
    sys.syncAllRanks();

    check(sys.getStudent("002")->get_RankSchool() == 1, "002 -> 1");
    check(sys.getStudent("001")->get_RankSchool() == 2, "001 -> 2");
    check(sys.getStudent("003")->get_RankSchool() == 3, "003 -> 3");
}

void test_sync_rank_major() {
    cout << "\n[TEST] syncAllRanks - rankMajor theo nhom\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.addStudent(makeSV("003", "KT", "K65", 4.0, 10.0, 100));
    sys.syncAllRanks();

    check(sys.getStudent("002")->get_RankMajor() == 1, "002 -> rankMajor 1 (CNTT)");
    check(sys.getStudent("001")->get_RankMajor() == 2, "001 -> rankMajor 2 (CNTT)");
    check(sys.getStudent("003")->get_RankMajor() == 1, "003 -> rankMajor 1 (KT)");
}

void test_sync_rank_sau_khi_them() {
    cout << "\n[TEST] syncAllRanks - cap nhat sau khi them\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.syncAllRanks();
    check(sys.getStudent("001")->get_RankSchool() == 1, "Ban dau: 001 -> 1");

    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.syncAllRanks();

    check(sys.getStudent("002")->get_RankSchool() == 1, "Sau: 002 -> 1");
    check(sys.getStudent("001")->get_RankSchool() == 2, "Sau: 001 -> 2");
}

// ============================================================================
// NHÓM 6 — ADD / DELETE / ĐỒNG BỘ CHỈ MỤC
// ============================================================================

void test_add_vao_3_cau_truc() {
    cout << "\n[TEST] addStudent - co mat o ca 3 cau truc\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    check(sys.getStudent("001") != nullptr, "Tra hash OK");
    auto top = sys.getTopKAllSchool(10);
    check(top.size() == 1 && top[0]->get_MSSV() == "001", "Co trong cay toan truong");
    auto grp = sys.getTopKByMajorAndCohort("CNTT", "K65", 10);
    check(grp.size() == 1, "Co trong cay nhom");
}

void test_add_rot_mon_khong_vao_cay() {
    cout << "\n[TEST] addStudent - SV rot mon khong vao cay\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90, true));

    check(sys.getStudent("001") != nullptr, "Van tra hash duoc");
    check(sys.getTopKAllSchool(10).empty(), "Khong co trong cay toan truong");
    check(sys.getTopKByMajorAndCohort("CNTT", "K65", 10).empty(), "Khong co trong cay nhom");
}

void test_add_trung_mssv() {
    cout << "\n[TEST] addStudent - MSSV trung\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.9, 9.5, 95));

    auto top = sys.getTopKAllSchool(10);
    check(top.size() == 1, "MSSV trung -> 1 ban trong cay");
}

void test_delete_trong_top() {
    cout << "\n[TEST] deleteStudent - Xoa SV trong Top K\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.addStudent(makeSV("002", "CNTT", "K65", 3.9, 9.5, 95));
    sys.deleteStudent("002");

    check(sys.getStudent("002") == nullptr, "Da xoa khoi hash");
    check(sys.getTopKAllSchool(10).size() == 1, "Da xoa khoi cay");
}

void test_delete_rot_mon() {
    cout << "\n[TEST] deleteStudent - Xoa SV rot mon\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90, true));

    check(sys.deleteStudent("001"), "Xoa thanh cong");
    check(sys.getStudent("001") == nullptr, "Da xoa khoi hash");
}

void test_delete_khong_ton_tai() {
    cout << "\n[TEST] deleteStudent - MSSV khong ton tai\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));

    check(!sys.deleteStudent("999"), "Tra false");
}

void test_delete_don_nhom_rong() {
    cout << "\n[TEST] deleteStudent - Xoa het nhom -> don key\n";
    ScholarshipSystem sys;
    sys.addStudent(makeSV("001", "CNTT", "K65", 3.5, 8.5, 90));
    sys.deleteStudent("001");

    auto top = sys.getTopKByMajorAndCohort("CNTT", "K65", 10);
    check(top.empty(), "Nhom da rong");
}

// ============================================================================
// MAIN
// ============================================================================
int main() {
    cout << "========================================================\n";
    cout << " BO TEST - Cao Gia Quoc (MC2, R1, R2)\n";
    cout << "========================================================\n";

    header("NHOM 1 - MC2: TOP K TOAN TRUONG");
    test_topk_k5_thu_tu();
    test_topk_k1();
    test_topk_k_bang_n();
    test_topk_k_lon_hon_n();
    test_topk_k_0();
    test_topk_k_am();
    test_topk_he_thong_rong();
    test_topk_bo_qua_rot_mon();

    header("NHOM 2 - R1: TOP K THEO NGANH/KHOA");
    test_r1_dung_nhom();
    test_r1_nhom_rong();
    test_r1_nhom_chi_rot_mon();
    test_r1_nhieu_nhom_doc_lap();
    test_r1_k_lon_hon_nhom();

    header("NHOM 3 - R2: TOP %");
    test_r2_hang1();
    test_r2_hang_cuoi();
    test_r2_hang_giua();
    test_r2_nhom();
    test_r2_rot_mon();
    test_r2_1_sv_duy_nhat();

    header("NHOM 4 - COMPARATOR");
    test_cmp_gpa4();
    test_cmp_drl();
    test_cmp_gpa10();
    test_cmp_mssv();
    test_cmp_hoa_hoan_toan();

    header("NHOM 5 - SYNCALLRANKS");
    test_sync_rank_school();
    test_sync_rank_major();
    test_sync_rank_sau_khi_them();

    header("NHOM 6 - ADD / DELETE / DONG BO CHI MUC");
    test_add_vao_3_cau_truc();
    test_add_rot_mon_khong_vao_cay();
    test_add_trung_mssv();
    test_delete_trong_top();
    test_delete_rot_mon();
    test_delete_khong_ton_tai();
    test_delete_don_nhom_rong();

    cout << "\n========================================================\n";
    cout << " KET QUA: " << passed << " PASS / " << failed << " FAIL\n";
    cout << "========================================================\n";
    return failed == 0 ? 0 : 1;
}