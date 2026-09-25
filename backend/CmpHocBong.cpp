struct CmpHocBong {
    bool operator()(const Student* a, const Student* b) const {

        // 1. Ưu tiên GPA hệ 4 giảm dần
        if (a->getGPA4() != b->getGPA4()) {
            return a->getGPA4() > b->getGPA4();
        }

        // 2. Nếu GPA bằng nhau -> Ưu tiên Điểm rèn luyện giảm dần
        if (a->getDRL() != b->getDRL()) {
            return a->getDRL() > b->getDRL();
        }

        // 3. Nếu Điểm rèn luyện bằng nhau -> Ưu tiên Số tín chỉ giảm dần
        if (a->getCredit() != b->getCredit()) {
            return a->getCredit() > b->getCredit();
        }

        // 4. Nếu bằng nhau tất cả -> Sắp xếp theo MSSV tăng dần (để không bị trùng)
        return a->getMSSV() < b->getMSSV();
    }
};