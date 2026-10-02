struct CmpHocBong {
    bool operator()(const Student* a, const Student* b) const {
        // ===== 1. Tiêu chí chính: GPA4 / credit (càng cao càng tốt) =====
        float scoreA = 0.0f;
        float scoreB = 0.0f;

        if (a->get_Credit() > 0)
            scoreA = a->get_GPA4() / a->get_Credit();
        if (b->get_Credit() > 0)
            scoreB = b->get_GPA4() / b->get_Credit();

        if (scoreA != scoreB)
            return scoreA > scoreB;   // giảm dần

        // ===== 2. Tiêu chí phụ 1: Điểm rèn luyện (cao hơn ưu tiên) =====
        if (a->get_DRL() != b->get_DRL())
            return a->get_DRL() > b->get_DRL();

        // ===== 3. Tiêu chí phụ 2: GPA4 tuyệt đối =====
        if (a->get_GPA4() != b->get_GPA4())
            return a->get_GPA4() > b->get_GPA4();

        // ===== 4. Tiêu chí cuối: MSSV (để thứ tự ổn định) =====
        return a->get_MSSV() < b->get_MSSV();
    }
};