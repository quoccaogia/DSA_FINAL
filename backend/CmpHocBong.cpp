struct CmpHocBong {
    bool operator()(Student* a, Student* b){

        // 1. Ưu tiên GPA hệ 4 giảm dần
        if (a->get_GPA4() != b->get_GPA4()) {
            return a->get_GPA4() > b->get_GPA4();
        }

        // 2. Nếu GPA bằng nhau -> Ưu tiên Điểm rèn luyện giảm dần
        if (a->get_DRL() != b->get_DRL()) {
            return a->get_DRL() > b->get_DRL();
        }

        // 3. Nếu Điểm rèn luyện bằng nhau -> Ưu tiên Số tín chỉ giảm dần
        if (a->get_Credit() != b->get_Credit()) {
            return a->get_Credit() > b->get_Credit();
        }

        // 4. Nếu bằng nhau tất cả -> Sắp xếp theo MSSV tăng dần (để không bị trùng)
        return a->get_MSSV() < b->get_MSSV();
    }
};