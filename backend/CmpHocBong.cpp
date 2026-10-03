struct CmpHocBong {
    bool operator()(const Student* a, const Student* b) const {
        // ===== 1. Tiêu chí chính: GPA hệ 4 (càng cao càng ưu tiên đứng trước) =====
        if (a->get_GPA4() != b->get_GPA4()) {
            return a->get_GPA4() > b->get_GPA4(); // Giảm dần
        }

        // ===== 2. Tiêu chí phụ 1: Điểm rèn luyện (DRL) (cao hơn ưu tiên) =====
        if (a->get_DRL() != b->get_DRL()) {
            return a->get_DRL() > b->get_DRL(); // Giảm dần
        }

        // ===== 3. Tiêu chí phụ 2: GPA hệ 10 (Dùng để phụ trợ khi điểm hệ 4 bằng nhau) =====
        if (a->get_GPA10() != b->get_GPA10()) {
            return a->get_GPA10() > b->get_GPA10(); // Giảm dần
        }

        // ===== 4. Tiêu chí cuối: MSSV (đảm bảo thứ tự ổn định, không bị trùng lặp trong Set) =====
        return a->get_MSSV() < b->get_MSSV(); // Tăng dần (A -> Z)
    }
};